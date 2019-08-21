#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>

#include "new.h"

/*Vector H file*/
typedef struct vector *vec_t;


#define DEFAULT_SIZE 16
#define push_back(self, value, type) do {type tmp = value;\
        vec_push_back(self, &tmp);} while(0)

struct vector {
    struct Class *class;
    void * array;
    size_t data_size;
    size_t len;
    size_t size;
};

static void * vec_ctor(void *_self, va_list *ap) {
    vec_t self = (vec_t) _self;
    size_t len = va_arg(*ap, size_t);
    size_t data_size = va_arg(*ap, size_t);
    
    assert(data_size != 0);

    void * array = len ? malloc(len * data_size) : malloc(DEFFAULT_SIZE);

    self->data_size = data_size;
    self->array = array;
    self->len = len ? len : DEFAULT_SIZE;
    self->size = 0;
    
    return (void *)self;
}
static void * vec_dtor(void *_self) {
    vec_t self = (vec_t) _self;
    free(self->array);
    return (void *)self;
}
static void vec_push_back(void *_self, const void * value) {
    vec_t self = (vec_t)_self;
    
    if (self->size >= self->len) {
        //vec_resize();
    }

    intptr_t dest = (intptr_t)self->array + self->size*self->data_size;
    memcpy((void *)dest, value, self->data_size);
    ++self->size;
}
/*static void vec_push_front(void *_self, const void * value) {}
static void * vec_pop_front(void *_self, const void * value) {}
static void * vec_pop_back(void *_self) {}
static void * vec_resize(void *_self, const size_t size) {}
static void * vec_at(const void *_self, const size_t size) {}
static void vec_in(void *_self, const void * value, const size_t position) {}
static void vec_insert(void *_self, const void * value, const size_t position) {}
static void * vec_begin(const void *_self) {}
static void * vec_end(const void *_self) {}
static void * vec_next(const void *iterator) {}
static void * vec_prev(const void *iterator) {}
*/


static const struct Class Vector_class = {
    .size=sizeof(struct vector),
    .ctor=vec_ctor,
    .dtor=vec_dtor
};

const struct Class *Vector = &Vector_class;

int main(void) {
    vec_t a = new(Vector, 10, sizeof(int));
    push_back(a, 2, int);
    printf("%d\n", *(int *)a->array);
    //delete(a);
}
