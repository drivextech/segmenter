// Copyright 2020 DriveX.Tech. All rights reserved.
// 
// Licensed under the License.

#pragma once
#ifndef _SEGMENTER_HPP_
#define _SEGMENTER_HPP_

#include <vector>
#include "types/types.h"


class Segmenter {
public:
    typedef enum {
        CONTINUE,
        FAILED,
        SUCCESSFUL,
    } Status;

public:
    Segmenter(dxt_common::WORD data_chunk_length = 180);
    ~Segmenter();

    void reset();

    bool set_data_chunk_length(dxt_common::WORD data_chunk_length);

    static bool test_is_started(const std::vector<dxt_common::BYTE>& seger);
    static bool test_is_finished(const std::vector<dxt_common::BYTE>& seger);

    bool skip(dxt_common::WORD len);

    Status try_pack(std::vector<dxt_common::BYTE>& seger);
    Status pack(std::vector<dxt_common::BYTE>& seger);

    Status try_unpack(const std::vector<dxt_common::BYTE>& seger);
    Status unpack(const std::vector<dxt_common::BYTE>& seger);

public:
    std::vector<dxt_common::BYTE> seger_data;

public:
    static const dxt_common::WORD SEGMENTER_HDER_LEN = 5; // segment header length 5B

private:
    dxt_common::WORD max_seger_length, max_seger_payload_length;
    dxt_common::BYTE seger_ind;
    dxt_common::WORD seger_off;
};


#endif
