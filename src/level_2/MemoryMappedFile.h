#pragma once
#include <filesystem>
#include <string_view>
class MemoryMappedFile {
public:
  MemoryMappedFile(const std::filesystem::path &path);
  std::string_view fileArray();
  size_t size();
  ~MemoryMappedFile();

private:
  std::filesystem::path filePath;
  int file = -1;
  std::size_t _size;
  void *_begin;
};