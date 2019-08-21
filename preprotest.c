#define EMPTY()
#define DEFER(x) x EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EXPAND(...) __VA_ARGS__

#define CREATE() #define ABC

#define A() 3

A();
DEFER(A)();
EXPAND(DEFER(A)())
CREATE()
