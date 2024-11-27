# NDK Build Path
NDK_BUILD := /home/aaron/share-aaron-1/android-ndk-r10e/ndk-build

# Source Files
SRC := firehose_protocol.c qfirehose.c sahara.c usb_linux.c stream_download_protocol.c md5.c usb2tcp.c

# Compiler Flags
CFLAGS += -Wall -Wextra -Werror -O1
LDFLAGS += -lpthread -ldl

# Default Compiler
CC := gcc

# Check for Cross Compile on ARM64 target
ifneq ($(MAKECMDGOALS),arm64)
CROSS_COMPILE :=
else
CROSS_COMPILE := aarch64-linux-gnu-
CC := ${CROSS_COMPILE}gcc
endif

# Linux Target
linux: clean
	${CC} ${CFLAGS} ${SRC} -o wfirehose ${LDFLAGS}

# Android Target
android: clean
	rm -rf android
	$(NDK_BUILD) V=0 APP_BUILD_SCRIPT=Android.mk NDK_PROJECT_PATH=`pwd` NDK_DEBUG=0 APP_ABI='armeabi-v7a,arm64-v8a' APP_PLATFORM=android-22
	rm -rf obj
	mv libs android

# ARM64 Target
arm64: clean
	${CC} ${CFLAGS} ${SRC} -o wfirehose_arm64 ${LDFLAGS}

# Clean Target
clean:
	rm -rf wfirehose wfirehose_arm64 obj libs usb2tcp *~
