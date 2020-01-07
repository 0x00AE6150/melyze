LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := target

APP_PLATFORM := android-19

LOCAL_SRC_FILES := ../main.cpp

include $(BUILD_EXECUTABLE)
