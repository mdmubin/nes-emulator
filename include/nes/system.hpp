#pragma once

#include <string>

#include "nes/bus.hpp"
#include "nes/cpu.hpp"
#include "nes/memory.hpp"

namespace nes {

class System {
public:
    System();
    ~System();

    void run();

    void load_rom(const std::string& filepath);

private:
    Bus bus;
    Cpu cpu;
    Mem mem;
    Rom *rom = nullptr;

    void draw();

    void handle_events();
};

} // namespace nes
