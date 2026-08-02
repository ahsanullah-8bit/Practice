/*
    Compile‑Time static_assert Replacement (C99‑style)

    Features: negative‑size array trick, #define, inline.
    Goal: A macro STATIC_ASSERT(cond) that causes a compile error if cond is false, mimicking C11’s _Static_assert.
    Plan:
    · Design the macro using sizeof(char[1-2*!(cond)]).
    · Write several test cases that should pass or fail.
    · Package the macro in a header and use it to check type sizes (sizeof(int)>=4).

    Notes:
        * Well, the question solves it already, by just guiding me. lol.
        * Some conversations:
            https://www.reddit.com/r/programming/comments/724ln/a_compile_time_assert_hack_for_c/
            https://curiouser.cheshireeng.com/2014/08/19/c-compile-time-assert/
            https://stackoverflow.com/questions/3385515/static-assert-in-c
*/

#define STATIC_ASSERT(cond) \
    sizeof(char[1-2*!(cond)])

// As per the conversations, an improved version would be
#define STATIC_ASSERT_CONCAT(a, b) a##b
#define STATIC_ASSERT_NAME(a, b) STATIC_ASSERT_CONCAT(a, b)
#define STATIC_ASSERT2(cond) \
    typedef char STATIC_ASSERT_NAME(static_assertion_, __LINE__)[(cond)?1:-1]

int main(void) {
    /*
        Evaluation (example):
            STATIC_ASSERT((sizeof(int) >= 6)
            sizeof(char[1 - 2 * !(sizeof(int) >= 6)])
            sizeof(char[1 - 2 * !(4 >= 6)])
            sizeof(char[1 - 2 * !(false)])
            sizeof(char[1 - 2 * true])
            sizeof(char[1 - 2])
            sizeof(char[-1]), which compilers don't allow.
    */
    STATIC_ASSERT(sizeof(int)>=4);

    STATIC_ASSERT2(sizeof(int)==4);

    return 0;
}