#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>

#include "proto/item.grpc.pb.h"

using grpc::Channel;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

class ItemizerImpl final : public Itemizer::Service {
 public:
  Status CreateItem(ServerContext* context, const Item* item, SuccessResponse* success_response) override;
  Status CreateItems(ServerContext* context, const Items* items, SuccessResponse* success_response) override;
  Status GetItems(ServerContext* context, const NullRequest* null_request, Items* items) override;
  Status CreateItemsStream(ServerContext* context, ServerReader<Item>* reader, SuccessResponse* success_response) override;
  Status GetItemsStream(ServerContext* context, const NullRequest* request, ServerWriter<Item>* writer) override;
  Status FindItemsStream(ServerContext* context, ServerReaderWriter<FoundResponse,Item>* stream) override;
 private:
  std::vector<Item> items_;
};
