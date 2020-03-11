#include <iostream>
#include <vector>
#include <stdio.h>
#include "segmenter.hpp"

using namespace std;



#define SERVICE_MTU 8192
#define DATA_CHUNK_LENGTH 256

Segmenter segmenter1(DATA_CHUNK_LENGTH), segmenter2(DATA_CHUNK_LENGTH);

vector<BYTE> service_data1(SERVICE_MTU), service_data2(SERVICE_MTU);


int main()
{
    for(auto i = 0; i < SERVICE_MTU; i++) {
        service_data1[i] = i;
    }

    segmenter1.seger_data = service_data1;
    // segmenter2.seger_data = vector<BYTE>(service_data2, service_data2 + SERVICE_MTU);

    UINT16 data_len, data_len_p, data_len_up;
    vector<BYTE> data_seger;
    do {
        auto status1 = segmenter1.pack(data_seger);
        
        auto status2 = segmenter2.unpack(data_seger);

        if(status1 == Segmenter::Status::CONTINUE) {
            printf("Segmenter::pack: CONTINUE: raw: %uB, packed: %uB\n", (unsigned)(data_seger.size() - Segmenter::SEGMENTER_HDER_LEN), (unsigned)data_seger.size());
        } else {
            if(status1 == Segmenter::Status::FAILED) {
                printf("Segmenter::pack: FAILED!\n");
            }
            if(status1 == Segmenter::Status::SUCCESSFUL) {
                printf("Segmenter::pack: SUCCESSFUL: raw: %uB, packed: %uB\n", (unsigned)(data_seger.size() - Segmenter::SEGMENTER_HDER_LEN), (unsigned)data_seger.size());
            }
        }
        if(status2 == Segmenter::Status::CONTINUE) {
            printf("Segmenter::unpack: CONTINUE: raw: %uB, unpacked: %uB\n", (unsigned)data_seger.size(), (unsigned)(data_seger.size() - Segmenter::SEGMENTER_HDER_LEN));
        } else {
            if(status2 == Segmenter::Status::FAILED) {
                printf("Segmenter::unpack: FAILED!\n");
            }
            if(status2 == Segmenter::Status::SUCCESSFUL) {
                printf("Segmenter::unpack: SUCCESSFUL: raw: %uB, unpacked: %uB\n", (unsigned)data_seger.size(), (unsigned)(data_seger.size() - Segmenter::SEGMENTER_HDER_LEN));
            }
        }
        if((status1 != Segmenter::Status::CONTINUE) || (status2 != Segmenter::Status::CONTINUE)) {
            break;
        }
    } while(true);

    service_data2 = segmenter2.seger_data;
    printf("segmenter_unpack: %uB\n", (unsigned)service_data2.size());

    bool ok = true;
    for(auto i = 0; i <SERVICE_MTU; i++) {
        ok = ok && (service_data1[i] == service_data2[i]);
    }

    printf("segmenter test %s!\n", ok ? "ok" : "failed");

    return !ok;
}