/*******************************************************************************************
*
*   raygui layout - Window About
*
*   MODULE USAGE:
*       #define GUI_WINDOW_ABOUT_IMPLEMENTATION
*       #include "gui_window_about.h"
*
*   On game init call:  GuiWindowAboutState state = InitGuiWindowAbout();
*   On game draw call:  GuiWindowAbout(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2018 raylib technologies (@raylibtech). All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

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

    bool windowAboutActive;
    bool chkLicenseChecked;

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
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
// Draw rTool generated icon
static void DrawTechIcon(int posX, int posY, int size, const char *text, int textSize, bool corner, Color color)
{
    int borderSize = (int)ceil((float)size/16.0f);

    int textPosX = posX + size - 2*borderSize - MeasureText(text, textSize);
    int textPosY = posY + size - 2*borderSize - textSize;

    DrawRectangle(posX - 1, posY - 1, size + 2, size + 2, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
    DrawRectangle(posX, posY, size, size, RAYWHITE);
    DrawRectangleLinesEx((Rectangle){ posX, posY, size, size }, borderSize, color);
    DrawText(text, textPosX, textPosY, textSize, color);
#if defined(VERSION_ONE)
    if (corner)
    {
        DrawTriangle((Vector2){ posX + size - 2*borderSize - size/4, posY + 2*borderSize },
                 (Vector2){ posX + size - 2*borderSize, posY + 2*borderSize + size/4 },
                 (Vector2){ posX + size - 2*borderSize, posY + 2*borderSize }, color);
    }
#endif
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Init Window About
GuiWindowAboutState InitGuiWindowAbout(void)
{
    GuiWindowAboutState state = { 0 };

    state.windowAboutActive = false;
    state.chkLicenseChecked = true;

    // Custom variables initialization
    state.windowWidth = 335;
    state.windowHeight = 340 + 25;
    state.position = (Vector2){ GetScreenWidth()/2 - state.windowWidth/2, GetScreenHeight()/2 - state.windowHeight/2 };

    return state;
}

// Gui about window
void GuiWindowAbout(GuiWindowAboutState *state)
{
    const char *windowAboutText = "#191#About rFXGen";
    const char *lblDescriptionText = "A simple and easy-to-use fx sounds generator";
    const char *lblNameVersionText = "rFXGen v2.0";
    const char *lblDateText = "(Feb. 2019)";
    const char *lblUsedLibsText = "Powered by:";
    const char *linkraylibText = "www.raylib.com";
    const char *linkGitraylibText = "github.com/raysan5/raylib";
    const char *linkGitrayguiText = "github.com/raysan5/raygui";
    const char *lblCopyrightText = "Copyright (c) 2019 raylib technologies (                 )";
    const char *linkraylibtechText = "@raylibtech";
    const char *lblMoreInfoText = "More info:";
    const char *linkToolWebText = "www.raylibtech.com/rfxgen";
    const char *linkMailText = "ray@raylibtech.com";
    const char *lblSupportText = "Support:";
    const char *chkLicenseText = "License Agreement (EULA)";
    const char *BtnBeONEText = "#186#Be ONE";
    const char *BtnCloseText = "#159#Close";
#if defined(VERSION_ONE)
    const char *mode = "ONE";
#else
    const char *mode = "ZERO";
#endif

    const int toolColor = 0x5197d4ff;

    if (state->windowAboutActive)
    {
        GuiEnable();

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)), 0.85f));

        state->windowAboutActive = !GuiWindowBox((Rectangle){ state->position.x + 0, state->position.y + 0, 335, 340 + 25 }, FormatText("%s %s", windowAboutText, mode));

        // Draw a background rectangle for convenience
        DrawRectangle(state->position.x + 1, state->position.y + 4 + 20, state->windowWidth - 2, 90 - 4, Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f));

        int labelTextAlign = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
        DrawTechIcon(state->position.x + 10, state->position.y + 35, 64, "rIP", 20, true, GetColor(toolColor));
        GuiLabel((Rectangle){ state->position.x + 85, state->position.y + 50, 200, 30 }, FormatText("%s %s %s", lblNameVersionText, mode, lblDateText));
        GuiLabel((Rectangle){ state->position.x + 85, state->position.y + 74, 245, 20 }, lblDescriptionText);

        GuiLine((Rectangle){ state->position.x + 0, state->position.y + 100, 335, 20 }, NULL);
        GuiLabel((Rectangle){ state->position.x + 8, state->position.y + 113, 126, 25 }, lblUsedLibsText);

        DrawTechIcon(state->position.x + 10, state->position.y + 135, 64, "raylib", 10, false, BLACK);
        DrawTechIcon(state->position.x + 80, state->position.y + 135, 64, "raygui", 10, false, LIGHTGRAY);

        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 135, 80, 15 }, linkraylibText)) { OpenURL("https://www.raylib.com/"); }
        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 160, 150, 15 }, linkGitraylibText)) { OpenURL("https://github.com/raysan5/raylib"); }
        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 180, 150, 15 }, linkGitrayguiText)) { OpenURL("https://github.com/raysan5/raygui"); }
        
        DrawTextEx(GetFontDefault(), "miniAUDIO", (Vector2){ state->position.x + 12, state->position.y + 205 }, 30, 2, GetColor(0x003800ff));
        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 210, 150, 15 }, "github.com/dr-soft/miniaudio")) { OpenURL("https://github.com/dr-soft/miniaudio"); }

        GuiLine((Rectangle){ state->position.x, state->position.y + 200 + 30, 335, 20 }, NULL);

        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 220 + 28, 289, 20 }, lblCopyrightText);
        if (GuiLabelButton((Rectangle){ state->position.x + 215, state->position.y + 220 + 28, 60, 20 }, linkraylibtechText)) { OpenURL("https://twitter.com/raylibtech"); }

        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 250 + 25, 65, 15 }, lblMoreInfoText);

        if (GuiLabelButton((Rectangle){ state->position.x + 80, state->position.y + 250 + 25, 165, 15 }, linkToolWebText)) { OpenURL("https://www.raylibtech.com/"); }
        if (GuiLabelButton((Rectangle){ state->position.x + 80, state->position.y + 270 + 25, 165, 15 }, linkMailText)) { OpenURL("mailto:ray@raylibtech.com"); }

        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 270 + 25, 65, 15 }, lblSupportText);
        GuiLine((Rectangle){ state->position.x, state->position.y + 285 + 25, 335, 20 }, NULL);
        GuiSetStyle(LABEL, TEXT_ALIGNMENT, labelTextAlign);

        DrawRectangle(state->position.x + 1, state->position.y + 285 + 11 + 25, state->windowWidth - 2, 43, Fade(GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)), 0.5f));

        int buttonTextAlign = GuiGetStyle(BUTTON, TEXT_ALIGNMENT);
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
        int buttonInnerPadding = GuiGetStyle(BUTTON, INNER_PADDING);
        GuiSetStyle(BUTTON, INNER_PADDING, 1);
#if defined(VERSION_ONE)
        // TODO: Define a proper EULA
        //state->chkLicenseChecked = GuiCheckBox((Rectangle){ state->position.x + 10, state->position.y + 310, 16, 16 }, chkLicenseText, state->chkLicenseChecked);
#else
        //GuiDisable();state->chkLicenseChecked = GuiCheckBox((Rectangle){ state->position.x + 10, state->position.y + 310, 16, 16 }, chkLicenseText, state->chkLicenseChecked); GuiEnable();
        if (GuiButton((Rectangle){ state->position.x + 175, state->position.y + 305 + 25, 75, 25 }, BtnBeONEText)) { OpenURL("https://raylibtech.itch.io/rfxgen"); }
#endif
        if (GuiButton((Rectangle){ state->position.x + 255, state->position.y + 305 + 25, 70, 25 }, BtnCloseText)) state->windowAboutActive = false;
        GuiSetStyle(BUTTON, TEXT_ALIGNMENT, buttonTextAlign);
        GuiSetStyle(BUTTON, INNER_PADDING, buttonInnerPadding);

        GuiDisable();
    }
}

#endif // GUI_WINDOW_ABOUT_IMPLEMENTATION