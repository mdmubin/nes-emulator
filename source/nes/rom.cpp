#include "nes/rom.hpp"

#include <fstream>

using namespace nes;

struct FileHeader {
    char ftype[4];
    u8 prgRomSize;
    u8 chrRomSize;
    u8 mapperFlags1;
    u8 mapperFlags2;
    u8 PrgRamSize;
    u8 systemFlags1;
    u8 systemFlags2;
    char padding[5];
};

Rom::Rom(const char *filepath) {
    std::ifstream rom(filepath, std::ios::binary);

    if (rom.is_open()) {
        FileHeader h = {0};
        rom.read((char *) (&h), sizeof(h));

        bool iNesFmt = false;
        if (h.ftype[0] == 'N' && h.ftype[1] == 'E' && h.ftype[2] == 'S' && h.ftype[3] == 0x1A) {
            iNesFmt = true;
        }

        bool iNes20Fmt = false;
//        if (iNesFmt && (h.mapperFlags2 & )) {
//            iNesFmt = true;
//        }
    }
}
