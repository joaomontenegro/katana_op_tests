# See README for instructions on how to use this Makefile

# The output objects dir
OBJDIR = ./out

OUTFILENAME = TestOp.so
OUTFILEPATH = $(OBJDIR)/$(OUTFILENAME)

# Plugin sources and includes
PLUGIN_SRC = $(KATANA_HOME)/plugin_apis/src
SOURCES = src/TestParentOp.cpp
SOURCES += src/TestChildOp.cpp
SOURCES += src/PointCloudOp.cpp
SOURCES += src/PonyfierOp.cpp
SOURCES += src/register.cpp	

# Directly suck in the FnAttribute/FnGeolibOp/FnGeolibUtil/pystring cpp files
SOURCES += $(shell find $(PLUGIN_SRC)/FnAttribute -name \*.cpp)
SOURCES += $(shell find $(PLUGIN_SRC)/FnGeolib/op -name \*.cpp)
SOURCES += $(shell find $(PLUGIN_SRC)/FnGeolib/util -name \*.cpp)
SOURCES += $(shell find $(PLUGIN_SRC)/FnPluginManager -name \*.cpp)
SOURCES += $(PLUGIN_SRC)/FnGeolibServices/client/FnGeolibCookInterfaceUtilsService.cpp
SOURCES += $(shell find $(PLUGIN_SRC)/pystring -name \*.cpp)

INCLUDES = -Iinclude -I$(KATANA_HOME)/plugin_apis/include

CXX=/workspace/Thirdparty/GCC/4.1.2/bin/linux-64-x86-release-410/bin/g++-4.1

CFLAGS=-ffloat-store
CXXFLAGS=-Wall -Wextra -Wshadow -Wconversion -Wcast-qual -fPIC -DPIC

# Object files and flags
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))

LIBPATH = -L$(KATANA_HOME)/bin/Geolib3/internal/CEL
LIBS = -lCEL

# Targets:
all: $(OUTFILEPATH)

$(OUTFILEPATH): $(OBJS)
	@echo "  Compiling TestOp "
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBPATH) $(LIBS) -shared -o $(OUTFILEPATH) -Wl,-soname,$(OUTFILENAME)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "  Cleaning TestOp"
	@rm -rf $(OBJDIR)

install: $(OUTFILEPATH)
	@echo " Installing to $(KATANA_HOME)/bin/Geolib3/Ops "
	@cp $(OUTFILEPATH) $(KATANA_HOME)/bin/Geolib3/Ops
        
