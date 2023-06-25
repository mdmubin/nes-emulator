#include "nes/bus.hpp"

using namespace nes;
using namespace std;

u8 Bus::read_u8(u16 address) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        return mem->read_u8(address);
    } else {
        // unknown address
        return 0;
    }
}

u16 Bus::read_u16(u16 address) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        return mem->read_u16(address);
    } else {
        // unknown address
        return 0;
    }
}

void Bus::write_u8(u8 data, u16 address) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        mem->write_u8(data, address);
    } else {
        // unknown address
    }
}

void Bus::write_u16(u16 data, u16 address) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        mem->write_u16(data, address);
    } else {
        // unknown address
    }
}

//

void Bus::attach_components(Cpu *c, Mem *m) {
    this->mem = m;
    c->connect_bus(this);
    m->reset();
}
