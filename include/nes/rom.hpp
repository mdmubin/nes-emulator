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

    u8 read_prg_u8(u16 address);
    u8 read_chr_u8(u16 address);

    void write_prg_u8(u16 address, u8 data);
    void write_chr_u8(u16 address, u8 data);

private:
    typedef enum ScreenMirroring {
        sm_UNDEFINED,
        sm_VERTICAL,
        sm_HORIZONTAL,
        sm_FOUR_SCREEN,
        sm_SINGLE_SCREEN, // may remain unused
    } sm;

    sm mirror = sm_UNDEFINED;

    std::vector<u8> prgMem;
    std::vector<u8> chrMem;

    bool romStatusOk = false;
};

} // namespace nes