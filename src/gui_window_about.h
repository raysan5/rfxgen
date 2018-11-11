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
    bool active;
    int width;
    int height;
    bool chkLicenseChecked;
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
static void DrawIcon(int posX, int posY, int size, const char *text, int textSize, bool pro, Color color)
{
    int triSize = size/4;
    int borderSize = (int)ceil((float)size/16.0f);

    int textPosX = posX + size - 2*borderSize - MeasureText(text, textSize);
    int textPosY = posY + size - 2*borderSize - textSize;

    DrawRectangle(posX, posY, size, size, RAYWHITE);
    DrawRectangleLinesEx((Rectangle){ posX, posY, size, size }, borderSize, color);
    DrawText(text, textPosX, textPosY, textSize, color);
    if (pro) DrawTriangle((Vector2){ posX + size - 2*borderSize - triSize, posY + 2*borderSize },
                          (Vector2){ posX + size - 2*borderSize, posY + 2*borderSize + triSize },
                          (Vector2){ posX + size - 2*borderSize, posY + 2*borderSize }, color);
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Init Window About
GuiWindowAboutState InitGuiWindowAbout(void)
{
    GuiWindowAboutState state = {
        .position = (Vector2){ GetScreenWidth()/2 - 330/2, GetScreenHeight()/2 - 380/2 },
        .active = false,
        .width = 330,
        .height = 380,
        .chkLicenseChecked = true
    };
    
    return state;
}

// Gui about window
void GuiWindowAbout(GuiWindowAboutState *state)
{
    // NOTE: const string literals are most-probably stored in read-only data section
    const char *lblNameVersionText = "rFXGen v1.0";
    const char *lblDateText = "(Dec. 2018)";
    const char *lblDescriptionText = "A simple and easy-to-use sounds generator";
    const char *lblUsedLibsText = "Used libraries:";
    const char *linkraylibText = "www.raylib.com";
    const char *linkGitraylibText = "github.com/raysan5/raylib";
    const char *linkGitrayguiText = "github.com/raysan5/raygui";
    const char *lblDevelopersText = "Developers:";
    const char *lblDev01Text = "- Ramon Santamaria (              )";
    const char *linkDev01Text = "@raysan5";
    const char *lblCopyrightText = "Copyright (c) 2019 raylib technologies (                 )";
    const char *linkraylibtech = "@raylibtech";
    const char *lblMoreInfoText = "More info:";
    const char *linkToolWebText = "www.raylibtech.com/rfxgen";
    const char *lblSupportText = "Support:";
    const char *linkMailText = "ray@raylibtech.com";
    
    const int toolColor = 0x5197d4ff;

    if (state->active)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GetColor(GuiGetStyleProperty(DEFAULT_BACKGROUND_COLOR)), 0.85f));

        state->active = !GuiWindowBox((Rectangle){ state->position.x + 0, state->position.y + 0, state->width, state->height }, "About rFxGen");
        
        // TODO: Create a color rectangle or color-config panel...
        DrawRectangle(state->position.x + 1, state->position.y + 5 + 20, 330 - 2, 90 - 5, GetColor(GuiGetStyleProperty(DEFAULT_BASE_COLOR_DISABLED)));
        
#if defined(VERSION_ONE)
        DrawIcon(state->position.x + 10, state->position.y + 35, 64, "rFX", 20, true, GetColor(toolColor));
        GuiLabel((Rectangle){ state->position.x + 85, state->position.y + 60, 200, 25 }, FormatText("%s ONE %s", lblNameVersionText, lblDateText));
#else
        DrawIcon(state->position.x + 10, state->position.y + 35, 64, "rFX", 20, false, GetColor(toolColor));
        GuiLabel((Rectangle){ state->position.x + 85, state->position.y + 60, 200, 25 }, FormatText("%s ZERO %s", lblNameVersionText, lblDateText));
#endif

        GuiLabel((Rectangle){ state->position.x + 85, state->position.y + 80, 225, 20 }, lblDescriptionText);

        GuiLine((Rectangle){ state->position.x + 0, state->position.y + 100, 330, 20 }, 1);
        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 110, 126, 25 }, lblUsedLibsText);

        DrawIcon(state->position.x + 10, state->position.y + 135, 64, "raylib", 10, false, BLACK);
        DrawIcon(state->position.x + 80, state->position.y + 135, 64, "raygui", 10, false, LIGHTGRAY);

        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 130, 126, 25 }, linkraylibText)) { OpenURL("https://www.raylib.com/"); }
        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 150, 165, 25 }, linkGitraylibText)) { OpenURL("https://github.com/raysan5/raylib"); }
        if (GuiLabelButton((Rectangle){ state->position.x + 155, state->position.y + 170, 165, 25 }, linkGitrayguiText)) { OpenURL("https://github.com/raysan5/raygui"); }

        GuiLine((Rectangle){ state->position.x + 10, state->position.y + 200, 310, 20 }, 1);
        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 210, 80, 25 }, lblDevelopersText);
        GuiLabel((Rectangle){ state->position.x + 20, state->position.y + 230, 180, 25 }, lblDev01Text);
        if (GuiLabelButton((Rectangle){ state->position.x + 130, state->position.y + 230, 56, 25 }, linkDev01Text)) { OpenURL("https://twitter.com/raysan5"); }
        GuiLine((Rectangle){ state->position.x + 10, state->position.y + 250, 310, 20 }, 1);
        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 265, 289, 25 }, lblCopyrightText);
        if (GuiLabelButton((Rectangle){ state->position.x + 215, state->position.y + 265, 76, 25 }, linkraylibtech)) { OpenURL("https://twitter.com/raylibtech"); }
        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 285, 80, 25 }, lblMoreInfoText);
        GuiLabel((Rectangle){ state->position.x + 10, state->position.y + 305, 80, 25 }, lblSupportText);
        if (GuiLabelButton((Rectangle){ state->position.x + 95, state->position.y + 285, 165, 25 }, linkToolWebText)) { OpenURL("https://www.raylibtech.com/"); }
        if (GuiLabelButton((Rectangle){ state->position.x + 95, state->position.y + 305, 165, 25 }, linkMailText)) { OpenURL("mailto:ray@raylibtech.com"); }
        GuiLine((Rectangle){ state->position.x + 0, state->position.y + 325, 330, 20 }, 1);
#if defined(VERSION_ONE)
        state->chkLicenseChecked = GuiCheckBoxEx((Rectangle){ state->position.x + 10, state->position.y + 350, 15, 15 }, state->chkLicenseChecked, "License Agreement (EULA)");
#else
        GuiDisable();state->chkLicenseChecked = GuiCheckBoxEx((Rectangle){ state->position.x + 10, state->position.y + 350, 15, 15 }, state->chkLicenseChecked, "License Agreement (EULA)"); GuiEnable();
        if (GuiButton((Rectangle){ state->position.x + 175, state->position.y + 345, 70, 25 }, "Be ONE!")) { OpenURL("https://raylibtech.itch.io/rfxgen"); }
#endif
        if (GuiButton((Rectangle){ state->position.x + 250, state->position.y + 345, 70, 25 }, "Close")) state->active = false;
    }
}

#endif // GUI_WINDOW_ABOUT_IMPLEMENTATION