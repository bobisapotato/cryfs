#include "FileBlob.h"

#include "MagicNumbers.h"
#include <messmer/blockstore/utils/Key.h>
#include <cassert>

using blobstore::Blob;
using cpputils::unique_ref;
using cpputils::make_unique_ref;

namespace cryfs {
namespace fsblobstore {

FileBlob::FileBlob(unique_ref<Blob> blob, std::function<void()> onDestruct)
: FsBlob(std::move(blob), onDestruct) {
}

unique_ref<FileBlob> FileBlob::InitializeEmptyFile(unique_ref<Blob> blob, std::function<void()> onDestruct) {
  InitializeBlobWithMagicNumber(blob.get(), MagicNumbers::FILE);
  return make_unique_ref<FileBlob>(std::move(blob), onDestruct);
}

ssize_t FileBlob::read(void *target, uint64_t offset, uint64_t count) const {
  return baseBlob().tryRead(target, offset + 1, count);
}

void FileBlob::write(const void *source, uint64_t offset, uint64_t count) {
  baseBlob().write(source, offset + 1, count);
}

void FileBlob::flush() {
  baseBlob().flush();
}

blockstore::Key FileBlob::key() const {
  	return baseBlob().key();
}

void FileBlob::resize(off_t size) {
  baseBlob().resize(size+1);
}

off_t FileBlob::lstat_size() const {
  return size();
}

off_t FileBlob::size() const {
  return baseBlob().size()-1;
}

}
}

