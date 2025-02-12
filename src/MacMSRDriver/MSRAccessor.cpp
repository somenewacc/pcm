// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2012, Intel Corporation
// written by Austen Ott
//    
#include "MSRAccessor.h"
#include <exception>
MSRAccessor::MSRAccessor(){
    service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching(kPcmMsrDriverClassName));
    openConnection();
}

int32_t MSRAccessor::buildTopology(uint32_t num_cores ,void* pTopos){
    topologyEntry *entries = (topologyEntry*)pTopos;
    size_t size = sizeof(topologyEntry)*num_cores;
    kern_return_t ret = getTopologyInfo(connect, entries, &size);
    return (ret == KERN_SUCCESS) ? 0 : -1;
}

int32_t MSRAccessor::read(uint32_t core_num, uint64_t msr_num, uint64_t * value){
    pcm_msr_data_t idatas, odatas;
    size_t size = sizeof(pcm_msr_data_t);
    idatas.msr_num = (uint32_t)msr_num;
    idatas.cpu_num = core_num;
    kern_return_t ret = readMSR(connect, &idatas, &size, &odatas, &size);
    if(ret == KERN_SUCCESS)
    {
        *value = odatas.value;
        return sizeof(uint64_t);
    }
    else{
        return -1;
    }
}

int32_t MSRAccessor::write(uint32_t core_num, uint64_t msr_num, uint64_t value){
    pcm_msr_data_t idatas;
    size_t size = sizeof(pcm_msr_data_t);
    idatas.value = value;
    idatas.msr_num = (uint32_t)msr_num;
    idatas.cpu_num = core_num;
    kern_return_t ret = writeMSR(connect, &idatas, &size);
    if(ret == KERN_SUCCESS)
    {
        return sizeof(uint64_t);
    }
    else
    {
        return -1;
    }
}

uint32_t MSRAccessor::getNumInstances(){
    uint32_t num_instances;
    getNumClients(connect, &num_instances);
    return num_instances;
}

uint32_t MSRAccessor::incrementNumInstances(){
    uint32_t num_instances;
    incrementNumClients(connect, &num_instances);
    return num_instances;
}

uint32_t MSRAccessor::decrementNumInstances(){
    uint32_t num_instances;
    decrementNumClients(connect, &num_instances);
    return num_instances;
}

MSRAccessor::~MSRAccessor(){
    closeConnection();
}

kern_return_t MSRAccessor::openConnection(){
    kern_return_t kernResult = IOServiceOpen(service, mach_task_self(), 0, &connect);
    
    if (kernResult != KERN_SUCCESS) {
        fprintf(stderr, "IOServiceOpen returned 0x%08x\n", kernResult);
    }
    else {
        kernResult = openMSRClient(connect);
        
        if (kernResult != KERN_SUCCESS) {
            fprintf(stderr, "openClient returned 0x%08x.\n\n", kernResult);
        }
    }
    
    return kernResult;
}

void MSRAccessor::closeConnection(){
    kern_return_t kernResult = closeMSRClient(connect);
    if (kernResult != KERN_SUCCESS) {
        fprintf(stderr, "closeClient returned 0x%08x.\n\n", kernResult);
    }
    
    kernResult = IOServiceClose(connect);
    if (kernResult != KERN_SUCCESS) {
        fprintf(stderr, "IOServiceClose returned 0x%08x\n\n", kernResult);
    }
}
