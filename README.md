# Segmentation Implementation mainly for BT packet
[![Build Status](https://travis-ci.com/drivextech/segmenter.svg?branch=master)](https://travis-ci.com/drivextech/segmenter)
[![Build Status](https://api.cirrus-ci.com/github/drivextech/segmenter.svg)](https://cirrus-ci.com/github/drivextech/segmenter)

This library implement segmentation/fragmentation mechanism espcially for ble pysical layer. A customized packet slicing protocol is used which has a cost of 5 Bytes' header.

## TODO

- [x] ~~remove directly use of `types.h` and `packer.h`/`packer.c`, should use external library `dxt_util` instead.~~
