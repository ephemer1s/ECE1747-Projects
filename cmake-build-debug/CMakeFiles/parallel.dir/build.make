# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = H:\Code\ECE1747\ECE1747-Projects

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = H:\Code\ECE1747\ECE1747-Projects\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/parallel.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/parallel.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/parallel.dir/flags.make

CMakeFiles/parallel.dir/ParallelMazeSolver.cpp.obj: CMakeFiles/parallel.dir/flags.make
CMakeFiles/parallel.dir/ParallelMazeSolver.cpp.obj: ../ParallelMazeSolver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=H:\Code\ECE1747\ECE1747-Projects\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/parallel.dir/ParallelMazeSolver.cpp.obj"
	D:\Mingw\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\parallel.dir\ParallelMazeSolver.cpp.obj -c H:\Code\ECE1747\ECE1747-Projects\ParallelMazeSolver.cpp

CMakeFiles/parallel.dir/ParallelMazeSolver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/parallel.dir/ParallelMazeSolver.cpp.i"
	D:\Mingw\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E H:\Code\ECE1747\ECE1747-Projects\ParallelMazeSolver.cpp > CMakeFiles\parallel.dir\ParallelMazeSolver.cpp.i

CMakeFiles/parallel.dir/ParallelMazeSolver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/parallel.dir/ParallelMazeSolver.cpp.s"
	D:\Mingw\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S H:\Code\ECE1747\ECE1747-Projects\ParallelMazeSolver.cpp -o CMakeFiles\parallel.dir\ParallelMazeSolver.cpp.s

# Object files for target parallel
parallel_OBJECTS = \
"CMakeFiles/parallel.dir/ParallelMazeSolver.cpp.obj"

# External object files for target parallel
parallel_EXTERNAL_OBJECTS =

parallel.exe: CMakeFiles/parallel.dir/ParallelMazeSolver.cpp.obj
parallel.exe: CMakeFiles/parallel.dir/build.make
parallel.exe: CMakeFiles/parallel.dir/linklibs.rsp
parallel.exe: CMakeFiles/parallel.dir/objects1.rsp
parallel.exe: CMakeFiles/parallel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=H:\Code\ECE1747\ECE1747-Projects\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable parallel.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\parallel.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/parallel.dir/build: parallel.exe
.PHONY : CMakeFiles/parallel.dir/build

CMakeFiles/parallel.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\parallel.dir\cmake_clean.cmake
.PHONY : CMakeFiles/parallel.dir/clean

CMakeFiles/parallel.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" H:\Code\ECE1747\ECE1747-Projects H:\Code\ECE1747\ECE1747-Projects H:\Code\ECE1747\ECE1747-Projects\cmake-build-debug H:\Code\ECE1747\ECE1747-Projects\cmake-build-debug H:\Code\ECE1747\ECE1747-Projects\cmake-build-debug\CMakeFiles\parallel.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/parallel.dir/depend

