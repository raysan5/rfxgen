/*******************************************************************************************
*
*   Window Sponsor
*
*   MODULE USAGE:
*       #define GUI_WINDOW_SPONSOR_IMPLEMENTATION
*       #include "gui_window_sponsor.h"
*
*   On game init call:  GuiWindowSponsorState state = InitGuiWindowSponsor();
*   New sponsor addition: GuiWindowSponsorAdd(Texture2D logo, const char *link);
*   On game draw call:  GuiWindowSponsor(&state);
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2022 raylib technologies (@raylibtech).
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

#ifndef GUI_WINDOW_SPONSOR_H
#define GUI_WINDOW_SPONSOR_H

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#define MAX_SPONSOR_SLOTS   3

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

    // Sponsors data
    Texture2D sponsorLogo[MAX_SPONSOR_SLOTS];
    char sponsorLink[MAX_SPONSOR_SLOTS][512];
    bool sponsoring[MAX_SPONSOR_SLOTS];

    int slotsAvailable;

} GuiWindowSponsorState;

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
GuiWindowSponsorState InitGuiWindowSponsor(void);
void GuiWindowSponsor(GuiWindowSponsorState *state);
void GuiWindowSponsorAdd(GuiWindowSponsorState *state, Texture2D logo, const char *link);

#ifdef __cplusplus
}
#endif

#endif // GUI_WINDOW_SPONSOR_H

/***********************************************************************************
*
*   GUI_WINDOW_SPONSOR IMPLEMENTATION
*
************************************************************************************/

#if defined(GUI_WINDOW_SPONSOR_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// The following define is required to customize the window info
#if !defined(TOOL_NAME)
    #define TOOL_NAME           "rTool"
#endif

//----------------------------------------------------------------------------------
// Internal Functions Declaration
//----------------------------------------------------------------------------------
// Image button control, returns true when clicked
static bool GuiImageButton(Rectangle bounds, Texture2D texture);

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Init Window Sponsor
GuiWindowSponsorState InitGuiWindowSponsor(void)
{
    GuiWindowSponsorState state = { 0 };

    state.windowActive = false;
    
    int windowWidth = MAX_SPONSOR_SLOTS*128 + MAX_SPONSOR_SLOTS*2 + 24 + (MAX_SPONSOR_SLOTS - 1)*8;    // Logo size (3*128) + button border (2) + margin + button spacing
    state.windowBounds = (Rectangle){ GetScreenWidth()/2 - windowWidth/2, GetScreenHeight()/2 - 282/2, windowWidth, 282 };
    state.panOffset = (Vector2){ 0, 0 };
    state.dragMode = false;
    state.supportDrag = false;

    // Sponsors data initialization
    for (int i = 0; i < MAX_SPONSOR_SLOTS; i++)
    {
        state.sponsorLogo[i] = (Texture2D){ 0 };
        memset(state.sponsorLink[i], 0, 512);
        state.sponsoring[i] = false;
    }

    state.slotsAvailable = MAX_SPONSOR_SLOTS;

    return state;
}

// Add new sponsor to the window
void GuiWindowSponsorAdd(GuiWindowSponsorState *state, Texture2D logo, const char *link)
{
    if (state->slotsAvailable > 0)
    {
        state->sponsorLogo[MAX_SPONSOR_SLOTS - state->slotsAvailable] = logo;
        strcpy(state->sponsorLink[MAX_SPONSOR_SLOTS - state->slotsAvailable], link);
        state->sponsoring[MAX_SPONSOR_SLOTS - state->slotsAvailable] = true;

        state->slotsAvailable--;
    }
}

// Sponsor Window update & draw
void GuiWindowSponsor(GuiWindowSponsorState *state)
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
        state->windowActive = !GuiWindowBox(state->windowBounds, TextFormat("#186#%s Sponsors", TOOL_NAME));

        // Draw a background rectangle for convenience
        DrawRectangle((int)state->windowBounds.x + 1, (int)state->windowBounds.y + 24, state->windowBounds.width - 2, 34, Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f));
        GuiLabel((Rectangle){ state->windowBounds.x + 8, state->windowBounds.y + 24 + 8, state->windowBounds.width - 24, 24 }, "This tool is sponsored by:");
        GuiLine((Rectangle){ state->windowBounds.x, state->windowBounds.y + 24 + 24, (float)state->windowBounds.width, 20 }, NULL);

        GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
        for (int i = 0; i < MAX_SPONSOR_SLOTS; i++)
        {
            if (state->sponsoring[i])
            {
                if (GuiImageButton((Rectangle){ state->windowBounds.x + 12 + (130 + 8)*i, state->windowBounds.y + 70, 130, 130 }, state->sponsorLogo[i])) OpenURL(state->sponsorLink[i]);
            }
            else GuiButton((Rectangle){ state->windowBounds.x + 12 + (130 + 8)*i, state->windowBounds.y + 70, 130, 130 }, TextFormat("SPONSOR #%i\nLOGO\n+\nLINK", i + 1));
        }
        GuiSetStyle(BUTTON, BORDER_WIDTH, 2);

        GuiLine((Rectangle){ state->windowBounds.x, state->windowBounds.y + 200, (float)state->windowBounds.width, 20 }, NULL);
        GuiLabel((Rectangle){ state->windowBounds.x + 8, state->windowBounds.y + 70 + 128 + 16, state->windowBounds.width - 24, 24 }, TextFormat("Sponsor slots available: %i/%i", state->slotsAvailable, MAX_SPONSOR_SLOTS));
        GuiLine((Rectangle){ state->windowBounds.x, state->windowBounds.y + 232, (float)state->windowBounds.width, 20 }, NULL);

        DrawRectangle((int)state->windowBounds.x + 1, state->windowBounds.y + state->windowBounds.height - 40 + 1, state->windowBounds.width - 2, 38, Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f));

        int buttonTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        if (state->slotsAvailable == 0) GuiDisable();
        if (GuiButton((Rectangle){ state->windowBounds.x + state->windowBounds.width - 80 - 90, state->windowBounds.y + state->windowBounds.height - 32, 80, 24 }, "#186#Sponsor")) { OpenURL("https://github.com/sponsors/raysan5"); }
        GuiEnable();
        if (GuiButton((Rectangle){ state->windowBounds.x + state->windowBounds.width - 80, state->windowBounds.y + state->windowBounds.height - 32, 70, 24 }, "#159#Close")) state->windowActive = false;
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, buttonTextAlign);
        //----------------------------------------------------------------------------------------
    }
    else state->windowBounds = (Rectangle){ GetScreenWidth()/2 - state->windowBounds.width/2, GetScreenHeight()/2 - state->windowBounds.height/2, state->windowBounds.width, state->windowBounds.height };
}

// Image button control, returns true when clicked
static bool GuiImageButton(Rectangle bounds, Texture2D texture)
{
    GuiState state = guiState;
    bool pressed = false;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) pressed = true;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    DrawTexturePro(texture, (Rectangle){ 0, 0, texture.width, texture.height }, 
        (Rectangle){ bounds.x + GuiGetStyle(BUTTON, BORDER_WIDTH), bounds.y + GuiGetStyle(BUTTON, BORDER_WIDTH), 
                     bounds.width - 2*GuiGetStyle(BUTTON, BORDER_WIDTH), bounds.height - 2*GuiGetStyle(BUTTON, BORDER_WIDTH) }, (Vector2){ 0 }, 0.0f, WHITE);
    
    GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, BORDER_WIDTH), Fade(GetColor(GuiGetStyle(BUTTON, BORDER + (state*3))), guiAlpha), (state == 0)? BLANK : Fade(GetColor(GuiGetStyle(BUTTON, BASE + (state*3))), 0.2f));
    
    //GuiDrawText(text, GetTextBounds(BUTTON, bounds), GuiGetStyle(BUTTON, TEXT_ALIGNMENT), Fade(GetColor(GuiGetStyle(BUTTON, TEXT + (state*3))), guiAlpha));
    //------------------------------------------------------------------

    return pressed;
}

#endif // GUI_WINDOW_SPONSOR_IMPLEMENTATION