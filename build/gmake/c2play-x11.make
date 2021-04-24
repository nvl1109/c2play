# GNU Make project makefile autogenerated by Premake
ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifndef CC
  CC = gcc
endif

ifndef CXX
  CXX = g++
endif

ifndef AR
  AR = ar
endif

ifeq ($(config),debug)
  OBJDIR     = obj/Debug/c2play-x11
  TARGETDIR  = ../..
  TARGET     = $(TARGETDIR)/c2play-x11
  DEFINES   += -DX11 -DDEBUG
  INCLUDES  += -I../../src/Media -I../../src/UI -I../../src/UI/X11
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -g -std=c++11 -Wall
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -lavformat -lavcodec -lavutil -lpthread -lasound -lrt -lX11 -lEGL -lGLESv2 -lass
  LIBS      += 
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(ARCH) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),release)
  OBJDIR     = obj/Release/c2play-x11
  TARGETDIR  = ../..
  TARGET     = $(TARGETDIR)/c2play-x11
  DEFINES   += -DX11 -D
  INCLUDES  += -I../../src/Media -I../../src/UI -I../../src/UI/X11
  CPPFLAGS  += -MMD -MP $(DEFINES) $(INCLUDES)
  CFLAGS    += $(CPPFLAGS) $(ARCH) -O2 -std=c++11 -Wall
  CXXFLAGS  += $(CFLAGS) 
  LDFLAGS   += -s -lavformat -lavcodec -lavutil -lpthread -lasound -lrt -lX11 -lEGL -lGLESv2 -lass
  LIBS      += 
  RESFLAGS  += $(DEFINES) $(INCLUDES) 
  LDDEPS    += 
  LINKCMD    = $(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(ARCH) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJECTS := \
	$(OBJDIR)/main.o \
	$(OBJDIR)/Codec.o \
	$(OBJDIR)/Element.o \
	$(OBJDIR)/MediaPlayer.o \
	$(OBJDIR)/AmlCodec.o \
	$(OBJDIR)/AmlVideoSink.o \
	$(OBJDIR)/Buffer.o \
	$(OBJDIR)/AlsaAudioSink.o \
	$(OBJDIR)/Mutex.o \
	$(OBJDIR)/AudioCodec.o \
	$(OBJDIR)/MediaSourceElement.o \
	$(OBJDIR)/InPin.o \
	$(OBJDIR)/Exception.o \
	$(OBJDIR)/Pin.o \
	$(OBJDIR)/Thread.o \
	$(OBJDIR)/Image.o \
	$(OBJDIR)/LockedQueue.o \
	$(OBJDIR)/OutPin.o \
	$(OBJDIR)/SubtitleCodecElement.o \
	$(OBJDIR)/AmlWindow.o \
	$(OBJDIR)/InputDevice.o \
	$(OBJDIR)/Vector3.o \
	$(OBJDIR)/Matrix4.o \
	$(OBJDIR)/Compositor.o \
	$(OBJDIR)/QuadBatch.o \
	$(OBJDIR)/Window.o \
	$(OBJDIR)/QuadBatchProgram.o \
	$(OBJDIR)/Osd.o \
	$(OBJDIR)/Egl.o \
	$(OBJDIR)/Vector4.o \
	$(OBJDIR)/Texture2D.o \
	$(OBJDIR)/Vector2.o \
	$(OBJDIR)/GL.o \
	$(OBJDIR)/Shader.o \
	$(OBJDIR)/X11Window.o \

RESOURCES := \

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

.PHONY: clean prebuild prelink

all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking c2play-x11
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning c2play-x11
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(GCH): $(PCH)
	@echo $(notdir $<)
	-$(SILENT) cp $< $(OBJDIR)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
endif

$(OBJDIR)/main.o: ../../src/main.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Codec.o: ../../src/Media/Codec.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Element.o: ../../src/Media/Element.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/MediaPlayer.o: ../../src/Media/MediaPlayer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/AmlCodec.o: ../../src/Media/AmlCodec.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/AmlVideoSink.o: ../../src/Media/AmlVideoSink.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Buffer.o: ../../src/Media/Buffer.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/AlsaAudioSink.o: ../../src/Media/AlsaAudioSink.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Mutex.o: ../../src/Media/Mutex.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/AudioCodec.o: ../../src/Media/AudioCodec.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/MediaSourceElement.o: ../../src/Media/MediaSourceElement.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/InPin.o: ../../src/Media/InPin.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Exception.o: ../../src/Media/Exception.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Pin.o: ../../src/Media/Pin.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Thread.o: ../../src/Media/Thread.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Image.o: ../../src/Media/Image.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/LockedQueue.o: ../../src/Media/LockedQueue.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/OutPin.o: ../../src/Media/OutPin.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/SubtitleCodecElement.o: ../../src/Media/SubtitleCodecElement.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/AmlWindow.o: ../../src/UI/AmlWindow.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/InputDevice.o: ../../src/UI/InputDevice.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Vector3.o: ../../src/UI/Vector3.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Matrix4.o: ../../src/UI/Matrix4.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Compositor.o: ../../src/UI/Compositor.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/QuadBatch.o: ../../src/UI/QuadBatch.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Window.o: ../../src/UI/Window.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/QuadBatchProgram.o: ../../src/UI/QuadBatchProgram.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Osd.o: ../../src/UI/Osd.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Egl.o: ../../src/UI/Egl.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Vector4.o: ../../src/UI/Vector4.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Texture2D.o: ../../src/UI/Texture2D.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Vector2.o: ../../src/UI/Vector2.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/GL.o: ../../src/UI/GL.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Shader.o: ../../src/UI/Shader.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/X11Window.o: ../../src/UI/X11/X11Window.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
