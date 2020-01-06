UNAME := $(shell uname)

INCDIR = ../../ags/Engine ../../ags/Common
SOURCES =
OBJS = libs/clip/clip.o libs/clip/image.o
LIBDIR =
CC = gcc
CXX = g++
CFLAGS = -fPIC -fvisibility=hidden -O2 -g -Wall -std=c++11
LIBS = -lstdc++

ifeq ($(UNAME), Darwin)
SOURCES += libs/clip/clip_osx.mm
TARGET = libagsimgui.dylib
CFLAGS += -DMAC_VERSION -framework Cocoa
else
TARGET = libagsimgui.so
CFLAGS += -DLINUX_VERSION
OBJS += libs/clip/clip_x11.o
endif

CXXFLAGS = $(CFLAGS)

include Makefile-objs


CFLAGS   := $(addprefix -I,$(INCDIR)) $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)
ASFLAGS  := $(CFLAGS) $(ASFLAGS)


.PHONY: clean

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJS)
	@$(CC) -shared -dynamiclib -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

%.o: %.c
	@echo $@
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	@echo $@
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@rm -f $(TARGET) $(OBJS)
