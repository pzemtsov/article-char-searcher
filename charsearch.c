#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <xmmintrin.h>
#include <pmmintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>


#ifdef _MSC_VER
#include <intrin.h>
#endif

typedef void* Searcher (const void* ptr, int ch, size_t count);

#ifdef __GNUC__
extern void* index_scas(const void* ptr, int ch, size_t count);
#endif

size_t bsf(uint32_t bits)
{
#ifdef __GNUC__
    return __builtin_ctz(bits);
#else
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward(&index, bits);
    return index;
#else
#error Undefined BSF
#endif
#endif
}

size_t bsf_64(uint64_t bits)
{
#ifdef __GNUC__
    return __builtin_ctzll(bits);
#else
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward64(&index, bits);
    return index;
#else
#error Undefined BSF
#endif
#endif
}

void* null_index(const void* ptr, int ch, size_t count)
{
    return NULL;
}

void* simple_index(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    for (size_t i = 0; i < count; i++) {
        if (p[i] == (char) ch) return (void*) (p + i);
    }
    return NULL;
}

void* simple_ptr(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    while (count) {
        if (*p == (char) ch) return (void*) p;
        ++p;
        --count;
    }
    return NULL;
}

void* simple_ptr2(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    const char * q = p + count;
    for (; p != q; p++) {
        if (*p == (char)ch) return (void*)p;
    }
    return NULL;
}

void* simple_dword(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    while (count >= 4) {
        uint32_t v;
        memcpy(&v, p, 4);
        for (size_t i = 0; i < 4; i++) {
            if ((char)v == (char)ch) return (void *)(p + i);
            v >>= 8;
        }
        p += 4;
        count -= 4;
    }
    while (count) {
        if (*p == ch) return (void*)p;
        ++p;
        --count;
    }
    return NULL;
}

void* dword(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    uint32_t value = ch * 0x01010101L;
    while (count >= 8) {
        uint32_t v;
        memcpy(&v, p, 8);
        v ^= value;
        if ((v - 0x01010101L) & ~v & 0x80808080L) {
            for (size_t i = 0; i < 4; i++) {
                if (!(char)v) return (void *)(p + i);
                v >>= 8;
            }
        }
        p += 4;
        count -= 4;
    }
    while (count) {
        if (*p == ch) return (void*)p;
        ++p;
        --count;
    }
    return NULL;
}

void* dword_bsf(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    uint32_t value = ch * 0x01010101L;
    while (count >= 4) {
        uint32_t v;
        memcpy(&v, p, 8);
        v ^= value;
        uint32_t bits = (v - 0x01010101UL) & ~v & 0x80808080UL;
        if (bits) {
            return (void *)(p + (bsf(bits) / 8));
        }
        p += 4;
        count -= 4;
    }
    while (count) {
        if (*p == ch) return (void*)p;
        ++p;
        --count;
    }
    return NULL;
}

void* simple_qword(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    while (count >= 8) {
        uint64_t v;
        memcpy(&v, p, 8);
        for (size_t i = 0; i < 8; i++) {
            if ((char)v == (char)ch) return (void *)(p + i);
            v >>= 8;
        }
        p += 8;
        count -= 8;
    }
    return simple_dword(p, ch, count);
}

void* qword(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    uint64_t value = ch * 0x0101010101010101LL;
    while (count >= 8) {
        uint64_t v;
        memcpy(&v, p, 8);
        v ^= value;
        if ((v - 0x0101010101010101LL) & ~v & 0x8080808080808080LL) {
            for (size_t i = 0; i < 8; i++) {
                if (!(char)v) return (void *)(p + i);
                v >>= 8;
            }
        }
        p += 8;
        count -= 8;
    }
    return dword(p, ch, count);
}

void* qword_bsf(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    uint64_t value = ch * 0x0101010101010101LL;
    while (count >= 8) {
        uint64_t v;
        memcpy(&v, p, 8);
        v ^= value;
        uint64_t bits = (v - 0x01010101010101010ULL) & ~v & 0x8080808080808080LL;
        if (bits) {
            return (void *)(p + (bsf_64(bits) / 8));
        }
        p += 8;
        count -= 8;
    }
    return dword_bsf(p, ch, count);
}

void* sse(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    __m128i value = _mm_set1_epi8((char)ch);
    while (count >= 16) {
        __m128i v = _mm_loadu_si128((__m128i *) p);
        int eq_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, value));
        if (eq_mask) {
            return (void *)(p + bsf(eq_mask));
        }
        p += 16;
        count -= 16;
    }
    return qword_bsf(p, ch, count);
}

void* sse_unlimited(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    __m128i value = _mm_set1_epi8((char)ch);
    for (ptrdiff_t cnt = (ptrdiff_t)count; cnt > 0; cnt -= 16, p += 16) {
        __m128i v = _mm_loadu_si128((__m128i *) p);
        int eq_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, value));
        if (eq_mask) {
            size_t offset = bsf(eq_mask);
            return offset >= (size_t) cnt ? NULL : (void *)(p + offset);
        }
    }
    return NULL;
}

void* sse_aligned(const void* ptr, int ch, size_t count)
{
    if (count < 16) return qword_bsf(ptr, ch, count);
        
    const char * p = (const char *)ptr;
    __m128i value = _mm_set1_epi8((char)ch);

    size_t align = (size_t)p & 0x0F;
    if (align > 0) {
        align = 16 - align;
        __m128i v = _mm_loadu_si128((__m128i *) p);
        int eq_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, value));
        if (eq_mask) {
            return (void *)(p + bsf(eq_mask));
        }
        p += align;
        count -= align;
    }
    while (count >= 16) {
        __m128i v = _mm_load_si128((__m128i *) p);
        int eq_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, value));
        if (eq_mask) {
            return (void *)(p + bsf(eq_mask));
        }
        p += 16;
        count -= 16;
    }
    return qword_bsf(p, ch, count);
}

void* sse_aligned64(const void* ptr, int ch, size_t count)
{
    if (count < 16) return qword_bsf(ptr, ch, count);

    const char * p = (const char *)ptr;
    __m128i value = _mm_set1_epi8((char)ch);

    size_t align = (size_t)p & 0x0F;
    if (align > 0) {
        align = 16 - align;
        __m128i v = _mm_loadu_si128((__m128i *) p);
        int eq_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, value));
        if (eq_mask) {
            return (void *)(p + bsf(eq_mask));
        }
        p += align;
        count -= align;
    }
    while (count >= 64) {
        __m128i v0 = _mm_load_si128((__m128i *) p);
        __m128i v1 = _mm_load_si128((__m128i *) (p + 16));
        __m128i v2 = _mm_load_si128((__m128i *) (p + 32));
        __m128i v3 = _mm_load_si128((__m128i *) (p + 48));

        __m128i m0 = _mm_cmpeq_epi8(v0, value);
        __m128i m1 = _mm_cmpeq_epi8(v1, value);
        __m128i m2 = _mm_cmpeq_epi8(v2, value);
        __m128i m3 = _mm_cmpeq_epi8(v3, value);

        __m128i max_val = _mm_max_epu8(_mm_max_epu8(m0, m1), _mm_max_epu8(m2, m3));

        int eq_mask = _mm_movemask_epi8(max_val);
        if (eq_mask) {
            eq_mask = _mm_movemask_epi8(m0);
            if (eq_mask) {
                return (void *)(p + bsf(eq_mask));
            }
            eq_mask = _mm_movemask_epi8(m1);
            if (eq_mask) {
                return (void *)(p + bsf(eq_mask) + 16);
            }
            eq_mask = _mm_movemask_epi8(m2);
            if (eq_mask) {
                return (void *)(p + bsf(eq_mask) + 32);
            }
            eq_mask = _mm_movemask_epi8(m3);
            return (void *)(p + bsf(eq_mask) + 48);
        }
        p += 64;
        count -= 64;
    }
    while (count >= 16) {
        __m128i v = _mm_load_si128((__m128i *) p);
        int eq_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, value));
        if (eq_mask) {
            return (void *)(p + bsf(eq_mask));
        }
        p += 16;
        count -= 16;
    }
    return qword_bsf(p, ch, count);
}

void* avx(const void* ptr, int ch, size_t count)
{
    const char * p = (const char *)ptr;
    __m256i value = _mm256_set1_epi8((char)ch);
    while (count >= 32) {
        __m256i v = _mm256_loadu_si256((__m256i *) p);
        int eq_mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(v, value));
        if (eq_mask) {
            return (void *)(p + bsf_64(eq_mask));
        }
        p += 32;
        count -= 32;
    }
    return sse(p, ch, count);
}

void* avx_aligned(const void* ptr, int ch, size_t count)
{
    if (count < 32) return sse(ptr, ch, count);

    const char * p = (const char *)ptr;
    __m256i value = _mm256_set1_epi8((char)ch);

    size_t align = (size_t)p & 0x1F;
    if (align > 0) {
        align = 32 - align;
        __m256i v = _mm256_loadu_si256((__m256i *) p);
        int eq_mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(v, value));
        if (eq_mask) {
            return (void *)(p + bsf_64(eq_mask));
        }
        p += align;
        count -= align;
    }
    while (count >= 32) {
        __m256i v = _mm256_load_si256((__m256i *) p);
        int eq_mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(v, value));
        if (eq_mask) {
            return (void *)(p + bsf_64(eq_mask));
        }
        p += 32;
        count -= 32;
    }
    return sse(p, ch, count);
}

void* avx_aligned64(const void* ptr, int ch, size_t count)
{
    if (count < 32) return sse(ptr, ch, count);

    const char * p = (const char *)ptr;
    __m256i value = _mm256_set1_epi8((char)ch);

    size_t align = (size_t)p & 0x1F;
    if (align > 0) {
        align = 32 - align;
        __m256i v = _mm256_loadu_si256((__m256i *) p);
        int eq_mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(v, value));
        if (eq_mask) {
            return (void *)(p + bsf_64(eq_mask));
        }
        p += align;
        count -= align;
    }
    while (count >= 128) {
        __m256i v0 = _mm256_load_si256((__m256i *) p);
        __m256i v1 = _mm256_load_si256((__m256i *) (p + 32));
        __m256i v2 = _mm256_load_si256((__m256i *) (p + 64));
        __m256i v3 = _mm256_load_si256((__m256i *) (p + 96));

        __m256i m0 = _mm256_cmpeq_epi8(v0, value);
        __m256i m1 = _mm256_cmpeq_epi8(v1, value);
        __m256i m2 = _mm256_cmpeq_epi8(v2, value);
        __m256i m3 = _mm256_cmpeq_epi8(v3, value);

        __m256i max_val = _mm256_max_epu8(_mm256_max_epu8(m0, m1), _mm256_max_epu8(m2, m3));

        int eq_mask = _mm256_movemask_epi8(max_val);
        if (eq_mask) {
            eq_mask = _mm256_movemask_epi8(m0);
            if (eq_mask) {
                return (void *)(p + bsf_64(eq_mask));
            }
            eq_mask = _mm256_movemask_epi8(m1);
            if (eq_mask) {
                return (void *)(p + bsf_64(eq_mask) + 32);
            }
            eq_mask = _mm256_movemask_epi8(m2);
            if (eq_mask) {
                return (void *)(p + bsf_64(eq_mask) + 64);
            }
            eq_mask = _mm256_movemask_epi8(m3);
            return (void *)(p + bsf_64(eq_mask) + 96);
        }
        p += 128;
        count -= 128;
    }
    while (count >= 32) {
        __m256i v = _mm256_load_si256((__m256i *) p);
        int eq_mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(v, value));
        if (eq_mask) {
            return (void *)(p + bsf_64(eq_mask));
        }
        p += 32;
        count -= 32;
    }
    return sse(p, ch, count);
}

#define TEST16_cmp_known_nz(p, cmp) \
    do {\
        int eq_mask = _mm_movemask_epi8(cmp);\
        return (void *)((p)+bsf(eq_mask));\
    } while (0)

#define TEST16_cmp(p, cmp) \
    do {\
        int eq_mask = _mm_movemask_epi8(cmp);\
        if (eq_mask) {\
            return (void *)((p)+bsf(eq_mask));\
        }\
    } while (0)

#define TEST16_v(p, v) TEST16_cmp (p, _mm_cmpeq_epi8(v, value))
#define TEST16(p) TEST16_v (p, _mm_load_si128((__m128i *) (p)))

#define TEST16_v_count(p, v) \
    do {\
        int eq_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, value));\
        if (eq_mask) {\
            size_t pos = bsf(eq_mask);\
            if (pos >= count) return NULL;\
            return (void *)(p + pos);\
        }\
    } while (0)

#define TEST16_count(p) TEST16_v_count (p, _mm_load_si128((__m128i *) (p)))

void* sse_memchr(const void* ptr, int ch, size_t count)
{
    if (count == 0) return NULL;

    const char * p = (const char*)ptr;

    __m128i value = _mm_set1_epi8((char)ch);
    size_t align = (size_t) p & 0x3F;

    if (align <= 48) {
        TEST16_v_count(p, _mm_loadu_si128((__m128i *) p));
        if (count <= 16) return NULL;
        count -= 16;
        p += 16;

        align &= 0x0F;
        p -= align;
        count += align;
    } else {
        align &= 0x0F;
        p -= align;
        __m128i v = _mm_load_si128((__m128i *) p);
        unsigned eq_mask = (unsigned) _mm_movemask_epi8(_mm_cmpeq_epi8(v, value));
        eq_mask >>= align;
        if (eq_mask) {
            size_t pos = bsf(eq_mask);
            if (pos >= count) return NULL;
            return (void *)(p + pos + align);
        }
        count += align;
        if (count <= 16) return NULL;
        p += 16;
        count -= 16;
    }
    if (count > 64) {
        TEST16(p);
        TEST16(p + 16);
        TEST16(p + 32);
        TEST16(p + 48);
        p += 64;
        count -= 64;

        if (count > 64) {
            align = (size_t) p & 0x3F;
            if (align) {
                TEST16(p);
                TEST16(p + 16);
                TEST16(p + 32);
                TEST16(p + 48);
                p += 64;
                count -= 64;
                p -= align;
                count += align;
            }

            for (; count > 64; count -= 64, p += 64) {

                __m128i v0 = _mm_load_si128((__m128i *) p);
                __m128i v1 = _mm_load_si128((__m128i *) (p + 16));
                __m128i v2 = _mm_load_si128((__m128i *) (p + 32));
                __m128i v3 = _mm_load_si128((__m128i *) (p + 48));

                __m128i cmp0 = _mm_cmpeq_epi8(v0, value);
                __m128i cmp1 = _mm_cmpeq_epi8(v1, value);
                __m128i cmp2 = _mm_cmpeq_epi8(v2, value);
                __m128i cmp3 = _mm_cmpeq_epi8(v3, value);

                __m128i max_val = _mm_max_epu8(_mm_max_epu8(cmp0, cmp1), _mm_max_epu8(cmp2, cmp3));
                int eq_mask = _mm_movemask_epi8(max_val);
                if (eq_mask == 0) continue;

                TEST16_cmp(p, cmp0);
                TEST16_cmp(p + 16, cmp1);
                TEST16_cmp(p + 32, cmp2);
                TEST16_cmp_known_nz(p + 48, cmp3); // returns from function
            }
        }
    }
    if (count > 32) {
        TEST16(p);
        TEST16(p + 16);
        count -= 32;
        p += 32;
    }

    TEST16_count(p);
    if (count > 16); {
        count -= 16;
        TEST16_count(p + 16);
    }
    return NULL;
}

void* sse_memchr2(const void* ptr, int ch, size_t count)
{
    if (count == 0) return NULL;

    const char * p = (const char*)ptr;

    __m128i value = _mm_set1_epi8((char)ch);
    size_t align = (size_t)p & 0xFFF;

    if (align <= 4096 - 16) {
        TEST16_v_count(p, _mm_loadu_si128((__m128i *) p));
        if (count <= 16) return NULL;
        count -= 16;
        p += 16;

        align &= 0x0F;
        p -= align;
        count += align;
    }
    else {
        align &= 0x0F;
        p -= align;
        __m128i v = _mm_load_si128((__m128i *) p);
        unsigned eq_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, value));
        eq_mask >>= align;
        if (eq_mask) {
            size_t pos = bsf(eq_mask);
            if (pos >= count) return NULL;
            return (void *)(p + pos + align);
        }
        count += align;
        if (count <= 16) return NULL;
        p += 16;
        count -= 16;
    }
    for (; count >= 64; count -= 64, p += 64) {

        __m128i v0 = _mm_load_si128((__m128i *) p);
        __m128i v1 = _mm_load_si128((__m128i *) (p + 16));
        __m128i v2 = _mm_load_si128((__m128i *) (p + 32));
        __m128i v3 = _mm_load_si128((__m128i *) (p + 48));

        __m128i cmp0 = _mm_cmpeq_epi8(v0, value);
        __m128i cmp1 = _mm_cmpeq_epi8(v1, value);
        __m128i cmp2 = _mm_cmpeq_epi8(v2, value);
        __m128i cmp3 = _mm_cmpeq_epi8(v3, value);

        __m128i max_val = _mm_max_epu8(_mm_max_epu8(cmp0, cmp1), _mm_max_epu8(cmp2, cmp3));
        int eq_mask = _mm_movemask_epi8(max_val);
        if (eq_mask == 0) continue;

        TEST16_cmp(p, cmp0);
        TEST16_cmp(p + 16, cmp1);
        TEST16_cmp(p + 32, cmp2);
        TEST16_cmp_known_nz(p + 48, cmp3); // returns from function
    }
    if (count >= 32) {
        TEST16(p);
        TEST16(p + 16);
        count -= 32;
        p += 32;
    }
    if (count >= 16) {
        TEST16(p);
        count -= 16;
        p += 16;
    }
    if (count > 0) {
        TEST16_count(p);
    }
    return NULL;
}

#ifdef __GNUC__

#pragma GCC push_options
#pragma GCC optimize ("no-unroll-loops")
void* sse_memchr_nounroll(const void* ptr, int ch, size_t count)
{
    if (count == 0) return NULL;

    const char * p = (const char*)ptr;

    __m128i value = _mm_set1_epi8((char)ch);
    size_t align = (size_t)p & 0x3F;

    if (align <= 48) {
        TEST16_v_count(p, _mm_loadu_si128((__m128i *) p));
        if (count <= 16) return NULL;
        count -= 16;
        p += 16;

        align &= 0x0F;
        p -= align;
        count += align;
    }
    else {
        align &= 0x0F;
        p -= align;
        __m128i v = _mm_load_si128((__m128i *) p);
        unsigned eq_mask = (unsigned)_mm_movemask_epi8(_mm_cmpeq_epi8(v, value));
        eq_mask >>= align;
        if (eq_mask) {
            size_t pos = bsf(eq_mask);
            if (pos >= count) return NULL;
            return (void *)(p + pos + align);
        }
        count += align;
        if (count <= 16) return NULL;
        p += 16;
        count -= 16;
    }
    if (count > 64) {
        TEST16(p);
        TEST16(p + 16);
        TEST16(p + 32);
        TEST16(p + 48);
        p += 64;
        count -= 64;

        if (count > 64) {
            align = (size_t)p & 0x3F;
            if (align) {
                TEST16(p);
                TEST16(p + 16);
                TEST16(p + 32);
                TEST16(p + 48);
                p += 64;
                count -= 64;
                p -= align;
                count += align;
            }

            for (; count > 64; count -= 64, p += 64) {

                __m128i v0 = _mm_load_si128((__m128i *) p);
                __m128i v1 = _mm_load_si128((__m128i *) (p + 16));
                __m128i v2 = _mm_load_si128((__m128i *) (p + 32));
                __m128i v3 = _mm_load_si128((__m128i *) (p + 48));

                __m128i cmp0 = _mm_cmpeq_epi8(v0, value);
                __m128i cmp1 = _mm_cmpeq_epi8(v1, value);
                __m128i cmp2 = _mm_cmpeq_epi8(v2, value);
                __m128i cmp3 = _mm_cmpeq_epi8(v3, value);

                __m128i max_val = _mm_max_epu8(_mm_max_epu8(cmp0, cmp1), _mm_max_epu8(cmp2, cmp3));
                int eq_mask = _mm_movemask_epi8(max_val);
                if (eq_mask == 0) continue;

                TEST16_cmp(p, cmp0);
                TEST16_cmp(p + 16, cmp1);
                TEST16_cmp(p + 32, cmp2);
                TEST16_cmp_known_nz(p + 48, cmp3); // returns from function
            }
        }
    }
    if (count > 32) {
        TEST16(p);
        TEST16(p + 16);
        count -= 32;
        p += 32;
    }

    TEST16_count(p);
    if (count > 16); {
        count -= 16;
        TEST16_count(p + 16);
    }
    return NULL;
}

void* sse_memchr2_nounroll(const void* ptr, int ch, size_t count)
{
    if (count == 0) return NULL;

    const char * p = (const char*)ptr;

    __m128i value = _mm_set1_epi8((char)ch);
    size_t align = (size_t)p & 0xFFF;

    if (align <= 4096 - 16) {
        TEST16_v_count(p, _mm_loadu_si128((__m128i *) p));
        if (count <= 16) return NULL;
        count -= 16;
        p += 16;

        align &= 0x0F;
        p -= align;
        count += align;
    }
    else {
        align &= 0x0F;
        p -= align;
        __m128i v = _mm_load_si128((__m128i *) p);
        unsigned eq_mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, value));
        eq_mask >>= align;
        if (eq_mask) {
            size_t pos = bsf(eq_mask);
            if (pos >= count) return NULL;
            return (void *)(p + pos + align);
        }
        count += align;
        if (count <= 16) return NULL;
        p += 16;
        count -= 16;
    }
    for (; count >= 64; count -= 64, p += 64) {

        __m128i v0 = _mm_load_si128((__m128i *) p);
        __m128i v1 = _mm_load_si128((__m128i *) (p + 16));
        __m128i v2 = _mm_load_si128((__m128i *) (p + 32));
        __m128i v3 = _mm_load_si128((__m128i *) (p + 48));

        __m128i cmp0 = _mm_cmpeq_epi8(v0, value);
        __m128i cmp1 = _mm_cmpeq_epi8(v1, value);
        __m128i cmp2 = _mm_cmpeq_epi8(v2, value);
        __m128i cmp3 = _mm_cmpeq_epi8(v3, value);

        __m128i max_val = _mm_max_epu8(_mm_max_epu8(cmp0, cmp1), _mm_max_epu8(cmp2, cmp3));
        int eq_mask = _mm_movemask_epi8(max_val);
        if (eq_mask == 0) continue;

        TEST16_cmp(p, cmp0);
        TEST16_cmp(p + 16, cmp1);
        TEST16_cmp(p + 32, cmp2);
        TEST16_cmp_known_nz(p + 48, cmp3); // returns from function
    }
    if (count >= 32) {
        TEST16(p);
        TEST16(p + 16);
        count -= 32;
        p += 32;
    }
    if (count >= 16) {
        TEST16(p);
        count -= 16;
        p += 16;
    }
    if (count > 0) {
        TEST16_count(p);
    }
    return NULL;
}

#pragma GCC pop_options

#endif // #ifdef __GNUC__

#undef TEST16_cmp_known_nz
#undef TEST16_cmp
#undef TEST16_v
#undef TEST16
#undef TEST16_v_count
#undef TEST16_count

#define TEST32_cmp_known_nz(p, cmp) \
    do {\
        unsigned eq_mask = (unsigned) _mm256_movemask_epi8(cmp);\
        return (void *)((p)+bsf(eq_mask));\
    } while (0)

#define TEST32_cmp(p, cmp) \
    do {\
        unsigned eq_mask = (unsigned) _mm256_movemask_epi8(cmp);\
        if (eq_mask) {\
            return (void *)((p)+bsf(eq_mask));\
        }\
    } while (0)

#define TEST32_v(p, v) TEST32_cmp (p, _mm256_cmpeq_epi8(v, value))
#define TEST32(p) TEST32_v (p, _mm256_load_si256((__m256i *) (p)))

#define TEST32_v_count(p, v) \
    do {\
        unsigned eq_mask = (unsigned) _mm256_movemask_epi8(_mm256_cmpeq_epi8(v, value));\
        if (eq_mask) {\
            size_t pos = bsf(eq_mask);\
            if (pos >= count) return NULL;\
            return (void *)(p + pos);\
        }\
    } while (0)

#define TEST32_count(p) TEST32_v_count (p, _mm256_load_si256((__m256i *) (p)))

void* avx_memchr(const void* ptr, int ch, size_t count)
{
    if (count == 0) return NULL;

    const char * p = (const char*)ptr;

    __m256i value = _mm256_set1_epi8((char)ch);
    size_t align = (size_t)p & 0x3F;

    if (align <= 32) {
        TEST32_v_count(p, _mm256_loadu_si256((__m256i *) p));
        if (count <= 32) return NULL;
        count -= 32;
        p += 32;

        align &= 0x1F;
        p -= align;
        count += align;
    }
    else {
        align &= 0x1F;
        p -= align;
        __m256i v = _mm256_load_si256((__m256i *) p);
        unsigned eq_mask = (unsigned)_mm256_movemask_epi8(_mm256_cmpeq_epi8(v, value));
        eq_mask >>= align;
        if (eq_mask) {
            size_t pos = bsf(eq_mask);
            if (pos >= count) return NULL;
            return (void *)(p + pos + align);
        }
        count += align;
        if (count <= 32) return NULL;
        p += 32;
        count -= 32;
    }
    if (count > 64) {
        TEST32(p);
        TEST32(p + 32);
        p += 64;
        count -= 64;

        if (count > 64) {
            align = (size_t)p & 0x3F;
            if (align) {
                TEST32(p);
                TEST32(p + 32);
                p += 64;
                count -= 64;
                p -= align;
                count += align;
            }

            for (; count > 64; count -= 64, p += 64) {

                __m256i v0 = _mm256_load_si256((__m256i *) p);
                __m256i v1 = _mm256_load_si256((__m256i *) (p + 32));

                __m256i cmp0 = _mm256_cmpeq_epi8(v0, value);
                __m256i cmp1 = _mm256_cmpeq_epi8(v1, value);

                __m256i max_val = _mm256_max_epu8(cmp0, cmp1);
                unsigned eq_mask = (unsigned) _mm256_movemask_epi8(max_val);
                if (eq_mask == 0) continue;

                TEST32_cmp(p, cmp0);
                TEST32_cmp_known_nz(p + 32, cmp1); // returns from function
            }
        }
    }
    if (count > 32) {
        TEST32(p);
        count -= 32;
        p += 32;
    }
    TEST32_count(p);
    return NULL;
}

void* avx_memchr2(const void* ptr, int ch, size_t count)
{
    if (count == 0) return NULL;

    const char * p = (const char*)ptr;

    __m256i value = _mm256_set1_epi8((char)ch);
    size_t align = (size_t)p & 0xFFF;

    if (align <= 4096-32) {
        TEST32_v_count(p, _mm256_loadu_si256((__m256i *) p));
        if (count <= 32) return NULL;
        count -= 32;
        p += 32;

        align &= 0x1F;
        p -= align;
        count += align;
    }
    else {
        align &= 0x1F;
        p -= align;
        __m256i v = _mm256_load_si256((__m256i *) p);
        unsigned eq_mask = (unsigned)_mm256_movemask_epi8(_mm256_cmpeq_epi8(v, value));
        eq_mask >>= align;
        if (eq_mask) {
            size_t pos = bsf(eq_mask);
            if (pos >= count) return NULL;
            return (void *)(p + pos + align);
        }
        count += align;
        if (count <= 32) return NULL;
        p += 32;
        count -= 32;
    }
    for (; count >= 64; count -= 64, p += 64) {

        __m256i v0 = _mm256_load_si256((__m256i *) p);
        __m256i v1 = _mm256_load_si256((__m256i *) (p + 32));

        __m256i cmp0 = _mm256_cmpeq_epi8(v0, value);
        __m256i cmp1 = _mm256_cmpeq_epi8(v1, value);

        __m256i max_val = _mm256_max_epu8(cmp0, cmp1);
        unsigned eq_mask = (unsigned)_mm256_movemask_epi8(max_val);
        if (eq_mask == 0) continue;

        TEST32_cmp(p, cmp0);
        TEST32_cmp_known_nz(p + 32, cmp1); // returns from function
    }
    if (count > 32) {
        TEST32(p);
        count -= 32;
        p += 32;
    }
    TEST32_count(p);
    return NULL;
}

#ifdef __GNUC__

#pragma GCC push_options
#pragma GCC optimize ("no-unroll-loops")

void* avx_memchr_nounroll(const void* ptr, int ch, size_t count)
{
    if (count == 0) return NULL;

    const char * p = (const char*)ptr;

    __m256i value = _mm256_set1_epi8((char)ch);
    size_t align = (size_t)p & 0x3F;

    if (align <= 32) {
        TEST32_v_count(p, _mm256_loadu_si256((__m256i *) p));
        if (count <= 32) return NULL;
        count -= 32;
        p += 32;

        align &= 0x1F;
        p -= align;
        count += align;
    }
    else {
        align &= 0x1F;
        p -= align;
        __m256i v = _mm256_load_si256((__m256i *) p);
        unsigned eq_mask = (unsigned)_mm256_movemask_epi8(_mm256_cmpeq_epi8(v, value));
        eq_mask >>= align;
        if (eq_mask) {
            size_t pos = bsf(eq_mask);
            if (pos >= count) return NULL;
            return (void *)(p + pos + align);
        }
        count += align;
        if (count <= 32) return NULL;
        p += 32;
        count -= 32;
    }
    if (count > 64) {
        TEST32(p);
        TEST32(p + 32);
        p += 64;
        count -= 64;

        if (count > 64) {
            align = (size_t)p & 0x3F;
            if (align) {
                TEST32(p);
                TEST32(p + 32);
                p += 64;
                count -= 64;
                p -= align;
                count += align;
            }

            for (; count > 64; count -= 64, p += 64) {

                __m256i v0 = _mm256_load_si256((__m256i *) p);
                __m256i v1 = _mm256_load_si256((__m256i *) (p + 32));

                __m256i cmp0 = _mm256_cmpeq_epi8(v0, value);
                __m256i cmp1 = _mm256_cmpeq_epi8(v1, value);

                __m256i max_val = _mm256_max_epu8(cmp0, cmp1);
                unsigned eq_mask = (unsigned)_mm256_movemask_epi8(max_val);
                if (eq_mask == 0) continue;

                TEST32_cmp(p, cmp0);
                TEST32_cmp_known_nz(p + 32, cmp1); // returns from function
            }
        }
    }
    if (count > 32) {
        TEST32(p);
        count -= 32;
        p += 32;
    }
    TEST32_count(p);
    return NULL;
}

void* avx_memchr2_nounroll(const void* ptr, int ch, size_t count)
{
    if (count == 0) return NULL;

    const char * p = (const char*)ptr;

    __m256i value = _mm256_set1_epi8((char)ch);
    size_t align = (size_t)p & 0xFFF;

    if (align <= 4096 - 32) {
        TEST32_v_count(p, _mm256_loadu_si256((__m256i *) p));
        if (count <= 32) return NULL;
        count -= 32;
        p += 32;

        align &= 0x1F;
        p -= align;
        count += align;
    }
    else {
        align &= 0x1F;
        p -= align;
        __m256i v = _mm256_load_si256((__m256i *) p);
        unsigned eq_mask = (unsigned)_mm256_movemask_epi8(_mm256_cmpeq_epi8(v, value));
        eq_mask >>= align;
        if (eq_mask) {
            size_t pos = bsf(eq_mask);
            if (pos >= count) return NULL;
            return (void *)(p + pos + align);
        }
        count += align;
        if (count <= 32) return NULL;
        p += 32;
        count -= 32;
    }
    for (; count >= 64; count -= 64, p += 64) {

        __m256i v0 = _mm256_load_si256((__m256i *) p);
        __m256i v1 = _mm256_load_si256((__m256i *) (p + 32));

        __m256i cmp0 = _mm256_cmpeq_epi8(v0, value);
        __m256i cmp1 = _mm256_cmpeq_epi8(v1, value);

        __m256i max_val = _mm256_max_epu8(cmp0, cmp1);
        unsigned eq_mask = (unsigned)_mm256_movemask_epi8(max_val);
        if (eq_mask == 0) continue;

        TEST32_cmp(p, cmp0);
        TEST32_cmp_known_nz(p + 32, cmp1); // returns from function
    }
    if (count > 32) {
        TEST32(p);
        count -= 32;
        p += 32;
    }
    TEST32_count(p);
    return NULL;
}

#pragma GCC pop_options
#endif // #ifdef __GNUC__

#undef TEST32_cmp_known_nz
#undef TEST32_cmp
#undef TEST32_v
#undef TEST32
#undef TEST32_v_count
#undef TEST32_count


char * text;
const size_t max_size = 16384;
const size_t max_offset = 64;
const size_t rep = 10;

volatile const void * pos;

void full_test_at(Searcher f, size_t offset, size_t size, size_t result)
{
    text[offset + result] = 'A';
    char* r1 = memchr(text + offset, 'A', size);
    char* r2 = f(text + offset, 'A', size);
    if (r1 != r2) {
        printf("Bad result at offset %zd size %zd result %zd; ", offset, size, result);
        printf("got ");
        if (r2) printf("%zd", r2 - (text + offset)); else printf("NULL");
        printf("; must be ");
        if (r1) printf("%zd", r1 - (text + offset)); else printf("NULL");
        printf("\n");
        exit(1);
    }
    text[offset + result] = 0xEE;
}

void full_test(Searcher f)
{
    for (size_t offset = 0; offset < max_offset; offset++) {
        for (size_t size = 1; size <= 512; size++) {
            for (size_t result = 0; result < size + 1; result++) {
                full_test_at(f, offset, size, result);
            }
        }
    }
}

void test(Searcher f, const char * name)
{
    printf("%s\n", name);

    if (f != null_index) full_test(f);

    for (size_t base_size = 4; base_size <= max_size; base_size *= 4) {
        size_t num_sizes = base_size < 16 ? base_size : 16;
        size_t N = (size_t) (1024LL * 1024L * 1024L * rep / base_size / num_sizes / max_offset);
        size_t total_bytes = 0;

        clock_t t0 = clock();
        for (size_t size = base_size - num_sizes / 2; size < base_size + num_sizes / 2; size++) {
            for (size_t offset = 0; offset < max_offset; offset++) {
                total_bytes += size;
                text[offset + size - 1] = 'A';

                for (size_t i = 0; i < N; i++) {
                    pos = f(text + offset, 'A', max_size);
                }

                text[offset + size - 1] = 0xEE;
            }
        }
        clock_t t1 = clock();

        double time = (double)(t1 - t0) / CLOCKS_PER_SEC;
        double time_nanos = time * 1.0E9 / N / total_bytes;
        printf("size: %10zd; time: %f; ns/byte: %f\n", base_size, time, time_nanos);
        fflush(stdout);
    }
}

int main(void)
{
    size_t alloc_size = max_size + max_offset;
    text = (char *) _mm_malloc(alloc_size, 64);
    if (!text) {
        printf("Out of memory\n");
        return 1;
    }
    memset(text, 0xEE, alloc_size);

#define TEST(f) test (f, #f)

    TEST(null_index);
    TEST(simple_index);
    TEST(simple_ptr);
    TEST(simple_ptr2);

    TEST(memchr);

#ifdef __GNUC__
    TEST(index_scas);
#endif
    TEST(simple_dword);
    TEST(dword);
    TEST(dword_bsf);
    TEST(simple_qword);
    TEST(qword);
    TEST(qword_bsf);
    TEST(sse);
    TEST(sse_unlimited);
    TEST(sse_aligned);
    TEST(sse_aligned64);
    TEST(avx);
    TEST(avx_aligned);
    TEST(avx_aligned64);
    TEST(sse_memchr);
    TEST(sse_memchr2);
#ifdef __GNUC__
    TEST(sse_memchr_nounroll);
    TEST(sse_memchr2_nounroll);
#endif
    TEST(avx_memchr);
    TEST(avx_memchr2);
#ifdef __GNUC__
    TEST(avx_memchr_nounroll);
    TEST(avx_memchr2_nounroll);
#endif
    return 0;
}