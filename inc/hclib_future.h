#ifndef HCLIB_FUTURE_H
#define HCLIB_FUTURE_H

#include "hclib-promise.h"

namespace hclib {

class future_t {
    public:
        hclib_future_t *internal;
        explicit future_t(hclib_future_t *set_internal) :
            internal(set_internal) { }
        ~future_t() { }

        void *get() {
            return hclib_future_get(internal);
        }

        void *wait() {
            return hclib_future_wait(internal);
        }

        void *wait_and_get() {
            return hclib_future_wait_and_get(internal);
        }

        bool test() {
            return hclib_future_is_satisfied(internal);
        }
};

}

#endif