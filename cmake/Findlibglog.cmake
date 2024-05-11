# - Try to find glog
# Once done, this will define
#
#  libglog_FOUND - system has libglog installed
#  libglog_INCLUDE_DIRS - the libglog include directories
#  libglog_LIBRARIES - link these to use libglog
#
# The user may wish to set, in the CMake GUI or otherwise, this variable:
#  libglog_DIR - path to start searching for the module

SET(libglog_DIR
        "${libglog_DIR}"
        CACHE
        PATH
        "Where to start looking for this component.")

UNSET(libglog_INCLUDE_DIR CACHE)
UNSET(libglog_LIBRARY CACHE)

IF (WIN32)
    FIND_PATH(libglog_INCLUDE_DIR
            NAMES
            glog/logging.h
            PATHS
            "."
            HINTS
            ${libglog_DIR}
            PATH_SUFFIXES
            include)

    FIND_LIBRARY(libglog_LIBRARY
            NAMES
            glog
            PATHS
            "."
            HINTS
            ${libglog_DIR}
            PATH_SUFFIXES
            lib64
            lib
            x86_64-linux-gnu)

ELSE ()
    FIND_PATH(libglog_INCLUDE_DIR
            NAMES
            glog/logging.h
            PATHS
            "/usr"
            "/usr/local"
            HINTS
            ${libglog_DIR}
            PATH_SUFFIXES
            include)

    FIND_LIBRARY(libglog_LIBRARY
            NAMES
            glog
            PATHS
            "/usr"
            "/usr/lib"
            "/usr/lib64"
            "/usr/local/lib"
            "/usr/local/lib64"
            HINTS
            ${libglog_DIR}
            PATH_SUFFIXES
            lib64
            lib
            x86_64-linux-gnu)
ENDIF ()

MARK_AS_ADVANCED(libglog_INCLUDE_DIR
        libglog_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libglog
        DEFAULT_MSG
        libglog_INCLUDE_DIR
        libglog_LIBRARY)

IF (libglog_FOUND)
    ADD_DEFINITIONS(-DGLOG_USE_GLOG_EXPORT)
    ADD_DEFINITIONS(-DGLOG_NO_ABBREVIATED_SEVERITIES)
    SET(libglog_INCLUDE_DIRS
            "${libglog_INCLUDE_DIR}")
    # Add any dependencies here
    SET(libglog_LIBRARIES
            "${libglog_LIBRARY}")
    # Add any dependencies here
    MARK_AS_ADVANCED(libglog_DIR)
ENDIF ()
