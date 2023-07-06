#pragma once

#include <string>
#include <vector>

#include "common/types.hpp"

namespace nes {

class Rom {
public:
    Rom(const char *filepath);

private:
    enum ScreenMirroring {
        VERTICAL,
        HORIZONTAL,
        FOUR_SCREEN,
    };

    ScreenMirroring mirror;

    std::vector<u8> prgMem;
    std::vector<u8> chrMem;

    u8 mapper;
};

} // namespace nes