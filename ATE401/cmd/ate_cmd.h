#pragma once
#include <vector>
#include <cstdint>

std::vector<uint8_t> time_le(const uint32_t tm);
void getTime(const void* data);