#pragma once

#ifdef NDEBUG    // NDEBUG定義時(RELESEコンパイル時)は何もしない.
#define pe_assert(expr, err_str)
#else            // NDEBUG未定儀時(DEBUGコンパイル時)は条件をチェック.
#define pe_assert(expr, err_str)   do {                                           \
            if (!(expr)) {                                             \
                printf("Assertion failed: %s, file %s, line %d\n"   \
                       , err_str, __FILE__, __LINE__);                   \
                exit(1); /*abort();*/                               \
            }                                                       \
        } while (0)
#endif
