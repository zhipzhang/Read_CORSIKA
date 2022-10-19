# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/home/zhipz/work/scripts

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/home/zhipz/work/scripts/build

# Include any dependencies generated for this target.
include CMakeFiles/Read_Corsika.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Read_Corsika.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Read_Corsika.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Read_Corsika.dir/flags.make

CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.o: CMakeFiles/Read_Corsika.dir/flags.make
CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.o: ../src/get_photons.cpp
CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.o: CMakeFiles/Read_Corsika.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/home/zhipz/work/scripts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.o -MF CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.o.d -o CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.o -c /data/home/zhipz/work/scripts/src/get_photons.cpp

CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/home/zhipz/work/scripts/src/get_photons.cpp > CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.i

CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/home/zhipz/work/scripts/src/get_photons.cpp -o CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.s

# Object files for target Read_Corsika
Read_Corsika_OBJECTS = \
"CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.o"

# External object files for target Read_Corsika
Read_Corsika_EXTERNAL_OBJECTS =

../compiled/bin/Read_Corsika: CMakeFiles/Read_Corsika.dir/src/get_photons.cpp.o
../compiled/bin/Read_Corsika: CMakeFiles/Read_Corsika.dir/build.make
../compiled/bin/Read_Corsika: ../compiled/lib/libclass.so
../compiled/bin/Read_Corsika: /data/home/zhipz/hessioxxx/lib/libhessio.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libCore.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libImt.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libRIO.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libNet.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libHist.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libGraf.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libGraf3d.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libGpad.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libROOTDataFrame.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libTree.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libTreePlayer.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libRint.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libPostscript.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libMatrix.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libPhysics.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libMathCore.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libThread.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libMultiProc.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libROOTVecOps.so
../compiled/bin/Read_Corsika: /data/home/zhipz/root/lib/libMinuit.so
../compiled/bin/Read_Corsika: CMakeFiles/Read_Corsika.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/home/zhipz/work/scripts/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../compiled/bin/Read_Corsika"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Read_Corsika.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Read_Corsika.dir/build: ../compiled/bin/Read_Corsika
.PHONY : CMakeFiles/Read_Corsika.dir/build

CMakeFiles/Read_Corsika.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Read_Corsika.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Read_Corsika.dir/clean

CMakeFiles/Read_Corsika.dir/depend:
	cd /data/home/zhipz/work/scripts/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/home/zhipz/work/scripts /data/home/zhipz/work/scripts /data/home/zhipz/work/scripts/build /data/home/zhipz/work/scripts/build /data/home/zhipz/work/scripts/build/CMakeFiles/Read_Corsika.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Read_Corsika.dir/depend

