/*******************************************************************************************
*
*   rFXGen v2.1 - A simple and easy to use sounds generator (based on Tomas Petterson sfxr)
*
*   CONFIGURATION:
*
*   #define VERSION_ONE
*       Enable PRO features for the tool. Usually command-line and export options related.
*
*   #define COMMAND_LINE_ONLY
*       Compile tool only for command line usage
*
*   #define CUSTOM_MODAL_DIALOGS
*       Use custom raygui generated modal dialogs instead of native OS ones
*       NOTE: Avoids including tinyfiledialogs depencency library
*
*   #define RENDER_WAVE_TO_TEXTURE (defined by default)
*       Use RenderTexture2D to render wave on. If not defined, wave is diretly drawn using lines.
*
*   VERSIONS HISTORY:
*       2.1  (09-Sep-2019) Ported to latest raygui 2.6
*                          Support custom file dialogs (on non-DESKTOP platforms)
*                          Slight screen resize to adapt to new styles fonts
*       2.0  (xx-Nov-2018) GUI redesigned, CLI improvements
*       1.8  (10-Oct-2018) Functions renaming, code reorganized, better consistency...
*       1.5  (23-Sep-2018) Support .wav export to code and sound playing on command line
*       1.4  (15-Sep-2018) Redesigned command line and comments
*       1.3  (15-May-2018) Reimplemented gui using rGuiLayout
*       1.2  (16-Mar-2018) Working on some code improvements and GUI review
*       1.1  (01-Oct-2017) Code review, simplified
*       1.0  (18-Mar-2017) First release
*       0.9x (XX-Jan-2017) Review complete file...
*       0.95 (14-Sep-2016) Reviewed comments and .rfx format
*       0.9  (12-Sep-2016) Defined WaveParams struct and command line functionality
*       0.8  (09-Sep-2016) Added open/save file dialogs using tinyfiledialogs library
*       0.7  (04-Sep-2016) Program variables renaming for consistency, code reorganized
*       0.6  (30-Aug-2016) Interface redesigned (reduced size) and new features added (wave drawing)
*       0.5  (27-Aug-2016) Completed port and adaptation from sfxr (only sound generation and playing)
*
*   DEPENDENCIES:
*       raylib 2.6-dev          - Windowing/input management and drawing.
*       raygui 2.6              - Immediate-mode GUI controls.
*       tinyfiledialogs 3.3.9   - Open/save file dialogs, it requires linkage with comdlg32 and ole32 libs.
*
*   COMPILATION (Windows - MinGW):
*       gcc -o rfxgen.exe rfxgen.c external/tinyfiledialogs.c -s rfxgen_icon -Iexternal /
*           -lraylib -lopengl32 -lgdi32 -lcomdlg32 -lole32 -std=c99 -Wl,--subsystem,windows
*
*   COMPILATION (Linux - GCC):
*       gcc -o rfxgen rfxgen.c external/tinyfiledialogs.c -s -Iexternal -no-pie -D_DEFAULT_SOURCE /
*           -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
*
*   NOTE: On PLATFORM_ANDROID and PLATFORM_WEB file dialogs are not available
*
*   DEVELOPERS:
*       Ramon Santamaria (@raysan5):   Developer, supervisor, updater and maintainer.
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2020 raylib technologies (@raylibtech).
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

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS        // Force custom modal dialogs usage

    #include <emscripten/emscripten.h>  // Emscripten library - LLVM to JavaScript compiler
    #include <emscripten/html5.h>       // Emscripten HTML5 library
#endif

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include "raygui.h"                     // Required for: IMGUI controls

#undef RAYGUI_IMPLEMENTATION            // Avoid including raygui implementation again

#define GUI_WINDOW_ABOUT_IMPLEMENTATION
#include "gui_window_about.h"           // GUI: About Window

#define GUI_FILE_DIALOGS_IMPLEMENTATION
#include "gui_file_dialogs.h"           // GUI: File Dialogs

#if !defined(COMMAND_LINE_ONLY)
    #include "style_jungle.h"           // raygui style: jungle
    #include "style_candy.h"            // raygui style: candy
    #include "style_lavanda.h"          // raygui style: lavanda
#endif

#include <math.h>                       // Required for: sinf(), pow()
#include <time.h>                       // Required for: clock()
#include <stdlib.h>                     // Required for: calloc(), free()
#include <string.h>                     // Required for: strcmp()
#include <stdio.h>                      // Required for: FILE, fopen(), fread(), fwrite(), ftell(), fseek() fclose()
                                        // NOTE: Used on functions: LoadSound(), SaveSound(), WriteWAV()
#if defined(_WIN32)
    #include <conio.h>                  // Required for: kbhit() [Windows only, no stardard library]
#else
    // Provide kbhit() function in non-Windows platforms
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Basic information
static const char *toolName = "rFXGen";
static const char *toolVersion = "2.1";
static const char *toolDescription = "A simple and easy-to-use sounds generator";

#define MAX_WAVE_SLOTS       4          // Number of wave slots for generation

// Float random number generation
#define frnd(range) ((float)GetRandomValue(0, 10000)/10000.0f*range)

#if (!defined(DEBUG) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)))
bool __stdcall FreeConsole(void);       // Close console from code (kernel32.lib)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// Wave parameters type (96 bytes)
typedef struct WaveParams {

    // Random seed used to generate the wave
    int randSeed;

    // Wave type (square, sawtooth, sine, noise)
    int waveTypeValue;

    // Wave envelope parameters
    float attackTimeValue;
    float sustainTimeValue;
    float sustainPunchValue;
    float decayTimeValue;

    // Frequency parameters
    float startFrequencyValue;
    float minFrequencyValue;
    float slideValue;
    float deltaSlideValue;
    float vibratoDepthValue;
    float vibratoSpeedValue;
    //float vibratoPhaseDelayValue;

    // Tone change parameters
    float changeAmountValue;
    float changeSpeedValue;

    // Square wave parameters
    float squareDutyValue;
    float dutySweepValue;

    // Repeat parameters
    float repeatSpeedValue;

    // Phaser parameters
    float phaserOffsetValue;
    float phaserSweepValue;

    // Filter parameters
    float lpfCutoffValue;
    float lpfCutoffSweepValue;
    float lpfResonanceValue;
    float hpfCutoffValue;
    float hpfCutoffSweepValue;

} WaveParams;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static float volumeValue = 0.6f;        // Master volume

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
#if defined(VERSION_ONE) || defined(COMMAND_LINE_ONLY)
static void ShowCommandLineInfo(void);                      // Show command line usage info
static void ProcessCommandLine(int argc, char *argv[]);     // Process command line input
#endif

// Load/Save/Export data functions
static WaveParams LoadWaveParams(const char *fileName);                 // Load wave parameters from file
static void SaveWaveParams(WaveParams params, const char *fileName);    // Save wave parameters to file
static void ResetWaveParams(WaveParams *params);                        // Reset wave parameters
static Wave GenerateWave(WaveParams params);                            // Generate wave data from parameters

// Sound generation functions
static WaveParams GenPickupCoin(void);      // Generate sound: Pickup/Coin
static WaveParams GenLaserShoot(void);      // Generate sound: Laser shoot
static WaveParams GenExplosion(void);       // Generate sound: Explosion
static WaveParams GenPowerup(void);         // Generate sound: Powerup
static WaveParams GenHitHurt(void);         // Generate sound: Hit/Hurt
static WaveParams GenJump(void);            // Generate sound: Jump
static WaveParams GenBlipSelect(void);      // Generate sound: Blip/Select
static WaveParams GenRandomize(void);       // Generate random sound
static void WaveMutate(WaveParams *params); // Mutate current sound

#if !defined(COMMAND_LINE_ONLY)
// Auxiliar functions
static void DrawWave(Wave *wave, Rectangle bounds, Color color);    // Draw wave data using lines
#endif

#if defined(VERSION_ONE) || defined(COMMAND_LINE_ONLY)
static void WaitTime(int ms);               // Simple time wait in milliseconds
static void PlayWaveCLI(Wave wave);         // Play provided wave through CLI

#if !defined(_WIN32)
static int kbhit(void);                         // Check if a key has been pressed
static char getch(void) { return getchar(); }   // Get pressed character
#endif
#endif  // VERSION_ONE || COMMAND_LINE_ONLY

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
#if !defined(DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif
#if defined(COMMAND_LINE_ONLY)
    ProcessCommandLine(argc, argv);
#else
    char inFileName[512] = { 0 };       // Input file name (required in case of drag & drop over executable)
    char outFileName[512] = { 0 };      // Output file name (required for file save/export)

    // Command-line usage mode
    //--------------------------------------------------------------------------------------
    if (argc > 1)
    {
        if ((argc == 2) &&
            (strcmp(argv[1], "-h") != 0) &&
            (strcmp(argv[1], "--help") != 0))       // One argument (file dropped over executable?)
        {
            if (IsFileExtension(argv[1], ".rfx") ||
                IsFileExtension(argv[1], ".sfs"))
            {
                strcpy(inFileName, argv[1]);        // Read input filename to open with gui interface
            }
        }
#if defined(VERSION_ONE)
        else
        {
            ProcessCommandLine(argc, argv);
            return 0;
        }
#endif      // VERSION_ONE
    }

#if (!defined(DEBUG) && defined(VERSION_ONE) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)))
    // WARNING (Windows): If program is compiled as Window application (instead of console),
    // no console is available to show output info... solution is compiling a console application
    // and closing console (FreeConsole()) when changing to GUI interface
    FreeConsole();
#endif

    // GUI usage mode - Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 512;
    const int screenHeight = 512;

    //SetConfigFlags(FLAG_MSAA_4X_HINT);        // Window configuration flags
    InitWindow(screenWidth, screenHeight, FormatText("%s v%s - %s", toolName, toolVersion, toolDescription));
    SetExitKey(0);

    InitAudioDevice();

    // GUI: Main Layout
    //-----------------------------------------------------------------------------------
    bool playOnChangeChecked = true;
    int sampleRateActive = 1;
    int sampleSizeActive = 1;
    int fileTypeActive = 0;
    int visualStyleActive = 0;

    bool screenSizeActive = false;
    //-----------------------------------------------------------------------------------

    // GUI: About Window
    //-----------------------------------------------------------------------------------
    GuiWindowAboutState windowAboutState = InitGuiWindowAbout();
    //-----------------------------------------------------------------------------------

    // GUI: Exit Window
    //-----------------------------------------------------------------------------------
    bool exitWindow = false;
    bool windowExitActive = false;
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
        wave[i].sampleRate = 44100;
        wave[i].sampleSize = 32;    // 32 bit -> float
        wave[i].channels = 1;
        wave[i].sampleCount = 10*wave[i].sampleRate*wave[i].channels;    // Max sampleCount for 10 seconds
        wave[i].data = calloc(wave[i].sampleCount*wave[i].sampleSize/8, sizeof(char));

        sound[i] = LoadSoundFromWave(wave[i]);
    }

    int wavSampleSize = 16;         // Wave sample size in bits (bitrate)
    int wavSampleRate = 44100;      // Wave sample rate (frequency)
    //-----------------------------------------------------------------------------------

    // Check if a wave parameters file has been provided on command line
    if (inFileName[0] != '\0')
    {
        // Clean everything (just in case)
        UnloadWave(wave[0]);
        UnloadSound(sound[0]);

        params[0] = LoadWaveParams(inFileName); // Load wave parameters from .rfx
        wave[0] = GenerateWave(params[0]);      // Generate wave from parameters
        sound[0] = LoadSoundFromWave(wave[0]);  // Load sound from new wave

        PlaySound(sound[0]);                    // Play generated sound
    }

    float prevVolumeValue = volumeValue;
    int prevWaveTypeValue[MAX_WAVE_SLOTS] = { params[0].waveTypeValue };
    int prevVisualStyleActive = visualStyleActive;
    bool regenerate = false;                    // Wave regeneration required
    int prevSlotActive = 0, slotActive = 0;     // Wave slot tracking

    Rectangle waveRec = { 8, 428, 496, 58 };        // Wave drawing rectangle box
    Rectangle slidersRec = { 238, 16, 104, 400 };   // Area defining sliders to allow sound replay when mouse-released

    // Set default sound volume
    for (int i = 0; i < MAX_WAVE_SLOTS; i++) SetSoundVolume(sound[i], volumeValue);

#define RENDER_WAVE_TO_TEXTURE
#if defined(RENDER_WAVE_TO_TEXTURE)
    // To avoid enabling MSXAAx4, we will render wave to a texture x2
    RenderTexture2D waveTarget = LoadRenderTexture(waveRec.width*2, waveRec.height*2);
    SetTextureFilter(waveTarget.texture, FILTER_BILINEAR);
#endif

    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    RenderTexture2D screenTarget = LoadRenderTexture(512, 512);
    SetTextureFilter(screenTarget.texture, FILTER_POINT);

    SetTargetFPS(60);
    //----------------------------------------------------------------------------------------

    // Main game loop
    while (!exitWindow)    // Detect window close button or ESC key
    {
        if (WindowShouldClose()) exitWindow = true;

        if (!windowAboutState.chkLicenseChecked) exitWindow = true;

        // Dropped files logic
        //----------------------------------------------------------------------------------
        if (IsFileDropped())
        {
            int dropsCount = 0;
            char **droppedFiles = GetDroppedFiles(&dropsCount);

            // Support loading .rfx or .sfs files (wave parameters)
            if (IsFileExtension(droppedFiles[0], ".rfx") ||
                IsFileExtension(droppedFiles[0], ".sfs"))
            {
                params[slotActive] = LoadWaveParams(droppedFiles[0]);
                regenerate = true;

                SetWindowTitle(FormatText("%s v%s - %s", toolName, toolVersion, GetFileName(droppedFiles[0])));
            }
#if defined(VERSION_ONE)
            else if (IsFileExtension(droppedFiles[0], ".rgs")) GuiLoadStyle(droppedFiles[0]);
#endif

            ClearDroppedFiles();
        }
        //----------------------------------------------------------------------------------

        // Keyboard shortcuts
        //------------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_SPACE)) PlaySound(sound[slotActive]);  // Play current sound

        // Show dialog: save sound (.rfx)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) showSaveFileDialog = true;

        // Show dialog: load sound (.rfx, .sfs)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_O)) showLoadFileDialog = true;

        // Show dialog: export wave (.wav, .raw, .h)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_E)) showExportFileDialog = true;

        // Toggle window about
        if (IsKeyPressed(KEY_F1)) windowAboutState.windowActive = !windowAboutState.windowActive;

        // Show closing window on ESC
        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (windowAboutState.windowActive) windowAboutState.windowActive = false;
            else windowExitActive = !windowExitActive;
        }
        //----------------------------------------------------------------------------------

        // Basic program flow logic
        //----------------------------------------------------------------------------------

        // Check for changed gui values
        if (volumeValue != prevVolumeValue)
        {
            SetMasterVolume(volumeValue);
            prevVolumeValue = volumeValue;
        }

        // Check gui combo box selected options
        if (sampleRateActive == 0) wavSampleRate = 22050;
        else if (sampleRateActive == 1) wavSampleRate = 44100;

        if (sampleSizeActive == 0) wavSampleSize = 8;
        else if (sampleSizeActive == 1) wavSampleSize = 16;
        else if (sampleSizeActive == 2) wavSampleSize = 32;

        if (params[slotActive].waveTypeValue != prevWaveTypeValue[slotActive]) regenerate = true;
        prevWaveTypeValue[slotActive] = params[slotActive].waveTypeValue;

        // Set new gui style if changed
        if (visualStyleActive != prevVisualStyleActive)
        {
            GuiLoadStyleDefault();

            switch (visualStyleActive)
            {
                case 1: GuiLoadStyleJungle(); break;
                case 2: GuiLoadStyleCandy(); break;
                case 3: GuiLoadStyleLavanda(); break;
                default: break;
            }

            prevVisualStyleActive = visualStyleActive;
        }

        if (!windowAboutState.windowActive && !windowExitActive)    // Avoid wave regeneration on Window About active
        {
            // Consider two possible cases to regenerate wave and update sound:
            // CASE1: regenerate flag is true (set by sound buttons functions)
            // CASE2: Mouse is moving sliders and mouse is released (checks against all sliders box - a bit crappy solution...)
            if (regenerate || ((CheckCollisionPointRec(GetMousePosition(), slidersRec)) && (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))))
            {
                UnloadWave(wave[slotActive]);
                wave[slotActive] = GenerateWave(params[slotActive]);        // Generate new wave from parameters

                UnloadSound(sound[slotActive]);
                sound[slotActive] = LoadSoundFromWave(wave[slotActive]);    // Reload sound from new wave
                //UpdateSound(sound[slotActive], wave[slotActive].data, wave[slotActive].sampleCount);    // Update sound buffer with new data --> CRASHES RANDOMLY!

                if (regenerate || playOnChangeChecked) PlaySound(sound[slotActive]);

                regenerate = false;
            }
        }

        if (slotActive != prevSlotActive)
        {
            PlaySound(sound[slotActive]);
            prevSlotActive = slotActive;
        }

        // Change window size to x2
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
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

#if defined(RENDER_WAVE_TO_TEXTURE)
            BeginTextureMode(waveTarget);
                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
                DrawWave(&wave[slotActive], (Rectangle){ 0, 0, waveTarget.texture.width, waveTarget.texture.height }, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_PRESSED)));
            EndTextureMode();
#endif
            // Render all screen to a texture (for scaling)
            BeginTextureMode(screenTarget);
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            if (windowAboutState.windowActive || windowExitActive) GuiDisable();
            else GuiEnable();

            // rFXGen Layout: controls drawing
            //----------------------------------------------------------------------------------
            if (showSaveFileDialog || showExportFileDialog) GuiLock();

            // Draw tool name and version, right aligned
            int prevTextAlignment = GuiGetStyle(LABEL, TEXT_ALIGNMENT);
            int prevTextPadding = GuiGetStyle(LABEL, TEXT_PADDING);
            GuiSetStyle(LABEL, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_RIGHT);
            GuiSetStyle(LABEL, TEXT_PADDING, 0);
            DrawText(FormatText("%s", toolName), 37, 18, 20, GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_PRESSED)));
            GuiLabel((Rectangle){ 84, 13, 30, 10 }, FormatText("v%s", toolVersion));
            GuiSetStyle(LABEL, TEXT_ALIGNMENT, prevTextAlignment);
            GuiSetStyle(LABEL, TEXT_PADDING, prevTextPadding);

            // Draw left buttons
            prevTextPadding = GuiGetStyle(BUTTON, TEXT_PADDING);
            GuiSetStyle(BUTTON, TEXT_PADDING, 3);
            GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
            if (GuiButton((Rectangle){ 8, 42, 106, 24 }, "#146#Pickup/Coin")) { params[slotActive] = GenPickupCoin(); regenerate = true; }
            if (GuiButton((Rectangle){ 8, 70, 106, 24 }, "#145#Laser/Shoot")) { params[slotActive] = GenLaserShoot(); regenerate = true; }
            if (GuiButton((Rectangle){ 8, 98, 106, 24 }, "#147#Explosion")) { params[slotActive] = GenExplosion(); regenerate = true; }
            if (GuiButton((Rectangle){ 8, 126, 106, 24 }, "#148#PowerUp")) { params[slotActive] = GenPowerup(); regenerate = true; }
            if (GuiButton((Rectangle){ 8, 154, 106, 24 }, "#152#Hit/Hurt")) { params[slotActive] = GenHitHurt(); regenerate = true; }
            if (GuiButton((Rectangle){ 8, 182, 106, 24 }, "#150#Jump")) { params[slotActive] = GenJump(); regenerate = true; }
            if (GuiButton((Rectangle){ 8, 210, 106, 24 }, "#144#Blip/Select")) { params[slotActive] = GenBlipSelect(); regenerate = true; }
            GuiSetStyle(BUTTON, TEXT_PADDING, prevTextPadding);
            GuiSetStyle(BUTTON, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);

            GuiLine((Rectangle){ 8, 234, 106, 12 }, NULL);

            GuiSetStyle(TOGGLE, TEXT_PADDING, 3);
            GuiSetStyle(TOGGLE, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
            params[slotActive].waveTypeValue = GuiToggleGroup((Rectangle){ 8, 250, 106, 24 }, "#126#Square\n#127#Sawtooth\n#125#Sinewave\n#124#Noise", params[slotActive].waveTypeValue);
            GuiSetStyle(TOGGLE, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
            GuiSetStyle(TOGGLE, TEXT_PADDING, 0);

            GuiLine((Rectangle){ 8, 356, 108, 12 }, NULL);

            if (GuiButton((Rectangle){ 8, 368, 106, 24 }, "#75#Mutate")) { WaveMutate(&params[slotActive]); regenerate = true; }
            if (GuiButton((Rectangle){ 8, 396, 106, 24 }, "#77#Randomize")) { params[slotActive] = GenRandomize(); regenerate = true; }

            GuiGroupBox((Rectangle){ 122, 12, 268, 22 }, NULL);
            GuiGroupBox((Rectangle){ 122, 33, 268, 69 }, NULL);
            GuiGroupBox((Rectangle){ 122, 101, 268, 104 }, NULL);
            GuiGroupBox((Rectangle){ 122, 204, 268, 37 }, NULL);
            GuiGroupBox((Rectangle){ 122, 240, 268, 36 }, NULL);
            GuiGroupBox((Rectangle){ 122, 275, 268, 23 }, NULL);
            GuiGroupBox((Rectangle){ 122, 297, 268, 39 }, NULL);
            GuiGroupBox((Rectangle){ 122, 335, 268, 86 }, NULL);

            // Parameters sliders
            //--------------------------------------------------------------------------------
            volumeValue = GuiSliderBar((Rectangle){ 246, 18, 100, 10 }, "VOLUME", TextFormat("%i", (int)(volumeValue*100)), volumeValue, 0, 1);

            params[slotActive].attackTimeValue = GuiSliderBar((Rectangle){ 246, 38, 100, 10 }, "ATTACK TIME", TextFormat("%.2f", params[slotActive].attackTimeValue), params[slotActive].attackTimeValue, 0, 1);
            params[slotActive].sustainTimeValue = GuiSliderBar((Rectangle){ 246, 54, 100, 10 }, "SUSTAIN TIME", TextFormat("%.2f", params[slotActive].sustainTimeValue), params[slotActive].sustainTimeValue, 0, 1);
            params[slotActive].sustainPunchValue = GuiSliderBar((Rectangle){ 246, 70, 100, 10 }, "SUSTAIN PUNCH", TextFormat("%.2f", params[slotActive].sustainPunchValue), params[slotActive].sustainPunchValue, 0, 1);
            params[slotActive].decayTimeValue = GuiSliderBar((Rectangle){ 246, 87, 100, 10 }, "DECAY TIME", TextFormat("%.2f", params[slotActive].decayTimeValue), params[slotActive].decayTimeValue, 0, 1);
            params[slotActive].startFrequencyValue = GuiSliderBar((Rectangle){ 246, 107, 100, 10 }, "START FREQUENCY", TextFormat("%.2f", params[slotActive].startFrequencyValue), params[slotActive].startFrequencyValue, 0, 1);
            params[slotActive].minFrequencyValue = GuiSliderBar((Rectangle){ 246, 124, 100, 10 }, "MIN FREQUENCY", TextFormat("%.2f", params[slotActive].minFrequencyValue), params[slotActive].minFrequencyValue, 0, 1);
            params[slotActive].slideValue = GuiSliderBar((Rectangle){ 246, 140, 100, 10 }, "SLIDE", TextFormat("%.2f", params[slotActive].slideValue), params[slotActive].slideValue, -1, 1);
            params[slotActive].deltaSlideValue = GuiSliderBar((Rectangle){ 246, 157, 100, 10 }, "DELTA SLIDE", TextFormat("%.2f", params[slotActive].deltaSlideValue), params[slotActive].deltaSlideValue, -1, 1);
            params[slotActive].vibratoDepthValue = GuiSliderBar((Rectangle){ 246, 173, 100, 10 }, "VIBRATO DEPTH", TextFormat("%.2f", params[slotActive].vibratoDepthValue), params[slotActive].vibratoDepthValue, 0, 1);
            params[slotActive].vibratoSpeedValue = GuiSliderBar((Rectangle){ 246, 189, 100, 10 }, "VIBRATO SPEED", TextFormat("%.2f", params[slotActive].vibratoSpeedValue), params[slotActive].vibratoSpeedValue, 0, 1);
            params[slotActive].changeAmountValue = GuiSliderBar((Rectangle){ 246, 209, 100, 10 }, "CHANGE AMOUNT", TextFormat("%.2f", params[slotActive].changeAmountValue), params[slotActive].changeAmountValue, -1, 1);
            params[slotActive].changeSpeedValue = GuiSliderBar((Rectangle){ 246, 225, 100, 10 }, "CHANGE SPEED", TextFormat("%.2f", params[slotActive].changeSpeedValue), params[slotActive].changeSpeedValue, 0, 1);
            params[slotActive].squareDutyValue = GuiSliderBar((Rectangle){ 246, 245, 100, 10 }, "SQUARE DUTY", TextFormat("%.2f", params[slotActive].squareDutyValue), params[slotActive].squareDutyValue, 0, 1);
            params[slotActive].dutySweepValue = GuiSliderBar((Rectangle){ 246, 261, 100, 10 }, "DUTY SWEEP", TextFormat("%.2f", params[slotActive].dutySweepValue), params[slotActive].dutySweepValue, -1, 1);
            params[slotActive].repeatSpeedValue = GuiSliderBar((Rectangle){ 246, 281, 100, 10 }, "REPEAT SPEED", TextFormat("%.2f", params[slotActive].repeatSpeedValue), params[slotActive].repeatSpeedValue, 0, 1);
            params[slotActive].phaserOffsetValue = GuiSliderBar((Rectangle){ 246, 304, 100, 10 }, "PHASER OFFSET", TextFormat("%.2f", params[slotActive].phaserOffsetValue), params[slotActive].phaserOffsetValue, -1, 1);
            params[slotActive].phaserSweepValue = GuiSliderBar((Rectangle){ 246, 320, 100, 10 }, "PHASER SWEEP", TextFormat("%.2f", params[slotActive].phaserSweepValue), params[slotActive].phaserSweepValue, -1, 1);
            params[slotActive].lpfCutoffValue = GuiSliderBar((Rectangle){ 246, 340, 100, 10 }, "LPF CUTOFF", TextFormat("%.2f", params[slotActive].lpfCutoffValue), params[slotActive].lpfCutoffValue, 0, 1);
            params[slotActive].lpfCutoffSweepValue = GuiSliderBar((Rectangle){ 246, 356, 100, 10 }, "LPF CUTOFF SWEEP", TextFormat("%.2f", params[slotActive].lpfCutoffSweepValue), params[slotActive].lpfCutoffSweepValue, -1, 1);
            params[slotActive].lpfResonanceValue = GuiSliderBar((Rectangle){ 246, 372, 100, 10 }, "LPF RESONANCE", TextFormat("%.2f", params[slotActive].lpfResonanceValue), params[slotActive].lpfResonanceValue, 0, 1);
            params[slotActive].hpfCutoffValue = GuiSliderBar((Rectangle){ 246, 388, 100, 10 }, "HPF CUTOFF", TextFormat("%.2f", params[slotActive].hpfCutoffValue), params[slotActive].hpfCutoffValue, 0, 1);
            params[slotActive].hpfCutoffSweepValue = GuiSliderBar((Rectangle){ 246, 404, 100, 10 }, "HPF CUTOFF SWEEP", TextFormat("%.2f", params[slotActive].hpfCutoffSweepValue), params[slotActive].hpfCutoffSweepValue, -1, 1);
            //--------------------------------------------------------------------------------

            playOnChangeChecked = GuiCheckBox((Rectangle){ 398, 16, 14, 14 }, "Play on change", playOnChangeChecked);
            if (GuiButton((Rectangle){ 398, 36, 106, 24 }, "#131#Play Sound")) PlaySound(sound[slotActive]);

            GuiLabel((Rectangle){ 404, 68, 25, 25 }, "Slot:");

            int tooglePadding = GuiGetStyle(TOGGLE, GROUP_PADDING);
            GuiSetStyle(TOGGLE, GROUP_PADDING, 2);
            slotActive = GuiToggleGroup((Rectangle){ 433, 72, 16, 16 }, "1;2;3;4", slotActive);
            GuiSetStyle(TOGGLE, GROUP_PADDING, tooglePadding);

            GuiLine((Rectangle){ 398, 93, 106, 16 }, NULL);

            if (GuiButton((Rectangle){ 398, 110, 106, 24 }, "#1#Load Sound")) showLoadFileDialog = true;
            if (GuiButton((Rectangle){ 398, 138, 106, 24 }, "#2#Save Sound")) showSaveFileDialog = true;

            GuiLine((Rectangle){ 398, 162, 106, 16 }, NULL);

            sampleRateActive = GuiComboBox((Rectangle){ 398, 178, 106, 24 }, "22050 Hz;44100 Hz", sampleRateActive);
            sampleSizeActive = GuiComboBox((Rectangle){ 398, 206, 106, 24 }, "8 bit;16 bit;32 bit", sampleSizeActive);

#if !defined(VERSION_ONE)
            fileTypeActive = GuiComboBox((Rectangle){ 398, 234, 106, 24 }, "WAV", fileTypeActive);
#else
            fileTypeActive = GuiComboBox((Rectangle){ 398, 234, 106, 24 }, "WAV;RAW;CODE", fileTypeActive);
#endif

            if (GuiButton((Rectangle){ 398, 264, 106, 24 }, "#7#Export Wave")) showExportFileDialog = true;

            GuiLine((Rectangle){ 398, 288, 106, 16 }, NULL);

            GuiLabel((Rectangle){ 398, 300, 106, 20 }, "Visual Style:");
            visualStyleActive = GuiComboBox((Rectangle){ 398, 320, 106, 24 }, "default;Jungle;Candy;Lavanda", visualStyleActive);

            screenSizeActive = GuiToggle((Rectangle){ 398, 348, 106, 24 }, "Screen Size x2", screenSizeActive);

            GuiLine((Rectangle){ 398, 372, 106, 20 }, NULL);

            if (GuiButton((Rectangle){ 398, 396, 106, 24 }, "#191#ABOUT")) windowAboutState.windowActive = true;

            // Draw status bar
            int textPadding = GuiGetStyle(STATUSBAR, TEXT_PADDING);
            GuiSetStyle(STATUSBAR, TEXT_PADDING, 0);
            GuiSetStyle(STATUSBAR, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
            GuiStatusBar((Rectangle){ 0, 492, 181, 20 }, FormatText("Total Samples: %i", wave[slotActive].sampleCount));
            GuiStatusBar((Rectangle){ 180, 492, 158, 20 }, FormatText("Duration: %i ms", wave[slotActive].sampleCount*1000/(wave[slotActive].sampleRate*wave[slotActive].channels)));
            GuiStatusBar((Rectangle){ 336, 492, 176, 20 }, FormatText("Size: %i bytes", wave[slotActive].sampleCount*wavSampleSize/8));
            GuiSetStyle(STATUSBAR, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
            GuiSetStyle(STATUSBAR, TEXT_PADDING, textPadding);
            //----------------------------------------------------------------------------------

            // Draw Wave form
            //--------------------------------------------------------------------------------
#if defined(RENDER_WAVE_TO_TEXTURE)
            DrawTextureEx(waveTarget.texture, (Vector2){ waveRec.x, waveRec.y }, 0.0f, 0.5f, WHITE);
#else
            DrawWave(&wave[slotActive], waveRec, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
#endif
            // TODO: FEATURE: Draw playing progress rectangle

            DrawRectangle(waveRec.x, waveRec.y + waveRec.height/2, waveRec.width, 1, Fade(GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_FOCUSED)), 0.6f));
            DrawRectangleLines(waveRec.x, waveRec.y, waveRec.width, waveRec.height, GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));
            //--------------------------------------------------------------------------------

            // GUI: About Window
            //--------------------------------------------------------------------------------
            GuiWindowAbout(&windowAboutState);
            //--------------------------------------------------------------------------------

            // GUI: Exit Window
            //----------------------------------------------------------------------------------------
            if (windowExitActive)
            {
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)), 0.85f));
                int result = GuiMessageBox((Rectangle){ GetScreenWidth()/2 - 125, GetScreenHeight()/2 - 50, 250, 100 }, "#159#Closing rFXGen", "Do you really want to exit?", "Yes;No");

                if ((result == 0) || (result == 2)) windowExitActive = false;
                else if (result == 1) exitWindow = true;
            }
            //----------------------------------------------------------------------------------------

            GuiUnlock();

            // GUI: Load File Dialog (and loading logic)
            //----------------------------------------------------------------------------------------
            if (showLoadFileDialog)
            {
#if defined(CUSTOM_MODAL_DIALOGS)
                int result = GuiFileDialog(DIALOG_MESSAGE, "Load sound file ...", inFileName, "Ok", "Just drag and drop your .rfx sound file!");
#else
                int result = GuiFileDialog(DIALOG_OPEN, "Load sound parameters file...", inFileName, "*.rfx;*.sfs", "Sound Param Files (*.rfx, *.sfs)");
#endif
                if (result == 1)
                {
                    // Load parameters file
                    params[slotActive] = LoadWaveParams(inFileName);
                    SetWindowTitle(FormatText("%s v%s - %s", toolName, toolVersion, GetFileName(inFileName)));
                    regenerate = true;
                }

                if (result >= 0) showLoadFileDialog = false;
            }
            //----------------------------------------------------------------------------------------

            // GUI: Save File Dialog (and saving logic)
            //----------------------------------------------------------------------------------------
            if (showSaveFileDialog)
            {
                strcpy(outFileName, "sound.rfx");
#if defined(CUSTOM_MODAL_DIALOGS)
                int result = GuiFileDialog(DIALOG_TEXTINPUT, "Save sound file as...", outFileName, "Ok;Cancel", NULL);
#else
                int result = GuiFileDialog(DIALOG_SAVE, "Save sound parameters file...", outFileName, "*.rfx", "Sound Param Files (*.rfx)");
#endif
                if (result == 1)
                {
                    // Save file: outFileName
                    // Check for valid extension and make sure it is
                    if ((GetExtension(outFileName) == NULL) || !IsFileExtension(outFileName, ".rfx")) strcat(outFileName, ".rfx\0");
                    SaveWaveParams(params[slotActive], outFileName);    // Save wave parameters

                #if defined(PLATFORM_WEB)
                    // Download file from MEMFS (emscripten memory filesystem)
                    // NOTE: Second argument must be a simple filename (we can't use directories)
                    emscripten_run_script(TextFormat("SaveFileFromMEMFSToDisk('%s','%s')", outFileName, GetFileName(outFileName)));
                #endif
                }

                if (result >= 0) showSaveFileDialog = false;
            }
            //----------------------------------------------------------------------------------------

            // GUI: Export File Dialog (and saving logic)
            //----------------------------------------------------------------------------------------
            if (showExportFileDialog)
            {
                // Consider different supported file types
                char filters[64] = { 0 };
                strcpy(outFileName, "sound");

                if (fileTypeActive == 0) { strcpy(filters, "*.wav"); strcat(outFileName, ".wav"); }
                else if (fileTypeActive == 1) { strcpy(filters, "*.raw"); strcat(outFileName, ".raw"); }
                else if (fileTypeActive == 2) { strcpy(filters, "*.h"); strcat(outFileName, ".h"); }

#if defined(CUSTOM_MODAL_DIALOGS)
                int result = GuiFileDialog(DIALOG_TEXTINPUT, "Export wave file...", outFileName, "Ok;Cancel", NULL);
#else
                int result = GuiFileDialog(DIALOG_SAVE, "Export wave file...", outFileName, filters, TextFormat("File type (%s)", filters));
#endif
                if (result == 1)
                {
                    // Export file: outFileName
                    Wave cwave = WaveCopy(wave[slotActive]);
                    WaveFormat(&cwave, wavSampleRate, wavSampleSize, 1);        // Before exporting wave data, we format it as desired

                    if (fileTypeActive == 0) ExportWave(cwave, outFileName);            // Export wave data as WAV file
                    else if (fileTypeActive == 2) ExportWaveAsCode(cwave, outFileName); // Export wave data as code file
                    else if (fileTypeActive == 1)
                    {
                        // Export Wave as RAW data
                        FILE *rawFile = fopen(outFileName, "wb");

                        if (rawFile != NULL)
                        {
                            fwrite(wave[slotActive].data, 1, wave[slotActive].sampleCount*wave[slotActive].sampleSize/8, rawFile);  // Write wave data
                            fclose(rawFile);
                        }
                    }

                    UnloadWave(cwave);

                #if defined(PLATFORM_WEB)
                    // Download file from MEMFS (emscripten memory filesystem)
                    // NOTE: Second argument must be a simple filename (we can't use directories)
                    emscripten_run_script(TextFormat("SaveFileFromMEMFSToDisk('%s','%s')", outFileName, GetFileName(outFileName)));
                #endif
                }

                if (result >= 0) showExportFileDialog = false;
            }
            //----------------------------------------------------------------------------------------

            EndTextureMode();

            // Draw render texture to screen
            if (screenSizeActive) DrawTexturePro(screenTarget.texture, (Rectangle){ 0, 0, screenTarget.texture.width, -screenTarget.texture.height }, (Rectangle){ 0, 0, screenTarget.texture.width*2, screenTarget.texture.height*2 }, (Vector2){ 0, 0 }, 0.0f, WHITE);
            else DrawTextureRec(screenTarget.texture, (Rectangle){ 0, 0, screenTarget.texture.width, -screenTarget.texture.height }, (Vector2){ 0, 0 }, WHITE);
#if defined(DEBUG)
            //DrawRectangleRec(slidersRec, Fade(RED, 0.5f));
#endif
        EndDrawing();
        //------------------------------------------------------------------------------------
    }

    // De-Initialization
    //----------------------------------------------------------------------------------------
    for (int i = 0; i < MAX_WAVE_SLOTS; i++)
    {
        UnloadSound(sound[i]);
        UnloadWave(wave[i]);
    }

    UnloadRenderTexture(screenTarget);
#if defined(RENDER_WAVE_TO_TEXTURE)
    UnloadRenderTexture(waveTarget);
#endif

    CloseAudioDevice();
    CloseWindow();          // Close window and OpenGL context
    //----------------------------------------------------------------------------------------

#endif  // COMMAND_LINE_ONLY
    return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//--------------------------------------------------------------------------------------------

#if defined(VERSION_ONE) || defined(COMMAND_LINE_ONLY)
// Show command line usage info
static void ShowCommandLineInfo(void)
{
    printf("\n//////////////////////////////////////////////////////////////////////////////////\n");
    printf("//                                                                              //\n");
    printf("// %s v%s ONE - %s               //\n", toolName, toolVersion, toolDescription);
    printf("// powered by raylib v2.6 (www.raylib.com) and raygui v2.6                      //\n");
    printf("// more info and bugs-report: github.com/raysan5/rfxgen                         //\n");
    printf("//                                                                              //\n");
    printf("// Copyright (c) 2014-2020 raylib technologies (@raylibtech)                    //\n");
    printf("//                                                                              //\n");
    printf("//////////////////////////////////////////////////////////////////////////////////\n\n");

    printf("USAGE:\n\n");
    printf("    > rfxgen [--help] --input <filename.ext> [--output <filename.ext>]\n");
    printf("             [--format <sample_rate>,<sample_size>,<channels>] [--play <filename.ext>]\n");

    printf("\nOPTIONS:\n\n");
    printf("    -h, --help                      : Show tool version and command line usage help\n\n");
    printf("    -i, --input <filename.ext>      : Define input file.\n");
    printf("                                      Supported extensions: .rfx, .sfs, .wav, .ogg, .flac, .mp3\n\n");
    printf("    -o, --output <filename.ext>     : Define output file.\n");
    printf("                                      Supported extensions: .wav, .raw, .h\n");
    printf("                                      NOTE: If not specified, defaults to: output.wav\n\n");
    printf("    -f, --format <sample_rate>,<sample_size>,<channels>\n");
    printf("                                    : Define output wave format. Comma separated values.\n");
    printf("                                      Supported values:\n");
    printf("                                          Sample rate:      22050, 44100\n");
    printf("                                          Sample size:      8, 16, 32\n");
    printf("                                          Channels:         1 (mono), 2 (stereo)\n");
    printf("                                      NOTE: If not specified, defaults to: 44100, 16, 1\n\n");
    printf("    -p, --play <filename.ext>       : Play provided sound.\n");
    printf("                                      Supported extensions: .wav, .ogg, .flac, .mp3\n");

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

    char inFileName[512] = { 0 };       // Input file name
    char outFileName[512] = { 0 };      // Output file name
    char playFileName[512] = { 0 };     // Play file name

    int sampleRate = 44100;             // Default conversion sample rate
    int sampleSize = 16;                // Default conversion sample size
    int channels = 1;                   // Default conversion channels number

#if defined(COMMAND_LINE_ONLY)
    if (argc == 1) showUsageInfo = true;
#endif

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
                    IsFileExtension(argv[i + 1], ".sfs") ||
                    IsFileExtension(argv[i + 1], ".wav") ||
                    IsFileExtension(argv[i + 1], ".ogg") ||
                    IsFileExtension(argv[i + 1], ".flac") ||
                    IsFileExtension(argv[i + 1], ".mp3"))
                {
                    strcpy(inFileName, argv[i + 1]);    // Read input filename
                }
                else printf("WARNING: Input file extension not recognized\n");

                i++;
            }
            else printf("WARNING: No input file provided\n");
        }
        else if ((strcmp(argv[i], "-o") == 0) || (strcmp(argv[i], "--output") == 0))
        {
            if (((i + 1) < argc) && (argv[i + 1][0] != '-'))
            {
                if (IsFileExtension(argv[i + 1], ".wav") ||
                    IsFileExtension(argv[i + 1], ".raw") ||
                    IsFileExtension(argv[i + 1], ".h"))
                {
                    strcpy(outFileName, argv[i + 1]);   // Read output filename
                }
                else printf("WARNING: Output file extension not recognized\n");

                i++;
            }
            else printf("WARNING: No output file provided\n");
        }
        else if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--format") == 0))
        {
            if (((i + 1) < argc) && (argv[i + 1][0] != '-'))
            {
                int numValues = 0;
                const char **values = TextSplit(argv[i + 1], ',', &numValues);

                if (numValues != 3) printf("WARNING: Incorrect number of format values\n");
                else
                {
                    // Read values text and convert to integer values
                    sampleRate = TextToInteger(values[0]);
                    sampleSize = TextToInteger(values[1]);
                    channels = TextToInteger(values[2]);

                    // Verify retrieved values are valid
                    if ((sampleRate != 44100) && (sampleRate != 22050))
                    {
                        printf("WARNING: Sample rate not supported. Default: 44100 Hz\n");
                        sampleRate = 44100;
                    }

                    if ((sampleSize != 8) && (sampleSize != 16) && (sampleSize != 32))
                    {
                        printf("WARNING: Sample size not supported. Default: 16 bit\n");
                        sampleSize = 16;
                    }

                    if ((channels != 1) && (channels != 2))
                    {
                        printf("WARNING: Channels number not supported. Default: 1 (mono)\n");
                        channels = 1;
                    }
                }
            }
            else printf("WARNING: Format parameters provided not valid\n");
        }
        else if ((strcmp(argv[i], "-p") == 0) || (strcmp(argv[i], "--play") == 0))
        {
            if (((i + 1) < argc) && (argv[i + 1][0] != '-'))
            {
                if (IsFileExtension(argv[i + 1], ".wav") ||
                    IsFileExtension(argv[i + 1], ".ogg") ||
                    IsFileExtension(argv[i + 1], ".flac") ||
                    IsFileExtension(argv[i + 1], ".mp3"))
                {
                    strcpy(playFileName, argv[i + 1]);   // Read filename to play
                    i++;
                }
                else printf("WARNING: Play file format not supported\n");

                i++;
            }
            else printf("WARNING: No file to play provided\n");
        }
    }

    // Process input file if provided
    if (inFileName[0] != '\0')
    {
        if (outFileName[0] == '\0') strcpy(outFileName, "output.wav");  // Set a default name for output in case not provided

        printf("\nInput file:       %s", inFileName);
        printf("\nOutput file:      %s", outFileName);
        printf("\nOutput format:    %i Hz, %i bits, %s\n\n", sampleRate, sampleSize, (channels == 1)? "Mono" : "Stereo");

        Wave wave = { 0 };

        if (IsFileExtension(inFileName, ".rfx") || IsFileExtension(inFileName, ".sfs"))
        {
            WaveParams params = LoadWaveParams(inFileName);
            wave = GenerateWave(params);
        }
        else if (IsFileExtension(inFileName, ".wav") ||
                 IsFileExtension(inFileName, ".ogg") ||
                 IsFileExtension(inFileName, ".flac") ||
                 IsFileExtension(inFileName, ".mp3"))
        {
            wave = LoadWave(inFileName);
        }

        // Format wave data to desired sampleRate, sampleSize and channels
        WaveFormat(&wave, sampleRate, sampleSize, channels);

        // Export wave data as audio file (.wav) or code file (.h)
        if (IsFileExtension(outFileName, ".wav")) ExportWave(wave, outFileName);
        else if (IsFileExtension(outFileName, ".h")) ExportWaveAsCode(wave, outFileName);
        else if (IsFileExtension(outFileName, ".raw"))
        {
            // Export Wave as RAW data
            FILE *rawFile = fopen(outFileName, "wb");

            if (rawFile != NULL)
            {
                fwrite(wave.data, 1, wave.sampleCount*wave.sampleSize/8, rawFile);  // Write wave data
                fclose(rawFile);
            }
        }

        UnloadWave(wave);
    }

    // Play audio file if provided
    if (playFileName[0] != '\0')
    {
        Wave wave = LoadWave(playFileName);     // Load audio (WAV, OGG, FLAC, MP3)
        PlayWaveCLI(wave);
        UnloadWave(wave);
    }

    if (showUsageInfo) ShowCommandLineInfo();
}
#endif      // VERSION_ONE

//--------------------------------------------------------------------------------------------
// Load/Save/Export functions
//--------------------------------------------------------------------------------------------

// Reset wave parameters
static void ResetWaveParams(WaveParams *params)
{
    // NOTE: Random seed is set to a random value
    params->randSeed = GetRandomValue(0x1, 0xFFFE);
    srand(params->randSeed);

    // Wave type
    params->waveTypeValue = 0;

    // Wave envelope params
    params->attackTimeValue = 0.0f;
    params->sustainTimeValue = 0.3f;
    params->sustainPunchValue = 0.0f;
    params->decayTimeValue = 0.4f;

    // Frequency params
    params->startFrequencyValue = 0.3f;
    params->minFrequencyValue = 0.0f;
    params->slideValue = 0.0f;
    params->deltaSlideValue = 0.0f;
    params->vibratoDepthValue = 0.0f;
    params->vibratoSpeedValue = 0.0f;
    //params->vibratoPhaseDelay = 0.0f;

    // Tone change params
    params->changeAmountValue = 0.0f;
    params->changeSpeedValue = 0.0f;

    // Square wave params
    params->squareDutyValue = 0.0f;
    params->dutySweepValue = 0.0f;

    // Repeat params
    params->repeatSpeedValue = 0.0f;

    // Phaser params
    params->phaserOffsetValue = 0.0f;
    params->phaserSweepValue = 0.0f;

    // Filter params
    params->lpfCutoffValue = 1.0f;
    params->lpfCutoffSweepValue = 0.0f;
    params->lpfResonanceValue = 0.0f;
    params->hpfCutoffValue = 0.0f;
    params->hpfCutoffSweepValue = 0.0f;
}

// Generates new wave from wave parameters
// NOTE: By default wave is generated as 44100Hz, 32bit float, mono
static Wave GenerateWave(WaveParams params)
{
    #define MAX_WAVE_LENGTH_SECONDS  10     // Max length for wave: 10 seconds
    #define WAVE_SAMPLE_RATE      44100     // Default sample rate

    // NOTE: GetRandomValue() is provided by raylib and seed is initialized at InitWindow()
    #define GetRandomFloat(range) ((float)GetRandomValue(0, 10000)/10000.0f*range)

    if (params.randSeed != 0) srand(params.randSeed);   // Initialize seed if required

    // Configuration parameters for generation
    // NOTE: Those parameters are calculated from selected values
    int phase = 0;
    double fperiod = 0.0;
    double fmaxperiod = 0.0;
    double fslide = 0.0;
    double fdslide = 0.0;
    int period = 0;
    float squareDuty = 0.0f;
    float squareSlide = 0.0f;
    int envelopeStage = 0;
    int envelopeTime = 0;
    int envelopeLength[3] = { 0 };
    float envelopeVolume = 0.0f;
    float fphase = 0.0f;
    float fdphase = 0.0f;
    int iphase = 0;
    float phaserBuffer[1024] = { 0.0f };
    int ipp = 0;
    float noiseBuffer[32] = { 0.0f };       // Required for noise wave, depends on random seed!
    float fltp = 0.0f;
    float fltdp = 0.0f;
    float fltw = 0.0f;
    float fltwd = 0.0f;
    float fltdmp = 0.0f;
    float fltphp = 0.0f;
    float flthp = 0.0f;
    float flthpd = 0.0f;
    float vibratoPhase = 0.0f;
    float vibratoSpeed = 0.0f;
    float vibratoAmplitude = 0.0f;
    int repeatTime = 0;
    int repeatLimit = 0;
    int arpeggioTime = 0;
    int arpeggioLimit = 0;
    double arpeggioModulation = 0.0;

    // HACK: Security check to avoid crash (why?)
    if (params.minFrequencyValue > params.startFrequencyValue) params.minFrequencyValue = params.startFrequencyValue;
    if (params.slideValue < params.deltaSlideValue) params.slideValue = params.deltaSlideValue;

    // Reset sample parameters
    //----------------------------------------------------------------------------------------
    fperiod = 100.0/(params.startFrequencyValue*params.startFrequencyValue + 0.001);
    period = (int)fperiod;
    fmaxperiod = 100.0/(params.minFrequencyValue*params.minFrequencyValue + 0.001);
    fslide = 1.0 - pow((double)params.slideValue, 3.0)*0.01;
    fdslide = -pow((double)params.deltaSlideValue, 3.0)*0.000001;
    squareDuty = 0.5f - params.squareDutyValue*0.5f;
    squareSlide = -params.dutySweepValue*0.00005f;

    if (params.changeAmountValue >= 0.0f) arpeggioModulation = 1.0 - pow((double)params.changeAmountValue, 2.0)*0.9;
    else arpeggioModulation = 1.0 + pow((double)params.changeAmountValue, 2.0)*10.0;

    arpeggioLimit = (int)(pow(1.0f - params.changeSpeedValue, 2.0f)*20000 + 32);

    if (params.changeSpeedValue == 1.0f) arpeggioLimit = 0;     // WATCH OUT: float comparison

    // Reset filter parameters
    fltw = pow(params.lpfCutoffValue, 3.0f)*0.1f;
    fltwd = 1.0f + params.lpfCutoffSweepValue*0.0001f;
    fltdmp = 5.0f/(1.0f + pow(params.lpfResonanceValue, 2.0f)*20.0f)*(0.01f + fltw);
    if (fltdmp > 0.8f) fltdmp = 0.8f;
    flthp = pow(params.hpfCutoffValue, 2.0f)*0.1f;
    flthpd = 1.0 + params.hpfCutoffSweepValue*0.0003f;

    // Reset vibrato
    vibratoSpeed = pow(params.vibratoSpeedValue, 2.0f)*0.01f;
    vibratoAmplitude = params.vibratoDepthValue*0.5f;

    // Reset envelope
    envelopeLength[0] = (int)(params.attackTimeValue*params.attackTimeValue*100000.0f);
    envelopeLength[1] = (int)(params.sustainTimeValue*params.sustainTimeValue*100000.0f);
    envelopeLength[2] = (int)(params.decayTimeValue*params.decayTimeValue*100000.0f);

    fphase = pow(params.phaserOffsetValue, 2.0f)*1020.0f;
    if (params.phaserOffsetValue < 0.0f) fphase = -fphase;

    fdphase = pow(params.phaserSweepValue, 2.0f)*1.0f;
    if (params.phaserSweepValue < 0.0f) fdphase = -fdphase;

    iphase = abs((int)fphase);

    for (int i = 0; i < 32; i++) noiseBuffer[i] = GetRandomFloat(2.0f) - 1.0f;      // WATCH OUT: GetRandomFloat()

    repeatLimit = (int)(pow(1.0f - params.repeatSpeedValue, 2.0f)*20000 + 32);

    if (params.repeatSpeedValue == 0.0f) repeatLimit = 0;
    //----------------------------------------------------------------------------------------

    // NOTE: We reserve enough space for up to 10 seconds of wave audio at given sample rate
    // By default we use float size samples, they are converted to desired sample size at the end
    float *buffer = (float *)calloc(MAX_WAVE_LENGTH_SECONDS*WAVE_SAMPLE_RATE, sizeof(float));
    bool generatingSample = true;
    int sampleCount = 0;

    for (int i = 0; i < MAX_WAVE_LENGTH_SECONDS*WAVE_SAMPLE_RATE; i++)
    {
        if (!generatingSample)
        {
            sampleCount = i;
            break;
        }

        // Generate sample using selected parameters
        //------------------------------------------------------------------------------------
        repeatTime++;

        if ((repeatLimit != 0) && (repeatTime >= repeatLimit))
        {
            // Reset sample parameters (only some of them)
            repeatTime = 0;

            fperiod = 100.0/(params.startFrequencyValue*params.startFrequencyValue + 0.001);
            period = (int)fperiod;
            fmaxperiod = 100.0/(params.minFrequencyValue*params.minFrequencyValue + 0.001);
            fslide = 1.0 - pow((double)params.slideValue, 3.0)*0.01;
            fdslide = -pow((double)params.deltaSlideValue, 3.0)*0.000001;
            squareDuty = 0.5f - params.squareDutyValue*0.5f;
            squareSlide = -params.dutySweepValue*0.00005f;

            if (params.changeAmountValue >= 0.0f) arpeggioModulation = 1.0 - pow((double)params.changeAmountValue, 2.0)*0.9;
            else arpeggioModulation = 1.0 + pow((double)params.changeAmountValue, 2.0)*10.0;

            arpeggioTime = 0;
            arpeggioLimit = (int)(pow(1.0f - params.changeSpeedValue, 2.0f)*20000 + 32);

            if (params.changeSpeedValue == 1.0f) arpeggioLimit = 0;     // WATCH OUT: float comparison
        }

        // Frequency envelopes/arpeggios
        arpeggioTime++;

        if ((arpeggioLimit != 0) && (arpeggioTime >= arpeggioLimit))
        {
            arpeggioLimit = 0;
            fperiod *= arpeggioModulation;
        }

        fslide += fdslide;
        fperiod *= fslide;

        if (fperiod > fmaxperiod)
        {
            fperiod = fmaxperiod;

            if (params.minFrequencyValue > 0.0f) generatingSample = false;
        }

        float rfperiod = fperiod;

        if (vibratoAmplitude > 0.0f)
        {
            vibratoPhase += vibratoSpeed;
            rfperiod = fperiod*(1.0 + sinf(vibratoPhase)*vibratoAmplitude);
        }

        period = (int)rfperiod;

        if (period < 8) period=8;

        squareDuty += squareSlide;

        if (squareDuty < 0.0f) squareDuty = 0.0f;
        if (squareDuty > 0.5f) squareDuty = 0.5f;

        // Volume envelope
        envelopeTime++;

        if (envelopeTime > envelopeLength[envelopeStage])
        {
            envelopeTime = 0;
            envelopeStage++;

            if (envelopeStage == 3) generatingSample = false;
        }

        if (envelopeStage == 0) envelopeVolume = (float)envelopeTime/envelopeLength[0];
        if (envelopeStage == 1) envelopeVolume = 1.0f + pow(1.0f - (float)envelopeTime/envelopeLength[1], 1.0f)*2.0f*params.sustainPunchValue;
        if (envelopeStage == 2) envelopeVolume = 1.0f - (float)envelopeTime/envelopeLength[2];

        // Phaser step
        fphase += fdphase;
        iphase = abs((int)fphase);

        if (iphase > 1023) iphase = 1023;

        if (flthpd != 0.0f)     // WATCH OUT!
        {
            flthp *= flthpd;
            if (flthp < 0.00001f) flthp = 0.00001f;
            if (flthp > 0.1f) flthp = 0.1f;
        }

        float ssample = 0.0f;

        #define MAX_SUPERSAMPLING   8

        // Supersampling x8
        for (int si = 0; si < MAX_SUPERSAMPLING; si++)
        {
            float sample = 0.0f;
            phase++;

            if (phase >= period)
            {
                //phase = 0;
                phase %= period;

                if (params.waveTypeValue == 3)
                {
                    for (int i = 0;i < 32; i++) noiseBuffer[i] = GetRandomFloat(2.0f) - 1.0f;   // WATCH OUT: GetRandomFloat()
                }
            }

            // base waveform
            float fp = (float)phase/period;

            switch (params.waveTypeValue)
            {
                case 0: // Square wave
                {
                    if (fp < squareDuty) sample = 0.5f;
                    else sample = -0.5f;

                } break;
                case 1: sample = 1.0f - fp*2; break;    // Sawtooth wave
                case 2: sample = sinf(fp*2*PI); break;  // Sine wave
                case 3: sample = noiseBuffer[phase*32/period]; break; // Noise wave
                default: break;
            }

            // LP filter
            float pp = fltp;
            fltw *= fltwd;

            if (fltw < 0.0f) fltw = 0.0f;
            if (fltw > 0.1f) fltw = 0.1f;

            if (params.lpfCutoffValue != 1.0f)  // WATCH OUT!
            {
                fltdp += (sample-fltp)*fltw;
                fltdp -= fltdp*fltdmp;
            }
            else
            {
                fltp = sample;
                fltdp = 0.0f;
            }

            fltp += fltdp;

            // HP filter
            fltphp += fltp - pp;
            fltphp -= fltphp*flthp;
            sample = fltphp;

            // Phaser
            phaserBuffer[ipp & 1023] = sample;
            sample += phaserBuffer[(ipp - iphase + 1024) & 1023];
            ipp = (ipp + 1) & 1023;

            // Final accumulation and envelope application
            ssample += sample*envelopeVolume;
        }

        #define SAMPLE_SCALE_COEFICIENT 0.2f    // NOTE: Used to scale sample value to [-1..1]

        ssample = (ssample/MAX_SUPERSAMPLING)*SAMPLE_SCALE_COEFICIENT;
        //------------------------------------------------------------------------------------

        // Accumulate samples in the buffer
        if (ssample > 1.0f) ssample = 1.0f;
        if (ssample < -1.0f) ssample = -1.0f;

        buffer[i] = ssample;
    }

    Wave genWave;
    genWave.sampleCount = sampleCount;
    genWave.sampleRate = WAVE_SAMPLE_RATE; // By default 44100 Hz
    genWave.sampleSize = 32;               // By default 32 bit float samples
    genWave.channels = 1;                  // By default 1 channel (mono)

    // NOTE: Wave can be converted to desired format after generation

    genWave.data = calloc(genWave.sampleCount*genWave.channels, genWave.sampleSize/8);
    memcpy(genWave.data, buffer, genWave.sampleCount*genWave.channels*genWave.sampleSize/8);

    free(buffer);

    return genWave;
}

// Load .rfx (rFXGen) or .sfs (sfxr) sound parameters file
static WaveParams LoadWaveParams(const char *fileName)
{
    WaveParams params = { 0 };

    if (IsFileExtension(fileName, ".rfx"))
    {
        FILE *rfxFile = fopen(fileName, "rb");

        if (rfxFile != NULL)
        {
            // Read .rfx file header
            unsigned char signature[5];
            fread(signature, 4, sizeof(unsigned char), rfxFile);

            // Check for valid .rfx file (FormatCC)
            if ((signature[0] == 'r') &&
                (signature[1] == 'F') &&
                (signature[2] == 'X') &&
                (signature[3] == ' '))
            {
                unsigned short version, length;
                fread(&version, 1, sizeof(unsigned short), rfxFile);
                fread(&length, 1, sizeof(unsigned short), rfxFile);

                if (version != 200) printf("[%s] rFX file version not supported (%i)\n", fileName, version);
                else
                {
                    if (length != sizeof(WaveParams)) printf("[%s] Wrong rFX wave parameters size\n", fileName);
                    else fread(&params, 1, sizeof(WaveParams), rfxFile);   // Load wave generation parameters
                }
            }
            else printf("[%s] rFX file does not seem to be valid\n", fileName);

            fclose(rfxFile);
        }
    }
    else if (IsFileExtension(fileName, ".sfs"))
    {
        FILE *sfsFile = fopen(fileName, "rb");

        if (sfsFile == NULL) return params;

        // Load .sfs sound parameters
        int version = 0;
        fread(&version, 1, sizeof(int), sfsFile);

        if ((version == 100) || (version == 101) || (version == 102))
        {
            fread(&params.waveTypeValue, 1, sizeof(int), sfsFile);

            volumeValue = 0.5f;

            if (version == 102) fread(&volumeValue, 1, sizeof(float), sfsFile);

            fread(&params.startFrequencyValue, 1, sizeof(float), sfsFile);
            fread(&params.minFrequencyValue, 1, sizeof(float), sfsFile);
            fread(&params.slideValue, 1, sizeof(float), sfsFile);

            if (version >= 101) fread(&params.deltaSlideValue, 1, sizeof(float), sfsFile);

            fread(&params.squareDutyValue, 1, sizeof(float), sfsFile);
            fread(&params.dutySweepValue, 1, sizeof(float), sfsFile);

            fread(&params.vibratoDepthValue, 1, sizeof(float), sfsFile);
            fread(&params.vibratoSpeedValue, 1, sizeof(float), sfsFile);

            float vibratoPhaseDelay = 0.0f;
            fread(&vibratoPhaseDelay, 1, sizeof(float), sfsFile); // Not used

            fread(&params.attackTimeValue, 1, sizeof(float), sfsFile);
            fread(&params.sustainTimeValue, 1, sizeof(float), sfsFile);
            fread(&params.decayTimeValue, 1, sizeof(float), sfsFile);
            fread(&params.sustainPunchValue, 1, sizeof(float), sfsFile);

            bool filterOn = false;
            fread(&filterOn, 1, sizeof(bool), sfsFile); // Not used

            fread(&params.lpfResonanceValue, 1, sizeof(float), sfsFile);
            fread(&params.lpfCutoffValue, 1, sizeof(float), sfsFile);
            fread(&params.lpfCutoffSweepValue, 1, sizeof(float), sfsFile);
            fread(&params.hpfCutoffValue, 1, sizeof(float), sfsFile);
            fread(&params.hpfCutoffSweepValue, 1, sizeof(float), sfsFile);

            fread(&params.phaserOffsetValue, 1, sizeof(float), sfsFile);
            fread(&params.phaserSweepValue, 1, sizeof(float), sfsFile);
            fread(&params.repeatSpeedValue, 1, sizeof(float), sfsFile);

            if (version >= 101)
            {
                fread(&params.changeSpeedValue, 1, sizeof(float), sfsFile);
                fread(&params.changeAmountValue, 1, sizeof(float), sfsFile);
            }
        }
        else printf("[%s] SFS file version not supported\n", fileName);

        fclose(sfsFile);
    }

    return params;
}

// Save .rfx sound parameters file
static void SaveWaveParams(WaveParams params, const char *fileName)
{
    if (IsFileExtension(fileName, ".rfx"))
    {
        // Fx Sound File Structure (.rfx)
        // ------------------------------------------------------
        // Offset | Size  | Type       | Description
        // ------------------------------------------------------
        // 0      | 4     | char       | Signature: "rFX "
        // 4      | 2     | short      | Version: 200
        // 6      | 2     | short      | Data length: 96 bytes
        // 8      | 96    | WaveParams | Wave parameters
        // ------------------------------------------------------

        FILE *rfxFile = fopen(fileName, "wb");

        if (rfxFile != NULL)
        {
            unsigned char signature[5] = "rFX ";
            unsigned short version = 200;
            unsigned short length = sizeof(WaveParams);

            // Write .rfx file header
            fwrite(signature, 4, sizeof(unsigned char), rfxFile);
            fwrite(&version, 1, sizeof(unsigned short), rfxFile);
            fwrite(&length, 1, sizeof(unsigned short), rfxFile);

            // Write wave generation parameters
            fwrite(&params, 1, sizeof(WaveParams), rfxFile);

            fclose(rfxFile);
        }
    }
}

//--------------------------------------------------------------------------------------------
// Sound generation functions
//--------------------------------------------------------------------------------------------

// Generate sound: Pickup/Coin
static WaveParams GenPickupCoin(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.startFrequencyValue = 0.4f + frnd(0.5f);
    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = frnd(0.1f);
    params.decayTimeValue = 0.1f + frnd(0.4f);
    params.sustainPunchValue = 0.3f + frnd(0.3f);

    if (GetRandomValue(0, 1))
    {
        params.changeSpeedValue = 0.5f + frnd(0.2f);
        params.changeAmountValue = 0.2f + frnd(0.4f);
    }

    return params;
}

// Generate sound: Laser shoot
static WaveParams GenLaserShoot(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.waveTypeValue = GetRandomValue(0, 2);

    if ((params.waveTypeValue == 2) && GetRandomValue(0, 1)) params.waveTypeValue = GetRandomValue(0, 1);

    params.startFrequencyValue = 0.5f + frnd(0.5f);
    params.minFrequencyValue = params.startFrequencyValue - 0.2f - frnd(0.6f);

    if (params.minFrequencyValue < 0.2f) params.minFrequencyValue = 0.2f;

    params.slideValue = -0.15f - frnd(0.2f);

    if (GetRandomValue(0, 2) == 0)
    {
        params.startFrequencyValue = 0.3f + frnd(0.6f);
        params.minFrequencyValue = frnd(0.1f);
        params.slideValue = -0.35f - frnd(0.3f);
    }

    if (GetRandomValue(0, 1))
    {
        params.squareDutyValue = frnd(0.5f);
        params.dutySweepValue = frnd(0.2f);
    }
    else
    {
        params.squareDutyValue = 0.4f + frnd(0.5f);
        params.dutySweepValue = -frnd(0.7f);
    }

    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = 0.1f + frnd(0.2f);
    params.decayTimeValue = frnd(0.4f);

    if (GetRandomValue(0, 1)) params.sustainPunchValue = frnd(0.3f);

    if (GetRandomValue(0, 2) == 0)
    {
        params.phaserOffsetValue = frnd(0.2f);
        params.phaserSweepValue = -frnd(0.2f);
    }

    if (GetRandomValue(0, 1)) params.hpfCutoffValue = frnd(0.3f);

    return params;
}

// Generate sound: Explosion
static WaveParams GenExplosion(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.waveTypeValue = 3;

    if (GetRandomValue(0, 1))
    {
        params.startFrequencyValue = 0.1f + frnd(0.4f);
        params.slideValue = -0.1f + frnd(0.4f);
    }
    else
    {
        params.startFrequencyValue = 0.2f + frnd(0.7f);
        params.slideValue = -0.2f - frnd(0.2f);
    }

    params.startFrequencyValue *= params.startFrequencyValue;

    if (GetRandomValue(0, 4) == 0) params.slideValue = 0.0f;
    if (GetRandomValue(0, 2) == 0) params.repeatSpeedValue = 0.3f + frnd(0.5f);

    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = 0.1f + frnd(0.3f);
    params.decayTimeValue = frnd(0.5f);

    if (GetRandomValue(0, 1) == 0)
    {
        params.phaserOffsetValue = -0.3f + frnd(0.9f);
        params.phaserSweepValue = -frnd(0.3f);
    }

    params.sustainPunchValue = 0.2f + frnd(0.6f);

    if (GetRandomValue(0, 1))
    {
        params.vibratoDepthValue = frnd(0.7f);
        params.vibratoSpeedValue = frnd(0.6f);
    }

    if (GetRandomValue(0, 2) == 0)
    {
        params.changeSpeedValue = 0.6f + frnd(0.3f);
        params.changeAmountValue = 0.8f - frnd(1.6f);
    }

    return params;
}

// Generate sound: Powerup
static WaveParams GenPowerup(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    if (GetRandomValue(0, 1)) params.waveTypeValue = 1;
    else params.squareDutyValue = frnd(0.6f);

    if (GetRandomValue(0, 1))
    {
        params.startFrequencyValue = 0.2f + frnd(0.3f);
        params.slideValue = 0.1f + frnd(0.4f);
        params.repeatSpeedValue = 0.4f + frnd(0.4f);
    }
    else
    {
        params.startFrequencyValue = 0.2f + frnd(0.3f);
        params.slideValue = 0.05f + frnd(0.2f);

        if (GetRandomValue(0, 1))
        {
            params.vibratoDepthValue = frnd(0.7f);
            params.vibratoSpeedValue = frnd(0.6f);
        }
    }

    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = frnd(0.4f);
    params.decayTimeValue = 0.1f + frnd(0.4f);

    return params;
}

// Generate sound: Hit/Hurt
static WaveParams GenHitHurt(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.waveTypeValue = GetRandomValue(0, 2);
    if (params.waveTypeValue == 2) params.waveTypeValue = 3;
    if (params.waveTypeValue == 0) params.squareDutyValue = frnd(0.6f);

    params.startFrequencyValue = 0.2f + frnd(0.6f);
    params.slideValue = -0.3f - frnd(0.4f);
    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = frnd(0.1f);
    params.decayTimeValue = 0.1f + frnd(0.2f);

    if (GetRandomValue(0, 1)) params.hpfCutoffValue = frnd(0.3f);

    return params;
}

// Generate sound: Jump
static WaveParams GenJump(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.waveTypeValue = 0;
    params.squareDutyValue = frnd(0.6f);
    params.startFrequencyValue = 0.3f + frnd(0.3f);
    params.slideValue = 0.1f + frnd(0.2f);
    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = 0.1f + frnd(0.3f);
    params.decayTimeValue = 0.1f + frnd(0.2f);

    if (GetRandomValue(0, 1)) params.hpfCutoffValue = frnd(0.3f);
    if (GetRandomValue(0, 1)) params.lpfCutoffValue = 1.0f - frnd(0.6f);

    return params;
}

// Generate sound: Blip/Select
static WaveParams GenBlipSelect(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.waveTypeValue = GetRandomValue(0, 1);
    if (params.waveTypeValue == 0) params.squareDutyValue = frnd(0.6f);
    params.startFrequencyValue = 0.2f + frnd(0.4f);
    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = 0.1f + frnd(0.1f);
    params.decayTimeValue = frnd(0.2f);
    params.hpfCutoffValue = 0.1f;

    return params;
}

// Generate random sound
static WaveParams GenRandomize(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.randSeed = GetRandomValue(0, 0xFFFE);

    params.startFrequencyValue = pow(frnd(2.0f) - 1.0f, 2.0f);

    if (GetRandomValue(0, 1)) params.startFrequencyValue = pow(frnd(2.0f) - 1.0f, 3.0f)+0.5f;

    params.minFrequencyValue = 0.0f;
    params.slideValue = pow(frnd(2.0f) - 1.0f, 5.0f);

    if ((params.startFrequencyValue > 0.7f) && (params.slideValue > 0.2f)) params.slideValue = -params.slideValue;
    if ((params.startFrequencyValue < 0.2f) && (params.slideValue < -0.05f)) params.slideValue = -params.slideValue;

    params.deltaSlideValue = pow(frnd(2.0f) - 1.0f, 3.0f);
    params.squareDutyValue = frnd(2.0f) - 1.0f;
    params.dutySweepValue = pow(frnd(2.0f) - 1.0f, 3.0f);
    params.vibratoDepthValue = pow(frnd(2.0f) - 1.0f, 3.0f);
    params.vibratoSpeedValue = frnd(2.0f) - 1.0f;
    //params.vibratoPhaseDelay = frnd(2.0f) - 1.0f;
    params.attackTimeValue = pow(frnd(2.0f) - 1.0f, 3.0f);
    params.sustainTimeValue = pow(frnd(2.0f) - 1.0f, 2.0f);
    params.decayTimeValue = frnd(2.0f)-1.0f;
    params.sustainPunchValue = pow(frnd(0.8f), 2.0f);

    if (params.attackTimeValue + params.sustainTimeValue + params.decayTimeValue < 0.2f)
    {
        params.sustainTimeValue += 0.2f + frnd(0.3f);
        params.decayTimeValue += 0.2f + frnd(0.3f);
    }

    params.lpfResonanceValue = frnd(2.0f) - 1.0f;
    params.lpfCutoffValue = 1.0f - pow(frnd(1.0f), 3.0f);
    params.lpfCutoffSweepValue = pow(frnd(2.0f) - 1.0f, 3.0f);

    if (params.lpfCutoffValue < 0.1f && params.lpfCutoffSweepValue < -0.05f) params.lpfCutoffSweepValue = -params.lpfCutoffSweepValue;

    params.hpfCutoffValue = pow(frnd(1.0f), 5.0f);
    params.hpfCutoffSweepValue = pow(frnd(2.0f) - 1.0f, 5.0f);
    params.phaserOffsetValue = pow(frnd(2.0f) - 1.0f, 3.0f);
    params.phaserSweepValue = pow(frnd(2.0f) - 1.0f, 3.0f);
    params.repeatSpeedValue = frnd(2.0f) - 1.0f;
    params.changeSpeedValue = frnd(2.0f) - 1.0f;
    params.changeAmountValue = frnd(2.0f) - 1.0f;

    return params;
}

// Mutate current sound
static void WaveMutate(WaveParams *params)
{
    if (GetRandomValue(0, 1)) params->startFrequencyValue += frnd(0.1f) - 0.05f;
    //if (GetRandomValue(0, 1)) params.minFrequencyValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->slideValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->deltaSlideValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->squareDutyValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->dutySweepValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->vibratoDepthValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->vibratoSpeedValue += frnd(0.1f) - 0.05f;
    //if (GetRandomValue(0, 1)) params.vibratoPhaseDelay += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->attackTimeValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->sustainTimeValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->decayTimeValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->sustainPunchValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->lpfResonanceValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->lpfCutoffValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->lpfCutoffSweepValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->hpfCutoffValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->hpfCutoffSweepValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->phaserOffsetValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->phaserSweepValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->repeatSpeedValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->changeSpeedValue += frnd(0.1f) - 0.05f;
    if (GetRandomValue(0, 1)) params->changeAmountValue += frnd(0.1f) - 0.05f;
}

//--------------------------------------------------------------------------------------------
// Auxiliar functions
//--------------------------------------------------------------------------------------------

#if !defined(COMMAND_LINE_ONLY)
// Draw wave data
// NOTE: For proper visualization, MSAA x4 is recommended, alternatively
// it should be rendered to a bigger texture and then scaled down with
// bilinear/trilinear texture filtering
static void DrawWave(Wave *wave, Rectangle bounds, Color color)
{
    float sample, sampleNext;
    float currentSample = 0.0f;
    float sampleIncrement = (float)wave->sampleCount/(float)(bounds.width*2);
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
#endif // COMMAND_LINE_ONLY

#if defined(VERSION_ONE) || defined(COMMAND_LINE_ONLY)
// Simple time wait in milliseconds
static void WaitTime(int ms)
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
                printf("\r[");
                for (int j = 0; j < 50; j++)
                {
                    if (j < percent/2) printf("=");
                    else printf(" ");
                }
                printf("] [%02i%%]", percent);

                prevPercent = percent;
            }
        }

        printf("\n\n");
    }
}

// Play provided wave through CLI
static void PlayWaveCLI(Wave wave)
{
    float waveTimeMs = (float)wave.sampleCount*1000.0/(wave.sampleRate*wave.channels);

    InitAudioDevice();                  // Init audio device
    Sound fx = LoadSoundFromWave(wave); // Load audio wave

    printf("\n//////////////////////////////////////////////////////////////////////////////////\n");
    printf("//                                                                              //\n");
    printf("// %s v%s ONE - CLI audio player                                           //\n", toolName, toolVersion);
    printf("//                                                                              //\n");
    printf("// more info and bugs-report: github.com/raysan5/rfxgen                         //\n");
    printf("//                                                                              //\n");
    printf("// Copyright (c) 2020 raylib technologies (@raylibtech)                         //\n");
    printf("//                                                                              //\n");
    printf("//////////////////////////////////////////////////////////////////////////////////\n\n");

    printf("Playing sound [%.2f sec.]. Press ENTER to finish.\n", waveTimeMs/1000.0f);

    PlaySound(fx);                      // Play sound
    WaitTime(waveTimeMs);               // Wait while audio is playing
    UnloadSound(fx);                    // Unload sound data
    CloseAudioDevice();                 // Close audio device
}

#if !defined(__WIN32)
// Check if a key has been pressed
static int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

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
#endif      // VERSION_ONE
