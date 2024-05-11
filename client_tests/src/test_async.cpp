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

class AsyncGreeterClient
{
public:
    explicit AsyncGreeterClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(greeting::Greeter::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string SayHelloAndListen(const std::string &user)
    {
        // Data we are sending to the server.
        greeting::HelloRequest request;
        request.set_name(user);

        // Container for the data we expect from the server.
        greeting::HelloReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        grpc::ClientContext context;

        // The producer-consumer queue we use to communicate asynchronously with the
        // gRPC runtime.
        grpc::CompletionQueue cq;

        // Storage for the status of the RPC upon completion.
        grpc::Status status;

        std::unique_ptr<grpc::ClientAsyncReader<greeting::HelloReply>> rpc(
            stub_->AsyncSayHelloAndListen(&context, request, &cq, nullptr));

        void* got_tag;
        bool ok = false;
        bool got = cq.Next(&got_tag, &ok);
        
        do
        {
            rpc->Read(&reply, nullptr);
            
            got = cq.Next(&got_tag, &ok);

            if (ok)
            {
                GDEBUG() << "received: " << reply.message();

                std::abort();
            }
            
        } while (got && ok);

        rpc->Finish(&status, nullptr);

        got = cq.Next(&got_tag, &ok);
        
        // Act upon the status of the actual RPC.
        if (status.ok())
        {
            return reply.message();
        }
        else
        {
            return "RPC failed";
        }
    }

private:
    // Out of the passed in Channel comes the stub, stored here, our view of the
    // server's exposed services.
    std::unique_ptr<greeting::Greeter::Stub> stub_;
};

static const int thread_count = 1;

int test_async(int argc, char **argv)
{
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint specified by
    // the argument "--target=" which is the only expected argument.

    std::string target_str = "127.0.0.1:8889";

    std::thread threads[thread_count] = {};

    for (size_t i = 0; i < thread_count; i++)
    {
        threads[i] = std::thread([&target_str] () {
            //std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 2));
            // We indicate that the channel isn't authenticated (use of
            // InsecureChannelCredentials()).
            ::AsyncGreeterClient greeter(
                grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
            std::string user("world");
            std::string reply = greeter.SayHelloAndListen(user);
            std::cout << "Greeter received: " << reply << std::endl;
        });
    }

    for (size_t i = 0; i < thread_count; i++)
    {
        threads[i].join();
    }

    return 0;
}
