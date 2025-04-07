# Makefile modified to meet assignment requirements.

# Makefile for System Programming 2 - Ex1

# C++ Compiler (ensure path is correct if g++ is not in PATH)
CXX := g++
# In your case, you might need the full path:
# CXX := C:/Strawberry/c/bin/g++.exe

# Compiler Flags
# -std=c++17 : Use C++17 standard
# -Wall       : Enable all warnings
# -Werror     : (Optional) Treat warnings as errors
# -g          : Add debugging information
CXXFLAGS := -std=c++17 -Wall -g
# CXXFLAGS += -Werror # Can be added for stricter compilation

# Preprocessor Flags (mainly for include paths)
# -I. : Add current directory to include search paths (for doctest.h)
CPPFLAGS := -I.

# Linker Flags - not currently needed
# LDFLAGS :=

# Shared source files (our "library")
SRCS_LIB := Graph.cpp Algorithms.cpp DataStructures.cpp
# Main source file
SRC_MAIN := main.cpp
# Test source file
SRC_TEST := tests.cpp

# Object files
OBJS_LIB := $(SRCS_LIB:.cpp=.o) # e.g., Graph.o Algorithms.o DataStructures.o
OBJ_MAIN := $(SRC_MAIN:.cpp=.o) # e.g., main.o
OBJ_TEST := $(SRC_TEST:.cpp=.o) # e.g., tests.o

# Executable names
EXEC_MAIN := main.exe
EXEC_TEST := runTests.exe

# Delete command (rm -f should work in MinGW, -f ignores errors if file not found)
# If it doesn't work, can try: RM := del /Q
RM := rm -f


# --- Targets ---

# The first target is the default (running 'make' without args executes this)
all: $(EXEC_MAIN) $(EXEC_TEST)

# 'Main' target as required by the assignment
Main: $(EXEC_MAIN)
    @echo "Running main program..."
    .\$(EXEC_MAIN)

# Build the main executable
$(EXEC_MAIN): $(OBJ_MAIN) $(OBJS_LIB)
    @echo "Linking main executable..."
    $(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# 'test' target as required by the assignment
test: $(EXEC_TEST)
    @echo "Running tests..."
    .\$(EXEC_TEST)

# Build the test executable
$(EXEC_TEST): $(OBJ_TEST) $(OBJS_LIB)
    @echo "Linking test executable..."
    $(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# 'valgrind' target as required by the assignment
# !!! Important: Valgrind is a Linux tool. This target only works if running make
# !!! in a Linux environment (like WSL) where valgrind is installed, and code compiled *for Linux*.
# !!! It will not work directly on Windows.
valgrind: $(EXEC_MAIN)
    @echo "Running valgrind (requires Linux environment and valgrind installed)..."
    valgrind --leak-check=full .\$(EXEC_MAIN)

# Generic rule for building object files (.o) from source files (.cpp)
%.o: %.cpp
    @echo "Compiling $<..."
    $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# 'clean' target as required by the assignment
clean:
    @echo "Cleaning up generated files..."
    $(RM) $(EXEC_MAIN) $(EXEC_TEST) *.o
    @echo "Cleanup finished."


# Define phony targets (prevents conflicts if files with these names exist)
.PHONY: all Main test valgrind clean
