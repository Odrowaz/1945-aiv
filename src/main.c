#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "assets_manager.h"

int a = 0;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);

    char title[100];

    const char* app_dir = GetApplicationDirectory();
    ChangeDirectory(app_dir);
    ChangeDirectory("..");

    AddNewAsset("GameIcon", "assets/ui/life.png", IMAGE);
    AddNewAsset("PlayerSprite", "assets/player/myplane_strip3.png", TEXTURE);

    Image *img = (Image*)GetAssetFromName("GameIcon");
    Texture2D *tex = (Texture2D*)GetAssetFromName("PlayerSprite");

    SetWindowIcon(*img);

    // InitAudioDevice();

    // Sound sound = LoadSound();
    // Music music = LoadMusicStream();

    // PlaySound()
    // PlayMusicStream()

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        int fps = GetFPS();
        sprintf(title, "Delta: %.4f - FPS: %d", delta_time, fps);
        SetWindowTitle(title);

        static float time = 0;
        time += delta_time;
        unsigned char r = (unsigned char)((sin(time) * 0.5f + 0.5f) * 255);

        Color clear = (Color){r, 0, 0, 255};

        Vector2 mouse_po = GetMousePosition();

        Color color = MAGENTA;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            color = GREEN;
        }

        BeginDrawing();
        {
            ClearBackground(clear);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            DrawLine(100, 100, 700, 600, BLUE);
            DrawLineEx((Vector2){100, 110}, (Vector2){700, 1100}, 4, LIME);
            DrawPoly((Vector2){300, 300}, 5, 10, 0, YELLOW);

            DrawTextureV(*tex, mouse_po, WHITE);
            // DrawCircleV(mouse_po, 20, color);
            // DrawRectangle()
        }
        EndDrawing();
    }

    FreeAssets();
    CloseWindow();
    return 0;
}