# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/build

# Include any dependencies generated for this target.
include CMakeFiles/Strbuf.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Strbuf.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Strbuf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Strbuf.dir/flags.make

CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.o: CMakeFiles/Strbuf.dir/flags.make
CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.o: /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/strbuf/ZhuBin.c
CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.o: CMakeFiles/Strbuf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.o -MF CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.o.d -o CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.o -c /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/strbuf/ZhuBin.c

CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/strbuf/ZhuBin.c > CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.i

CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/strbuf/ZhuBin.c -o CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.s

CMakeFiles/Strbuf.dir/strbuf/test.cpp.o: CMakeFiles/Strbuf.dir/flags.make
CMakeFiles/Strbuf.dir/strbuf/test.cpp.o: /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/strbuf/test.cpp
CMakeFiles/Strbuf.dir/strbuf/test.cpp.o: CMakeFiles/Strbuf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Strbuf.dir/strbuf/test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Strbuf.dir/strbuf/test.cpp.o -MF CMakeFiles/Strbuf.dir/strbuf/test.cpp.o.d -o CMakeFiles/Strbuf.dir/strbuf/test.cpp.o -c /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/strbuf/test.cpp

CMakeFiles/Strbuf.dir/strbuf/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Strbuf.dir/strbuf/test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/strbuf/test.cpp > CMakeFiles/Strbuf.dir/strbuf/test.cpp.i

CMakeFiles/Strbuf.dir/strbuf/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Strbuf.dir/strbuf/test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/strbuf/test.cpp -o CMakeFiles/Strbuf.dir/strbuf/test.cpp.s

# Object files for target Strbuf
Strbuf_OBJECTS = \
"CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.o" \
"CMakeFiles/Strbuf.dir/strbuf/test.cpp.o"

# External object files for target Strbuf
Strbuf_EXTERNAL_OBJECTS =

/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/Strbuf: CMakeFiles/Strbuf.dir/strbuf/ZhuBin.c.o
/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/Strbuf: CMakeFiles/Strbuf.dir/strbuf/test.cpp.o
/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/Strbuf: CMakeFiles/Strbuf.dir/build.make
/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/Strbuf: lib/libgtest_main.a
/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/Strbuf: lib/libgtest.a
/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/Strbuf: CMakeFiles/Strbuf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/Strbuf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Strbuf.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -D TEST_TARGET=Strbuf -D TEST_EXECUTABLE=/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/Strbuf -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/build -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=Strbuf_TESTS -D CTEST_FILE=/home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/build/Strbuf[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /usr/share/cmake-3.28/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/Strbuf.dir/build: /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/Strbuf
.PHONY : CMakeFiles/Strbuf.dir/build

CMakeFiles/Strbuf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Strbuf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Strbuf.dir/clean

CMakeFiles/Strbuf.dir/depend:
	cd /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/build /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/build /home/zhubin/Strbuf动态扩容缓冲区/StrbufTest/build/CMakeFiles/Strbuf.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Strbuf.dir/depend
