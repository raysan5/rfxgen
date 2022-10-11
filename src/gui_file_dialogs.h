/*******************************************************************************************
*
*   File Dialogs
*
*   MODULE USAGE:
*       #define GUI_FILE_DIALOGS_IMPLEMENTATION
*       #include "gui_file_dialogs.h"
*
*   On game draw call:  GuiFileDialog(...);
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

#ifndef GUI_FILE_DIALOGS_H
#define GUI_FILE_DIALOGS_H

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Dialog type
typedef enum DialogType {
    DIALOG_OPEN_FILE = 0,
    DIALOG_OPEN_FILE_MULTI,
    DIALOG_OPEN_DIRECTORY,
    DIALOG_SAVE_FILE,
    DIALOG_MESSAGE,
    DIALOG_TEXTINPUT,
    DIALOG_OTHER
} DialogType;


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
// Multiplatform file dialogs
// NOTE 1: fileName parameters is used to display and store selected file name
// NOTE 2: Value returned is the operation result, on custom dialogs represents button option pressed
// NOTE 3: filters and message are used for buttons and dialog messages on DIALOG_MESSAGE and DIALOG_TEXTINPUT
int GuiFileDialog(int dialogType, const char *title, char *fileName, const char *filters, const char *message);

#ifdef __cplusplus
}
#endif

#endif // GUI_FILE_DIALOGS_H


/***********************************************************************************
*
*   GUI_FILE_DIALOGS IMPLEMENTATION
*
************************************************************************************/

#if defined(GUI_FILE_DIALOGS_IMPLEMENTATION)

#include "raylib.h"     // Required for: TextSplit()

#if defined(PLATFORM_DESKTOP) && !defined(CUSTOM_MODAL_DIALOGS)
    #include "external/tinyfiledialogs.h"   // Required for: Native open/save file dialogs
#else
    #include "raygui.h"
#endif

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

// Multiplatform file dialogs
int GuiFileDialog(int dialogType, const char *title, char *fileName, const char *filters, const char *message)
{
    int result = -1;

#if defined(CUSTOM_MODAL_DIALOGS)
    switch (dialogType)
    {
        case DIALOG_OPEN_FILE: /* TODO: Load file modal dialog */ break;
        case DIALOG_OPEN_FILE_MULTI: /* TODO: Load multiple files modal dialog */ break;
        case DIALOG_OPEN_DIRECTORY: /* TODO: Load directory modal dialog */ break;
        case DIALOG_SAVE_FILE: /* TODO: Load file modal dialog */ break;
        case DIALOG_MESSAGE: result = GuiMessageBox((Rectangle){ GetScreenWidth()/2 - 160, GetScreenHeight()/2 - 120, 320, 120 }, title, message, filters); break;
        case DIALOG_TEXTINPUT: result = GuiTextInputBox((Rectangle){ GetScreenWidth()/2 - 160, GetScreenHeight()/2 - 120, 320, 120 }, title, message, filters, fileName, 512, NULL); break;
        default: break;
    }
#else   // Use native OS dialogs (tinyfiledialogs)

    const char *tempFileName = NULL;
    int filterCount = 0;
    const char **filterSplit = TextSplit(filters, ';', &filterCount);
    
    switch (dialogType)
    {
        case DIALOG_OPEN_FILE: tempFileName = tinyfd_openFileDialog(title, fileName, filterCount, filterSplit, message, 0); break;
        case DIALOG_OPEN_FILE_MULTI: tempFileName = tinyfd_openFileDialog(title, fileName, filterCount, filterSplit, message, 1); break;
        case DIALOG_OPEN_DIRECTORY: tempFileName = tinyfd_selectFolderDialog(title, fileName); break;
        case DIALOG_SAVE_FILE: tempFileName = tinyfd_saveFileDialog(title, fileName, filterCount, filterSplit, message); break;
        case DIALOG_MESSAGE: result = tinyfd_messageBox(title, message, "ok", "info", 0); break;
        case DIALOG_TEXTINPUT: tempFileName = tinyfd_inputBox(title, message, ""); break;
        default: break;
    }

    if (tempFileName != NULL) 
    {
        strcpy(fileName, tempFileName);
        result = 1;
    }
    else result = 0;
#endif

    return result;
}

#endif // GUI_WINDOW_ABOUT_IMPLEMENTATION