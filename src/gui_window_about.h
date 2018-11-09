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
// Module Functions Definition
//----------------------------------------------------------------------------------

// Init Window About
GuiWindowAboutState InitGuiWindowAbout(void)
{
    GuiWindowAboutState state = {
        .position = (Vector2){ 0, 0 },
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
    const char *lblNameVersionText = "rFXGen v1.0 ZERO";
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

    Vector2 position = state->position;

    if (state->active)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GetColor(GuiGetStyleProperty(DEFAULT_BACKGROUND_COLOR)), 0.85f));

        state->active = !GuiWindowBox((Rectangle){ position.x + 0, position.y + 0, state->width, state->height }, "About rFxGen");

        //DrawIcon(position.x + 10, position.y + 35, 64, "rFX", 20, true, GetColor(0x5197d4ff));    // Draw raylib style icon

        GuiLabel((Rectangle){ position.x + 85, position.y + 60, 136, 25 }, lblNameVersionText);
        GuiLabel((Rectangle){ position.x + 220, position.y + 60, 65, 25 }, lblDateText);
        GuiLabel((Rectangle){ position.x + 85, position.y + 80, 225, 20 }, lblDescriptionText);

        GuiLine((Rectangle){ position.x + 0, position.y + 100, 330, 20 }, 1);
        GuiLabel((Rectangle){ position.x + 10, position.y + 110, 126, 25 }, lblUsedLibsText);

        //DrawIcon(position.x + 10, position.y + 135, 64, "raylib", 16, false, BLACK);    // Draw raylib style icon
        //GuiDummyRec((Rectangle){ position.x + 10, position.y + 135, 65, 65 }, "logo_raylib");
        GuiDummyRec((Rectangle){ position.x + 80, position.y + 135, 65, 65 }, "logo_raygui");
        if (GuiLabelButton((Rectangle){ position.x + 155, position.y + 130, 126, 25 }, linkraylibText)) { /*OpenLinkURL("");*/ }
        if (GuiLabelButton((Rectangle){ position.x + 155, position.y + 150, 165, 25 }, linkGitraylibText)) { /*OpenLinkURL("");*/ }
        if (GuiLabelButton((Rectangle){ position.x + 155, position.y + 170, 165, 25 }, linkGitrayguiText)) { /*OpenLinkURL("");*/ }
        GuiLine((Rectangle){ position.x + 10, position.y + 200, 310, 20 }, 1);
        GuiLabel((Rectangle){ position.x + 10, position.y + 210, 80, 25 }, lblDevelopersText);
        GuiLabel((Rectangle){ position.x + 20, position.y + 230, 180, 25 }, lblDev01Text);
        if (GuiLabelButton((Rectangle){ position.x + 130, position.y + 230, 56, 25 }, linkDev01Text)) { /*OpenLinkURL("");*/ }
        GuiLine((Rectangle){ position.x + 10, position.y + 250, 310, 20 }, 1);
        GuiLabel((Rectangle){ position.x + 10, position.y + 265, 289, 25 }, lblCopyrightText);
        if (GuiLabelButton((Rectangle){ position.x + 215, position.y + 265, 76, 25 }, linkraylibtech)) { /*OpenLinkURL("");*/ }
        GuiLabel((Rectangle){ position.x + 10, position.y + 285, 80, 25 }, lblMoreInfoText);
        GuiLabel((Rectangle){ position.x + 10, position.y + 305, 80, 25 }, lblSupportText);
        if (GuiLabelButton((Rectangle){ position.x + 95, position.y + 285, 165, 25 }, linkToolWebText)) { /*OpenLinkURL("");*/ }
        if (GuiLabelButton((Rectangle){ position.x + 95, position.y + 305, 165, 25 }, linkMailText)) { /*OpenLinkURL("");*/ }
        GuiLine((Rectangle){ position.x + 0, position.y + 325, 330, 20 }, 1);

        GuiDisable(); state->chkLicenseChecked = GuiCheckBoxEx((Rectangle){ position.x + 10, position.y + 350, 15, 15 }, state->chkLicenseChecked, "License Agreement (EULA)"); GuiEnable();

        if (GuiButton((Rectangle){ position.x + 175, position.y + 345, 70, 25 }, "Be ONE!")) { /* OpenURL(); */ }
        if (GuiButton((Rectangle){ position.x + 250, position.y + 345, 70, 25 }, "Close")) state->active = false;
    }
}

#endif // GUI_WINDOW_ABOUT_IMPLEMENTATION