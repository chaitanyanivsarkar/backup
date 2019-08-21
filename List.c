#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "new.h"
#include "List.h"




struct node {
    void *data;
    struct node *next;
    struct node *prev;
};

struct list {
    struct Class *class;   
    size_t data_size;
    size_t len;
    struct node *begin;
    struct node *last;
};



static void * list_ctor(void *_self, va_list *ap) {
    struct list *self = (struct list *)_self;
    const size_t len = va_arg(*ap, const size_t);
    self->data_size = va_arg(*ap, size_t);
    
    if (len > 0) {
        struct node *n = (struct node *)malloc(len * sizeof(struct node));

        for (size_t i = 0; i < len; ++i) {
            n[i].data = malloc(self->data_size);
        }
    
        if (len > 1) {
            n[0].next = &n[1];
            n[1].prev = &n[0];
        }
        for (size_t i = 1; i < len-1; ++i) {
            n[i].next = &n[i+1];
            n[i+1].prev = &n[i];
        }
    
        self->begin = n;
        self->last = n + len-1;
        self->len = len;
    } else {
        self->begin = NULL;
        self->last = NULL;
        self->len = len;
    }

    return self;
}

static void * list_dtor(void *_self) {
    struct list *self = (struct list *)_self;    
    struct node *head = self->begin, *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }

    return self;
}

static void * list_clone(const void *_self) {
    /*const struct list *self = (struct list *)_self;
    struct list *clone = (struct list *)malloc(sizeof(struct list));

    clone = list_ctor(clone, self->len, self->data_size);
    struct node *head_self = self->begin;
    struct node *head_clone = clone->begin;

    while(head_self != NULL) {
        head_clone->data = malloc(self->data_size);
        for (size_t i = 0; i < self->data_size; ++i) {
            *(char *)(head_clone->data + i) = *(char *)(head_self->data + i);
        }
        
        head_self = head_self->next;
        head_clone = head_clone->next;
    }

    return clone;*/

    return NULL;    
}

static char list_compare(const void *_self, const void *_other) {
    const struct list *self = (struct list *)_self;
    const struct list *other = (struct list *)_other;
    
    if (self->len != other->len || self->data_size != other->data_size)
        return 1;
    
    struct node *head_self = self->begin, *head_clone = self->begin;

    while (head_self) {
        for (size_t i = 0; i < self->data_size; ++i) {
            if (*((char *)head_clone->data + i)
                == *((char *)head_self->data + i)) 
                return 1;    
        }

        head_self = head_self->next;
        head_clone = head_clone->next;
    }

    return 0;
}



static void list_push_back(void *_self, const void *value) {
    struct list *self = (struct list *)_self;    
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->data = malloc(self->data_size);

    for (size_t i = 0; i < self->data_size; ++i) {
        *((char *)temp->data + i) = *((char *)value + i);
    }
    
    if (self->len > 0) {
        self->last->next = temp;
        temp->prev = self->last;
    
        self->last = temp;
    } else {
        self->begin = temp;
        self->last = temp;
    }


    ++self->len;
}

static void list_push_front(void *_self, const void *value) {
    struct list *self = (struct list *)_self;    
    struct node *temp = (struct node *)malloc(self->data_size);
    
    for (size_t i = 0; i < self->data_size; ++i) {
        *((char *)temp->data + i) = *((char *)value + i);
    }
    if (self->len > 0) {
        temp->next = self->begin;
        self->begin->prev = temp;
    
        self->begin = temp;
    } else {
        self->begin = temp;
        self->last = temp;
    }

    ++self->len;
}

static void * list_pop_back(void *_self) {
    struct list *self = (struct list *)_self;
    
    if (self->len == 0) return NULL;
    void * value = malloc(self->data_size);

    for (size_t i = 0; i < self->data_size; ++i) {
        *((char *)value + i) = *((char *)self->last->data + i);
    }
    struct node *tmp = self->last;
    
    self->last = self->last->prev;
    if (self->last->next) self->last->next = NULL;
    free(tmp);

    --self->len;

    return value;
}

static void * list_pop_front(void *_self) {
    struct list *self = (struct list *)_self;
    void * value = malloc(self->data_size);

    for (size_t i = 0; i < self->data_size; ++i) {
        *((char *)value + i) = *((char *)self->begin->data + i);
    }
    struct node *tmp = self->begin;

    self->begin = self->begin->next;
    if (self->begin->prev) self->begin->prev = NULL;
    free(tmp);
    
    --self->len;

    return value;
}

static void * list_at(const void *_self, const size_t position) {
    struct list *self = (struct list *)_self;
    
    assert(position < self->len);

    void * value = malloc(self->data_size);
    struct node *head = self->begin;

    for(size_t i = 0; i != position; ++i) {
        head = head->next;
    }
    
    for (size_t i = 0; i < self->data_size; ++i) {
        *((char *)value + i) = *((char *)head->data + i);
    }

    return value;
}

static void list_insert(void *_self, const void *value, const size_t position) {
    struct list *self = (struct list *)_self;
    
    assert(position < self->len);

    struct node *temp = malloc(self->data_size);
    struct node *head = self->begin;
    
    for (size_t i = 0; i < self->data_size; ++i) {
        *((char *)temp + i) = *((char *)value + i);
    }
    
    for(size_t i = 0; i < position; ++i) {
        head = head->next;
    }
    
    struct node *temp1 = head->next;
    
    head->next = temp;
    temp->prev = head;
    temp->next = temp1;
    temp1->prev = temp;
}

static void list_in(void *_self, const void *value, const size_t position) {
    struct list *self = (struct list *)_self;
    
    assert(position < self->len);
    struct node *head = self->begin;

    for(size_t i = 0; i != position; ++i) {
        head = head->next;
    }
    
    for (size_t i = 0; i < self->data_size; ++i) {
        *((char *)head->data + i) = *((char *)value + i);
    }
}

static void * list_begin(const void *self) {
    return ((struct list *)self)->begin;
}

static void * list_end(const void *self) {
    return NULL;
}

static void * list_next(const void *iterator) {
    return ((struct node *)iterator)->next;
}

static void * list_prev(const void *iterator) {
    return ((struct node *)iterator)->prev;
}



static const struct Class list_Class = {
    .size=sizeof(struct list),
    .ctor=list_ctor,
    .dtor=list_dtor,
    .clone=list_clone,
    .compare=list_compare,
    .push_back=list_push_back,
    .push_front=list_push_front,
    .pop_back=list_pop_back,
    .pop_front=list_pop_front,
    .insert=list_insert,
    .at=list_at,
    .in=list_in,
    .begin=list_begin,
    .end=list_end,
    .next=list_next,
    .prev=list_prev
};

const struct Class *List = &list_Class;

int main() {
    list_t l = (list_t) new(List, (const size_t)0, sizeof(int));
    int a = 10;
    //push_front(l, (void *)&a);
    push_back(l, (void *)&a);
    //for (int i = 0; i < 10; ++i) {push_front(l, (void *)&i);}

    return 0;
}



void push_back(void *_self, const void *value) {
    struct class *self = (struct class *)_self;    
    self->push_back(_self, value);
}

void push_front(void *_self, const void *value) {
    struct class *self = (struct class *)_self;
    self->push_front(_self, value);
}

void pop_back(void *_self) {
    struct class *self = (struct class *)_self;
    self->pop_back(_self);
}

void pop_front(void *_self) {
    struct class *self = (struct class *)_self;
    self->pop_front(_self);
}

void insert(void *_self, const void *value, const size_t position) {
    struct class *self = (struct class *)_self;
    self->pop_front(_self);
}

void * at(const void *_self, const size_t position) {
    struct class *self = (struct class *)_self;
    self->pop_front(_self);
}

void in(void *_self, const void *value, const size_t position) {
    struct list *self = (struct list *)_self;
    self->class->in(self, value, position);
}
