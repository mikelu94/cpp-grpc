#include "service_impl.h"

#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>

#include "proto/item.grpc.pb.h"

using grpc::ServerContext;
using grpc::Status;

Status ItemizerImpl::CreateItem(ServerContext* context, const Item* item, SuccessResponse* success_response) {
    this->items_.push_back(*item);
    return Status::OK;
}


Status ItemizerImpl::CreateItems(ServerContext* context, const Items* items, SuccessResponse* success_response) {
    for (auto const& item : items->items()) this->items_.push_back(item);
    return Status::OK;
}

Status ItemizerImpl::GetItems(ServerContext* context, const NullRequest* null_request, Items* items) {
    for (auto const& item : this->items_) {
        Item* return_item = items->add_items();
        return_item->set_id(item.id());
        return_item->set_name(item.name());
    }
    return Status::OK;
}

Status ItemizerImpl::CreateItemsStream(ServerContext* context, ServerReader<Item>* reader, SuccessResponse* success_response) {
    Item stream_item;
    while (reader->Read(&stream_item)) this->items_.push_back(stream_item);
    return Status::OK;
}

Status ItemizerImpl::GetItemsStream(ServerContext* context, const NullRequest* request, ServerWriter<Item>* writer) {
    for (auto const& item : this->items_) {
        Item return_item;
        return_item.set_id(item.id());
        return_item.set_name(item.name());
        writer->Write(return_item);
    }
    return Status::OK;
}

Status ItemizerImpl::FindItemsStream(ServerContext* context, ServerReaderWriter<FoundResponse, Item>* stream) {
    Item stream_item;
    while (stream->Read(&stream_item)) {
        bool found = false;
        for (auto const& item : this->items_) {
            if (stream_item.id() == item.id() && stream_item.name() == item.name()) {
                found = true;
                break;
            }
        }
        FoundResponse found_response;
        found_response.set_found(found);
        stream->Write(found_response);
    }
    return Status::OK;
}
