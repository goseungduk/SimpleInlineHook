#include <jni.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <thread>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <pthread.h>
#include <asm-generic/fcntl.h>
#include <asm-generic/mman-common.h>
#include <sys/mman.h>
#include <iostream>
#include <future>
#include <chrono>
#include <dlfcn.h>
#include "SimpleInlineHook.hpp"
#include "Logger.h"

uint32_t LDR = 0x58000051u;
uint32_t BR = 0xd61f0220u;

void print_hex_bytes(void* address, size_t byte_count) {
    char hex_string[3 * byte_count + 1];  // 각 바이트당 2자리 16진수 + 1 공백 포함
    hex_string[0] = '\0';  // 문자열 초기화

    unsigned char* opcode_list = reinterpret_cast<unsigned char*>(address);

    for (size_t i = 0; i < byte_count; ++i) {
        char byte_string[4];  // 한 바이트에 대해 2자리 16진수 + 공백 포함
        sprintf(byte_string, "%02X ", opcode_list[i]);
        strcat(hex_string, byte_string);
    }

    LOGI("Hex dump: %s", hex_string);
}

void rwx_protect(uint64_t *addr){
    LOGI("ADDR :0x%lx",addr);
    uint64_t page_addr = reinterpret_cast<uint64_t>(addr) & -0xfff;
    if(mprotect(reinterpret_cast<uint64_t *>(page_addr), 4096, (PROT_READ | PROT_WRITE | PROT_EXEC)) == 0){
        LOGI("0x%lx mprotect succeeded\n", page_addr);
    }
    else{
        LOGI("0x%lx mprotect failed\n", page_addr);
    }
}

void AllocateBackupSpace(int64_t (**backup)(const char*, const char*)){
    void* allocated_space = malloc(4096);
    *backup = reinterpret_cast<int64_t (*)(const char*, const char*)>(allocated_space);
}
void BackupOriginCode(uint64_t* origin, uint64_t* backup, int size){
    // origin -> backup
    char before_hex[100], after_hex[100];  // 16바이트는 32문자 + null (각 바이트당 2문자 + 공백 포함)
    before_hex[0] = '\0',after_hex[0]='\0';
    unsigned char* opcode_list = reinterpret_cast<unsigned char*>(origin);
    unsigned char* trampoline = reinterpret_cast<unsigned char*>(backup);
    LOGI("Trampoline Addr : 0x%lx ",trampoline);
    LOGI("Origin Code");
    print_hex_bytes(origin, 16);
    for(int i=0;i<16; i++){
        trampoline[i] = opcode_list[i];
    }
    reinterpret_cast<uint32_t *>(trampoline+16)[0] = LDR;
    reinterpret_cast<uint32_t *>(trampoline+16)[1] = BR;
    *reinterpret_cast<uint64_t *>(reinterpret_cast<uint32_t *>(trampoline + 16) + 2) = reinterpret_cast<uint64_t>(
            origin + 2);

    LOGI("Trampoline Code");
    print_hex_bytes(backup, 32);
}
void InlineHook(uint64_t *origin, uint64_t *replace, uint64_t *pInt) {
    // replace -> origin
    reinterpret_cast<uint32_t *>(origin)[0] = LDR;
    reinterpret_cast<uint32_t *>(origin)[1] = BR;
    *reinterpret_cast<uint64_t *>(reinterpret_cast<uint32_t *>(origin) + 2) = reinterpret_cast<uint64_t>(replace);

    LOGI("Hooked Hex");
    print_hex_bytes(origin, 32);
}

