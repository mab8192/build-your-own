#include "raylib.h"
#include <vector>
#include <algorithm>

struct Cell
{
    float u, v; // velocity components
    float t; // temperature (K)
    float p; // pressure
    float d; // density

    Color getColor() const
    {
        return {255, 255, 255, (unsigned char)std::clamp((int)d, 0, 255) };
    }
};

class Simulation
{
public:
    Simulation(int width, int height)
        : m_Width(width), m_Height(height)
    {
        m_Cells.resize(width * height);
        m_Cells_prev.resize(width * height);

        init();
    }

    void init()
    {
        // Construct initial simulation state

    }

    void diffuse(float dt)
    {
        for (int y = 0; y < m_Height; y++)
        {
            for (int x = 0; x < m_Width; x++)
            {
                float s = m_Cells_prev[IX(x, y)].d;
                int n = 1;
                if (x > 0)
                {
                    s += m_Cells_prev[IX(x - 1, y)].d;
                    n++;
                }
                if (x < m_Width - 1)
                {
                    s += m_Cells_prev[IX(x + 1, y)].d;
                    n++;
                }
                if (y > 0)
                {
                    s += m_Cells_prev[IX(x, y - 1)].d;
                    n++;
                }
                if (y < m_Height - 1)
                {
                    s += m_Cells_prev[IX(x, y + 1)].d;
                    n++;
                }

                m_Cells[IX(x, y)].d = s / n;
            }
        }
    }

    void step(float dt)
    {
        m_Cells_prev = m_Cells; // copy by assignment
        diffuse(dt);
    }
     
    void addDensity(int x, int y, float amount)
    {
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height) return;
        m_Cells[IX(x, y)].d += amount;
    }

    void addDensity(int x, int y, int rx, int ry, float amount)
    {
        for (int yi = y - ry; yi < y + ry; yi++)
        {
            for (int xi = x - rx; xi < x + rx; xi++)
            {
                if (xi < 0 || xi >= m_Width || yi < 0 || yi >= m_Height) continue;
                m_Cells[IX(xi, yi)].d += amount;
            }
        }
    }

    void addDensity(int x, int y, int r, float amount)
    {
        // todo: add in circle
    }

    const std::vector<Cell>& getCells() const { return m_Cells; }

    void render(float scale)
    {
        for (size_t y = 0; y < m_Height; y++)
        {
            for (size_t x = 0; x < m_Width; x++)
            {
                DrawRectangle(x * scale, y * scale, scale, scale, m_Cells[IX(x, y)].getColor());
            }
        }
    }

    const Cell& getCell(int x, int y)
    {
        return m_Cells[IX(x, y)];
    }

private:
    int IX(int x, int y)
    {
        return y * m_Width + x;
    }

    int m_Width, m_Height;
    std::vector<Cell> m_Cells;
    std::vector<Cell> m_Cells_prev;
};

int main() {
    const int gridW = 100, gridH = 100; // size of the simulation grid
    const int SCALE = 10.0f; // side-length of grid cells for rendering
    const int WIDTH = gridW * SCALE, HEIGHT = gridH * SCALE;

    InitWindow(WIDTH, HEIGHT, "CFD");

    Simulation sim(gridW, gridH);

    // Grid texture
    RenderTexture2D gridTex = LoadRenderTexture(WIDTH, HEIGHT);
    BeginTextureMode(gridTex);
    {
        ClearBackground(BLANK);
        for (size_t y = 0; y < gridH; y++)
        {
            for (size_t x = 0; x < gridW; x++)
            {
                DrawRectangleLines(x * SCALE, y * SCALE, SCALE, SCALE, Color{ 130, 130, 130, 128 });
            }
        }
    }
    EndTextureMode();

    while (!WindowShouldClose()) {
        // Input
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            sim.addDensity(mouse.x / SCALE, mouse.y / SCALE, 2, 2, 3.0f);
        }

        // Update
        sim.step(GetFrameTime());

        // Render
        BeginDrawing();
        {
            ClearBackground(BLACK);
            sim.render(SCALE);
            DrawTexture(gridTex.texture, 0, 0, WHITE);
            DrawFPS(10, 10);
        }
        
        if (mouse.x > 0 && mouse.x < WIDTH && mouse.y > 0 && mouse.y < HEIGHT)
            DrawText(TextFormat("Density at cursor: %f", sim.getCell(mouse.x / SCALE, mouse.y / SCALE).d), 10, 28, 14, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
