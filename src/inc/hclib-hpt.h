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
 * hclib-hpt.h
 *  
 *      Author: Vivek Kumar (vivekk@rice.edu)
 *      Ported from Habanero-C
 *      Acknowledgments: https://wiki.rice.edu/confluence/display/HABANERO/People
 */

#ifndef HCLIB_HPT_H_
#define HCLIB_HPT_H_

#include "hclib-internal.h"

place_t * read_hpt(place_t *** all_places, int * num_pl, int * nproc,
        hclib_worker_state *** all_workers, int * num_wk);
void free_hpt(place_t * hpt);
void hc_hpt_init(hc_context * context);
void hc_hpt_cleanup(hc_context * context);
void hc_hpt_dev_init(hc_context * context);
void hc_hpt_dev_cleanup(hc_context * context);
hc_deque_t * get_deque_place(hclib_worker_state * ws, place_t * pl);
hclib_task_t* hpt_pop_task(hclib_worker_state * ws);
hclib_task_t* hpt_steal_task(hclib_worker_state* ws);
int deque_push_place(hclib_worker_state *ws, place_t * pl, hclib_task_t * ele);

#endif /* HCLIB_HPT_H_ */
