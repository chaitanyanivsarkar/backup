#ifndef NEW_H
#define NEW_H

#include <stdlib.h>
#include <stdarg.h>

struct Class {
    size_t size;
    void * (*ctor)(void *self, va_list *varargs);
    void * (*dtor)(void *self);
    void * (*clone)(const void *self);
    char (*compare)(const void *self, const void *other);
    void (*push_back)(void *self, const void *value);
    void (*push_front)(void *self, const void *value);
    void * (*pop_back)(void *self);
    void * (*pop_front)(void *self);
    void (*insert)(void *self, const void *value, const size_t position);
    void * (*at)(const void *self, const size_t position);
    void (*in)(void *self, const void *value, const size_t position);
    void * (*begin)(const void *self);
    void * (*end)(const void *self);
    void * (*next)(const void *iterator);
    void * (*prev)(const void *iterator);
};


void * new(const struct Class *class, ...);
void delete(void *self);
/*void * clone(const void *self);
char compare(const void *self, const void *other);
size_t size_of(const void *self);
*/
#endif
