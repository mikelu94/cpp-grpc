#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "protos/item.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::CreateChannel;
using grpc::Status;

void print_help() {
    std::cout << "Options:\n" << "  --help\n\n" << "Commands:\n" 
              << "  create_item [name]\n" << "  create_items [name_1] ... [name_n]\n" << "  get_items\n"
              << "  create_items_stream\n" << "  get_items_stream\n" << "  find_items_stream\n";
}

int main(int argc, char* argv[]) {
    std::string server_address("0.0.0.0:3000");
    std::shared_ptr<Channel> channel = CreateChannel(server_address, grpc::InsecureChannelCredentials());
    std::unique_ptr<Itemizer::Stub> stub(Itemizer::NewStub(channel));
    
    ClientContext context;
    Items items;
    Item item;
    NullRequest null_request;
    SuccessResponse success_response;
    FoundResponse found_response;

    if (argc > 1) {
        std::string subcommand(argv[1]);
        if (subcommand == "create_item" && argc == 3) {
            item.set_name(argv[2]);
            stub->CreateItem(&context, item, &success_response);
        } else if (subcommand == "create_items" && argc > 2) {
            for (int i=2; i<argc; ++i) items.add_items()->set_name(argv[i]);
            stub->CreateItems(&context, items, &success_response);
        } else if (subcommand == "get_items") {
            stub->GetItems(&context, null_request, &items);
            std::cout << "[";
            int item_size = items.items_size();
            for (int i=0; i<item_size; ++i) {
                std::cout << items.items(i).name();
                if (i != item_size - 1) std::cout << ",";
            }
            std::cout << "]\n";
        } else if (subcommand == "create_items_stream") {
            std::unique_ptr<ClientWriter<Item>> writer(stub->CreateItemsStream(&context, &success_response));
            std::string item_name;
            std::cout << "New item name: ";
            while (std::cin >> item_name && item_name.length()) {
                item.set_name(item_name);
                writer->Write(item);
                std::cout << "New item name: ";
            }
            writer->WritesDone();
            writer->Finish();
        } else if (subcommand == "get_items_stream") {
            std::unique_ptr<ClientReader<Item>> reader(stub->GetItemsStream(&context, null_request));
            while (reader->Read(&item)) std::cout << item.name() << "\n";
            reader->Finish();
        } else if (subcommand == "find_items_stream") {
            std::shared_ptr<ClientReaderWriter<Item,FoundResponse>> stream(stub->FindItemsStream(&context));
            std::cout << "item name to query: ";
            std::string item_name;
            std::cin >> item_name;
            item.set_name(item_name);
            stream->Write(item);
            while (stream->Read(&found_response)) {
                std::cout << (found_response.found() ? "true\n" : "false\n" );
                std::cout << "item name to query: ";
                std::cin >> item_name;
                item.set_name(item_name);
                stream->Write(item);
            }
        } else {
            print_help();
        }
    } else {
        print_help();
    }
    return 0;
}