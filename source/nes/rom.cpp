#include "nes/rom.hpp"

#include <fstream>
#include <raylib.h>
#include <fmt/core.h>

using namespace nes;

//

static constexpr u32 CHR_ROM_PAGE_SIZE =  8 * 1024;
static constexpr u32 PRG_ROM_PAGE_SIZE = 16 * 1024;

static constexpr u16 PRG_BASE_ADDRESS = 0x8000;

//

Rom::Rom(const char *romFile) {
    std::ifstream rom(romFile, std::ios::binary);

    if (rom.good()) {
        u8 header[16];
        rom.read((char *) header, sizeof(header));

        // check file format
        if (header[0] == 0x4E && // 'N'
            header[1] == 0x45 && // 'E'
            header[2] == 0x53 && // 'S'
            header[3] == 0x1A    // EOF
        ) {
            if (header[7] & 0x0C) {
                TraceLog(LOG_ERROR, "NES 2.0 Rom not supported");
                return;
            }
        } else {
            TraceLog(LOG_ERROR, "Unrecognized Rom file format");
            return;
        }

        u32 prgRomSize = header[4] * PRG_ROM_PAGE_SIZE;
        u32 chrRomSize = header[5] * CHR_ROM_PAGE_SIZE;

        bool fourScreen = (header[6] & 0x08) != 0;
        bool vertMirror = (header[6] & 0x01) == 1;

        mirror = fourScreen ? sm_FOUR_SCREEN : (vertMirror ? sm_VERTICAL : sm_HORIZONTAL);

        // check for trainer data, ignoring it for now
        if (header[6] & 0x04) {
            rom.seekg(512, std::ios::cur);
        }

        // get mapper id and set mapper. TODO: implement mapper and set mapper here
        u8 mapperId = (header[7] & 0xF0) | (header[6] >> 4);
        if (mapperId != 0) {
            // currently only NROM mapper (mapper-0) is supported.
            TraceLog(LOG_ERROR, fmt::format("Unsupported mapper. MapperID: {}", mapperId).c_str());
        }

        // read rom data

        prgMem.resize(prgRomSize);
        rom.read((char *) (&prgMem[0]), prgRomSize);

        chrMem.resize(chrRomSize);
        rom.read((char *) (&chrMem[0]), chrRomSize);

        rom.close();
        romStatusOk = true;
    } else {
        TraceLog(LOG_ERROR, fmt::format("Failed to open rom file: {}", romFile).c_str());
    }
}

//

u8 Rom::read_prg_u8(u16 address) {
    if (address < PRG_BASE_ADDRESS) {
        // NOTE: check for battery backed ram, if I decide to add it, but currently not valid
        TraceLog(LOG_WARNING, fmt::format("PRG_ROM attempt to read address below PRG base address."
                                          "Read attempt at: 0x{:04x}",
                                          address)
                                  .c_str());
    } else {
        // ideally we get this from a mapper, but currently we only use mapper-0
        u16 mappedAddr = address - PRG_BASE_ADDRESS;
        if (mappedAddr >= prgMem.size()) {
            TraceLog(LOG_ERROR, "PRG_ROM attempt to read address beyond PRG length.");
        } else {
            return prgMem[mappedAddr];
        }
    }

    return 0;
}

u8 Rom::read_chr_u8(u16 address) {
    if (address > 0x1FFF) {
        TraceLog(LOG_ERROR, fmt::format("CHR_ROM attempt to read beyond CHR Max Address."
                                        "Read attempt at: 0x{:04x}",
                                        address)
                                .c_str());
    } else {
        if (address >= chrMem.size()) {
            TraceLog(LOG_ERROR, "CHR_ROM attempt to read address beyond CHR length.");
        } else {
            return chrMem[address];
        }
    }

    return 0;
}

void Rom::write_prg_u8(u16 address, u8 data) {
    if (address < PRG_BASE_ADDRESS) {
        // NOTE: fix this if battery backed RAM added
        TraceLog(LOG_WARNING,
                 fmt::format("PRG_ROM attempt to write to address below PRG base address."
                             "Write attempt at: 0x{:04x}",
                             address)
                     .c_str());
    } else {
        u16 mappedAddr = address - PRG_BASE_ADDRESS;
        if (mappedAddr >= prgMem.size()) {
            TraceLog(LOG_ERROR, "PRG_ROM attempt to write to address beyond PRG length.");
        } else {
            prgMem[mappedAddr] = data;
        }
    }
}

void Rom::write_chr_u8(u16 address, u8 data) {
    if (address > 0x1FFF) {
        TraceLog(LOG_ERROR, fmt::format("CHR_ROM attempt to read beyond CHR Max Address."
                                        "Read attempt at: 0x{:04x}",
                                        address)
                                .c_str());
    } else {
        if (address >= chrMem.size()) {
            TraceLog(LOG_ERROR, "CHR_ROM attempt to read address beyond CHR length.");
        } else {
            chrMem[address] = data;
        }
    }
}
