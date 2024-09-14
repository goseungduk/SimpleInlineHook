#include <jni.h>
#include <stdio.h>
#include <dlfcn.h>
#include <malloc.h>
#include "Logger.h"
#include "SimpleInlineHook.hpp"

int64_t (*org_func)(const char* path, const char* mode);
int64_t hook_func(const char* path, const char* mode){
    LOGI("this is custom fopen : %s", path);
    return org_func(path, mode);
}

extern "C" JNIEXPORT void JNICALL Java_com_sdssd_sdaad_MainActivity_hookcheck(JNIEnv *env, jobject thiz) {
    fopen("/proc/self/maps","rw");
}
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("Original fopen : %lx\n",(uintptr_t)fopen("/proc/self/maps","rw"));
    AllocateBackupSpace(&org_func);
    rwx_protect(reinterpret_cast<uint64_t *>(fopen)); // libc.so:make RWX fopen page
    rwx_protect(reinterpret_cast<uint64_t *>(org_func)); // libtester.so:make RWX trampoline page
    BackupOriginCode(reinterpret_cast<uint64_t *>(fopen), reinterpret_cast<uint64_t *>(org_func), 16);
    InlineHook(reinterpret_cast<uint64_t *>(fopen), reinterpret_cast<uint64_t *>(hook_func),
               reinterpret_cast<uint64_t *>(org_func));
    return JNI_VERSION_1_6;
}