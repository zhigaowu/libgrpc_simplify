/*
 * ==========================================================================
 *
 *       Filename:  test_cases.cpp
 *
 *    Description:  test cases
 *
 *        Version:  1.0
 *        Created:  2024-02-27 16:22:54
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  
 * ==========================================================================
 */

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "hello.grpc.pb.h"

#include "logger/glog_logger.h"

#include <thread>
#include <filesystem>

class GreeterServiceImpl final : public greeting::Greeter::Service {
    grpc::Status SayHello(grpc::ServerContext* context, const greeting::HelloRequest* request,
                    greeting::HelloReply* reply) override {

        std::string prefix("Hello: ");
        reply->set_message(prefix + request->name() + ", you are from: " + context->peer());

        GDEBUG() << "before sleep";
        std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 5)); 
        GDEBUG() << "after sleep";

        return grpc::Status::OK;
    }

};

int test_sync(int argc, char **argv) 
{
    std::filesystem::path param_path(argv[0]);
    glog::Logger::Instance().Initialize(argv[0], std::filesystem::current_path().string() + "/logs", param_path.filename().string());

    GreeterServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    grpc::ServerBuilder builder;

    std::string server_address("0.0.0.0:8889");

    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();

    return 0;
}
