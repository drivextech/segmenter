#include <string.h>
#include <assert.h>
#include "packer.h"

#define UNUSED(x) (void)(x)
#define roundval(dbl) dbl >= 0.0 ? (int)(dbl + 0.5) : ((dbl - (double)(int)dbl) <= -0.5 ? (int)dbl : (int)(dbl - 0.5))

#define UNKOWN_ENDIAN 0x8888

#if defined(_WIN32)
#define BIG_ENDIAN      0x8000
#define LITTLE_ENDIAN   0x0008
#define BYTE_ORDER      LITTLE_ENDIAN
#elif defined(__APPLE__)
/* macros already defined */
#elif defined(__DragonFly__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
#include <sys/endian.h>
#elif defined(__linux__)
#include <endian.h>
#elif defined(_AIX)
#include <sys/machine.h>
#elif defined(__unix) || defined(unix) || defined(__unix)
#include <endian.h>
#else
#define BYTE_ORDER UNKOWN_ENDIAN
#endif

#ifdef __BYTE_ORDER
#define BYTE_ORDER __BYTE_ORDER
#endif
#ifdef __BIG_ENDIAN
#define BIG_ENDIAN __BIG_ENDIAN
#endif
#ifdef __LITTLE_ENDIAN
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#endif

#ifndef BYTE_ORDER
#error "BYTE_ORDER not defined"
#endif
//#ifndef BIG_ENDIAN
//#error "BIG_ENDIAN not defined"
//#endif
//#ifndef LITTLE_ENDIAN
//#error "LITTLE_ENDIAN not defined"
//#endif
//#if BIG_ENDIAN == LITTLE_ENDIAN
//#error "BIG_ENDIAN == LITTLE_ENDIAN"
//#endif
#if BYTE_ORDER!=BIG_ENDIAN && BYTE_ORDER!=LITTLE_ENDIAN && BYTE_ORDER!=UNKOWN_ENDIAN
#error "BYTE_ORDER not supported"
#endif


int pack_INT8(INT8 i8, BYTE buf[1])
{
    return pack_BYTE(i8, buf);
}
int pack_UINT8(UINT8 ui8, BYTE buf[1])
{
    return pack_BYTE(ui8, buf);
}
int pack_BYTE(BYTE b8, BYTE buf[1])
{
    buf[0] = b8;

    return 1;
}

int pack_INT16(INT16 i16, BYTE buf[2])
{
    return pack_WORD(i16, buf);
}
int pack_UINT16(UINT16 ui16, BYTE buf[2])
{
    return pack_WORD(ui16, buf);
}
int pack_WORD(WORD w16, BYTE buf[2])
{
#if BYTE_ORDER == LITTLE_ENDIAN
    BYTE* orig = (BYTE*)&w16;
    buf[0] = orig[1];
    buf[1] = orig[0];
#elif BYTE_ORDER == BIG_ENDIAN
    BYTE* orig = (BYTE*)&w16;
    buf[0] = orig[0];
    buf[1] = orig[1];
#else
    buf[0] = w16 >> 8;
    buf[1] = w16;
#endif
    
    return 2;
}

int pack_INT32(INT32 i32, BYTE buf[4])
{
    return pack_DWORD(i32, buf);
}
int pack_UINT32(UINT32 ui32, BYTE buf[4])
{
    return pack_DWORD(ui32, buf);
}
int pack_DWORD(DWORD d32, BYTE buf[4])
{
#if BYTE_ORDER == LITTLE_ENDIAN
    BYTE* orig = (BYTE*)&d32;
    buf[0] = orig[3];
    buf[1] = orig[2];
    buf[2] = orig[1];
    buf[3] = orig[0];
#elif BYTE_ORDER == BIG_ENDIAN
    BYTE* orig = (BYTE*)&d32;
    buf[0] = orig[0];
    buf[1] = orig[1];
    buf[2] = orig[2];
    buf[3] = orig[3];
#else
    buf[0] = d32 >> 24;
    buf[1] = d32 >> 16;
    buf[2] = d32 >> 8;
    buf[3] = d32;
#endif

    return 4;
}

int pack_INT64(INT64 i64, BYTE buf[8])
{
    return pack_QWORD(i64, buf);
}
int pack_UINT64(UINT64 ui64, BYTE buf[8])
{
    return pack_QWORD(ui64, buf);
}
int pack_QWORD(QWORD q64, BYTE buf[8])
{
#if BYTE_ORDER == LITTLE_ENDIAN
    BYTE* orig = (BYTE*)&q64;
    buf[0] = orig[7];
    buf[1] = orig[6];
    buf[2] = orig[5];
    buf[3] = orig[4];
    buf[4] = orig[3];
    buf[5] = orig[2];
    buf[6] = orig[1];
    buf[7] = orig[0];
#elif BYTE_ORDER == BIG_ENDIAN
    BYTE* orig = (BYTE*)&q64;
    buf[0] = orig[0];
    buf[1] = orig[1];
    buf[2] = orig[2];
    buf[3] = orig[3];
    buf[4] = orig[4];
    buf[5] = orig[5];
    buf[6] = orig[6];
    buf[7] = orig[7];
#else
    buf[0] = q64 >> 56;
    buf[1] = q64 >> 48;
    buf[2] = q64 >> 40;
    buf[3] = q64 >> 32;
    buf[4] = q64 >> 24;
    buf[5] = q64 >> 16;
    buf[6] = q64 >> 8;
    buf[7] = q64;
#endif

    return 8;
}

int pack_FLOAT32(FLOAT32 f32, BYTE buf[4])
{
    DWORD* p = (DWORD*)&f32;
    return pack_DWORD(*p, buf);
}

int pack_FLOAT64(FLOAT64 f64, BYTE buf[8])
{
    QWORD* p = (QWORD*)&f64;
    return pack_QWORD(*p, buf);
}

int pack_FLOAT128(FLOAT128 f128, BYTE buf[16])
{
    assert(0 && "Not-Impl Now!");
    return -1;
}

int pack_STRING(char msg[], BYTE buf[])
{
    return pack_BYTES((BYTE*)msg, strlen(msg), buf);
}

int pack_BYTES(BYTE msg[], int msg_len, BYTE buf[])
{
    assert(0 && "Not-Impl Now!");
    return -1;
}


int unpack_INT8(const BYTE buf[1], INT8* i8)
{
    BYTE* b8 = (BYTE*)i8;
    return unpack_BYTE(buf, b8);
}
int unpack_UINT8(const BYTE buf[1], UINT8* ui8)
{
    BYTE* b8 = (BYTE*)ui8;
    return unpack_BYTE(buf, b8);
}
int unpack_BYTE(const BYTE buf[1], BYTE* b8)
{
    *b8 = buf[0];

    return 1;
}

int unpack_INT16(const BYTE buf[2], INT16* i16)
{
    WORD* w8 = (WORD*)i16;
    return unpack_WORD(buf, w8);
}
int unpack_UINT16(const BYTE buf[2], UINT16* ui16)
{
    WORD* w16 = (WORD*)ui16;
    return unpack_WORD(buf, w16);
}
int unpack_WORD(const BYTE buf[2], WORD* w16)
{
#if BYTE_ORDER == LITTLE_ENDIAN
    BYTE* to = (BYTE*)w16;
    to[0] = buf[1];
    to[1] = buf[0];
#elif BYTE_ORDER == BIG_ENDIAN
    BYTE* to = (BYTE*)w16;
    to[0] = buf[0];
    to[1] = buf[1];
#else
    *w16 = 0;
    *w16 = buf[0];
    *w16 <<= 8;
    *w16 += buf[1];
#endif
    
    return 2;
}

int unpack_INT32(const BYTE buf[4], INT32* i32)
{
    DWORD* d32 = (DWORD*)i32;
    return unpack_DWORD(buf, d32);
}
int unpack_UINT32(const BYTE buf[4], UINT32* ui32)
{
    DWORD* d32 = (DWORD*)ui32;
    return unpack_DWORD(buf, d32);
}
int unpack_DWORD(const BYTE buf[4], DWORD* d32)
{
#if BYTE_ORDER == LITTLE_ENDIAN
    BYTE* to = (BYTE*)d32;
    to[0] = buf[3];
    to[1] = buf[2];
    to[2] = buf[1];
    to[3] = buf[0];
#elif BYTE_ORDER == BIG_ENDIAN
    BYTE* to = (BYTE*)d32;
    to[0] = buf[0];
    to[1] = buf[1];
    to[2] = buf[2];
    to[3] = buf[3];
#else
    *d32 = 0;
    *d32 = buf[0];
    *d32 <<= 8;
    *d32 += buf[1];
    *d32 <<= 8;
    *d32 += buf[2];
    *d32 <<= 8;
    *d32 += buf[3];
#endif

    return 4;
}

int unpack_INT64(const BYTE buf[8], INT64* i64)
{
    QWORD* q64 = (QWORD*)i64;
    return unpack_QWORD(buf, q64);
}
int unpack_UINT64(const BYTE buf[8], UINT64* ui64)
{
    QWORD* q64 = (QWORD*)ui64;
    return unpack_QWORD(buf, q64);
}
int unpack_QWORD(const BYTE buf[8], QWORD* q64)
{
#if BYTE_ORDER == LITTLE_ENDIAN
    BYTE* to = (BYTE*)q64;
    to[0] = buf[7];
    to[1] = buf[6];
    to[2] = buf[5];
    to[3] = buf[4];
    to[4] = buf[3];
    to[5] = buf[2];
    to[6] = buf[1];
    to[7] = buf[0];
#elif BYTE_ORDER == BIG_ENDIAN
    BYTE* to = (BYTE*)q64;
    to[0] = buf[0];
    to[1] = buf[1];
    to[2] = buf[2];
    to[3] = buf[3];
    to[4] = buf[4];
    to[5] = buf[5];
    to[6] = buf[6];
    to[7] = buf[7];
#else
    *q64 = 0;
    *q64 = buf[0];
    *q64 <<= 8;
    *q64 += buf[1];
    *q64 <<= 8;
    *q64 += buf[2];
    *q64 <<= 8;
    *q64 += buf[3];
    *q64 <<= 8;
    *q64 += buf[4];
    *q64 <<= 8;
    *q64 += buf[5];
    *q64 <<= 8;
    *q64 += buf[6];
    *q64 <<= 8;
    *q64 += buf[7];
#endif

    return 8;
}

int unpack_FLOAT32(const BYTE buf[4], FLOAT32* f32)
{
    DWORD* d32 = (DWORD*)f32;
    return unpack_DWORD(buf, d32);
}

int unpack_FLOAT64(const BYTE buf[8], FLOAT64* f64)
{
    QWORD* q64 = (QWORD*)f64;
    return unpack_QWORD(buf, q64);
}

int unpack_FLOAT128(const BYTE buf[16], FLOAT128* f128)
{
    assert(0 && "Not-Impl Now!");
    return -1;
}

int unpack_STRING(const BYTE buf[], char msg[])
{
    return unpack_BYTES(buf, strlen((char*)buf), (BYTE*)msg);
}

int unpack_BYTES(const BYTE buf[], int buf_len, BYTE msg[])
{
    assert(0 && "Not-Impl Now!");
    return -1;
}
