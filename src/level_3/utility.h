#pragma once
#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
auto hash = [](const std::string_view &key_view) { return std::hash<std::string_view>()(key_view); };
auto equal = [](const std::string_view &left, const std::string_view &right) { return left == right; };
