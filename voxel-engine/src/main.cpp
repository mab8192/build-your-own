#include "raylib.h"
#include "raymath.h" // Required for Vector3 math
#include <vector>
#include <cmath>

// --- Constants ---
const int MAX_BLOCKS_X = 64;
const int MAX_BLOCKS_Y = 32;
const int MAX_BLOCKS_Z = 64;
const float BLOCK_SIZE = 1.0f;
const int WATER_LEVEL = 8;

// --- Block Types ---
enum BlockType {
    AIR = 0,
    STONE,
    DIRT,
    GRASS,
    SAND,
    WATER
};

struct Block {
    bool active;
    BlockType type;
    Color color;
};

// --- Global Map Data ---
Block map[MAX_BLOCKS_X][MAX_BLOCKS_Y][MAX_BLOCKS_Z];

// --- Helper to Get Colors ---
Color GetBlockColor(BlockType type) {
    switch (type) {
        case STONE: return DARKGRAY;
        case DIRT:  return BROWN;
        case GRASS: return GREEN;
        case SAND:  return BEIGE;
        case WATER: return Fade(BLUE, 0.6f);
        default:    return WHITE;
    }
}

// --- Generation Logic ---
void GenerateMap() {
    Image noiseImage = GenImagePerlinNoise(MAX_BLOCKS_X, MAX_BLOCKS_Z, GetRandomValue(-1000, 1000), GetRandomValue(-1000, 1000), 4.0f);
    
    Color* noisePixels = LoadImageColors(noiseImage);

    for (int x = 0; x < MAX_BLOCKS_X; x++) {
        for (int z = 0; z < MAX_BLOCKS_Z; z++) {
            // Calculate height from noise (0 to 255 mapped to height limits)
            int index = z * MAX_BLOCKS_X + x;
            // Limit max height so we don't go out of array bounds
            int height = (noisePixels[index].r * (MAX_BLOCKS_Y - 10)) / 255;

            for (int y = 0; y < MAX_BLOCKS_Y; y++) {
                Block& b = map[x][y][z];
                
                b.active = false;
                b.type = AIR;

                // Terrain Logic
                if (y <= height) {
                    b.active = true;
                    if (y == height) {
                        if (y <= WATER_LEVEL + 1) b.type = SAND; 
                        else b.type = GRASS;
                    } else if (y > height - 3) {
                        b.type = DIRT;
                    } else {
                        b.type = STONE;
                    }
                }
                // Water Logic
                else if (y <= WATER_LEVEL) {
                    b.active = true;
                    b.type = WATER;
                }

                if (b.active) b.color = GetBlockColor(b.type);
            }
        }
    }
    
    UnloadImageColors(noisePixels);
    UnloadImage(noiseImage);
}

int main(void) {
    InitWindow(1280, 720, "Voxels");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = { (float)MAX_BLOCKS_X/2.0f, (float)MAX_BLOCKS_Y + 5.0f, (float)MAX_BLOCKS_Z/2.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    DisableCursor();

    GenerateMap();

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        
        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(camera);
            
            for (int x = 0; x < MAX_BLOCKS_X; x++) {
                for (int y = 0; y < MAX_BLOCKS_Y; y++) {
                    for (int z = 0; z < MAX_BLOCKS_Z; z++) {
                        Block& b = map[x][y][z];
                        
                        if (!b.active) continue;

                        // Optimization: Hidden face culling
                        bool left   = (x > 0) ? map[x-1][y][z].active && map[x-1][y][z].type != WATER : false;
                        bool right  = (x < MAX_BLOCKS_X-1) ? map[x+1][y][z].active && map[x+1][y][z].type != WATER : false;
                        bool top    = (y < MAX_BLOCKS_Y-1) ? map[x][y+1][z].active && map[x][y+1][z].type != WATER : false;
                        bool bottom = (y > 0) ? map[x][y-1][z].active && map[x][y-1][z].type != WATER : false;
                        bool front  = (z < MAX_BLOCKS_Z-1) ? map[x][y][z+1].active && map[x][y][z+1].type != WATER : false;
                        bool back   = (z > 0) ? map[x][y][z-1].active && map[x][y][z-1].type != WATER : false;

                        if (left && right && top && bottom && front && back) continue;

                        Vector3 pos = { (float)x, (float)y, (float)z };
                        DrawCube(pos, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, b.color);
                    }
                }
            }

        EndMode3D();

        // UI
        DrawRectangle(10, 10, 220, 30, Fade(SKYBLUE, 0.5f));
        DrawFPS(20, 15);
        DrawText("+", GetScreenWidth()/2 - 10, GetScreenHeight()/2 - 10, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}