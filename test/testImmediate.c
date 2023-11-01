#include <assert.h>
#include <stdlib.h>
#include "../src/include/Immediate.h"

int main(void) {
    char *test = immediateUnshiftedAdd(0x0);
    assert(test[0] == '\x02');
    assert(test[1] == '\x00');
    free(test);

    test = immediateUnshiftedAdd(0x3);
    assert(test[0] == '\x0e');
    assert(test[1] == '\x00');

    free(test);

    test = immediateUnshiftedAdd(0x6);
    assert(test[0] == '\x1a');
    assert(test[1] == '\x00');

    free(test);

    test = immediateUnshiftedAdd(0x441);
    assert(test[0] == '\x06');
    assert(test[1] == '\x11');

    free(test);

    test = immediateUnshiftedAdd(0x580);
    assert(test[0] == '\x02');
    assert(test[1] == '\x16');

    free(test);
}