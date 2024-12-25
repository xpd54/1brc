#pragma once
#include <filesystem>
#include <mutex>
#include <string_view>
class MemoryMappedFile {
public:
  MemoryMappedFile(const std::filesystem::path &path);
  std::string_view fileArray();
  std::string_view next_section_array(size_t size_in_mb = 64);
  size_t size();
  ~MemoryMappedFile();

private:
  std::filesystem::path filePath;
  int file = -1;
  std::size_t _size;
  void *_begin;
  const char *file_begin;
  const char *section_begin;
  std::mutex mutex;
};