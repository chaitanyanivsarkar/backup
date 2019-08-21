#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "new.h"



void * new(const struct Class *class, ...) {
    assert(class);

    void * object = malloc(class->size);

    *(const struct Class **)object = class;
    
    if (class->ctor) {
        va_list ap;
        va_start(ap, class);
        object = class->ctor(object, &ap);
        va_end(ap);
    }

    return object;
}

void delete(void *self) {
    struct Class **class = (struct Class **)self;
    assert(class);
    if (*class && (*class)->dtor) {
       self = (*class)->dtor(self);
    }

    free(self);
}


#ifdef __MAIN__

int main(void) {
    return 0;
}

#endif
