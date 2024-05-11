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

#include <thread>

class ServerImpl final
{
public:
    ~ServerImpl()
    {
        server_->Shutdown();
        // Always shutdown the completion queue after the server.
        cq_->Shutdown();
    }

    // There is no shutdown handling in this code.
    void Run()
    {
        std::string server_address("0.0.0.0:8889");

        grpc::ServerBuilder builder;
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        // Register "service_" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *asynchronous* service.
        builder.RegisterService(&service_);
        // Get hold of the completion queue used for the asynchronous communication
        // with the gRPC runtime.
        cq_ = builder.AddCompletionQueue();
        // Finally assemble the server.
        server_ = builder.BuildAndStart();

        GDEBUG() << "Server listening on " << server_address;

        // Proceed to the server's main loop.
        HandleRpcs();
    }

private:
    // Class encompasing the state and logic needed to serve a request.
    class CallData
    {
    public:
        // Take in the "service" instance (in this case representing an asynchronous
        // server) and the completion queue "cq" used for asynchronous communication
        // with the gRPC runtime.
        CallData(greeting::Greeter::AsyncService *service, grpc::ServerCompletionQueue *cq, int id = 0)
            : service_(service), cq_(cq), responder_(&ctx_), status_(CREATE)
            , id_(id + 1)
            , times_(0)
        {
            // Invoke the serving logic right away.
            Proceed();
        }

        void Proceed()
        {
            if (status_ == CREATE)
            {
                // Make this instance progress to the PROCESS state.
                status_ = PROCESS;

                // As part of the initial CREATE state, we *request* that the system
                // start processing SayHello requests. In this request, "this" acts are
                // the tag uniquely identifying the request (so that different CallData
                // instances can serve different requests concurrently), in this case
                // the memory address of this CallData instance.
                service_->RequestSayHelloAndListen(&ctx_, &request_, &responder_, cq_, cq_,
                                          this);

                GDEBUG() << "requesting: " << id_;
            }
            else if (status_ == PROCESS)
            {
                ++times_;

                if (1 == times_)
                {
                    // Spawn a new CallData instance to serve new clients while we process
                    // the one for this CallData. The instance will deallocate itself as
                    // part of its FINISH state.
                    new CallData(service_, cq_, id_);
                }
                
                if (times_ < 5)
                {
                    // The actual processing.
                    std::string prefix("Hello ");
                    reply_.set_message(prefix + request_.name());

                    //std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 10));
                    /* if (3 == times_)
                    {
                        std::abort();
                    } */

                    GDEBUG() << "processing: " << id_;
                    responder_.Write(reply_, this);
                }
                else
                {
                    // And we are done! Let the gRPC runtime know we've finished, using the
                    // memory address of this instance as the uniquely identifying tag for
                    // the event.
                    status_ = FINISH;

                    //std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 10));

                    GDEBUG() << "byte: " << id_;
                    responder_.Finish(grpc::Status::CANCELLED, this);
                }
            }
            else
            {
                GPR_ASSERT(status_ == FINISH);
                // Once in the FINISH state, deallocate ourselves (CallData).
                GDEBUG() << "done: " << id_;
                delete this;
            }
        }

    private:
        // The means of communication with the gRPC runtime for an asynchronous
        // server.
        greeting::Greeter::AsyncService *service_;
        // The producer-consumer queue where for asynchronous server notifications.
        grpc::ServerCompletionQueue *cq_;
        // Context for the rpc, allowing to tweak aspects of it such as the use
        // of compression, authentication, as well as to send metadata back to the
        // client.
        grpc::ServerContext ctx_;

        // What we get from the client.
        greeting::HelloRequest request_;
        // What we send back to the client.
        greeting::HelloReply reply_;

        // The means to get back to the client.
        grpc::ServerAsyncWriter<greeting::HelloReply> responder_;

        // Let's implement a tiny state machine with the following states.
        enum CallStatus
        {
            CREATE,
            PROCESS,
            FINISH
        };
        CallStatus status_; // The current serving state.
        int id_;
        int times_;
    };

    // This can be run in multiple threads if needed.
    void HandleRpcs()
    {
        // Spawn a new CallData instance to serve new clients.
        new CallData(&service_, cq_.get());
        void *tag; // uniquely identifies a request.
        bool ok;
        while (true)
        {
            // Block waiting to read the next event from the completion queue. The
            // event is uniquely identified by its tag, which in this case is the
            // memory address of a CallData instance.
            // The return value of Next should always be checked. This return value
            // tells us whether there is any kind of event or cq_ is shutting down.
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<CallData *>(tag)->Proceed();
        }
    }

    std::unique_ptr<grpc::ServerCompletionQueue> cq_;
    greeting::Greeter::AsyncService service_;
    std::unique_ptr<grpc::Server> server_;
};

int test_async(int argc, char **argv)
{
    ServerImpl server;
    server.Run();

    return 0;
}
