#include "nes/system.hpp"

#include <raylib.h>

using namespace nes;

//

// static variables used accross the system class implementations

// variable is initialized in System constructor
// done this way as raylib font requires gl context to be initialized first
static Font FONT_16PX;
static Font FONT_20PX;

static const Color COLOR_BG    = {0xF9, 0xFB, 0xE7, 0xFF}; // color code: #f9fbe7
static const Color COLOR_FG    = {0x20, 0x20, 0x20, 0xFF}; // color code: #202020
static const Color COLOR_INFO  = {0x24, 0xA1, 0x9C, 0xFF}; // color code: #24a19c
static const Color COLOR_ERROR = {0xFB, 0x25, 0x76, 0xFF}; // color code: #fb2576

//

System::System() : bus(), cpu(), mem() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
    InitWindow(900, 900, "NES emulator test");

    bus.attach_components(&cpu, &mem);

    FONT_16PX = LoadFontEx("assets/fonts/firacode-nf.ttf", 16, nullptr, 256);
    FONT_20PX = LoadFontEx("assets/fonts/firacode-nf.ttf", 20, nullptr, 256);
}

System::~System() {
    CloseWindow();
}

//

void System::load_program(const std::vector<u8> &program) {
    mem.load_program(program);
    mem.write_u16(0x0800, 0xFFFC);
    cpu.reset();
}

void System::run() {
    while (!WindowShouldClose()) {
        draw();
        handle_events();
    }
}

//

void System::draw() {
    BeginDrawing();

    ClearBackground(COLOR_BG);

    DrawText("SPACE = Advance    R = RESET    I = IRQ    N = NMI", 10, 850, 16, COLOR_FG);

    // print registers
    DrawText("Registers", 10, 12, 16, COLOR_FG);
    DrawRectangleLinesEx({5, 33, 150, 175}, 5, COLOR_FG);
    auto pos = Vector2{20, 50};
    for (const auto &r : cpu.registers_to_strings()) {
        DrawTextEx(FONT_16PX, r.c_str(), pos, 16, 1.2, COLOR_FG);
        pos.y += 25;
    }

    // print instruction
    DrawTextEx(FONT_16PX, cpu.get_executing_instruction().c_str(), {10, 300}, 16, 1.2, COLOR_INFO);

    EndDrawing();
}

void System::handle_events() {
    int key;
    do {
        key = GetKeyPressed();
        switch (key) {
        case KEY_SPACE:
            cpu.clock();
            break;
        case KEY_R:
            cpu.reset();
            break;
        case KEY_I:
            cpu.irq();
            break;
        case KEY_N:
            cpu.nmi();
            break;
        default:
            break;
        }
    } while (key != 0);
}
