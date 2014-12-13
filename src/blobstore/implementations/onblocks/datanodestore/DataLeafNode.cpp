#include <blobstore/implementations/onblocks/datanodestore/DataLeafNode.h>

using std::unique_ptr;
using blockstore::Block;
using blockstore::Data;
using blockstore::Key;

namespace blobstore {
namespace onblocks {
namespace datanodestore {

DataLeafNode::DataLeafNode(DataNodeView view, const Key &key, DataNodeStore *nodestorage)
: DataNode(std::move(view), key, nodestorage) {
  assert(numBytesInThisNode() <= MAX_STORED_BYTES);
}

DataLeafNode::~DataLeafNode() {
}

void DataLeafNode::read(off_t offset, size_t count, Data *result) const {
  assert(count <= result->size());
  assert(offset+count <= numBytesInThisNode());
  std::memcpy(result->data(), node().DataBegin<unsigned char>()+offset, count);
}

void DataLeafNode::write(off_t offset, size_t count, const Data &data) {
  assert(count <= data.size());
  assert(offset+count <= numBytesInThisNode());
  std::memcpy(node().DataBegin<unsigned char>()+offset, data.data(), count);
}

void DataLeafNode::InitializeNewNode() {
  *node().Depth() = 0;
  *node().Size() = 0;
  //fillDataWithZeroes(); not needed, because a newly created block will be zeroed out. DataLeafNodeTest.SpaceIsZeroFilledWhenGrowing ensures this.
}

void DataLeafNode::fillDataWithZeroesFromTo(off_t begin, off_t end) {
  std::memset(node().DataBegin<unsigned char>()+begin, 0, end-begin);
}

uint64_t DataLeafNode::numBytesInThisNode() const {
  return *node().Size();
}

void DataLeafNode::resize(uint64_t newsize_bytes) {
  assert(newsize_bytes <= MAX_STORED_BYTES);

  // If we're shrinking, we want to delete the old data
  // (overwrite it with zeroes).
  // TODO Mention this in thesis
  if (newsize_bytes < *node().Size()) {
    fillDataWithZeroesFromTo(newsize_bytes, *node().Size());
  }

  *node().Size() = newsize_bytes;
}

}
}
}
