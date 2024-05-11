cmake_minimum_required(VERSION 3.22)

PROJECT(libgrpc_simplify VERSION 1.0.0 LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED TRUE)

MESSAGE(STATUS "===== ${PROJECT_NAME} ====")
MESSAGE(STATUS "--------------------------------------------------")

# 设置 CMAKE_BUILD_TYPE
IF ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

#设置Release版本的编译参数
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# 编译器参数
IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    ADD_COMPILE_OPTIONS(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
    IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_COMPILE_OPTIONS(-g)
    ENDIF()
    IF ("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 9)
        SET(GCC8OL TRUE)
    ENDIF()
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # note: not working for current version because C++17 support is poor
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -inline-forceinline")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    ADD_COMPILE_OPTIONS(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
    IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_COMPILE_OPTIONS(-g)
    ELSEIF ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        # C99 是允许使用的，但 Clang 依然可能会给出警告，因此屏蔽
        ADD_COMPILE_OPTIONS(-Wno-c99-extensions)
    ENDIF()
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    ADD_COMPILE_OPTIONS(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
    IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_COMPILE_OPTIONS(-g)
    ELSEIF ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        # C99 是允许使用的，但 Clang 依然可能会给出警告，因此屏蔽
        ADD_COMPILE_OPTIONS(-Wno-c99-extensions)
    ENDIF()
ELSE()
    MESSAGE(WARNING "Unsupported CXX compiler ${CMAKE_CXX_COMPILER_ID}")
ENDIF()

# 通用外部库依赖导入函数
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/cmake/PrintFlags.cmake)

# glog
FIND_PACKAGE(libglog REQUIRED)
PRINT_LIB_INFO("libglog" "${libglog_INCLUDE_DIRS}" "${libglog_LIBRARIES}")

LINK_DIRECTORIES(${gRPC_DIR}/lib)

# protobuf
FIND_PACKAGE(Protobuf CONFIG REQUIRED)
SET(libProtobuf_LINK_LIBRARIES protobuf)
IF (NOT TARGET protobuf::protoc)
    MESSAGE(FATAL_ERROR "protobuf compiler not found")
ENDIF()

# libgrpc
SET(libgRPC_INCLUDE_DIRS ${gRPC_DIR}/include)
SET(libgRPC_LINK_LIBRARIES gRPC::grpc gRPC::grpc++ gRPC::grpc++_reflection)

FIND_PACKAGE(gRPC CONFIG REQUIRED)
PRINT_LIB_INFO("libgRPC" "${libgRPC_INCLUDE_DIRS}" "${libgRPC_LINK_LIBRARIES}")

# include 3rd headers
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rd/include ${CMAKE_SOURCE_DIR}/stub)

# windows 下额外的库文件
IF (WIN32)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .dll .dll.a .a)
    FIND_LIBRARY(WSOCk "wsock32")
    FIND_LIBRARY(IPHLPAPI "iphlpapi")

    PRINT_LIB_INFO("Win32lib" "" "${WSOCk} ${IPHLPAPI}")
ENDIF()

# 打包
SET(CPACK_GENERATOR "ZIP")
SET(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

IF (NOT "${PACKAGE_NAME_SUFFIX}" STREQUAL "")
    MESSAGE(STATUS "Append '${PACKAGE_NAME_SUFFIX}' to the package file name")
    SET(CPACK_ARCHIVE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${PACKAGE_NAME_SUFFIX}")
ENDIF()

INCLUDE(CPack)

MESSAGE(STATUS "--------------------------------------------------")

ADD_SUBDIRECTORY(server_tests)
ADD_SUBDIRECTORY(client_tests)


