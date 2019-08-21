#ifndef LIST_H
#define LIST_H

#include "new.h"

typedef struct list *list_t;

extern const struct Class *list;

inline void push_back(void *self, const void *value);
inline void push_front(void *self, const void *value);
inline void * pop_back(void *self);
inline void * pop_front(void *self);
inline void insert(void *self, const void *value, const size_t position);
inline void * at(const void *self, const size_t position);
inline void in(void *self, const void *value, const size_t position);
inline void * begin(const void *self);
inline void * end(const void *self);
inline void * next(const void *iterator);
inline void * prev(const void *iterator);



#endif
