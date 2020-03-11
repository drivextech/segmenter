// Copyright 2020 DriveX.Tech. All rights reserved.
// 
// Licensed under the License.

#pragma once
#ifndef _SEGMENTER_HPP_
#define _SEGMENTER_HPP_

#include <vector>
#include "util/types.h"


class Segmenter {
public:
    typedef enum {
        CONTINUE,
        FAILED,
        SUCCESSFUL,
    } Status;

public:
    Segmenter(WORD data_chunk_length = 80);
    ~Segmenter();

    void reset();

    bool set_data_chunk_length(WORD data_chunk_length);

    static bool test_is_started(const std::vector<BYTE>& seger);
    static bool test_is_finished(const std::vector<BYTE>& seger);

    bool skip(WORD len);

    Status try_pack(std::vector<BYTE>& seger);
    Status pack(std::vector<BYTE>& seger);

    Status try_unpack(const std::vector<BYTE>& seger);
    Status unpack(const std::vector<BYTE>& seger);

public:
    std::vector<BYTE> seger_data;

public:
    static const WORD SEGMENTER_HDER_LEN = 5; // segment header length 5B

private:
    WORD max_seger_length, max_seger_payload_length;
    BYTE seger_ind;
    WORD seger_off;
};


#endif
