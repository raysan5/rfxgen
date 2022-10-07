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
    Vector2 position;

    bool windowActive;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required
    int windowWidth;
    int windowHeight;

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

    // Custom variables initialization
    state.windowWidth = MAX_SPONSOR_SLOTS*128 + MAX_SPONSOR_SLOTS*2 + 24 + (MAX_SPONSOR_SLOTS - 1)*8;    // Logo size (3*128) + button border (2) + margin + button spacing
    state.windowHeight = 282;
    state.position = (Vector2){ GetScreenWidth()/2 - state.windowWidth/2, GetScreenHeight()/2 - state.windowHeight/2 };

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
        state->windowActive = !GuiWindowBox((Rectangle){ state->position.x, state->position.y, (float)state->windowWidth, (float)state->windowHeight }, TextFormat("#186#%s Sponsors", TOOL_NAME));

        // Draw a background rectangle for convenience
        GuiLabel((Rectangle){ state->position.x + 8, state->position.y + 24 + 8, state->windowWidth - 24, 24 }, "This tool is sponsored by:");
        GuiLine((Rectangle){ state->position.x, state->position.y + 24 + 24, (float)state->windowWidth, 20 }, NULL);
        DrawRectangle((int)state->position.x + 1, (int)state->position.y + 58, state->windowWidth - 2, 128 + 24, Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f));

        GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
        for (int i = 0; i < MAX_SPONSOR_SLOTS; i++)
        {
            if (state->sponsoring[i])
            {
                if (GuiImageButton((Rectangle){ state->position.x + 12 + (130 + 8)*i, state->position.y + 70, 130, 130 }, state->sponsorLogo[i])) OpenURL(state->sponsorLink[i]);
            }
            else GuiButton((Rectangle){ state->position.x + 12 + (130 + 8)*i, state->position.y + 70, 130, 130 }, TextFormat("SPONSOR #%i\nLOGO\n+\nLINK", i + 1));
        }
        GuiSetStyle(BUTTON, BORDER_WIDTH, 2);

        GuiLine((Rectangle){ state->position.x, state->position.y + 200, (float)state->windowWidth, 20 }, NULL);
        GuiLabel((Rectangle){ state->position.x + 8, state->position.y + 70 + 128 + 16, state->windowWidth - 24, 24 }, TextFormat("Sponsor slots available: %i/%i", state->slotsAvailable, MAX_SPONSOR_SLOTS));
        GuiLine((Rectangle){ state->position.x, state->position.y + 232, (float)state->windowWidth, 20 }, NULL);

        DrawRectangle((int)state->position.x + 1, state->position.y + state->windowHeight - 40 + 1, state->windowWidth - 2, 38, Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f));

        int buttonTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
        if (state->slotsAvailable == 0) GuiDisable();
        if (GuiButton((Rectangle){ state->position.x + state->windowWidth - 80 - 90, state->position.y + state->windowHeight - 32, 80, 24 }, "#186#Sponsor")) { OpenURL("https://github.com/sponsors/raysan5"); }
        GuiEnable();
        if (GuiButton((Rectangle){ state->position.x + state->windowWidth - 80, state->position.y + state->windowHeight - 32, 70, 24 }, "#159#Close")) state->windowActive = false;
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, buttonTextAlign);
    }
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

#endif // GUI_WINDOW_ABOUT_IMPLEMENTATION