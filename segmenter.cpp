// Copyright 2020 DriveX.Tech. All rights reserved.
// 
// Licensed under the License.

#include "segmenter.hpp"
#include "packer.h"
#include <string.h>
#include <assert.h>


Segmenter::Segmenter(WORD data_chunk_length)
{
    set_data_chunk_length(data_chunk_length);

    reset();
}

Segmenter::~Segmenter()
{
}

void Segmenter::reset()
{
    seger_ind = 0;
    seger_off = 0;
    seger_data.clear();
}

bool Segmenter::set_data_chunk_length(WORD data_chunk_length)
{
    max_seger_length = data_chunk_length;
    max_seger_payload_length = max_seger_length - SEGMENTER_HDER_LEN;

    return true;
}

bool Segmenter::test_is_started(const std::vector<BYTE>& seger)
{
    assert(seger.size() > 0);
    return (seger[0] >> 1) == 0;
}

bool Segmenter::test_is_finished(const std::vector<BYTE>& seger)
{
    assert(seger.size() > 0);
    return !(seger[0] & 1);
}

bool Segmenter::skip(WORD len)
{
    seger_ind++; // set next data segment ind
    seger_off += len; // next expected seger off
    assert(seger_off <= seger_data.size());

    return true;
}

Segmenter::Status Segmenter::try_pack(std::vector<BYTE>& seger)
{
    Status status = FAILED;

    bool is_last_seger = false;
    WORD data_seg_len =  max_seger_payload_length;

    if(seger_off + max_seger_payload_length >= seger_data.size()) { // last segment, set MS flag to 1
        is_last_seger = true;
        data_seg_len = seger_data.size() - seger_off;
    }

    if(data_seg_len == 0) {
        return FAILED;
    }

    seger.resize(SEGMENTER_HDER_LEN + data_seg_len);

    int len = 0;

    BYTE MS_IND = 0; // more segment flag && segment index, bit-0: MS flag, bit-1:8, ind
    MS_IND = is_last_seger ? 0 : 1;
    MS_IND |= (seger_ind << 1); // set highest 7-bit to segment ind
    len = pack_BYTE(MS_IND, seger.data());

    len += pack_WORD(seger_off, seger.data() + len);
    len += pack_WORD(data_seg_len, seger.data() + len);

    memcpy(seger.data() + len, seger_data.data() + seger_off, data_seg_len);
    len += data_seg_len;

    status = is_last_seger ? SUCCESSFUL : CONTINUE;
    return status;
}

Segmenter::Status Segmenter::pack(std::vector<BYTE>& seger)
{
    Status status = try_pack(seger);
    if(status != FAILED) {
        bool ret = skip(seger.size() - SEGMENTER_HDER_LEN);
        if(ret) {
            ;
        }
    }
    
    return status;
}

Segmenter::Status Segmenter::try_unpack(const std::vector<BYTE>& seger)
{
    Status status = FAILED;

    bool is_last_seger = false;

    int len = 0;

    BYTE MS_IND; // more segment flag && segment index, bit-0: MS flag, bit-1:8, ind
    len = unpack_BYTE(seger.data(), &MS_IND);
    if(len > max_seger_length) {
        return FAILED;
    }

    BYTE ind = (MS_IND >> 1);
    // assert(ind == seger_ind);
    if(ind != seger_ind) {
        return FAILED;
    }
    is_last_seger = !(MS_IND & 1);

    WORD data_seg_len =  0, data_seg_off = 0;
    len += unpack_WORD(seger.data() + len, &data_seg_off);
    // assert(data_seg_off == seger_off);
    if(data_seg_off != seger_off) {
        return FAILED;
    }
    len += unpack_WORD(seger.data() + len, &data_seg_len);
    if(len > max_seger_length) {
        return FAILED;
    }
    if(data_seg_len + SEGMENTER_HDER_LEN != seger.size()) {
        return FAILED;
    }

    //seger_data.insert(seger_data.end(), seger.data() + len, seger.data() + len + data_seg_len);
    seger_data.insert(seger_data.begin() + seger_off, seger.data() + len, seger.data() + len + data_seg_len);
    len += data_seg_len;
    if(len > max_seger_length) {
        return FAILED;
    }

    status = is_last_seger ? SUCCESSFUL : CONTINUE;
    return status;
}

Segmenter::Status Segmenter::unpack(const std::vector<BYTE>& seger)
{
    Status status = try_unpack(seger);
    if(status != FAILED) {
        bool ret = skip(seger.size() - SEGMENTER_HDER_LEN);
        if(ret) {
            ;
        }
    }
    
    return status;
}
