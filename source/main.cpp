#include "nes/system.hpp"

int main() {
    nes::System system;
    system.load_rom("assets/test/nestest.nes");
    system.run();
}
