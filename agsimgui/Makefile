UNAME := $(shell uname)

INCDIR = ../../ags/Engine ../../ags/Common imgui/examples/libs/gl3w
LIBDIR =
CC = gcc
CXX = g++
CFLAGS = -fPIC -fvisibility=hidden -O2 -g -Wall -std=c++11
LIBS = -lstdc++ -lGL -ldl

ifeq ($(UNAME), Darwin)
TARGET = libagsimgui.dylib
CFLAGS += -DMAC_VERSION
else
TARGET = libagsimgui.so
CFLAGS += -DLINUX_VERSION
endif

CXXFLAGS = $(CFLAGS)

include Makefile-objs


CFLAGS   := $(addprefix -I,$(INCDIR)) $(CFLAGS)
CXXFLAGS := $(CFLAGS) $(CXXFLAGS)
ASFLAGS  := $(CFLAGS) $(ASFLAGS)

CXXFLAGS += -DIMGUI_IMPL_OPENGL_LOADER_GL3W

.PHONY: clean

all: $(TARGET)

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
