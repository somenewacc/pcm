// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2012, Intel Corporation
// written by Austen Ott
//    
#ifndef MSRACCESSOR_HEADER
#define MSRACCESSOR_HEADER

#include <stdint.h>
#include <MSRKernel.h>
class MSRAccessor{
private:
    uint64_t num_cores;
    void closeConnection();
public:
    MSRAccessor();
    int32_t read(uint32_t cpu_num, uint64_t msr_num, uint64_t * value);
    int32_t write(uint32_t cpu_num, uint64_t msr_num, uint64_t value);
    int32_t buildTopology(uint32_t num_cores, void*);
    uint32_t getNumInstances();
    uint32_t incrementNumInstances();
    uint32_t decrementNumInstances();
    ~MSRAccessor();
};

#endif
