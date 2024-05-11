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

#include <grpcpp/grpcpp.h>

#include "hello.grpc.pb.h"

#include "logger/glog_logger.h"

#include <memory>
#include <thread>

class SyncGreeterClient {
public:
    SyncGreeterClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(greeting::Greeter::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string SayHello(const std::string& user) {
        // Data we are sending to the server.
        greeting::HelloRequest request;
        request.set_name(user);

        // Container for the data we expect from the server.
        greeting::HelloReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        grpc::ClientContext context;

        // The actual RPC.
        grpc::Status status = stub_->SayHello(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.message();
        } else {
            std::cout << status.error_code() << ": " << status.error_message()
                    << std::endl;
            return "RPC failed";
        }
    }

 private:
    std::unique_ptr<greeting::Greeter::Stub> stub_;
};

static const int thread_count = 1000;

int test_sync(int argc, char **argv) 
{
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint specified by
    // the argument "--target=" which is the only expected argument.
    
    std::string target_str = "127.0.0.1:8889";

    std::thread threads[thread_count] = {};

    for (size_t i = 0; i < thread_count; i++)
    {
        threads[i] = std::thread([&target_str] () {
            std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 2));
            // We indicate that the channel isn't authenticated (use of
            // InsecureChannelCredentials()).
            ::SyncGreeterClient greeter(
                grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
            std::string user("world");
            std::string reply = greeter.SayHello(user);
            std::cout << "Greeter received: " << reply << std::endl;
        });
    }

    for (size_t i = 0; i < thread_count; i++)
    {
        threads[i].join();
    }

    return 0;
}
