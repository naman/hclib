/* Copyright (c) 2015, Rice University

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1.  Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.
3.  Neither the name of Rice University
     nor the names of its contributors may be used to endorse or
     promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

/*
 * hclib-deque.cpp
 *
 *      Author: Vivek Kumar (vivekk@rice.edu)
 *      Acknowledgments: https://wiki.rice.edu/confluence/display/HABANERO/People
 */

#include "hclib-internal.h"
#include "hclib-atomics.h"

#if 0 // UNUSED
void deque_init(deque_t *deq) {
    deq->head = 0;
    //@ FIXME ATOMIC: this should be a RELEASE to ensure synchronization?
    deq->tail = 0;
}

void deque_destroy(deque_t *deq) {
    free(deq);
}
#endif

/*
 * push an entry onto the tail of the deque
 */
int deque_push(deque_t *deq, hclib_task_t *entry) {
    int tail = _hclib_atomic_load_relaxed(&deq->tail);
    int head = _hclib_atomic_load_relaxed(&deq->head);
    int size = tail - head;
    if (size == INIT_DEQUE_CAPACITY) { /* deque looks full */
        /* may not grow the deque if some interleaving steal occur */
        // std::cout<<getenv("PMI_RANK") <<": Deque full for worker-"<<current_ws()->id << std::endl;
        // HASSERT("DEQUE full, increase deque's size " && 0);
        return 0;
    }
    int n = tail % INIT_DEQUE_CAPACITY;
    deq->data[n] = entry;
    //@ ATOMIC: this should be a RELEASE to ensure synchronization
    _hclib_atomic_inc_release(&deq->tail);
    return 1;
}

/*
 * the steal protocol
 */
hclib_task_t *deque_steal(deque_t *deq) {
    int head;
    /* Cannot read deq->data[head] here
     * Can happen that head=tail=0, then the owner of the deq pushes
     * a new task when stealer is here in the code, resulting in head=0, tail=1
     * All other checks down-below will be valid, but the old value of the buffer head
     * would be returned by the steal rather than the new pushed value.
     */
    int tail;

    head = _hclib_atomic_load_relaxed(&deq->head);
    //@ ATOMIC: load acquire
    //@ We want all the writes from the producing thread to read the task data
    //@ and we're using the tail as the synchronization variable
    //@- hc_mfence();
    //@- tail = deq->tail;
    tail = _hclib_atomic_load_acquire(&deq->tail);
    if ((tail - head) <= 0) {
        return NULL;
    }

    hclib_task_t *t = deq->data[head % INIT_DEQUE_CAPACITY];
    /* compete with other thieves and possibly the owner (if the size == 1) */
    //@ FIXME ATOMIC: __atomic_compare_exchange_n(&deq->head, &head, head+1, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
    //@ We only need an atomic update on the head index here
    //@ (nothing else has been updated by this thread)
    //@ the preceding fence already takes care of any other sync issues
    //@- if (hc_cas(&deq->head, head, head + 1)) { /* competing */
    if (_hclib_cas_relaxed(&deq->head, head, head + 1)) { /* competing */
        return t;
    }
    return NULL;
}

/*
 * pop the task out of the deque from the tail
 */
hclib_task_t *deque_pop(deque_t *deq) {
    //@ ATOMIC:
    //@- hc_mfence(); //@ this seems unecessary
    //@ do an ATOMIC_ACQ_REL fetch_and_add on the tail to decrement
    int tail = _hclib_atomic_dec_acq_rel(&deq->tail);
    //@- hc_mfence(); //@ this seems unecessary
    int head = _hclib_atomic_load_relaxed(&deq->head);

    int size = tail - head;
    if (size < 0) {
        _hclib_atomic_store_release(&deq->tail, head);
        return NULL;
    }
    hclib_task_t *t = deq->data[tail % INIT_DEQUE_CAPACITY];

    if (size > 0) {
        return t;
    }

    /* now the deque appears empty */
    /* I need to compete with the thieves for the last task */
    //@- if (!hc_cas(&deq->head, head, head + 1)) {
    if (!_hclib_cas_relaxed(&deq->head, head, head + 1)) {
        t = NULL;
    }

    _hclib_atomic_inc_release(&deq->tail);

    return t;
}

/******************************************************/
/* Semi Concurrent DEQUE                              */
/******************************************************/

#if 0 // UNUSED
void semi_conc_deque_init(semi_conc_deque_t *semiDeq) {
    deque_t *deq = &semiDeq->deque;
    deq->head = 0;
    deq->tail = 0;
    semiDeq->lock = 0;
}

void semi_conc_deque_destroy(semi_conc_deque_t *semiDeq) {
    free(semiDeq);
}

void semi_conc_deque_locked_push(semi_conc_deque_t *semiDeq, hclib_task_t *entry) {
    deque_t *deq = &semiDeq->deque;
    int success = 0;
    while (!success) {
        int size = deq->tail - deq->head;
        if (INIT_DEQUE_CAPACITY == size) {
            HASSERT("DEQUE full, increase deque's size" && 0);
        }

        if (hc_cas(&semiDeq->lock, 0, 1) ) {
            success = 1;
            int n = deq->tail % INIT_DEQUE_CAPACITY;
            deq->data[n] = (hclib_task_t *) entry;
            hc_mfence();
            ++deq->tail;
            semiDeq->lock= 0;
        }
    }
}

hclib_task_t *semi_conc_deque_non_locked_pop(semi_conc_deque_t *semiDeq) {
    deque_t *deq = &semiDeq->deque;
    int head = deq->head;
    int tail = deq->tail;

    if ((tail - head) > 0) {
        hclib_task_t *t = (hclib_task_t *) deq->data[head % INIT_DEQUE_CAPACITY];
        ++deq->head;
        return t;
    }
    return NULL;
}
#endif


