#include <iostream>
#include <memory>
#include <string>

#include "service_impl.h"

#include <grpcpp/grpcpp.h>

#include "proto/item.grpc.pb.h"

using grpc::Server;
using grpc::Channel;
using grpc::ServerBuilder;

void RunServer() {
    std::string server_address("0.0.0.0:3000");
    ItemizerImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "gRPC server listening on port 3000.\n";
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();

    return 0;
}
