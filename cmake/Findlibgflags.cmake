# - Try to find glog
# Once done, this will define
#
#  libgflags_FOUND - system has libgflags installed
#  libgflags_INCLUDE_DIRS - the libgflags include directories
#  libgflags_LIBRARIES - link these to use libgflags
#
# The user may wish to set, in the CMake GUI or otherwise, this variable:
#  libgflags_DIR - path to start searching for the module

SET(libgflags_DIR
        "${libgflags_DIR}"
        CACHE
        PATH
        "Where to start looking for this component.")

UNSET(libgflags_INCLUDE_DIR CACHE)
UNSET(libgflags_LIBRARY CACHE)

IF (WIN32)
    FIND_PATH(libgflags_INCLUDE_DIR
            NAMES
            gflags/gflags.h
            PATHS
            "."
            HINTS
            ${libgflags_DIR}
            PATH_SUFFIXES
            include)

    FIND_LIBRARY(libgflags_LIBRARY
            NAMES
            gflags_nothreads
            PATHS
            "."
            HINTS
            ${libgflags_DIR}
            PATH_SUFFIXES
            lib64
            lib
            x86_64-linux-gnu)

ELSE ()
    FIND_PATH(libgflags_INCLUDE_DIR
            NAMES
            gflags/gflags.h
            PATHS
            "/usr"
            "/usr/local"
            HINTS
            ${libgflags_DIR}
            PATH_SUFFIXES
            include)

    FIND_LIBRARY(libgflags_LIBRARY
            NAMES
            gflags_nothreads
            PATHS
            "/usr"
            "/usr/lib"
            "/usr/lib64"
            "/usr/local/lib"
            "/usr/local/lib64"
            HINTS
            ${libgflags_DIR}
            PATH_SUFFIXES
            x86_64-linux-gnu)
ENDIF ()

MARK_AS_ADVANCED(libgflags_INCLUDE_DIR
        libgflags_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libgflags
        DEFAULT_MSG
        libgflags_INCLUDE_DIR
        libgflags_LIBRARY)

IF (libgflags_FOUND)
    SET(libgflags_INCLUDE_DIRS
            "${libgflags_INCLUDE_DIR}")
    # Add any dependencies here
    SET(libgflags_LIBRARIES
            "${libgflags_LIBRARY}")
    # Add any dependencies here
    MARK_AS_ADVANCED(libgflags_DIR)
ENDIF ()
