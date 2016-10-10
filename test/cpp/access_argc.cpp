#include <hclib.hpp>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    hclib::launch([=] {
        printf("I see argc = %d, argv contains %s\n", argc, argv[0]);
        assert(argc == 1);
        assert(strcmp(argv[0], "./access_argc") == 0);
        printf("Check results: OK\n");
    });
    return 0;
}
