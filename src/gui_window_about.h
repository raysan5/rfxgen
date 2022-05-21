/*******************************************************************************************
*
*   Window About
* 
*   NOTES:
*       This module is generic for all tools, so, tools info should be customized
*       just defining some tool info before including this file:
*
*       #define TOOL_NAME           "rToolName"
*       #define TOOL_SHORT_NAME     "rTN"
*       #define TOOL_VERSION        "1.0"
*       #define TOOL_DESCRIPTION    "Tool description"
*       #define TOOL_RELEASE_DATE   "Jan.2022"
*       #define TOOL_LOGO_COLOR     0x000000ff
*
*   MODULE USAGE:
*       #define GUI_WINDOW_ABOUT_IMPLEMENTATION
*       #include "gui_window_about.h"
*
*   On game init call:  GuiWindowAboutState state = InitGuiWindowAbout();
*   On game draw call:  GuiWindowAbout(&state);
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2019-2022 raylib technologies (@raylibtech).
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header

#ifndef GUI_WINDOW_ABOUT_H
#define GUI_WINDOW_ABOUT_H

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Gui window structure declaration
typedef struct {
    Vector2 position;

    bool windowActive;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required
    int windowWidth;
    int windowHeight;

} GuiWindowAboutState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiWindowAboutState InitGuiWindowAbout(void);
void GuiWindowAbout(GuiWindowAboutState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_WINDOW_ABOUT_H

/***********************************************************************************
*
*   GUI_WINDOW_ABOUT IMPLEMENTATION
*
************************************************************************************/

#if defined(GUI_WINDOW_ABOUT_IMPLEMENTATION)

#include "raygui.h"

#include <math.h>       // Required for: ceil()

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// The following defines are required to customize the about info,
// they are usually defined externally in the tool code
#if !defined(TOOL_NAME)
    #define TOOL_NAME           "rTool"
#endif
#if !defined(TOOL_SHORT_NAME)
    #define TOOL_SHORT_NAME     "rTN"    
#endif
#if !defined(TOOL_VERSION)
    #define TOOL_VERSION        "1.0"
#endif
#if !defined(TOOL_DESCRIPTION)
    #define TOOL_DESCRIPTION    "A simple and easy-to-use tool to do something"
#endif
#if !defined(TOOL_RELEASE_DATE)
    #define TOOL_RELEASE_DATE   "Dec.2021"
#endif
#if !defined(TOOL_LOGO_COLOR)
    #define TOOL_LOGO_COLOR      0x000000ff
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const char *lblUsedLibsText = "Powered by:";
static const char *linkraylibText = "www.raylib.com";
static const char *linkGitraylibText = "github.com/raysan5/raylib";
static const char *linkGitrayguiText = "github.com/raysan5/raygui";
static const char *lblCopyrightText = "Copyright (c) 2021 raylib technologies.";
static const char *linkraylibtechText = "[@raylibtech]";
static const char *lblMoreInfoText = "More info:";
static const char *linkMailText = "ray@raylibtech.com";
static const char *lblSupportText = "Support:";
static const char* btnDonateText = "#146#Donate";
static const char *btnCloseText = "#159#Close";

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
// Draw rTool generated icon
static void DrawTechIcon(int posX, int posY, int size, const char *text, int textSize, bool corner, Color color)
{
    float borderSize = ceilf((float)size/16.0f);
    bool offsetY = true;
    
    // Make sure there is no character with pixels down the text baseline for a perfect y-aligned icon
    for (int i = 0; text[i] != '\0'; i++) if ((text[i] == 'q') || (text[i] == 'y') || (text[i] == 'p') || (text[i] == 'j') || (text[i] == 'g')) { offsetY = false; break; }

    int textPosX = posX + size - (int)(2.0f*borderSize) - MeasureText(text, textSize);
    int textPosY = posY + size - (int)(2.0f*borderSize) - textSize + (offsetY? (2*textSize/10) : 0);

    DrawRectangle(posX - 1, posY - 1, size + 2, size + 2, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
    DrawRectangle(posX, posY, size, size, RAYWHITE);
    DrawRectangleLinesEx((Rectangle){ (float)posX, (float)posY, (float)size, (float)size }, borderSize, color);
    DrawText(text, textPosX, textPosY, textSize, color);

    if (corner)
    {
        DrawTriangle((Vector2){ (float)posX + (float)size - 2*borderSize - (float)size/4, (float)posY + 2*borderSize },
                 (Vector2){ (float)posX + (float)size - 2*borderSize, (float)posY + 2*borderSize + (float)size/4 },
                 (Vector2){ (float)posX + (float)size - 2*borderSize, (float)posY + 2*borderSize }, color);
    }
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Init Window About
GuiWindowAboutState InitGuiWindowAbout(void)
{
    GuiWindowAboutState state = { 0 };

    state.windowActive = false;

    // Custom variables initialization
    state.windowWidth = 360;
    state.windowHeight = 340 + 25;
    state.position = (Vector2){ GetScreenWidth()/2 - state.windowWidth/2, GetScreenHeight()/2 - state.windowHeight/2 };

    return state;
}

// Gui about window
void GuiWindowAbout(GuiWindowAboutState *state)
{
    if (state->windowActive)
    {
        state->windowActive = !GuiWindowBox((Rectangle){ state->position.x, state->position.y, (float)state->windowWidth, (float)state->windowHeight }, TextFormat("#191#About %s", TOOL_NAME));

        // Draw a background rectangle for convenience
        DrawRectangle((int)state->position.x + 1, (int)state->position.y + 4 + 20, state->windowWidth - 2, 90 - 4, Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f));

        int labelTextAlign = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        DrawTechIcon((int)state->position.x + 10, (int)state->position.y + 35, 64, TOOL_SHORT_NAME, 20, true, GetColor(TOOL_LOGO_COLOR));
        
        bool singleLine = true;
        GuiLabel((Rectangle){ state->position.x + 85, state->position.y + (singleLine? 55 : 35), 200, 30 }, TextFormat("%s %s (%s)", TOOL_NAME, TOOL_VERSION, TOOL_RELEASE_DATE));
        GuiLabel((Rectangle){ state->position.x + 85, state->position.y + (singleLine? 78 : 60), 245, 20 }, TOOL_DESCRIPTION);

        GuiLine((Rectangle){ state->position.x, state->position.y + 100, (float)state->windowWidth, 20 }, NULL);
        GuiLabel((Rectangle){ state->position.x + 8, state->position.y + 112, 126, 24 }, lblUsedLibsText);

        DrawTechIcon((int)state->position.x + 10, (int)state->position.y + 135, 64, "raylib", 10, false, BLACK);
        DrawTechIcon((int)state->position.x + 80, (int)state->position.y + 135, 64, "raygui", 10, false, GRAY);

        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 135, 80, 16 }, linkraylibText)) { OpenURL("https://www.raylib.com/"); }
        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 160, 150, 16 }, linkGitraylibText)) { OpenURL("https://github.com/raysan5/raylib"); }
        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 180, 150, 16 }, linkGitrayguiText)) { OpenURL("https://github.com/raysan5/raygui"); }
        
        DrawTextEx(GetFontDefault(), "miniAUDIO", (Vector2){ state->position.x + 12, state->position.y + 205 }, 30, 2, GetColor(0x003800ff));
        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 210, 150, 15 }, "github.com/dr-soft/miniaudio")) { OpenURL("https://github.com/dr-soft/miniaudio"); }

        GuiLine((Rectangle){ state->position.x, state->position.y + 200 + 30, (float)state->windowWidth, 20 }, NULL);

        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 220 + 28, 289, 20 }, lblCopyrightText);
        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 250 + 25, 65, 16 }, lblMoreInfoText);

        float linkMailTextWidth = MeasureTextEx(GuiGetFont(), linkMailText, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x;
        if (GuiLabelButton((Rectangle){ state->position.x + 90, state->position.y + 250 + 25, 165, 16 }, TextFormat("www.raylibtech.com/%s", TextToLower(TOOL_NAME)))) { OpenURL("https://www.raylibtech.com/"); }
        if (GuiLabelButton((Rectangle){ state->position.x + 90, state->position.y + 270 + 25, linkMailTextWidth, 16 }, linkMailText)) { OpenURL("mailto:ray@raylibtech.com"); }
        if (GuiLabelButton((Rectangle){ state->position.x + 90 + linkMailTextWidth + 4, state->position.y + 270 + 25, MeasureTextEx(GuiGetFont(), linkraylibtechText, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x, 16 }, linkraylibtechText)) { OpenURL("https://twitter.com/raylibtech"); }

        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 270 + 25, 65, 16 }, lblSupportText);
        GuiLine((Rectangle){ state->position.x, state->position.y + 285 + 25, (float)state->windowWidth, 20 }, NULL);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, labelTextAlign);

        DrawRectangle((int)state->position.x + 1, (int)state->position.y + 285 + 11 + 25, state->windowWidth - 2, 43, Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f));

        int buttonTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

        if (GuiButton((Rectangle) { state->position.x + state->windowWidth - 80 - 90, state->position.y + 305 + 25, 80, 24 }, btnDonateText)) { OpenURL(TextFormat("https://raylibtech.itch.io/%s/purchase", TOOL_NAME)); }
        if (GuiButton((Rectangle){ state->position.x + state->windowWidth - 80, state->position.y + 305 + 25, 70, 24 }, btnCloseText)) state->windowActive = false;
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, buttonTextAlign);
    }
}

#endif // GUI_WINDOW_ABOUT_IMPLEMENTATION