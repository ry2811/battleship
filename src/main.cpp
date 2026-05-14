#include <raylib.h>
#include "GameLogic.h"
#include "Renderer.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

GameLogic* game;
Renderer* renderer;

void UpdateDrawFrame(void) {
    float dt = GetFrameTime();
    renderer->Update(dt, game->state);
    renderer->Draw(*game);
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Battleship - Raylib");
    SetExitKey(0); // Disable ESC from exiting the game

    game = new GameLogic();
    renderer = new Renderer();

    renderer->LoadResources();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    renderer->UnloadResources();
    CloseWindow();

    delete renderer;
    delete game;

    return 0;
}
