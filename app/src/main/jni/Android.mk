LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS := -Wno-error=format-security -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS := -Wno-error=format-security -ffunction-sections -fdata-sections -w -Werror -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS += -Wl,--gc-sections, -llog
LOCAL_ARM_MODE := arm

LOCAL_MODULE := tester
LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)
LOCAL_SRC_FILES := tester.cpp \
					SimpleInlineHook.cpp
LOCAL_LDLIBS := -llog -landroid
include $(BUILD_SHARED_LIBRARY)
