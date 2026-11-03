#include <assert.h>
#define TOOL_NAME "rFXGen"
#define RAYGUI_IMPLEMENTATION
#include "../src/external/raygui.h"
#undef RAYGUI_IMPLEMENTATION
#define GUI_WINDOW_HELP_IMPLEMENTATION
#include "../src/gui_window_help.h"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(1280, 1160, "Help window regression");
    RenderTexture2D target = LoadRenderTexture(640, 580);
    GuiWindowHelpState state = InitGuiWindowHelp();
    state.windowActive = true;
    for (int scale = 1; scale <= 2; scale++)
    {
        SetWindowSize(640*scale, 580*scale);
        BeginDrawing();
        BeginTextureMode(target);
        GuiWindowHelp(&state, 640, 580);
        EndTextureMode();
        EndDrawing();
        assert(state.windowBounds.x >= 0);
        assert(state.windowBounds.y >= 0);
        assert(state.windowBounds.x + state.windowBounds.width <= 640);
        assert(state.windowBounds.y + state.windowBounds.height <= 580);
    }
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}
