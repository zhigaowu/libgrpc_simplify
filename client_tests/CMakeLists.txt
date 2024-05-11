cmake_minimum_required(VERSION 3.22)

PROJECT(client_tests VERSION 1.0.0 LANGUAGES CXX)

# 打印项目信息
MESSAGE(STATUS "===== ${PROJECT_NAME} ====")
MESSAGE(STATUS "--------------------------------------------------")
MESSAGE(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# 打印编译标志
PRINT_CPLUS_FLAGS()

# 打印空行，结束项目信息
MESSAGE(STATUS "--------------------------------------------------")

# 将所有 proto/*.proto 文件列出来
file(GLOB PROTO_SOURCES ${CMAKE_SOURCE_DIR}/proto/*.proto)

# 将所有 src/*.cpp 文件列出来
file(GLOB SIMPLIFY_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB MAIN_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# 所有的源文件+头文件需要在这个地方添加
ADD_EXECUTABLE(client_tests ${SIMPLIFY_SOURCES} ${PROTO_SOURCES} ${MAIN_SOURCES})

# 编译所有的 .proto 文件生成对应的 C++ 代码
protobuf_generate(
    TARGET client_tests
    IMPORT_DIRS ${CMAKE_SOURCE_DIR}/proto
    PROTOC_OUT_DIR "${CMAKE_SOURCE_DIR}/stub")

protobuf_generate(
    TARGET client_tests
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=\$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
    IMPORT_DIRS ${CMAKE_SOURCE_DIR}/proto
    PROTOC_OUT_DIR "${CMAKE_SOURCE_DIR}/stub")

TARGET_INCLUDE_DIRECTORIES(client_tests PRIVATE ${libgRPC_INCLUDE_DIRS} ${libglog_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)

TARGET_LINK_LIBRARIES(client_tests PRIVATE ${libgRPC_LINK_LIBRARIES} ${libProtobuf_LINK_LIBRARIES})
TARGET_LINK_LIBRARIES(client_tests PRIVATE ${libglog_LIBRARIES} pthread)

IF (WIN32)
    TARGET_LINK_LIBRARIES(client_tests PRIVATE wsock32 ws2_32 iphlpapi)
ENDIF()

install(TARGETS client_tests
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/install COMPONENT client_tests)

