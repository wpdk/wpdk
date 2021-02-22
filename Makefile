#
#  BSD LICENSE
#
#  Copyright (c) Intel Corporation.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in
#      the documentation and/or other materials provided with the
#      distribution.
#    * Neither the name of Intel Corporation nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

SPDK_ROOT_DIR := $(abspath $(CURDIR)/..)
include $(SPDK_ROOT_DIR)/mk/spdk.common.mk

.PHONY: all clean install uninstall

WPDK_CFLAGS =

ifeq ($(CONFIG_DEBUG),y)
WPDK_OPTS += --buildtype=debug
endif

WPDK_OPTS += -Dmachine=$(TARGET_ARCHITECTURE)

ifneq ($(CONFIG_CROSS_PREFIX),)
WPDK_OPTS += --cross-file $(SPDK_ROOT_DIR)/wpdk/config/$(CONFIG_CROSS_PREFIX)
endif

ifneq ($(CC_TYPE),clang)
WPDK_CFLAGS += -fPIC
endif

ifeq ($(CONFIG_WERROR),y)
WPDK_CFLAGS += -Werror
else
WPDK_CFLAGS += -Wno-error
endif

ifeq ($(CONFIG_CET),y)
WPDK_CFLAGS += -fcf-protection
WPDK_LDFLAGS += -fcf-protection
endif

# Force-disable scan-build
SUB_CC = $(patsubst %ccc-analyzer,$(DEFAULT_CC),$(CC))
SUB_CXX = $(patsubst %c++-analyzer,$(DEFAULT_CXX),$(CXX))

MESON_PREFIX = $(SPDK_ROOT_DIR)/wpdk/build

# Some ninja versions come with a (broken?) jobserver which defaults to use
# only 1 thread for the build. We workaround this by specifying -j to ninja
# with the same value as top-makefile. This is OK as long as WPDK is not built
# in parralel with anything else, which is the case for now.
ifeq ($(MAKE_PID),)
MAKE_PID := $(shell echo $$PPID)
endif

MAKE_NUMJOBS := $(shell ps T | sed -nE 's/\s*$(MAKE_PID)\s.* (-j|--jobs=)( *[0-9]+).*/\1\2/p')

all: $(SPDK_ROOT_DIR)/wpdk/build-tmp
	$(Q)env -u MAKEFLAGS ninja -C $(SPDK_ROOT_DIR)/wpdk/build-tmp $(MAKE_NUMJOBS)
	$(Q)env -u MAKEFLAGS meson install -C $(SPDK_ROOT_DIR)/wpdk/build-tmp --only-changed > /dev/null;

$(SPDK_ROOT_DIR)/wpdk/build-tmp: $(SPDK_ROOT_DIR)/mk/cc.mk $(SPDK_ROOT_DIR)/include/spdk/config.h
	$(Q)rm -rf $(SPDK_ROOT_DIR)/wpdk/build-tmp
	$(Q)cd "$(SPDK_ROOT_DIR)/wpdk"; CC="$(SUB_CC)" CXX="$(SUB_CXX)" meson --prefix="$(MESON_PREFIX)" \
		--libdir lib -Dc_args="$(WPDK_CFLAGS)" -Dc_link_args="$(WPDK_LDFLAGS)" $(WPDK_OPTS) build-tmp

clean:
	$(Q)rm -rf $(SPDK_ROOT_DIR)/wpdk/build-tmp

install:
	@:

uninstall:
	@:
