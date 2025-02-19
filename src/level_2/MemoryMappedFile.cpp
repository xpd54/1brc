#include "MemoryMappedFile.h"
#include <cstddef>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <system_error>
#include <unistd.h>
MemoryMappedFile::MemoryMappedFile(const std::filesystem::path &path) : filePath(path) {
  // Open file in read only
  file = open(filePath.c_str(), O_RDONLY);
  struct stat sb;

  // get size of the file
  if (fstat(file, &sb) == -1)
    throw std::system_error(errno, std::iostream_category(), "Can't get size of file");
  // file size in bytes
  _size = sb.st_size;

  /*
   * maps the file into virtual memory ref (The Linux Programming Interface 49.2 Creating a Mapping: mmap())
   */
  _begin = mmap(NULL, _size, PROT_READ, MAP_PRIVATE, file, 0);
  if (_begin == MAP_FAILED)
    throw std::system_error(errno, std::iostream_category(), "Can't map file");
}

/*Access each element as fileArray[i]*/
std::string_view MemoryMappedFile::fileArray() {
  char *char_ptr = static_cast<char *>(_begin);
  return std::string_view(char_ptr, _size);
}

size_t MemoryMappedFile::size() { return _size; }

MemoryMappedFile::~MemoryMappedFile() {
  if (_begin) {
    munmap(_begin, _size);
  }
  if (file) {
    close(file);
  }
}