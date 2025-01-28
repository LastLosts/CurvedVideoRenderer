#pragma once

#include <inttypes.h>
#include <vector>

struct Frame
{
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> data;
    double time;
};
