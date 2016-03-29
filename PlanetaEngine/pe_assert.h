#pragma once

#ifdef NDEBUG    // NDEBUG��`��(RELESE�R���p�C����)�͉������Ȃ�.
#define pe_assert(expr, err_str)
#else            // NDEBUG����V��(DEBUG�R���p�C����)�͏������`�F�b�N.
#define pe_assert(expr, err_str)   do {                                           \
            if (!(expr)) {                                             \
                printf("Assertion failed: %s, file %s, line %d\n"   \
                       , err_str, __FILE__, __LINE__);                   \
                exit(1); /*abort();*/                               \
            }                                                       \
        } while (0)
#endif
