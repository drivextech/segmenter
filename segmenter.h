// Copyright 2020 DriveX.Tech. All rights reserved.
// 
// Licensed under the License.

#pragma once
#ifndef _SEGMENTER_H_
#define _SEGMENTER_H_

#include <stdbool.h>
#include "types/types.h"


#ifdef __cplusplus
extern "C" {
#endif


const WORD SEGMENTER_HDER_LEN;
// const WORD SEGMENTER_HDER_LEN = 5; // segment header length 5B

typedef enum {
    CONTINUE,
    FAILED,
    SUCCESSFUL,
} SegmenterStatus;

typedef struct _segmenter_t {
    data_buf_hder_t data_buf_hdr;

    WORD max_seger_length, max_seger_payload_length;
    BYTE seger_ind;
    WORD seger_off;
} segmenter_t;

bool
segmenter_init(segmenter_t* segmenter, WORD data_chunk_length);

bool
segmenter_set_data_chunk_length(segmenter_t* segmenter, WORD data_chunk_length);

void
segmenter_reset(segmenter_t* segmenter);

bool
segmenter_segment(
    segmenter_t* segmenter,
    const BYTE* data_buf,
    const WORD data_len);

bool segmenter_test_started(segmenter_t* segmenter, const BYTE* seger_buf);

bool segmenter_test_finished(segmenter_t* segmenter, const BYTE* seger_buf);

bool segmenter_skip(segmenter_t* segmenter, WORD len);

SegmenterStatus
segmenter_try_pack(
    segmenter_t* segmenter,
    BYTE* seger_buf,
    WORD* seger_buf_len);

SegmenterStatus
segmenter_pack(
    segmenter_t* segmenter,
    BYTE* seger_buf,
    WORD* seger_buf_len);

SegmenterStatus
segmenter_try_unpack(
    segmenter_t* segmenter,
    const BYTE* seger_buf,
    const WORD seger_buf_len);

SegmenterStatus
segmenter_unpack(
    segmenter_t* segmenter,
    const BYTE* seger_buf,
    const WORD seger_buf_len);


#ifdef __cplusplus
}
#endif
#endif