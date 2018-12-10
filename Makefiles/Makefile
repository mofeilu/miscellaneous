srcdir = .
include mkfiles/Make.common

CPP_SRCS := $(shell find $(topdir)/src -name "*.cpp")
CU_SRCS := $(shell find $(topdir)/src -name "*.cu")
CPP_OBJS := $(patsubst %.cpp, %.$(obj-suffix), $(CPP_SRCS))
CU_OBJS := $(patsubst %.cu, %.$(obj-suffix), $(CU_SRCS))


CPP_HEADERS := $(shell find $(topdir)/src -name "*.hpp")


# int main()
main_src :=$(shell find $(topdir)/main -name "*.cpp")
main_OBJ := $(patsubst %.cpp, %.$(obj-suffix), $(main_src))

# for the unit test
TEST_SRCS :=$(shell find $(topdir)/unitTest -name "*.cpp")
TEST_OBJ := $(patsubst %.cpp, %.$(obj-suffix), $(TEST_SRCS))



all: bin/GSSTest  # bin/Gss

# generate python binding
.PHONY: install
install: #pyInterface/mobopy/swig/_cMobo.so
	@echo "generate GSS python binding"
	@echo "copying headers files needed to use libGSS.$(shared_libext)..."
	@mkdir -p lib/include; rm -f lib/include/*.h; cp -p  $(CPP_HEADERS) lib/include/.

bin/GSSTest: lib/libGSS.$(shared_libext) unit-test 
	@echo "Building test executable..."
	@echo -L$(Proj_home)/lib -lGSS $(LINK_LIBS) > lib/GSS_libs
	@$(MPICXX) $(CXXFLAGS) $(TEST_OBJ) -o bin/GSSTest -Bdynamic -Wl,-rpath,$(Proj_home)/lib -L$(Proj_home)/lib -lGSS $(LINK_LIBS)

bin/Gss: lib/libGSS.$(shared_libext) main 
	@echo "Building Gss  executable..."
	@$(MPICXX) $(CXXFLAGS) $(main_OBJ) -o bin/Gss -Bdynamic -Wl,-rpath,$(Proj_home)/lib -L$(Proj_home)/lib -lGSS $(LINK_LIBS)


#$(main_OBJ): src/main/main.cpp
#	@$(MAKE) -C main

#$(TEST_OBJ): $(TEST_SRCS)
#	@echo "Building test objs: $(TEST_OBJ)..."
#	@$(MAKE) -C unitTest #$(MAKECMDGOALS)


lib/libGSS.$(shared_libext): src
	@echo "Linking libGSS.$(shared_libext) ..."
	@mkdir -p lib; mkdir -p bin
	@(export TMP_PATH=$(LD_LIBRARY_PATH); \
	  export LD_LIBRARY_PATH=$(MY_LD_LIB_PATH); \
	  $(MPICXX) -shared $(CXXFLAGS) $(CU_OBJS) $(CPP_OBJS) -o $@ $(LDFLAGS); \
    export LD_LIBRARY_PATH=$(TMP_PATH); \
	  export TMP_PATH=; )
#@echo "copying headers files needed to use libMobo.$(shared_libext)..."
#@mkdir -p lib/include; rm -f lib/include/*.h; cp -p  $(CPP_HEADERS) lib/include/.  # cp -p will not change the time stamp of origional files

# generate SCMG swig shared lib
#pyInterface/gsspy/swig/_cGSS.so: lib/libGSS.$(shared_libext)
#	@echo "copying headers files needed to use libGSS.$(shared_libext)..."
#	@mkdir -p lib/include; rm -f lib/include/*.h; cp -p  $(CPP_HEADERS) lib/include/.  # cp -p will not change the time stamp of origional files
#	@$(MAKE) --no-print-directory -C pyInterface/gsspy/swig   # call makefile in folder swig

# ------------------------------------------------------------------------------------------------------



clean:src unit-test
	@rm -f -r lib/* bin/*
	@find . -name "*gcov" -type f -delete
	@find . -name "*gcno" -type f -delete
	@find . -name "*gcda" -type f -delete

distclean: src unit-test
	@rm -f -r lib/* bin/*
	@find . -name "*gcov" -type f -delete
	@find . -name "*gcno" -type f -delete
	@find . -name "*gcda" -type f -delete


.PHONY: src
src:
	@$(MAKE) --no-print-directory -C src $(MAKECMDGOALS)
	
.PHONY: unit-test
unit-test:
	@$(MAKE) -C unitTest $(MAKECMDGOALS)

.PHONY: main
main:
	@$(MAKE) -C main $(MAKECMDGOALS)

include mkfiles/Make.rules
