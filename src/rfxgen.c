/*******************************************************************************************
*
*   rFXGen v5.0 - A simple and easy to use sounds generator (based on Tomas Petterson sfxr)
*
*   FEATURES:
*       - Predefined sound presets (Coin, Shoot, Explosion, PowerUp...)
*       - Multiple wave types supported (Square, Sawtooth, Sine, Noise)
*       - Up to 5 sound slots to store generated sound (temporary save)
*       - Load/Save .rfx files with sound generation parameters (104 bytes only)
*       - Export wave data as .wav, .qoa, .raw or .h code file (byte array)
*       - Configurable sample-rate, bits-per-sample and channels on export
*       - Multiple UI styles with support for custom ones (.rgs)
*       - Sound generator as OSS single-file header-only lib: rfxgen.h
*
*   LIMITATIONS:
*       - Limitation 01
*       - Limitation 02
*
*   POSSIBLE IMPROVEMENTS:
*       - Improvement 01
*       - Improvement 02
*
*   CONFIGURATION:
*       #define CUSTOM_MODAL_DIALOGS
*           Use custom raygui generated modal dialogs instead of native OS ones
*           NOTE: Avoids including tinyfiledialogs depencency library
*
*   VERSIONS HISTORY:
*       5.0  (10-Nov-2025)  ADDED: Up to 10 sound slots available
*                           ADDED: New UI default style: Genesis
*                           REVIEWED: UI design, longer sliders 
*                           REVIEWED: Available UI styles
*                           REVIEWED: Welcome window
*                           UPDATED: Using raylib 5.6-dev and raygui 4.5-dev
* 
*       4.2  (23-Apr-2024)  ADDED: Issue report window
*                           ADDED: Support splash screen
*                           REMOVED: Sponsors window
*                           REVIEWED: Main toolbar and help window
*                           UPDATED: Using raylib 5.1-dev and raygui 4.1-dev
*
*       4.0  (20-Sep-2023)  ADDED: Using pseudo-random number generator
*                           ADDED: Support macOS builds (x86_64 + arm64)
*                           ADDED: Support loading .qoa wave data
*                           ADDED: Support .qoa export and play on CLI
*                           REVIEWED: Initial UI style
*                           REVIEWED: Regenerated tool imagery
*                           REVIEWED: Sound generation issues, improved
*                           UPDATED: Using raygui 4.0 and latest raylib 4.6-dev
*
*       3.3  (06-Mar-2023)  ADDED: Support export to .qoa file format
*
*       3.2  (14-Dec-2022)  ADDED: Welcome window with sponsors info
*                           REDESIGNED: Main toolbar to add tooltips
*                           REVIEWED: Help window implementation
*
*       3.1  (06-Sep-2022)  REDESIGNED: Sound generator as a header-only library: rfxgen.h
*                           ADDED: Sponsor window for tools support
*                           Updated to raylib 4.5-dev and raygui 3.5-dev
*
*       3.0  (30-Sep-2022)  UI redesigned to follow raylibtech UI conventions
*                           ADDED: Main toolbar to access File/Tools/Visual options
*                           ADDED: Help window with keyboard shortcuts info
*                           ADDED: One extra sound slot and key selection
*                           REMOVED: Support for .sfs files (import issues)
*                           REVIEWED: Issues when exporting wave to code file
*                           ADDED: a new gui style: terminal
*                           Updated to raylib 4.2 and raygui 3.2
*
*       2.5  (28-Dec-2021)  REVIEWED: Issue with 32bit float WAV export
*                           REVIEWED: Issue with WaveMutate() convergence
*                           REVIEWED: Code naming conventions
*                           REMOVED: Tool references to ZERO or ONE
*                           ADDED: New gui style: lavanda
*                           Updated to raylib 4.2-dev and raygui 3.1
*
*       2.3  (20-Dec-2020)  Updated to raylib 3.5
*       2.2  (23-Feb-2019)  Updated to raylib 3.0, raygui 2.7 and adapted for web
*       2.1  (09-Sep-2019)  ADDED: Support custom file dialogs (on non-DESKTOP platforms)
*                           REVIEWED: Screen resize to adapt to new styles fonts
*                           Updated to raygui 2.6
*
*       2.0  (xx-Nov-2018)  GUI redesigned, CLI improvements
*       1.8  (10-Oct-2018)  Functions renaming, code reorganized, better consistency...
*       1.5  (23-Sep-2018)  Support .wav export to code and sound playing on command line
*       1.4  (15-Sep-2018)  Redesigned command line and comments
*       1.3  (15-May-2018)  Reimplemented gui using rGuiLayout
*       1.2  (16-Mar-2018)  Working on some code improvements and GUI review
*       1.1  (01-Oct-2017)  Code review, simplified
*       1.0  (18-Mar-2017)  First release
*       0.95 (14-Sep-2016)  Reviewed comments and .rfx format
*       0.9  (12-Sep-2016)  Defined WaveParams struct and command line functionality
*       0.8  (09-Sep-2016)  Added open/save file dialogs using tinyfiledialogs library
*       0.7  (04-Sep-2016)  Program variables renaming for consistency, code reorganized
*       0.6  (30-Aug-2016)  Interface redesigned (reduced size) and new features added (wave drawing)
*       0.5  (27-Aug-2016)  Completed port and adaptation from sfxr (only sound generation and playing)
*
*   DEPENDENCIES:
*       raylib 5.6-dev          - Windowing/input management and drawing
*       raygui 4.5-dev          - Immediate-mode GUI controls with custom styling and icons
*       tinyfiledialogs 3.20    - Open/save file dialogs, it requires linkage with comdlg32 and ole32 libs
*
*   BUILDING:
*     - Windows (MinGW-w64):
*       gcc -o rfxgen.exe rfxgen.c external/tinyfiledialogs.c -s rfxgen_icon -Iexternal /
*           -lraylib -lopengl32 -lgdi32 -lcomdlg32 -lole32 -std=c99 -Wl,--subsystem,windows
*
*     - Linux (GCC):
*       gcc -o rfxgen rfxgen.c external/tinyfiledialogs.c -s -Iexternal -no-pie -D_DEFAULT_SOURCE /
*           -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
*
*   ADDITIONAL NOTES:
*       On PLATFORM_ANDROID and PLATFORM_WEB file dialogs are not available
*
*   DEVELOPERS:
*       Ramon Santamaria (@raysan5):   Developer, supervisor, updater and maintainer
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2025 raylib technologies (@raylibtech) / Ramon Santamaria (@raysan5)
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

#define TOOL_NAME               "rFXGen"
#define TOOL_SHORT_NAME         "rFX"
#define TOOL_VERSION            "5.0"
#define TOOL_DESCRIPTION        "A simple and easy-to-use fx sounds generator"
#define TOOL_RELEASE_DATE       "Nov.2025"
#define TOOL_LOGO_COLOR         0x5197d4ff

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#define RFXGEN_IMPLEMENTATION
#define RFXGEN_CALLOC           RL_CALLOC
#define RFXGEN_FREE             RL_FREE
#define RFXGEN_RAND             GetRandomValue      // Using raylib provided generator: rprand
#define RFXGEN_SRAND            SetRandomSeed       // Using raylib provided generator: rprand
#define RFXGEN_ISFILEEXTENSION  IsFileExtension
#include "rfxgen.h"                         // Sound generation library

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"                         // Required for: IMGUI controls

#undef RAYGUI_IMPLEMENTATION                // Avoid including raygui implementation again

#define GUI_MAIN_TOOLBAR_IMPLEMENTATION
#include "gui_main_toolbar.h"               // GUI: Main toolbar

#define GUI_WINDOW_HELP_IMPLEMENTATION
#include "gui_window_help.h"                // GUI: Help Window

#define GUI_WINDOW_ABOUT_IMPLEMENTATION
#include "gui_window_about.h"               // GUI: About Window

#define GUI_FILE_DIALOGS_IMPLEMENTATION
#include "gui_file_dialogs.h"               // GUI: File Dialogs

// raygui embedded styles
// NOTE: Included in the same order as selector
#define MAX_GUI_STYLES_AVAILABLE   5        // NOTE: Included light style
#include "styles/style_genesis.h"           // raygui style: genesis
#include "styles/style_cyber.h"             // raygui style: cyber
#include "styles/style_terminal.h"          // raygui style: terminal
#include "styles/style_lavanda.h"           // raygui style: lavanda
#include "styles/style_amber.h"             // raygui style: amber

// C standard library
#include <math.h>                   // Required for: sinf(), powf()
#include <time.h>                   // Required for: clock()
#include <stdlib.h>                 // Required for: calloc(), free()
#include <string.h>                 // Required for: strcmp()
#include <stdio.h>                  // Required for: FILE, fopen(), fread(), fwrite(), ftell(), fseek() fclose()
                                    // NOTE: Used on functions: LoadSound(), SaveSound(), WriteWAV()
#if defined(_WIN32)
    #include <conio.h>              // Required for: kbhit() [Windows only, no stardard library]
#else
    // Provide kbhit() function in non-Windows platforms
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#if (!defined(_DEBUG) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)))
bool __stdcall FreeConsole(void);           // Close console from code (kernel32.lib)
#endif

// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO) && defined(_DEBUG)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

#define MAX_WAVE_SLOTS       10             // Number of wave slots for generation

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const char *toolName = TOOL_NAME;
static const char *toolVersion = TOOL_VERSION;
static const char *toolDescription = TOOL_DESCRIPTION;

// NOTE: Max length depends on OS, in Windows MAX_PATH = 256
static char inFileName[512] = { 0 };        // Input file name (required in case of drag & drop over executable)
static char outFileName[512] = { 0 };       // Output file name (required for file save/export)
static char presetType[512] = { 0 };        // Type of the preset to be generated

static float volumeValue = 0.6f;            // Master volume

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
#if defined(PLATFORM_DESKTOP)
static void ShowCommandLineInfo(void);                      // Show command line usage info
static void ProcessCommandLine(int argc, char *argv[]);     // Process command line input
#endif

// Auxiliar functions
static void DrawWave(Wave *wave, Rectangle bounds, Color color);    // Draw wave data using lines

#if defined(PLATFORM_DESKTOP)
static void WaitTimePlayer(int ms);             // Simple time wait in milliseconds for the CLI player
static void PlayWaveCLI(Wave wave);             // Play provided wave through CLI
#if !defined(_WIN32)
static int kbhit(void);                         // Check if a key has been pressed
static char getch(void) { return getchar(); }   // Get pressed character
#endif
#endif  // PLATFORM_DESKTOP

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif
#if defined(PLATFORM_DESKTOP)
    // Command-line usage mode
    //--------------------------------------------------------------------------------------
    if (argc > 1)
    {
        if ((argc == 2) &&
            (strcmp(argv[1], "-h") != 0) &&
            (strcmp(argv[1], "--help") != 0))       // One argument (file dropped over executable?)
        {
            if (IsFileExtension(argv[1], ".rfx"))   // || IsFileExtension(argv[1], ".sfs"))
            {
                strcpy(inFileName, argv[1]);        // Read input filename to open with gui interface
            }
        }
        else
        {
            ProcessCommandLine(argc, argv);
            return 0;
        }
    }
#endif  // PLATFORM_DESKTOP
#if (!defined(_DEBUG) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)))
    // WARNING (Windows): If program is compiled as Window application (instead of console),
    // no console is available to show output info... solution is compiling a console application
    // and closing console (FreeConsole()) when changing to GUI interface
    FreeConsole();
#endif

    // GUI usage mode - Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 640;
    const int screenHeight = 580;

    //SetConfigFlags(FLAG_MSAA_4X_HINT);        // Window configuration flags
    InitWindow(screenWidth, screenHeight, TextFormat("%s v%s | %s", toolName, toolVersion, toolDescription));
    SetExitKey(0);

    InitAudioDevice();

    // GUI: Main Layout
    //-----------------------------------------------------------------------------------
    bool playOnChange = true;           // Automatically play sound on parameter change
    bool screenSizeActive = false;      // Scale screen x2 (useful for HighDPI screens)
    //-----------------------------------------------------------------------------------

    // GUI: Main toolbar panel (file and visualization)
    //-----------------------------------------------------------------------------------
    GuiMainToolbarState mainToolbarState = InitGuiMainToolbar();
    //-----------------------------------------------------------------------------------

    // GUI: Help Window
    //-----------------------------------------------------------------------------------
    GuiWindowHelpState windowHelpState = InitGuiWindowHelp();
    //-----------------------------------------------------------------------------------

    // GUI: About Window
    //-----------------------------------------------------------------------------------
    GuiWindowAboutState windowAboutState = InitGuiWindowAbout();
    //-----------------------------------------------------------------------------------

    // GUI: Support Message Box
    //-----------------------------------------------------------------------------------
#if defined(PLATFORM_WEB)
    bool showSupportMessage = false;
#else
    bool showSupportMessage = false;
#endif
    int supportMessageRandBtn = GetRandomValue(0, 1);
    //-----------------------------------------------------------------------------------

    // GUI: Issue Report Window
    //-----------------------------------------------------------------------------------
    bool showIssueReportWindow = false;
    //-----------------------------------------------------------------------------------

    // GUI: Export Window
    //-----------------------------------------------------------------------------------
    bool showExportWindow = false;

    int fileTypeActive = 0;         // ComboBox file type selection
    int sampleRateActive = 1;       // ComboBox sample rate selection
    int sampleSizeActive = 1;       // ComboBox sample size selection
    int channelsActive = 0;         // ComboBox channels selection

    int exportSampleSize = 32;      // Export wave sample size in bits (bitrate)
    int exportSampleRate = 44100;   // Export wave sample rate (frequency)
    int exportChannels = 1;         // Export wave channels
    //-----------------------------------------------------------------------------------

    // GUI: Exit Window
    //-----------------------------------------------------------------------------------
    bool closeWindow = false;
    bool showExitWindow = false;
    //-----------------------------------------------------------------------------------

    // GUI: Custom file dialogs
    //-----------------------------------------------------------------------------------
    bool showLoadFileDialog = false;
    bool showSaveFileDialog = false;
    bool showExportFileDialog = false;
    //-----------------------------------------------------------------------------------

    // Wave and Sound Initialization
    //-----------------------------------------------------------------------------------
    WaveParams params[MAX_WAVE_SLOTS] = { 0 }; // Wave parameters for generation
    Wave wave[MAX_WAVE_SLOTS] = { 0 };
    Sound sound[MAX_WAVE_SLOTS] = { 0 };

    for (int i = 0; i < MAX_WAVE_SLOTS; i++)
    {
        // Reset generation parameters
        // NOTE: Random seed for generation is set
        ResetWaveParams(&params[i]);

        // Default wave values
        wave[i].sampleRate = RFXGEN_GEN_SAMPLE_RATE;
        wave[i].sampleSize = RFXGEN_GEN_SAMPLE_SIZE;
        wave[i].channels = RFXGEN_GEN_CHANNELS;
        wave[i].frameCount = 10*wave[i].sampleRate;    // Max frame count for 10 seconds
        wave[i].data = (float *)RL_CALLOC(wave[i].frameCount, sizeof(float));

        sound[i] = LoadSoundFromWave(wave[i]);
    }
    //-----------------------------------------------------------------------------------

    // Check if a wave parameters file has been provided on command line
    if (inFileName[0] != '\0')
    {
        // Clean everything (just in case)
        UnloadWave(wave[0]);
        UnloadSound(sound[0]);

        params[0] = LoadWaveParams(inFileName); // Load wave parameters from .rfx

        // NOTE: GenerateWave() returns data as 32bit float, 1 channel by default
        wave[0].sampleRate = RFXGEN_GEN_SAMPLE_RATE;
        wave[0].sampleSize = 32;
        wave[0].channels = 1;
        wave[0].data = GenerateWave(params[0], &wave[0].frameCount);

        sound[0] = LoadSoundFromWave(wave[0]);  // Load sound from new wave

        PlaySound(sound[0]);                    // Play generated sound
    }

    bool regenerate = false;                    // Wave regeneration required

    float prevVolumeValue = volumeValue;
    int prevWaveTypeValue[MAX_WAVE_SLOTS] = { params[0].waveTypeValue };

    Rectangle waveRec = { 12, 484, 616, 64 };       // Wave drawing rectangle box
    Rectangle slidersRec = { 256, 82, 226, 392 };   // Area defining sliders to allow sound replay when mouse-released

    // Set default sound volume
    for (int i = 0; i < MAX_WAVE_SLOTS; i++) SetSoundVolume(sound[i], volumeValue);

    // Render texture to draw wave at x2, it will be scaled down with bilinear filtering (cheapre than MSAA x4)
    RenderTexture2D waveTarget = LoadRenderTexture((int)waveRec.width*2, (int)waveRec.height*2);
    SetTextureFilter(waveTarget.texture, TEXTURE_FILTER_BILINEAR);

    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    RenderTexture2D screenTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    SetTextureFilter(screenTarget.texture, TEXTURE_FILTER_POINT);

    GuiLoadStyleGenesis();  // Load initial style

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!closeWindow)    // Detect window close button
    {
        // WARNING: ASINCIFY requires this line,
        // it contains the call to emscripten_sleep() for PLATFORM_WEB
        if (WindowShouldClose()) closeWindow = true;

        // Dropped files logic
        //----------------------------------------------------------------------------------
        if (IsFileDropped())
        {
            FilePathList droppedFiles = LoadDroppedFiles();

            // Support loading .rfx files (wave parameters)
            if (IsFileExtension(droppedFiles.paths[0], ".rfx")) // || IsFileExtension(droppedFiles.paths[0], ".sfs"))
            {
                params[mainToolbarState.soundSlotActive] = LoadWaveParams(droppedFiles.paths[0]);
                regenerate = true;

                SetWindowTitle(TextFormat("%s v%s | File: %s", toolName, toolVersion, GetFileName(droppedFiles.paths[0])));
            }
            else if (IsFileExtension(droppedFiles.paths[0], ".rgs")) GuiLoadStyle(droppedFiles.paths[0]);

            UnloadDroppedFiles(droppedFiles);    // Unload filepaths from memory
        }
        //----------------------------------------------------------------------------------

        // Keyboard shortcuts
        //------------------------------------------------------------------------------------
        // Show dialog: save sound (.rfx)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S))
        {
            strcpy(outFileName, "sound.rfx");
            showSaveFileDialog = true;
        }

        // Show dialog: load sound (.rfx)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_O)) showLoadFileDialog = true;

        // Show dialog: export wave (.wav, .qoa, .raw, .h)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_E)) showExportWindow = true;

        if (!showSaveFileDialog)
        {
            // Select current sound slot
            if (IsKeyPressed(KEY_ONE)) mainToolbarState.soundSlotActive = 0;
            else if (IsKeyPressed(KEY_TWO)) mainToolbarState.soundSlotActive = 1;
            else if (IsKeyPressed(KEY_THREE)) mainToolbarState.soundSlotActive = 2;
            else if (IsKeyPressed(KEY_FOUR)) mainToolbarState.soundSlotActive = 3;
            else if (IsKeyPressed(KEY_FIVE)) mainToolbarState.soundSlotActive = 4;

            // Play current sound
            if (IsKeyPressed(KEY_SPACE)) PlaySound(sound[mainToolbarState.soundSlotActive]);

            // Toggle play on change option
            if (IsKeyPressed(KEY_P)) playOnChange = !playOnChange;
        }

        // Select visual style
        //if (IsKeyPressed(KEY_LEFT)) mainToolbarState.visualStyleActive--;
        //else if (IsKeyPressed(KEY_RIGHT)) mainToolbarState.visualStyleActive++;
        //if (mainToolbarState.visualStyleActive < 0) mainToolbarState.visualStyleActive = MAX_GUI_STYLES_AVAILABLE - 1;
        //else if (mainToolbarState.visualStyleActive > (MAX_GUI_STYLES_AVAILABLE - 1)) mainToolbarState.visualStyleActive = 0;

#if !defined(PLATFORM_WEB)
        // Toggle screen size (x2) mode
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F)) screenSizeActive = !screenSizeActive;
#endif
        // Toggle window: help
        if (IsKeyPressed(KEY_F1)) windowHelpState.windowActive = !windowHelpState.windowActive;

        // Toggle window: about
        if (IsKeyPressed(KEY_F2)) windowAboutState.windowActive = !windowAboutState.windowActive;

        // Toggle window: issue report
        if (IsKeyPressed(KEY_F3)) showIssueReportWindow = !showIssueReportWindow;

        // Show closing window on ESC
        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (windowAboutState.windowActive) windowAboutState.windowActive = false;
            else if (windowHelpState.windowActive) windowHelpState.windowActive = false;
            else if (showIssueReportWindow) showIssueReportWindow = false;
            else if (showSupportMessage) showSupportMessage = false;
            else if (showExportWindow) showExportWindow = false;
        #if defined(PLATFORM_DESKTOP)
            else showExitWindow = !showExitWindow;
        #else
            else if (showLoadFileDialog) showLoadFileDialog = false;
            else if (showSaveFileDialog) showSaveFileDialog = false;
            else if (showExportFileDialog) showExportFileDialog = false;
        #endif
        }
        //----------------------------------------------------------------------------------

        // Main toolbar logic
        //----------------------------------------------------------------------------------
        // File options logic
        if (mainToolbarState.btnNewFilePressed)
        {
            // Reload current slot
            UnloadSound(sound[mainToolbarState.soundSlotActive]);
            UnloadWave(wave[mainToolbarState.soundSlotActive]);

            wave[mainToolbarState.soundSlotActive].data = (float *)RL_CALLOC(wave[mainToolbarState.soundSlotActive].frameCount, sizeof(float));
            sound[mainToolbarState.soundSlotActive] = LoadSoundFromWave(wave[mainToolbarState.soundSlotActive]);
        }
        else if (mainToolbarState.btnLoadFilePressed) showLoadFileDialog = true;
        else if (mainToolbarState.btnSaveFilePressed)
        {
            strcpy(outFileName, "sound.rfx");
            showSaveFileDialog = true;
        }
        else if (mainToolbarState.btnExportFilePressed) showExportWindow = true;

        if (mainToolbarState.visualStyleActive != mainToolbarState.prevVisualStyleActive)
        {
            // Reset to default internal style
            // NOTE: Required to unload any previously loaded font texture
            GuiLoadStyleDefault();

            switch (mainToolbarState.visualStyleActive)
            {
                case 0: GuiLoadStyleGenesis(); break;
                case 1: GuiLoadStyleCyber(); break;
                case 2: GuiLoadStyleTerminal(); break;
                case 3: GuiLoadStyleLavanda(); break;
                case 4: GuiLoadStyleAmber(); break;
                default: break;
            }

            mainToolbarState.prevVisualStyleActive = mainToolbarState.visualStyleActive;
        }

        // Help options logic
        if (mainToolbarState.btnHelpPressed) windowHelpState.windowActive = true;
        if (mainToolbarState.btnAboutPressed) windowAboutState.windowActive = true;
        if (mainToolbarState.btnIssuePressed) showIssueReportWindow = true;
        //----------------------------------------------------------------------------------

        // Basic program flow logic
        //----------------------------------------------------------------------------------
        // Check for changed gui values
        if (volumeValue != prevVolumeValue)
        {
            SetMasterVolume(volumeValue);
            prevVolumeValue = volumeValue;
        }

        // Check wave type combobox selection to regenerate wave
        if (params[mainToolbarState.soundSlotActive].waveTypeValue != prevWaveTypeValue[mainToolbarState.soundSlotActive]) regenerate = true;
        prevWaveTypeValue[mainToolbarState.soundSlotActive] = params[mainToolbarState.soundSlotActive].waveTypeValue;

        // Avoid wave regeneration when some window is active
        if (!windowHelpState.windowActive &&
            !windowAboutState.windowActive &&
            !showIssueReportWindow &&
            !showSupportMessage &&
            !showLoadFileDialog &&
            !showSaveFileDialog &&
            !showExportFileDialog &&
            !showExportWindow &&
            !showExitWindow)
        {
            // Consider two possible cases to regenerate wave and update sound:
            // CASE1: regenerate flag is true (set by sound buttons functions)
            // CASE2: Mouse is moving sliders and mouse is released (checks against slidersRec)
            if (regenerate || ((CheckCollisionPointRec(GetMousePosition(), slidersRec)) && (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))))
            {
                UnloadWave(wave[mainToolbarState.soundSlotActive]);
                UnloadSound(sound[mainToolbarState.soundSlotActive]);

                // Generate new wave from parameters
                // NOTE: GenerateWave() returns data as 32bit float, 1 channel by default
                wave[mainToolbarState.soundSlotActive].sampleRate = RFXGEN_GEN_SAMPLE_RATE;
                wave[mainToolbarState.soundSlotActive].sampleSize = 32;
                wave[mainToolbarState.soundSlotActive].channels = 1;
                wave[mainToolbarState.soundSlotActive].data = GenerateWave(params[mainToolbarState.soundSlotActive], &wave[mainToolbarState.soundSlotActive].frameCount);

                sound[mainToolbarState.soundSlotActive] = LoadSoundFromWave(wave[mainToolbarState.soundSlotActive]);    // Reload sound from new wave

                if ((regenerate || playOnChange) &&
                    !windowAboutState.windowActive &&
                    !windowHelpState.windowActive &&
                    !showIssueReportWindow)
                {
                    PlaySound(sound[mainToolbarState.soundSlotActive]);
                }

                regenerate = false;
            }
        }

        // Check slot change to play next one selected
        if (mainToolbarState.soundSlotActive != mainToolbarState.prevSoundSlotActive)
        {
            PlaySound(sound[mainToolbarState.soundSlotActive]);
            mainToolbarState.prevSoundSlotActive = mainToolbarState.soundSlotActive;
        }

        // Screen and mouse scale logic (x2)
        //----------------------------------------------------------------------------------
        if (screenSizeActive)
        {
            // Screen size x2
            if (GetScreenWidth() < screenWidth*2)
            {
                SetWindowSize(screenWidth*2, screenHeight*2);
                SetMouseScale(0.5f, 0.5f);
            }
        }
        else
        {
            // Screen size x1
            if (screenWidth*2 >= GetScreenWidth())
            {
                SetWindowSize(screenWidth, screenHeight);
                SetMouseScale(1.0f, 1.0f);
            }
        }

        // WARNING: Some windows should lock the main screen controls when shown
        if (windowHelpState.windowActive ||
            windowAboutState.windowActive ||
            showIssueReportWindow ||
            showSupportMessage ||
            showExportWindow ||
            showExitWindow ||
            showLoadFileDialog ||
            showSaveFileDialog ||
            showExportFileDialog) GuiLock();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // Render wave data to texture
        BeginTextureMode(waveTarget);
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            DrawWave(&wave[mainToolbarState.soundSlotActive], (Rectangle){ 0, 0, (float)waveTarget.texture.width, (float)waveTarget.texture.height }, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_PRESSED)));
        EndTextureMode();

        // Render all screen to texture (for scaling)
        BeginTextureMode(screenTarget);
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            // rFXGen Layout: controls drawing
            //----------------------------------------------------------------------------------
            // Draw left buttons
            int prevTextPadding = GuiGetStyle(BUTTON, TEXT_PADDING);
            GuiSetStyle(BUTTON, TEXT_PADDING, 3);
            GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            if (GuiButton((Rectangle){ 12, 48, 108, 24 }, "#131#Play Sound")) PlaySound(sound[mainToolbarState.soundSlotActive]);

            if (GuiButton((Rectangle){ 12, 88, 108, 24 }, "#146#Pickup/Coin")) { params[mainToolbarState.soundSlotActive] = GenPickupCoin(); regenerate = true; }
            if (GuiButton((Rectangle){ 12, 116, 108, 24 }, "#145#Laser/Shoot")) { params[mainToolbarState.soundSlotActive] = GenLaserShoot(); regenerate = true; }
            if (GuiButton((Rectangle){ 12, 144, 108, 24 }, "#147#Explosion")) { params[mainToolbarState.soundSlotActive] = GenExplosion(); regenerate = true; }
            if (GuiButton((Rectangle){ 12, 172, 108, 24 }, "#148#PowerUp")) { params[mainToolbarState.soundSlotActive] = GenPowerup(); regenerate = true; }
            if (GuiButton((Rectangle){ 12, 200, 108, 24 }, "#152#Hit/Hurt")) { params[mainToolbarState.soundSlotActive] = GenHitHurt(); regenerate = true; }
            if (GuiButton((Rectangle){ 12, 228, 108, 24 }, "#150#Jump")) { params[mainToolbarState.soundSlotActive] = GenJump(); regenerate = true; }
            if (GuiButton((Rectangle){ 12, 256, 108, 24 }, "#144#Blip/Select")) { params[mainToolbarState.soundSlotActive] = GenBlipSelect(); regenerate = true; }
            GuiSetStyle(BUTTON, TEXT_PADDING, prevTextPadding);
            GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

            GuiLine((Rectangle){ 12, 248 + 32, 108, 16 }, NULL);

            GuiSetStyle(TOGGLE, TEXT_PADDING, 3);
            GuiSetStyle(TOGGLE, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            GuiToggleGroup((Rectangle){ 12, 248 + 32 + 16, 108, 24 }, "#126#Square\n#127#Sawtooth\n#125#Sinewave\n#124#Noise", &params[mainToolbarState.soundSlotActive].waveTypeValue);
            GuiSetStyle(TOGGLE, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
            GuiSetStyle(TOGGLE, TEXT_PADDING, 0);

            GuiLine((Rectangle){ 12, 248 + 32 + 16 + 4*24 + 3*2, 108, 16 }, NULL);

            if (GuiButton((Rectangle){ 12, 414, 108, 24 }, "#75#Mutate")) { WaveMutate(&params[mainToolbarState.soundSlotActive]); regenerate = true; }
            if (GuiButton((Rectangle){ 12, 414 + 24 + 4, 108, 24 }, "#77#Randomize")) { params[mainToolbarState.soundSlotActive] = GenRandomize(); regenerate = true; }

            // Parameters sliders
            //--------------------------------------------------------------------------------
            Vector2 paramsPos = { 260, 56 };

            GuiGroupBox((Rectangle){ paramsPos.x - 130, paramsPos.y - 8, 498, 24 }, NULL);
            GuiGroupBox((Rectangle){ paramsPos.x - 130, paramsPos.y + 24, 498, 16*4 + 8 }, NULL);
            GuiGroupBox((Rectangle){ paramsPos.x - 130, paramsPos.y + 95, 498, 16*2 + 8 + 1 }, NULL);
            GuiGroupBox((Rectangle){ paramsPos.x - 130, paramsPos.y + 135, 498, 16*4 + 8 + 2 }, NULL);
            GuiGroupBox((Rectangle){ paramsPos.x - 130, paramsPos.y + 208, 498, 16*4 + 8 + 1 }, NULL);
            GuiGroupBox((Rectangle){ paramsPos.x - 130, paramsPos.y + 280, 498, 16*3 + 8 }, NULL);
            GuiGroupBox((Rectangle){ paramsPos.x - 130, paramsPos.y + 335, 498, 16*5 + 8 + 1 }, NULL);

            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y - 2, 320, 12 }, "VOLUME", TextFormat("%i", (int)(volumeValue*100)), &volumeValue, 0, 1);

            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 30, 320, 12 }, "ATTACK TIME", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].attackTimeValue), &params[mainToolbarState.soundSlotActive].attackTimeValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "SUSTAIN TIME", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].sustainTimeValue), &params[mainToolbarState.soundSlotActive].sustainTimeValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "SUSTAIN PUNCH", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].sustainPunchValue), &params[mainToolbarState.soundSlotActive].sustainPunchValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "DECAY TIME", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].decayTimeValue), &params[mainToolbarState.soundSlotActive].decayTimeValue, 0, 1);

            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 24, 320, 12 }, "START FREQUENCY", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].startFrequencyValue), &params[mainToolbarState.soundSlotActive].startFrequencyValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "MIN FREQUENCY", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].minFrequencyValue), &params[mainToolbarState.soundSlotActive].minFrequencyValue, 0, 1);

            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 24, 320, 12 }, "SLIDE", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].slideValue), &params[mainToolbarState.soundSlotActive].slideValue, -1, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "DELTA SLIDE", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].deltaSlideValue), &params[mainToolbarState.soundSlotActive].deltaSlideValue, -1, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "VIBRATO DEPTH", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].vibratoDepthValue), &params[mainToolbarState.soundSlotActive].vibratoDepthValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "VIBRATO SPEED", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].vibratoSpeedValue), &params[mainToolbarState.soundSlotActive].vibratoSpeedValue, 0, 1);

            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 24, 320, 12 }, "CHANGE AMOUNT", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].changeAmountValue), &params[mainToolbarState.soundSlotActive].changeAmountValue, -1, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "CHANGE SPEED", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].changeSpeedValue), &params[mainToolbarState.soundSlotActive].changeSpeedValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "SQUARE DUTY", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].squareDutyValue), &params[mainToolbarState.soundSlotActive].squareDutyValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "DUTY SWEEP", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].dutySweepValue), &params[mainToolbarState.soundSlotActive].dutySweepValue, -1, 1);

            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 24, 320, 12 }, "REPEAT SPEED", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].repeatSpeedValue), &params[mainToolbarState.soundSlotActive].repeatSpeedValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "PHASER OFFSET", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].phaserOffsetValue), &params[mainToolbarState.soundSlotActive].phaserOffsetValue, -1, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "PHASER SWEEP", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].phaserSweepValue), &params[mainToolbarState.soundSlotActive].phaserSweepValue, -1, 1);

            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 24, 320, 12 }, "LPF CUTOFF", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].lpfCutoffValue), &params[mainToolbarState.soundSlotActive].lpfCutoffValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "LPF CUTOFF SWEEP", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].lpfCutoffSweepValue), &params[mainToolbarState.soundSlotActive].lpfCutoffSweepValue, -1, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "LPF RESONANCE", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].lpfResonanceValue), &params[mainToolbarState.soundSlotActive].lpfResonanceValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "HPF CUTOFF", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].hpfCutoffValue), &params[mainToolbarState.soundSlotActive].hpfCutoffValue, 0, 1);
            GuiSliderBar((Rectangle){ paramsPos.x, paramsPos.y += 16, 320, 12 }, "HPF CUTOFF SWEEP", TextFormat("%.2f", params[mainToolbarState.soundSlotActive].hpfCutoffSweepValue), &params[mainToolbarState.soundSlotActive].hpfCutoffSweepValue, -1, 1);
            //--------------------------------------------------------------------------------

            // Draw Wave form
            //--------------------------------------------------------------------------------
            DrawTextureEx(waveTarget.texture, (Vector2){ waveRec.x, waveRec.y }, 0.0f, 0.5f, WHITE);
            DrawRectangle((int)waveRec.x, (int)waveRec.y + (int)waveRec.height/2, (int)waveRec.width, 1, Fade(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_FOCUSED)), 0.6f));
            DrawRectangleLinesEx(waveRec, 1.0f, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
            //--------------------------------------------------------------------------------

            // GUI: Main toolbar panel
            //----------------------------------------------------------------------------------
            GuiMainToolbar(&mainToolbarState);
            //----------------------------------------------------------------------------------

            // GUI: Status bar
            //----------------------------------------------------------------------------------
            int textPadding = GuiGetStyle(STATUSBAR, TEXT_PADDING);
            GuiSetStyle(STATUSBAR, TEXT_PADDING, 0);
            GuiSetStyle(STATUSBAR, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
            GuiStatusBar((Rectangle){ 0, screenHeight - 24, 190, 24 }, TextFormat("Total Frames: %i", wave[mainToolbarState.soundSlotActive].frameCount));
            GuiStatusBar((Rectangle){ 190 - 1, screenHeight - 24, 170, 24 }, TextFormat("Duration: %i ms", wave[mainToolbarState.soundSlotActive].frameCount*1000/(wave[mainToolbarState.soundSlotActive].sampleRate)));
            GuiStatusBar((Rectangle){ 190 + 170 - 2, screenHeight - 24, screenWidth - (190 + 170 - 2), 24 }, TextFormat("Size: %i bytes", wave[mainToolbarState.soundSlotActive].frameCount *wave[mainToolbarState.soundSlotActive].channels *exportSampleSize/8));
            GuiSetStyle(STATUSBAR, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
            GuiSetStyle(STATUSBAR, TEXT_PADDING, textPadding);
            //----------------------------------------------------------------------------------

            // NOTE: If some overlap window is open and main window is locked, we draw a background rectangle
            if (GuiIsLocked()) DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)), 0.85f));

            // WARNING: Before drawing the windows, we unlock them
            GuiUnlock();

            // GUI: Help Window
            //----------------------------------------------------------------------------------------
            windowHelpState.windowBounds.x = (float)screenWidth/2 - windowHelpState.windowBounds.width/2;
            windowHelpState.windowBounds.y = (float)screenHeight/2 - windowHelpState.windowBounds.height/2 - 20;
            GuiWindowHelp(&windowHelpState);
            //----------------------------------------------------------------------------------------

            // GUI: About Window
            //----------------------------------------------------------------------------------------
            windowAboutState.windowBounds.x = (float)screenWidth/2 - windowAboutState.windowBounds.width/2;
            windowAboutState.windowBounds.y = (float)screenHeight/2 - windowAboutState.windowBounds.height/2 - 20;
            GuiWindowAbout(&windowAboutState);
            //----------------------------------------------------------------------------------------

            // GUI: Issue Report Window
            //----------------------------------------------------------------------------------------
            if (showIssueReportWindow)
            {
                Rectangle messageBox = { (float)screenWidth/2 - 300/2, (float)screenHeight/2 - 190/2 - 20, 300, 190 };
                int result = GuiMessageBox(messageBox, "#220#Report Issue",
                    "Do you want to report any issue or\nfeature request for this program?\n\ngithub.com/raysan5/rfxgen", "#186#Report on GitHub");

                if (result == 1)    // Report issue pressed
                {
                    OpenURL("https://github.com/raysan5/rfxgen/issues");
                    showIssueReportWindow = false;
                }
                else if (result == 0) showIssueReportWindow = false;
            }
            //----------------------------------------------------------------------------------------

            // GUI: Export Window
            //----------------------------------------------------------------------------------------
            if (showExportWindow)
            {
                Rectangle messageBox = { (float)screenWidth/2 - 248/2, (float)screenHeight/2 - 150, 248, 208 };
                int result = GuiMessageBox(messageBox, "#7#Export Wave File", " ", "#7# Export Wave");

                GuiLabel((Rectangle){ messageBox.x + 12, messageBox.y + 24 + 12, 106, 24 }, "File Format:");
                GuiLabel((Rectangle){ messageBox.x + 12, messageBox.y + 24 + 12 + 24 + 8, 106, 24 }, "Sample Rate:");
                GuiLabel((Rectangle){ messageBox.x + 12, messageBox.y + 24 + 12 + 48 + 16, 106, 24 }, "Sample Size:");
                GuiLabel((Rectangle){ messageBox.x + 12, messageBox.y + 24 + 12 + 72 + 24, 106, 24 }, "Channels:");

                GuiComboBox((Rectangle){ messageBox.x + 12 + 100, messageBox.y + 24 + 12, 124, 24 }, "WAV;QOA;RAW;CODE", &fileTypeActive);
                GuiComboBox((Rectangle){ messageBox.x + 12 + 100, messageBox.y + 24 + 12 + 24 + 8, 124, 24 }, "22050 Hz;44100 Hz", &sampleRateActive);
                if (fileTypeActive == 1) { sampleSizeActive = 1; GuiDisable(); }
                GuiComboBox((Rectangle){ messageBox.x + 12 + 100, messageBox.y + 24 + 12 + 48 + 16, 124, 24 }, "8 bit;16 bit;32 bit", &sampleSizeActive);
                GuiEnable();
                GuiComboBox((Rectangle){ messageBox.x + 12 + 100, messageBox.y + 24 + 12 + 72 + 24, 124, 24 }, "Mono;Stereo", &channelsActive);

                if (result == 1)    // Export button pressed
                {
                    // Update export option from combobox selections
                    if (sampleRateActive == 0) exportSampleRate = 22050;
                    else if (sampleRateActive == 1) exportSampleRate = 44100;

                    if (sampleSizeActive == 0) exportSampleSize = 8;
                    else if (sampleSizeActive == 1) exportSampleSize = 16;
                    else if (sampleSizeActive == 2) exportSampleSize = 32;

                    exportChannels = channelsActive + 1;

                    showExportWindow = false;
                    showExportFileDialog = true;

                    memset(outFileName, 0, 512);
                    strcpy(outFileName, "sound");
                }
                else if (result == 0) showExportWindow = false;
            }
            //----------------------------------------------------------------------------------

            // GUI: Exit Window
            //----------------------------------------------------------------------------------------
            if (showExitWindow)
            {
                int result = GuiMessageBox((Rectangle){ (float)screenWidth/2 - 125, (float)screenHeight/2 - 50, 250, 100 }, "#159#Closing rFXGen", "Do you really want to exit?", "Yes;No");

                if ((result == 0) || (result == 2)) showExitWindow = false;
                else if (result == 1) closeWindow = true;
            }
            //----------------------------------------------------------------------------------------

            // GUI: Load File Dialog (and loading logic)
            //----------------------------------------------------------------------------------------
            if (showLoadFileDialog)
            {
#if defined(CUSTOM_MODAL_DIALOGS)
                int result = GuiFileDialog(DIALOG_MESSAGE, "Load sound file ...", inFileName, "Ok", "Just drag and drop your .rfx sound file!");
#else
                int result = GuiFileDialog(DIALOG_OPEN_FILE, "Load sound parameters file...", inFileName, "*.rfx", "Sound Param Files (*.rfx)");
#endif
                if (result == 1)
                {
                    // Load parameters file
                    params[mainToolbarState.soundSlotActive] = LoadWaveParams(inFileName);
                    SetWindowTitle(TextFormat("%s v%s - %s", toolName, toolVersion, GetFileName(inFileName)));
                    regenerate = true;
                }

                if (result >= 0) showLoadFileDialog = false;
            }
            //----------------------------------------------------------------------------------------

            // GUI: Save File Dialog (and saving logic)
            //----------------------------------------------------------------------------------------
            if (showSaveFileDialog)
            {
#if defined(CUSTOM_MODAL_DIALOGS)
                int result = GuiTextInputBox((Rectangle){ screenWidth/2 - 280/2, screenHeight/2 - 112/2 - 30, 280, 112 }, "#2#Save sound file as...", NULL, "#2#Save", outFileName, 512, NULL);
#else
                int result = GuiFileDialog(DIALOG_SAVE_FILE, "Save sound parameters file...", outFileName, "*.rfx", "Sound Param Files (*.rfx)");
#endif
                if (result == 1)
                {
                    // Save file: outFileName
                    // Check for valid extension and make sure it is
                    if ((GetFileExtension(outFileName) == NULL) || !IsFileExtension(outFileName, ".rfx")) strcat(outFileName, ".rfx\0");
                    SaveWaveParams(params[mainToolbarState.soundSlotActive], outFileName);    // Save wave parameters

                #if defined(PLATFORM_WEB)
                    // Download file from MEMFS (emscripten memory filesystem)
                    // NOTE: Second argument must be a simple filename (we can't use directories)
                    // NOTE: Included security check to (partially) avoid malicious code on PLATFORM_WEB
                    if (strchr(outFileName, '\'') == NULL) emscripten_run_script(TextFormat("saveFileFromMEMFSToDisk('%s','%s')", outFileName, GetFileName(outFileName)));
                #endif
                }

                if (result >= 0) showSaveFileDialog = false;
            }
            //----------------------------------------------------------------------------------------

            // GUI: Export File Dialog (and saving logic)
            //----------------------------------------------------------------------------------------
            if (showExportFileDialog)
            {
#if defined(CUSTOM_MODAL_DIALOGS)
                //int result = GuiFileDialog(DIALOG_TEXTINPUT, "Export wave file...", outFileName, "Ok;Cancel", NULL);
                int result = GuiTextInputBox((Rectangle){ screenWidth/2 - 280/2, screenHeight/2 - 112/2 - 30, 280, 112 }, "#7#Export wave file...", NULL, "#7#Export", outFileName, 512, NULL);
#else
                // Consider different supported file types
                char fileTypeFilters[64] = { 0 };

                if (fileTypeActive == 0) { strcpy(fileTypeFilters, "*.wav"); strcat(outFileName, ".wav"); }
                else if (fileTypeActive == 1) { strcpy(fileTypeFilters, "*.qoa"); strcat(outFileName, ".qoa"); }
                else if (fileTypeActive == 2) { strcpy(fileTypeFilters, "*.raw"); strcat(outFileName, ".raw"); }
                else if (fileTypeActive == 3) { strcpy(fileTypeFilters, "*.h"); strcat(outFileName, ".h"); }

                int result = GuiFileDialog(DIALOG_SAVE_FILE, "Export wave file...", outFileName, fileTypeFilters, TextFormat("File type (%s)", fileTypeFilters));
#endif
                if (result == 1)
                {
                    // Export file: outFileName
                    Wave cwave = WaveCopy(wave[mainToolbarState.soundSlotActive]);
                    WaveFormat(&cwave, exportSampleRate, exportSampleSize, exportChannels);   // Before exporting wave data, we format it as desired

                    if (fileTypeActive == 0)
                    {
                        // Check for valid extension and make sure it is
                        if ((GetFileExtension(outFileName) == NULL) || !IsFileExtension(outFileName, ".wav")) strcat(outFileName, ".wav\0");
                        ExportWave(cwave, outFileName);            // Export wave data as WAV file
                    }
                    else if (fileTypeActive == 1)   // Export Wave as QOA data
                    {
                        // Check for valid extension and make sure it is
                        if ((GetFileExtension(outFileName) == NULL) || !IsFileExtension(outFileName, ".qoa")) strcat(outFileName, ".qoa\0");
                        ExportWave(cwave, outFileName);            // Export wave data as QOA file
                    }
                    else if (fileTypeActive == 2)   // Export Wave as RAW data
                    {
                        // Check for valid extension and make sure it is
                        if ((GetFileExtension(outFileName) == NULL) || !IsFileExtension(outFileName, ".raw")) strcat(outFileName, ".raw\0");
                        FILE *rawFile = fopen(outFileName, "wb");

                        if (rawFile != NULL)
                        {
                            fwrite(cwave.data, 1, cwave.frameCount*cwave.channels*cwave.sampleSize/8, rawFile);  // Write wave data
                            fclose(rawFile);
                        }
                    }
                    else if (fileTypeActive == 3)   // Export as code file
                    {
                        // Check for valid extension and make sure it is
                        if ((GetFileExtension(outFileName) == NULL) || !IsFileExtension(outFileName, ".h")) strcat(outFileName, ".h\0");
                        ExportWaveAsCode(cwave, outFileName); // Export wave data as code file
                    }

                    UnloadWave(cwave);

                #if defined(PLATFORM_WEB)
                    // Download file from MEMFS (emscripten memory filesystem)
                    // NOTE: Second argument must be a simple filename (we can't use directories)
                    // NOTE: Included security check to (partially) avoid malicious code on PLATFORM_WEB
                    if (strchr(outFileName, '\'') == NULL) emscripten_run_script(TextFormat("saveFileFromMEMFSToDisk('%s','%s')", outFileName, GetFileName(outFileName)));
                #endif
                }

                if (result >= 0) showExportFileDialog = false;
            }
            //----------------------------------------------------------------------------------------

            // GUI: Show support message box (splash)
            //----------------------------------------------------------------------------------------
            if (showSupportMessage)
            {
                GuiPanel((Rectangle){ -10, screenHeight/2 - 180, screenWidth + 20, 290 }, NULL);

                GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize*3);
                GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, GuiGetStyle(DEFAULT, TEXT_COLOR_PRESSED));
                GuiLabel((Rectangle){ -10, screenHeight/2 - 140, screenWidth + 20, 30 }, TextFormat("Enjoying %s???", TOOL_NAME));
                GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
                GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize*2);
                GuiLabel((Rectangle){ -10, screenHeight/2 - 30, screenWidth + 20, 30 }, "Please, consider a small donation\n\nto support free tools development.");

                if (supportMessageRandBtn)
                {
                    if (GuiButton((Rectangle){ 10, screenHeight/2 + 40, screenWidth/2 - 15, 40 }, "Next time...")) showSupportMessage = false;
                    if (GuiButton((Rectangle){ 10 + screenWidth/2 - 5, screenHeight/2 + 40, screenWidth/2 - 15, 40 }, "Sure!")) { OpenURL("https://github.com/sponsors/raysan5"); showSupportMessage = false; }
                }
                else
                {
                    if (GuiButton((Rectangle){ 10, screenHeight/2 + 40, screenWidth/2 - 15, 40 }, "Sure!")) { OpenURL("https://github.com/sponsors/raysan5"); showSupportMessage = false; }
                    if (GuiButton((Rectangle){ 10 + screenWidth/2 - 5, screenHeight/2 + 40, screenWidth/2 - 15, 40 }, "Next time...")) showSupportMessage = false;
                }

                GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
                GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize);
            }
            //----------------------------------------------------------------------------------------

        EndTextureMode();

        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            // Draw render texture to screen
            if (screenSizeActive) DrawTexturePro(screenTarget.texture, (Rectangle){ 0, 0, (float)screenTarget.texture.width, -(float)screenTarget.texture.height }, (Rectangle){ 0, 0, (float)screenTarget.texture.width*2, (float)screenTarget.texture.height*2 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
            else DrawTextureRec(screenTarget.texture, (Rectangle){ 0, 0, (float)screenTarget.texture.width, -(float)screenTarget.texture.height }, (Vector2){ 0, 0 }, WHITE);

        EndDrawing();
        //------------------------------------------------------------------------------------
    }

    // De-Initialization
    //----------------------------------------------------------------------------------------
    for (int i = 0; i < MAX_WAVE_SLOTS; i++)
    {
        UnloadSound(sound[i]);  // Unload sounds
        UnloadWave(wave[i]);    // Unload wave slots (free done internally)
    }

    UnloadRenderTexture(screenTarget);
    UnloadRenderTexture(waveTarget);

    CloseAudioDevice();         // Close audio device
    CloseWindow();              // Close window and OpenGL context
    //----------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definition
//--------------------------------------------------------------------------------------------
#if defined(PLATFORM_DESKTOP)
// Show command line usage info
static void ShowCommandLineInfo(void)
{
    printf("\n//////////////////////////////////////////////////////////////////////////////////\n");
    printf("//                                                                              //\n");
    printf("// %s v%s - %s                   //\n", toolName, toolVersion, toolDescription);
    printf("// powered by raylib v%s and raygui v%s                               //\n", RAYLIB_VERSION, RAYGUI_VERSION);
    printf("// more info and bugs-report: github.com/raysan5/rfxgen                         //\n");
    printf("//                                                                              //\n");
    printf("// Copyright (c) 2014-2025 raylib technologies (@raylibtech)                    //\n");
    printf("//                                                                              //\n");
    printf("//////////////////////////////////////////////////////////////////////////////////\n\n");

    printf("USAGE:\n\n");
    printf("    > rfxgen [--help] --input <filename.ext> [--output <filename.ext>]\n");
    printf("             [--format <sample_rate>,<sample_size>,<channels>] [--play <filename.ext>]\n");

    printf("\nOPTIONS:\n\n");
    printf("    -h, --help                      : Show tool version and command line usage help\n\n");
    printf("    -i, --input <filename.ext>      : Define input file.\n");
    printf("                                      Supported extensions: .rfx, .wav, .qoa, .ogg, .flac, .mp3\n\n");
    printf("    -o, --output <filename.ext>     : Define output file.\n");
    printf("                                      Supported extensions: .wav, .qoa, .raw, .h\n");
    printf("                                      NOTE: If not specified, defaults to: output.wav\n\n");
    printf("    -g, --generate <preset>            : Generate file based on the preset.\n");
    printf("                                      Supported presets: coin, laser, explosion, powerup, hit, jump, blip\n");
    printf("    -f, --format <sample_rate>,<sample_size>,<channels>\n");
    printf("                                    : Define output wave format. Comma separated values.\n");
    printf("                                      Supported values:\n");
    printf("                                          Sample rate:      22050, 44100\n");
    printf("                                          Sample size:      8, 16, 32\n");
    printf("                                          Channels:         1 (mono), 2 (stereo)\n");
    printf("                                      NOTE: If not specified, defaults to: 44100, 16, 1\n\n");
    printf("    -p, --play <filename.ext>       : Play provided sound.\n");
    printf("                                      Supported extensions: .wav, .qoa, .ogg, .flac, .mp3\n");

    printf("\nEXAMPLES:\n\n");
    printf("    > rfxgen --input sound.rfx --output jump.wav\n");
    printf("        Process <sound.rfx> to generate <sound.wav> at 44100 Hz, 32 bit, Mono\n\n");
    printf("    > rfxgen --input sound.rfx --output jump.raw --format 22050,16,2\n");
    printf("        Process <sound.rfx> to generate <jump.raw> at 22050 Hz, 16 bit, Stereo\n\n");
    printf("    > rfxgen --input sound.ogg --play output.wav\n");
    printf("        Process <sound.ogg> to generate <output.wav> and play <output.wav>\n\n");
    printf("    > rfxgen --input sound.mp3 --output jump.wav --format 22050,8,1 --play jump.wav\n");
    printf("        Process <sound.mp3> to generate <jump.wav> at 22050 Hz, 8 bit, Stereo.\n");
    printf("        Plays generated sound <jump.wav>.\n");
}

// Process command line input
static void ProcessCommandLine(int argc, char *argv[])
{
    // CLI required variables
    bool showUsageInfo = false;         // Toggle command line usage info
    char playFileName[512] = { 0 };     // Play file name

    int sampleRate = 44100;             // Default conversion sample rate
    int sampleSize = 16;                // Default conversion sample size
    int channels = 1;                   // Default conversion channels number

    if (argc == 1) showUsageInfo = true;

    // Process command line arguments
    for (int i = 1; i < argc; i++)
    {
        if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
        {
            showUsageInfo = true;
        }
        else if ((strcmp(argv[i], "-i") == 0) || (strcmp(argv[i], "--input") == 0))
        {
            // Check for valid argument and valid file extension
            if (((i + 1) < argc) && (argv[i + 1][0] != '-'))
            {
                if (IsFileExtension(argv[i + 1], ".rfx") ||
                    IsFileExtension(argv[i + 1], ".wav") ||
                    IsFileExtension(argv[i + 1], ".qoa") ||
                    IsFileExtension(argv[i + 1], ".ogg") ||
                    IsFileExtension(argv[i + 1], ".flac") ||
                    IsFileExtension(argv[i + 1], ".mp3"))
                {
                    strcpy(inFileName, argv[i + 1]);    // Read input filename
                }
                else LOG("WARNING: Input file extension not recognized\n");

                i++;
            }
            else LOG("WARNING: No input file provided\n");
        }
        else if ((strcmp(argv[i], "-o") == 0) || (strcmp(argv[i], "--output") == 0))
        {
            if (((i + 1) < argc) && (argv[i + 1][0] != '-'))
            {
                if (IsFileExtension(argv[i + 1], ".wav") ||
                    IsFileExtension(argv[i + 1], ".qoa") ||
                    IsFileExtension(argv[i + 1], ".raw") ||
                    IsFileExtension(argv[i + 1], ".h"))
                {
                    strcpy(outFileName, argv[i + 1]);   // Read output filename
                }
                else LOG("WARNING: Output file extension not recognized\n");

                i++;
            }
            else LOG("WARNING: No output file provided\n");
        }
        else if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--format") == 0))
        {
            if (((i + 1) < argc) && (argv[i + 1][0] != '-'))
            {
                int numValues = 0;
                char **values = TextSplit(argv[i + 1], ',', &numValues);

                if (numValues != 3) LOG("WARNING: Incorrect number of format values\n");
                else
                {
                    // Read values text and convert to integer values
                    sampleRate = TextToInteger(values[0]);
                    sampleSize = TextToInteger(values[1]);
                    channels = TextToInteger(values[2]);

                    // Verify retrieved values are valid
                    if ((sampleRate != 44100) && (sampleRate != 22050))
                    {
                        LOG("WARNING: Sample rate not supported. Default: 44100 Hz\n");
                        sampleRate = 44100;
                    }

                    if ((sampleSize != 8) && (sampleSize != 16) && (sampleSize != 32))
                    {
                        LOG("WARNING: Sample size not supported. Default: 16 bit\n");
                        sampleSize = 16;
                    }

                    if ((channels != 1) && (channels != 2))
                    {
                        LOG("WARNING: Channels number not supported. Default: 1 (mono)\n");
                        channels = 1;
                    }
                }
            }
            else LOG("WARNING: Format parameters provided not valid\n");
        }
        else if ((strcmp(argv[i], "-p") == 0) || (strcmp(argv[i], "--play") == 0))
        {
            if (((i + 1) < argc) && (argv[i + 1][0] != '-'))
            {
                if (IsFileExtension(argv[i + 1], ".wav") ||
                    IsFileExtension(argv[i + 1], ".qoa") ||
                    IsFileExtension(argv[i + 1], ".ogg") ||
                    IsFileExtension(argv[i + 1], ".flac") ||
                    IsFileExtension(argv[i + 1], ".mp3"))
                {
                    strcpy(playFileName, argv[i + 1]);   // Read filename to play
                    i++;
                }
                else LOG("WARNING: Play file format not supported\n");

                i++;
            }
            else LOG("WARNING: No file to play provided\n");
        }
        else if ((strcmp(argv[i], "-g") == 0) || (strcmp(argv[i], "--generate") == 0))
        {
            strcpy(presetType, argv[i + 1]); // saves the preset to generate
        }
    }

    // Wave that will be generated in case an input file or preset type is provided
    Wave wave = { 0 };

    // Process input file if provided
    if (inFileName[0] != '\0')
    {
        if (outFileName[0] == '\0') strcpy(outFileName, "output.wav");  // Set a default name for output in case not provided

        LOG("\nInput file:       %s", inFileName);
        LOG("\nOutput file:      %s", outFileName);
        LOG("\nOutput format:    %i Hz, %i bits, %s\n\n", sampleRate, sampleSize, (channels == 1)? "Mono" : "Stereo");

        if (IsFileExtension(inFileName, ".rfx")) // || IsFileExtension(inFileName, ".sfs"))
        {
            WaveParams params = LoadWaveParams(inFileName);

            // NOTE: GenerateWave() returns data as 32bit float, 1 channel by default
            wave.sampleRate = RFXGEN_GEN_SAMPLE_RATE;
            wave.sampleSize = RFXGEN_GEN_SAMPLE_SIZE;
            wave.channels = RFXGEN_GEN_CHANNELS;
            wave.data = GenerateWave(params, &wave.frameCount);
        }
        else if (IsFileExtension(inFileName, ".wav") ||
                 IsFileExtension(inFileName, ".qoa") ||
                 IsFileExtension(inFileName, ".ogg") ||
                 IsFileExtension(inFileName, ".flac") ||
                 IsFileExtension(inFileName, ".mp3"))
        {
            wave = LoadWave(inFileName);
        }

    }

    // Process generate sound if type provided
    if (presetType[0] != '\0')
    {
        if (outFileName[0] == '\0') strcpy(outFileName, "output.wav");

        wave.sampleRate = RFXGEN_GEN_SAMPLE_RATE;
        wave.sampleSize = RFXGEN_GEN_SAMPLE_SIZE;
        wave.channels = RFXGEN_GEN_CHANNELS;

        if (strcmp(presetType, "coin") == 0)            wave.data = GenerateWave(GenPickupCoin(), &wave.frameCount);
        else if (strcmp(presetType, "laser") == 0)      wave.data = GenerateWave(GenLaserShoot(), &wave.frameCount);
        else if (strcmp(presetType, "explosion") == 0)  wave.data = GenerateWave(GenExplosion(), &wave.frameCount);
        else if (strcmp(presetType, "powerup") == 0)    wave.data = GenerateWave(GenPowerup(), &wave.frameCount);
        else if (strcmp(presetType, "hit") == 0)        wave.data = GenerateWave(GenHitHurt(), &wave.frameCount);
        else if (strcmp(presetType, "jump") == 0)       wave.data = GenerateWave(GenJump(), &wave.frameCount);
        else if (strcmp(presetType, "blip") == 0)       wave.data = GenerateWave(GenBlipSelect(), &wave.frameCount);
        else LOG("Unrecognized type of preset.\n");
    }

    // Save the generated wave data if valid
    if (wave.data != NULL) {
        // Format wave data to desired sampleRate, sampleSize and channels
        WaveFormat(&wave, sampleRate, sampleSize, channels);

        // Export wave data as audio file (.wav) or code file (.h)
        if (IsFileExtension(outFileName, ".wav") || IsFileExtension(outFileName, ".qoa")) ExportWave(wave, outFileName);
        else if (IsFileExtension(outFileName, ".h")) ExportWaveAsCode(wave, outFileName);
        else if (IsFileExtension(outFileName, ".raw"))
        {
            // Export Wave as RAW data
            FILE *rawFile = fopen(outFileName, "wb");

            if (rawFile != NULL)
            {
                fwrite(wave.data, 1, wave.frameCount*wave.channels*wave.sampleSize/8, rawFile);  // Write wave data
                fclose(rawFile);
            }
        }

        UnloadWave(wave);
    }

    // Play audio file if provided
    if (playFileName[0] != '\0')
    {
        Wave wave = LoadWave(playFileName);     // Load audio (WAV, QOA, OGG, FLAC, MP3)
        PlayWaveCLI(wave);
        UnloadWave(wave);
    }

    if (showUsageInfo) ShowCommandLineInfo();
}
#endif      // PLATFORM_DESKTOP

//--------------------------------------------------------------------------------------------
// Auxiliar functions
//--------------------------------------------------------------------------------------------
// Draw wave data
// NOTE: For proper visualization, MSAA x4 is recommended but it could be costly for the GPU
// Alternative: Rendered to a bigger texture and scale down with bilinear/trilinear texture filtering
static void DrawWave(Wave *wave, Rectangle bounds, Color color)
{
    float sample = 0.0f;
    float sampleNext = 0.0f;
    float currentSample = 0.0f;
    float sampleIncrement = (float)wave->frameCount*wave->channels/(float)(bounds.width*2);
    float sampleScale = (float)bounds.height;

    for (int i = 1; i < bounds.width*2 - 1; i++)
    {
        sample = ((float *)wave->data)[(int)currentSample]*sampleScale;
        sampleNext = ((float *)wave->data)[(int)(currentSample + sampleIncrement)]*sampleScale;

        if (sample > bounds.height/2) sample = bounds.height/2;
        else if (sample < -bounds.height/2) sample = -bounds.height/2;

        if (sampleNext > bounds.height/2) sampleNext = bounds.height/2;
        else if (sampleNext < -bounds.height/2) sampleNext = -bounds.height/2;

        DrawLineV((Vector2){ (float)bounds.x + (float)i/2.0f, (float)(bounds.y + bounds.height/2) + sample },
                  (Vector2){ (float)bounds.x + (float)i/2.0f, (float)(bounds.y  + bounds.height/2) + sampleNext }, color);

        currentSample += sampleIncrement;
    }
}

#if defined(PLATFORM_DESKTOP)
// Simple time wait in milliseconds
static void WaitTimePlayer(int ms)
{
    if (ms > 0)
    {
        int currentTime = clock()*1000/CLOCKS_PER_SEC;  // Current time in milliseconds
        int totalTime = currentTime + ms;               // Total required time in ms to return from this timeout

        int percent = 0;
        int prevPercent = percent;

        // Wait until current ms time matches total ms time
        while (currentTime <= totalTime)
        {
            // Check for key pressed to stop playing
            if (kbhit())
            {
                int key = getch();
                if ((key == 13) || (key == 27)) break;    // KEY_ENTER || KEY_ESCAPE
            }

            currentTime = clock()*1000/CLOCKS_PER_SEC;

            // Print console time bar
            percent = (int)(((float)currentTime/totalTime)*100.0f);

            if (percent != prevPercent)
            {
                LOG("\r[");
                for (int j = 0; j < 50; j++)
                {
                    if (j < percent/2) LOG("=");
                    else LOG(" ");
                }
                LOG("] [%02i%%]", percent);

                prevPercent = percent;
            }
        }

        LOG("\n\n");
    }
}

// Play provided wave through CLI
static void PlayWaveCLI(Wave wave)
{
    float waveTimeMs = (float)wave.frameCount*1000.0f/wave.sampleRate;

    InitAudioDevice();                  // Init audio device
    Sound fx = LoadSoundFromWave(wave); // Load audio wave

    printf("\n//////////////////////////////////////////////////////////////////////////////////\n");
    printf("//                                                                              //\n");
    printf("// %s v%s - CLI audio player                                         //\n", toolName, toolVersion);
    printf("//                                                                              //\n");
    printf("// more info and bugs-report: github.com/raysan5/rfxgen                         //\n");
    printf("//                                                                              //\n");
    printf("// Copyright (c) 2020-2025 raylib technologies (@raylibtech)                    //\n");
    printf("//                                                                              //\n");
    printf("//////////////////////////////////////////////////////////////////////////////////\n\n");

    printf("Playing sound [%.2f sec.]. Press ENTER to finish.\n", waveTimeMs/1000.0f);

    PlaySound(fx);                      // Play sound
    WaitTimePlayer(waveTimeMs);         // Wait while audio is playing
    UnloadSound(fx);                    // Unload sound data
    CloseAudioDevice();                 // Close audio device
}

#if !defined(_WIN32)
// Check if a key has been pressed
static int kbhit(void)
{
    struct termios oldt = { 0 };
    struct termios newt = { 0 };
    int ch = 0;
    int oldf = 0;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif
#endif      // PLATFORM_DESKTOP
