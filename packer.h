// Copyright 2020 DriveX.Tech. All rights reserved.
// 
// Licensed under the License.

#pragma once
#ifndef _PACKER_H_
#define _PACKER_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif


int pack_INT8(INT8 i8, BYTE buf[1]);
int pack_UINT8(UINT8 ui8, BYTE buf[1]);
int pack_BYTE(BYTE b8, BYTE buf[1]);

int pack_INT16(INT16 i16, BYTE buf[2]);
int pack_UINT16(UINT16 ui16, BYTE buf[2]);
int pack_WORD(WORD w16, BYTE buf[2]);

int pack_INT32(INT32 i32, BYTE buf[4]);
int pack_UINT32(UINT32 ui32, BYTE buf[4]);
int pack_DWORD(DWORD d32, BYTE buf[4]);

int pack_INT64(INT64 i64, BYTE buf[8]);
int pack_UINT64(UINT64 ui64, BYTE buf[8]);
int pack_QWORD(QWORD q64, BYTE buf[8]);

int pack_FLOAT32(FLOAT32 f32, BYTE buf[4]);

int pack_FLOAT64(FLOAT64 f64, BYTE buf[8]);

int pack_FLOAT128(FLOAT128 f128, BYTE buf[16]);

int pack_STRING(char msg[], BYTE buf[]); // string with 0-ended!

int pack_BYTES(BYTE msg[], int msg_len, BYTE buf[]); // string


int unpack_INT8(const BYTE buf[1], INT8* i8);
int unpack_UINT8(const BYTE buf[1], UINT8* ui8);
int unpack_BYTE(const BYTE buf[1], BYTE* b8);

int unpack_INT16(const BYTE buf[2], INT16* i16);
int unpack_UINT16(const BYTE buf[2], UINT16* ui16);
int unpack_WORD(const BYTE buf[2], WORD* w16);

int unpack_INT32(const BYTE buf[4], INT32* i32);
int unpack_UINT32(const BYTE buf[4], UINT32* ui32);
int unpack_DWORD(const BYTE buf[4], DWORD* d32);

int unpack_INT64(const BYTE buf[8], INT64* i64);
int unpack_UINT64(const BYTE buf[8], UINT64* ui64);
int unpack_QWORD(const BYTE buf[8], QWORD* q64);

int unpack_FLOAT32(const BYTE buf[4], FLOAT32* f32);

int unpack_FLOAT64(const BYTE buf[8], FLOAT64* f64);

int unpack_FLOAT128(const BYTE buf[16], FLOAT128* f128);

int unpack_STRING(const BYTE buf[], char msg[]); // string with 0-ended!

int unpack_BYTES(const BYTE buf[], int buf_len, BYTE msg[]); // string


#ifdef __cplusplus
}
#endif

#endif
