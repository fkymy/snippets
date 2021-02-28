#include <unistd.h>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cmath>

/* name this executable filter_multiple */

int main(int argc, char** argv) {
    assert(argc >= 2);
    long p = strtol(argv[1], nullptr, 10);
    assert(p > 1);

    long value;
    while (scanf("%ld", &value) == 1) {
        if (value % p != 0) {
            printf("%ld\n", value);
        }
    }
}
