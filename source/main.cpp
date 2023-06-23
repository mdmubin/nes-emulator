#include <raylib.h>

int main() {
    InitWindow(500, 500, "NES Emulator");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Hello Raylib!", 10, 10, 13, YELLOW);
        EndDrawing();
    }
}
