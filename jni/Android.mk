LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := melyze

LOCAL_CFLAGS := -w

LOCAL_SRC_FILES := 	\
	../src/bitswap.c 	\
	../src/cli.c 		\
	../src/main.c 		\
	../src/melyze.c 	\
	../src/value.c

include $(BUILD_EXECUTABLE)
