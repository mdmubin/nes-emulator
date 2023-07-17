#pragma once

#include <string>
#include <vector>

#include "common/types.hpp"

namespace nes {

class Rom {
public:
    Rom(const char *romFile);

    inline bool is_ok() const {
        return romStatusOk;
    }

private:


    typedef enum ScreenMirroring {
        sm_UNDEFINED,
        sm_VERTICAL,
        sm_HORIZONTAL,
        sm_FOUR_SCREEN,
        sm_SINGLE_SCREEN, // may remain unused
    } sm;

    typedef enum Mapper {
        mp_UNDEFINED,
        mp_NROM,
    } mp;

    mp mapper = mp_UNDEFINED;
    sm mirror = sm_UNDEFINED;

    std::vector<u8> prgMem;
    std::vector<u8> chrMem;

    bool romStatusOk = false;
};

} // namespace nes