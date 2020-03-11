#include <stdio.h>

#include "segmenter.h"



#define SERVICE_MTU 8192
#define DATA_CHUNK_LENGTH 256

segmenter_t segmenter1, segmenter2;

BYTE service_data1[SERVICE_MTU], service_data2[SERVICE_MTU];
BYTE data_chunk_buf[DATA_CHUNK_LENGTH];


int main()
{
    segmenter_init(&segmenter1, DATA_CHUNK_LENGTH);
    segmenter_init(&segmenter2, DATA_CHUNK_LENGTH);

    for(int i = 0; i < SERVICE_MTU; i++) {
        service_data1[i] = i;
    }

    segmenter1.data_buf_hdr.data = service_data1;
    segmenter1.data_buf_hdr.data_len = sizeof(service_data1);
    segmenter2.data_buf_hdr.data = service_data2;
    segmenter2.data_buf_hdr.data_len = sizeof(service_data2);

    WORD data_len, data_len_p;
    do {
        SegmenterStatus status1 = segmenter_pack(&segmenter1, data_chunk_buf, &data_len_p);

        SegmenterStatus status2 = segmenter_unpack(&segmenter2, data_chunk_buf, data_len_p);

        if(status1 == CONTINUE) {
            printf("segmenter_pack: CONTINUE: raw: %uB, packed: %uB\n", (unsigned)(data_len_p - SEGMENTER_HDER_LEN), (unsigned)data_len_p);
        } else {
            if(status1 == FAILED) {
                printf("segmenter_pack: FAILED!\n");
            }
            if(status1 == SUCCESSFUL) {
                printf("segmenter_pack: SUCCESSFUL: raw: %uB, packed: %uB\n", (unsigned)(data_len_p - SEGMENTER_HDER_LEN), (unsigned)data_len_p);
            }
        }
        if(status2 == CONTINUE) {
            printf("segmenter_unpack: CONTINUE: raw: %uB, unpacked: %uB\n", (unsigned)data_len_p, (unsigned)(data_len_p - SEGMENTER_HDER_LEN));
        } else {
            if(status2 == FAILED) {
                printf("segmenter_unpack: FAILED!\n");
            }
            if(status2 == SUCCESSFUL) {
                printf("segmenter_unpack: SUCCESSFUL: raw: %uB, unpacked: %uB\n", (unsigned)data_len_p, (unsigned)(data_len_p - SEGMENTER_HDER_LEN));
            }
        }
        if((status1 != CONTINUE) || (status2 != CONTINUE)) {
            break;
        }
    } while(true);

    printf("segmenter_unpack: %uB\n", segmenter2.seger_off);

    bool ok = true;
    for(int i = 0; i <SERVICE_MTU; i++) {
        ok = ok && (service_data1[i] == service_data2[i]);
    }

    printf("segmenter test %s!\n", ok ? "ok" : "failed");

    return !ok;
}