/**
 * Copyright 2020 DriveX.Tech. All rights reserved.
 * 
 * Licensed under the License.
 */

#include "segmenter.h"
#include "marshalling/packer.h"
#include <string.h>
#include <assert.h>


const WORD SEGMENTER_HDER_LEN = 5; // segment header length 5B


bool
segmenter_init(segmenter_t* segmenter, WORD data_chunk_length)
{
    assert(segmenter != NULL && data_chunk_length > 0);

    segmenter_reset(segmenter);

    return segmenter_set_data_chunk_length(segmenter, data_chunk_length);
}


bool
segmenter_set_data_chunk_length(segmenter_t* segmenter, WORD data_chunk_length)
{
    segmenter->max_seger_length = data_chunk_length;
    segmenter->max_seger_payload_length = segmenter->max_seger_length - SEGMENTER_HDER_LEN;

    return true;
}


void
segmenter_reset(segmenter_t* segmenter)
{
    assert(segmenter != NULL);

    segmenter->seger_ind = 0;
    segmenter->seger_off = 0;
    segmenter->data_buf_hdr.data = NULL;
    segmenter->data_buf_hdr.data_len = 0;
}


/*
bool
segmenter_segment(
    segmenter_t* segmenter,
    const BYTE* data_buf,
    const WORD data_len)
{
    data_buf_const_slice_t* p_seger_info = NULL;
    BYTE num_chunk = 0;
    WORD off = 0;
    while(off + segmenter->max_seger_payload_length <= data_len) {
        p_seger_info = RAWBUF_ITEM_PTR(segmenter->segers_info, num_chunk);
        p_seger_info->data = data_buf;
        p_seger_info->data_len = segmenter->max_seger_payload_length;
        p_seger_info->data_off = off;
        num_chunk++;
        off += p_seger_info->data_len;
    }
    if(off < data_len) {
        p_seger_info = RAWBUF_ITEM_PTR(segmenter->segers_info, num_chunk);
        p_seger_info->data = data_buf;
        p_seger_info->data_len = data_len - off;
        p_seger_info->data_off = off;
        num_chunk++;
        off += p_seger_info->data_len;
    }
    assert(num_chunk < 128 - 1);
    assert(off == data_len);

    segmenter->seger_num = num_chunk;

    return true;
}
*/


bool segmenter_test_started(segmenter_t* segmenter, const BYTE* seger_buf)
{
    assert(segmenter != NULL && seger_buf != NULL);

    return (seger_buf[0] >> 1) == 0;
}

bool segmenter_test_finished(segmenter_t* segmenter, const BYTE* seger_buf)
{
    assert(segmenter != NULL && seger_buf != NULL);

    return !(seger_buf[0] & 1);
}


/**
 * @brief increment `segmenter`'s internal `seger_off` by step `len`, and `seger_ind` by 1
 * 
 * @param segmenter 
 * @param len 
 * @return true 
 * @return false 
 */
bool segmenter_skip(segmenter_t* segmenter, WORD len)
{
    assert(segmenter != NULL && len > 0);

    segmenter->seger_ind++; // set next data segment ind
    segmenter->seger_off += len; // next expected seger off
    assert(segmenter->seger_off <= segmenter->data_buf_hdr.data_len);

    return true;
}


/**
 * @brief try pack a segment of ind `seger_ind` to byte buf, WITHOUT modify `segmenter`'s internal `seger_off`, prefix with `(flag, data_off, data_len)` header
 * 
 * @param segmenter 
 * @param seger_buf 
 * @param seger_buf_len packed data length
 * @return packed status
 */
SegmenterStatus
segmenter_try_pack(
    segmenter_t* segmenter,
    BYTE* seger_buf,
    WORD* seger_buf_len)
{
    assert(segmenter != NULL
            && seger_buf != NULL
            && seger_buf_len != NULL);

    assert(segmenter->data_buf_hdr.data != NULL
            && segmenter->data_buf_hdr.data_len > 0);
    
    SegmenterStatus status = FAILED;

    bool is_last_seger = false;
    WORD data_seg_len =  segmenter->max_seger_payload_length;

    if(segmenter->seger_off + segmenter->max_seger_payload_length >= segmenter->data_buf_hdr.data_len) { // last segment, set MS flag to 1
        is_last_seger = true;
        data_seg_len = segmenter->data_buf_hdr.data_len - segmenter->seger_off;
    }

    int len = 0;

    BYTE MS_IND = 0; // more segment flag && segment index, bit-0: MS flag, bit-1:8, ind
    MS_IND = is_last_seger ? 0 : 1;
    MS_IND |= (segmenter->seger_ind << 1); // set highest 7-bit to segment ind
    len = pack_BYTE(MS_IND, seger_buf);

    len += pack_WORD(segmenter->seger_off, seger_buf + len);
    len += pack_WORD(data_seg_len, seger_buf + len);

    memcpy(seger_buf + len, segmenter->data_buf_hdr.data + segmenter->seger_off, data_seg_len);
    len += data_seg_len;

    *seger_buf_len = len;
    // *seger_buf_len = data_seg_len;

    status = is_last_seger ? SUCCESSFUL : CONTINUE;
    return status;
}


/**
 * @brief pack a segment of ind `seger_ind` to byte buf, modify `segmenter`'s internal `seger_off`, prefix with `(flag, data_off, data_len)` header
 * equivalent to `segmenter_try_pack` + `segmenter_skip`
 * 
 * @param segmenter 
 * @param seger_buf 
 * @param seger_buf_len packed data length
 * @return packed status
 */
SegmenterStatus
segmenter_pack(
    segmenter_t* segmenter,
    BYTE* seger_buf,
    WORD* seger_buf_len)
{
    SegmenterStatus status = segmenter_try_pack(segmenter, seger_buf, seger_buf_len);
    if(status != FAILED) {
        bool ret = segmenter_skip(segmenter, *seger_buf_len - SEGMENTER_HDER_LEN);
        if(ret) {
            ;
        }
    }
    
    return status;
}


/**
 * @brief unpack a byte buf to segment info, WITHOUT modify `segmenter`'s internal `seger_off`, prefix with `(flag, data_off, data_len)` header
 * 
 * @param segmenter 
 * @param seger_buf 
 * @param seger_buf_len `seger_buf` raw data length to be unpacked
 * @return unpacked status
 */
SegmenterStatus
segmenter_try_unpack(
    segmenter_t* segmenter,
    const BYTE* seger_buf,
    const WORD seger_buf_len)
{
    assert(segmenter != NULL && seger_buf != NULL);

    assert(segmenter->data_buf_hdr.data != NULL
            && segmenter->data_buf_hdr.data_len > 0);
    
    SegmenterStatus status = FAILED;

    bool is_last_seger = false;

    int len = 0;

    BYTE MS_IND; // more segment flag && segment index, bit-0: MS flag, bit-1:8, ind
    len = unpack_BYTE(seger_buf, &MS_IND);
    if(len > segmenter->max_seger_length) {
        return FAILED;
    }

    BYTE seger_ind = (MS_IND >> 1);
    // assert(seger_ind == segmenter->seger_ind);
    if(seger_ind != segmenter->seger_ind) {
        return FAILED;
    }
    is_last_seger = !(MS_IND & 1);

    WORD data_seg_len =  0, data_seg_off = 0;
    len += unpack_WORD(seger_buf + len, &data_seg_off);
    // assert(data_seg_off == segmenter->seger_off);
    if(data_seg_off != segmenter->seger_off) {
        return FAILED;
    }
    len += unpack_WORD(seger_buf + len, &data_seg_len);
    if(len > segmenter->max_seger_length) {
        return FAILED;
    }
    if(data_seg_len + SEGMENTER_HDER_LEN != seger_buf_len) {
        return FAILED;
    }

    memcpy(segmenter->data_buf_hdr.data + data_seg_off, seger_buf + len, data_seg_len);
    len += data_seg_len;
    // if(len > segmenter->max_seger_length) {
    //     //TODO: fatal error
    //     assert(len <= segmenter->max_seger_length);
    // }
    if(len > segmenter->max_seger_length) {
        return FAILED;
    }

    status = is_last_seger ? SUCCESSFUL : CONTINUE;
    return status;
}


/**
 * @brief unpack a byte buf to segment info, modify `segmenter`'s internal `seger_off`, prefix with `(flag, data_off, data_len)` header
 * 
 * @param segmenter 
 * @param seger_buf 
 * @param seger_buf_len `seger_buf` raw data length unpacked
 * @return unpacked status
 */
SegmenterStatus
segmenter_unpack(
    segmenter_t* segmenter,
    const BYTE* seger_buf,
    const WORD seger_buf_len)
{
    SegmenterStatus status = segmenter_try_unpack(segmenter, seger_buf, seger_buf_len);
    if(status != FAILED) {
        bool ret = segmenter_skip(segmenter, seger_buf_len - SEGMENTER_HDER_LEN);
        if(ret) {
            ;
        }
    }
    
    return status;
}