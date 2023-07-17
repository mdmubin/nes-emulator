#include "nes/rom.hpp"

#include <fstream>
#include <raylib.h>
#include <fmt/core.h>

using namespace nes;

//

constexpr u32 CHR_ROM_PAGE_SIZE =  8 * 1024;
constexpr u32 PRG_ROM_PAGE_SIZE = 16 * 1024;

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
