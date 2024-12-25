#include "MemoryMappedFile.h"
#include <cstddef>
#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <string_view>
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
  file_begin = static_cast<char *>(_begin);
  section_begin = static_cast<char *>(_begin);
}

/*Access each element as fileArray[i]*/
std::string_view MemoryMappedFile::fileArray() { return std::string_view(file_begin, _size); }

std::string_view MemoryMappedFile::next_section_array(size_t size_in_mb) {
  /*Mutext lock till this scope ends*/
  std::lock_guard<std::mutex> mutex_lock(mutex);
  if (section_begin == file_begin + _size)
    return std::string_view{};
  size_t section_size = size_in_mb * 1024 * 1024;
  const char *section_end = nullptr;
  // if requested size is bigger than last point of file return till the eof
  if (section_begin + section_size > file_begin + _size) {
    section_end = file_begin + _size;
  } else {
    section_end = section_begin + section_size;
    /* Keep going till we finish covering the whole row
      Our data is in form of following
      Omaha;4.0
      Nairobi;22.6
      ....
      and we don't want to returing section where section_end is only till "i" in Nairobi so would cover till end of
      row.
    */

    while (section_end != file_begin + _size && *section_end != '\n') {
      ++section_end;
    }
    // send pointer should point to one pass last element;
    ++section_end;
  }
  std::string_view section(section_begin, section_end - section_begin);
  section_begin = section_end;
  return section;
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