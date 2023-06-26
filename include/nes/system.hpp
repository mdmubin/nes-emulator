#pragma once

#include <vector>

#include "nes/bus.hpp"
#include "nes/cpu.hpp"
#include "nes/memory.hpp"

namespace nes {

class System {
public:
    System();
    ~System();

    void run();

    void load_program(const std::vector<u8>& program);

private:
    Bus bus;
    Cpu cpu;
    Mem mem;

    void draw();

    void handle_events();
};

} // namespace nes
