# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/debian/Desktop/GetObject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/debian/Desktop/GetObject

# Include any dependencies generated for this target.
include CMakeFiles/GetObject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/GetObject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GetObject.dir/flags.make

CMakeFiles/GetObject.dir/driver.o: CMakeFiles/GetObject.dir/flags.make
CMakeFiles/GetObject.dir/driver.o: driver.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/debian/Desktop/GetObject/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/GetObject.dir/driver.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/GetObject.dir/driver.o -c /home/debian/Desktop/GetObject/driver.cpp

CMakeFiles/GetObject.dir/driver.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GetObject.dir/driver.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/debian/Desktop/GetObject/driver.cpp > CMakeFiles/GetObject.dir/driver.i

CMakeFiles/GetObject.dir/driver.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GetObject.dir/driver.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/debian/Desktop/GetObject/driver.cpp -o CMakeFiles/GetObject.dir/driver.s

CMakeFiles/GetObject.dir/driver.o.requires:
.PHONY : CMakeFiles/GetObject.dir/driver.o.requires

CMakeFiles/GetObject.dir/driver.o.provides: CMakeFiles/GetObject.dir/driver.o.requires
	$(MAKE) -f CMakeFiles/GetObject.dir/build.make CMakeFiles/GetObject.dir/driver.o.provides.build
.PHONY : CMakeFiles/GetObject.dir/driver.o.provides

CMakeFiles/GetObject.dir/driver.o.provides.build: CMakeFiles/GetObject.dir/driver.o

# Object files for target GetObject
GetObject_OBJECTS = \
"CMakeFiles/GetObject.dir/driver.o"

# External object files for target GetObject
GetObject_EXTERNAL_OBJECTS =

GetObject: CMakeFiles/GetObject.dir/driver.o
GetObject: CMakeFiles/GetObject.dir/build.make
GetObject: CMakeFiles/GetObject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable GetObject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GetObject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GetObject.dir/build: GetObject
.PHONY : CMakeFiles/GetObject.dir/build

CMakeFiles/GetObject.dir/requires: CMakeFiles/GetObject.dir/driver.o.requires
.PHONY : CMakeFiles/GetObject.dir/requires

CMakeFiles/GetObject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GetObject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GetObject.dir/clean

CMakeFiles/GetObject.dir/depend:
	cd /home/debian/Desktop/GetObject && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/debian/Desktop/GetObject /home/debian/Desktop/GetObject /home/debian/Desktop/GetObject /home/debian/Desktop/GetObject /home/debian/Desktop/GetObject/CMakeFiles/GetObject.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GetObject.dir/depend

