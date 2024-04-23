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
*   Copyright (c) 2018-2024 raylib technologies (@raylibtech) / Ramon Santamaria (@raysan5)
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
    bool windowActive;

    Rectangle windowBounds;
    Vector2 panOffset;
    bool dragMode;
    bool supportDrag;

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

#if defined(NO_ALPHA_BLENDING)
    #define FADE(c,a)   c
#else
    #define FADE(c,a)   Fade(c,a)
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
static const char *lblCopyrightText = "Copyright (c) 2023 raylib technologies.";
static const char *linkraylibtechText = "[@raylibtech]";
static const char *lblMoreInfoText = "More info:";
static const char *linkMailText = "ray@raylibtech.com";
static const char *lblSupportText = "Support:";
static const char *btnSponsorText = "#146#Sponsor";
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

    state.windowBounds = (Rectangle){ GetScreenWidth()/2 - 380/2, GetScreenHeight()/2 - 366/2, 380, 366 };
    state.panOffset = (Vector2){ 0, 0 };
    state.dragMode = false;
    state.supportDrag = false;

    return state;
}

// Gui about window
void GuiWindowAbout(GuiWindowAboutState *state)
{
    if (state->windowActive)
    {
        // Update window dragging
        //----------------------------------------------------------------------------------------
        if (state->supportDrag)
        {
            Vector2 mousePosition = GetMousePosition();

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Window can be dragged from the top window bar
                if (CheckCollisionPointRec(mousePosition, (Rectangle){ state->windowBounds.x, state->windowBounds.y, (float)state->windowBounds.width, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT }))
                {
                    state->dragMode = true;
                    state->panOffset.x = mousePosition.x - state->windowBounds.x;
                    state->panOffset.y = mousePosition.y - state->windowBounds.y;
                }
            }

            if (state->dragMode)
            {
                state->windowBounds.x = (mousePosition.x - state->panOffset.x);
                state->windowBounds.y = (mousePosition.y - state->panOffset.y);

                // Check screen limits to avoid moving out of screen
                if (state->windowBounds.x < 0) state->windowBounds.x = 0;
                else if (state->windowBounds.x > (GetScreenWidth() - state->windowBounds.width)) state->windowBounds.x = GetScreenWidth() - state->windowBounds.width;

                if (state->windowBounds.y < 40) state->windowBounds.y = 40;
                else if (state->windowBounds.y > (GetScreenHeight() - state->windowBounds.height - 24)) state->windowBounds.y = GetScreenHeight() - state->windowBounds.height - 24;

                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) state->dragMode = false;
            }
        }
        //----------------------------------------------------------------------------------------

        // Draw window and controls
        //----------------------------------------------------------------------------------------
        state->windowActive = !GuiWindowBox(state->windowBounds, TextFormat("#191#About %s", TOOL_NAME));

        // Draw a background rectangle for convenience
        DrawRectangle((int)state->windowBounds.x + 1, (int)state->windowBounds.y + 4 + 20, state->windowBounds.width - 2, 90 - 4, Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f));

        int labelTextAlign = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
        DrawTechIcon((int)state->windowBounds.x + 10, (int)state->windowBounds.y + 35, 64, TOOL_SHORT_NAME, 20, true, GetColor(TOOL_LOGO_COLOR));

        bool singleLine = true;
        GuiLabel((Rectangle){ state->windowBounds.x + 85, state->windowBounds.y + (singleLine? 55 : 35), 200, 30 }, TextFormat("%s %s (%s)", TOOL_NAME, TOOL_VERSION, TOOL_RELEASE_DATE));
        GuiLabel((Rectangle){ state->windowBounds.x + 85, state->windowBounds.y + (singleLine? 78 : 60), (float)state->windowBounds.width, 20 }, TOOL_DESCRIPTION);

        GuiLine((Rectangle){ state->windowBounds.x, state->windowBounds.y + 100, (float)state->windowBounds.width, 20 }, NULL);
        GuiLabel((Rectangle){ state->windowBounds.x + 8, state->windowBounds.y + 112, 126, 24 }, lblUsedLibsText);

        DrawTechIcon((int)state->windowBounds.x + 10, (int)state->windowBounds.y + 135, 64, "raylib", 10, false, BLACK);
        DrawTechIcon((int)state->windowBounds.x + 80, (int)state->windowBounds.y + 135, 64, "raygui", 10, false, GRAY);

        if (GuiLabelButton((Rectangle){ state->windowBounds.x + 155, state->windowBounds.y + 135, 80, 16 }, linkraylibText)) { OpenURL("https://www.raylib.com/"); }
        if (GuiLabelButton((Rectangle){ state->windowBounds.x + 155, state->windowBounds.y + 160, 150, 16 }, linkGitraylibText)) { OpenURL("https://github.com/raysan5/raylib"); }
        if (GuiLabelButton((Rectangle){ state->windowBounds.x + 155, state->windowBounds.y + 180, 150, 16 }, linkGitrayguiText)) { OpenURL("https://github.com/raysan5/raygui"); }

        DrawTextEx(GetFontDefault(), "MINIAUDIO", (Vector2){ state->windowBounds.x + 13, state->windowBounds.y + 206 }, 30, 2, GREEN);
        DrawTextEx(GetFontDefault(), "MINIAUDIO", (Vector2){ state->windowBounds.x + 12, state->windowBounds.y + 205 }, 30, 2, GetColor(0x003800ff));
        if (GuiLabelButton((Rectangle){ state->windowBounds.x + 174, state->windowBounds.y + 210, 150, 15 }, "github.com/mackron/miniaudio")) { OpenURL("https://github.com/dr-soft/miniaudio"); }

        GuiLine((Rectangle){ state->windowBounds.x, state->windowBounds.y + 200 + 30, (float)state->windowBounds.width, 20 }, NULL);

        GuiLabel((Rectangle){ state->windowBounds.x + 10, state->windowBounds.y + 220 + 28, 289, 20 }, lblCopyrightText);
        GuiLabel((Rectangle){ state->windowBounds.x + 10, state->windowBounds.y + 250 + 25, 65, 16 }, lblMoreInfoText);

        float linkMailTextWidth = MeasureTextEx(GuiGetFont(), linkMailText, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x;
        if (GuiLabelButton((Rectangle){ state->windowBounds.x + 90, state->windowBounds.y + 250 + 25, 165, 16 }, TextFormat("www.raylibtech.com/%s", TextToLower(TOOL_NAME)))) { OpenURL("https://www.raylibtech.com/"); }
        if (GuiLabelButton((Rectangle){ state->windowBounds.x + 90, state->windowBounds.y + 270 +25, linkMailTextWidth, 16 }, linkMailText)) { OpenURL("mailto:ray@raylibtech.com"); }
        if (GuiLabelButton((Rectangle){ state->windowBounds.x + 90 + linkMailTextWidth + 4, state->windowBounds.y + 270 +25, MeasureTextEx(GuiGetFont(), linkraylibtechText, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x, 16 }, linkraylibtechText)) { OpenURL("https://twitter.com/raylibtech"); }

        GuiLabel((Rectangle){ state->windowBounds.x + 10, state->windowBounds.y + 270 + 25, 65, 16 }, lblSupportText);
        GuiLine((Rectangle){ state->windowBounds.x, state->windowBounds.y + 285 + 25, (float)state->windowBounds.width, 20 }, NULL);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, labelTextAlign);

        DrawRectangle((int)state->windowBounds.x + 1, (int)state->windowBounds.y + 285 + 11 + 25, state->windowBounds.width - 2, 43, Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f));

        int buttonTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

        if (GuiButton((Rectangle){ state->windowBounds.x + state->windowBounds.width - 80 - 90, state->windowBounds.y + 305 + 25, 80, 24 }, btnSponsorText)) { OpenURL("https://github.com/sponsors/raysan5"); }
        if (GuiButton((Rectangle){ state->windowBounds.x + state->windowBounds.width - 80, state->windowBounds.y + 305 + 25, 70, 24 }, btnCloseText)) state->windowActive = false;
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, buttonTextAlign);
        //----------------------------------------------------------------------------------------
    }
    //else state->windowBounds = (Rectangle){ GetScreenWidth()/2 - state->windowBounds.width/2, GetScreenHeight()/2 - state->windowBounds.height/2, state->windowBounds.width, state->windowBounds.height };
}

#endif // GUI_WINDOW_ABOUT_IMPLEMENTATION