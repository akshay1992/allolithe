cmake_minimum_required (VERSION 2.8)
project(allolithe)

include(ExternalProject)

message(STATUS "==== Configuring allolithe")

#  -------------------------- Collate all the files -------------------------- 
file( GLOB_RECURSE ALLOLITHE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
file( GLOB_RECURSE ALLOLITHE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/allolithe/*)
set(ALLOLITHE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

# -------------------------- Create the allolithe library -------------------------- 
add_library( allolithe ${ALLOLITHE_SOURCE})

# -------------------------- Get Allocore dependency -------------------------- 
set(ALLOCORE_LIBRARY "allocore${DEBUG_SUFFIX}")
get_target_property(ALLOCORE_DEP_INCLUDE_DIRS allocore${DEBUG_SUFFIX} ALLOCORE_DEP_INCLUDE_DIRS)

# -------------------------- Build and use Lithe -------------------------- 
# set(LITHE_DONT_BUILD_TESTS true)
# ExternalProject_Add(
	# Lithe
	# GIT_REPOSITORY "git@github.com:akshay1992/Lithe.git"
  	# GIT_TAG "master"
    # UPDATE_DISCONNECTED 1	# Don't update everytime you build

    # PREFIX ${CMAKE_BINARY_DIR}/Lithe
    # INSTALL_DIR ${CMAKE_BINARY_DIR}/Lithe
    # CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
# )
# ExternalProject_Get_Property(Lithe source_dir binary_dir)
link_directories( ${binary_dir})
# include_directories( allolithe PUBLIC ${source_dir})
# add_dependencies(allolithe Lithe)

# add_subdirectory(Lithe)

## -------------------------- Linking -------------------------- 

target_link_libraries( allolithe lithe_core ${ALLOCORE_LIBRARY} alloGLV)
target_include_directories( allolithe PUBLIC ${ALLOLITHE_INCLUDE_DIR} ${ALLOCORE_DEP_INCLUDE_DIRS})

# --------------------------- Tests  ---------------------------

# Add the unit tests -- this is excluded from the default make all target
# (to run unit tests use the command: `make test_allolithe`)
add_subdirectory(test EXCLUDE_FROM_ALL)

#  -------------------------- Documentation  -------------------------- 
add_subdirectory(doc)

# --------------------------- Examples  -------------------------- 
add_subdirectory(examples)
