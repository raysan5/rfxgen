/*******************************************************************************************
*
*   rfxgen - A simple and easy-to-use sounds generator (based on Tomas Petterson sfxr)
*
*   CONFIGURATION:
*
*   #define RFXGEN_IMPLEMENTATION
*       Use this prior to #include "rfxgen.h" to indicate the function definitions should be included.
*
*   #define RFXGEN_LOG
*       Used to switch how logging occurs within the library. By default, will log with printf().
*
*   #define RFXGEN_RAND(min, max)
*       Used to generate a random value between the given min and max integers. Defaults to rand().
*
*   #define RFXGEN_SRAND
*       Used to seed the random number generator. Defaults to srand().
*
*   #define RFXGEN_CALLOC
*       By default, will use calloc() for allocating memory.
*
*   #define RFXGEN_FREE
*       By default, will use free() to deallocate memory.
*
*   #define RFXGEN_MEMCPY
*       By default, will use memcpy() for copying memory.
*
*   #define RFXGEN_ISFILEEXTENSION
*       Used to determine if a file has the given file extension.
*
*
*   DEVELOPERS:
*       Ramon Santamaria (@raysan5): Developer, supervisor, updater and maintainer.
*       Rob Loach (@RobLoach): Port generator to single-file header-only library (Oct.2022)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2026 raylib technologies (@raylibtech) / Ramon Santamaria (@raysan5)
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

#ifndef RFXGEN_H
#define RFXGEN_H

#ifndef RLAPI
    #define RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#if !defined(RFXGEN_GEN_SAMPLE_RATE)
    #define RFXGEN_GEN_SAMPLE_RATE  44100       // Generation sample rate
#endif

#define RFXGEN_GEN_SAMPLE_SIZE         32       // Bit size of generated waves (32 bit -> float)
#define RFXGEN_GEN_CHANNELS             1       // Channels for generated waves (only 1 - MONO)

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
// Module Functions Declaration
//----------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

// Load/Save/Export data functions
RLAPI WaveParams LoadWaveParams(const char *fileName);                 // Load wave parameters from file
RLAPI void SaveWaveParams(WaveParams params, const char *fileName);    // Save wave parameters to file
RLAPI void ResetWaveParams(WaveParams *params);                        // Reset wave parameters
RLAPI float *GenerateWave(WaveParams params, unsigned int *frameCount); // Generate wave data from parameters (32bit, mono)

// Sound generation functions
RLAPI WaveParams GenPickupCoin(void);      // Generate sound: Pickup/Coin
RLAPI WaveParams GenLaserShoot(void);      // Generate sound: Laser shoot
RLAPI WaveParams GenExplosion(void);       // Generate sound: Explosion
RLAPI WaveParams GenPowerup(void);         // Generate sound: Powerup
RLAPI WaveParams GenHitHurt(void);         // Generate sound: Hit/Hurt
RLAPI WaveParams GenJump(void);            // Generate sound: Jump
RLAPI WaveParams GenBlipSelect(void);      // Generate sound: Blip/Select
RLAPI WaveParams GenRandomize(void);       // Generate random sound
RLAPI void WaveMutate(WaveParams *params); // Mutate current sound

#if defined(__cplusplus)
}
#endif

#endif      // RFXGEN_H

/***********************************************************************************
*
*   RFXGEN IMPLEMENTATION
*
************************************************************************************/

#if defined(RFXGEN_IMPLEMENTATION)

// C standard library dependencies
#include <stdlib.h>     // Required for: calloc(), free(), [srand(), rand()]
#include <stdio.h>      // Required for: printf()
#include <string.h>     // Required for: strcmp(), strlen(), memcpy()
#include <math.h>       // Required for: abs(), pow(), powf()
#include <time.h>       // Required for: time()

#ifndef RFXGEN_SRAND
    #define RFXGEN_SRAND  srand
#endif

// Random integer between the given range
// WARNING: RFXGEN_RAND(0, 1) always return 0 
// because we do the module 1 with random number generated! 
#ifndef RFXGEN_RAND
    #define RFXGEN_RAND(min, max) ((rand()%(max - min) + 1) + min)
#endif

// Random condition, true or false
#define RFXGEN_RAND01 (RFXGEN_RAND(0, 100) > 50)

// Float random number generation
#define RFXGEN_RANDF(range) (((float)RFXGEN_RAND(0, 10000)/10000.0f)*range)

// Simple log system to avoid printf() calls if required
#ifndef RFXGEN_LOG
    #define RFXGEN_LOG(...) printf(__VA_ARGS__)
#endif

// Allocate requested memory and return a pointer to it
#ifndef RFXGEN_CALLOC
    #define RFXGEN_CALLOC calloc
#endif

// Deallocate previously allocated memory
#ifndef RFXGEN_FREE
    #define RFXGEN_FREE free
#endif

// Copy memory
#ifndef RFXGEN_MEMCPY
    #define RFXGEN_MEMCPY memcpy
#endif

// Check if a file extension matches the given extension
#ifndef RFXGEN_ISFILEEXTENSION
    #define RFXGEN_ISFILEEXTENSION(file, extension) (strcmp((file) + strlen(file) - strlen(extension), (extension)) == 0)
#endif


//--------------------------------------------------------------------------------------------
// Load/Save/Export functions
//--------------------------------------------------------------------------------------------

// Reset wave parameters
RLAPI void ResetWaveParams(WaveParams *params)
{
    // NOTE: Random seed is set to a random value
    params->randSeed = RFXGEN_RAND(0x1, 0xFFFE);
    RFXGEN_SRAND(params->randSeed);

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
RLAPI float *GenerateWave(WaveParams params, unsigned int *frameCount)
{
    #define RFXGEN_MAX_GEN_BUFFER_LENGTH  10      // Max length for generation buffer: 10 seconds

    if (params.randSeed != 0) RFXGEN_SRAND(params.randSeed);   // Initialize seed if required

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
    float phaserBuffer[1024] = { 0 };
    int ipp = 0;
    float noiseBuffer[32] = { 0 };       // Required for noise wave, depends on random seed!
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

    arpeggioLimit = (int)(powf(1.0f - params.changeSpeedValue, 2.0f)*20000 + 32);

    if (params.changeSpeedValue == 1.0f) arpeggioLimit = 0;     // WATCH OUT: float comparison

    // Reset filter parameters
    fltw = powf(params.lpfCutoffValue, 3.0f)*0.1f;
    fltwd = 1.0f + params.lpfCutoffSweepValue*0.0001f;
    fltdmp = 5.0f/(1.0f + powf(params.lpfResonanceValue, 2.0f)*20.0f)*(0.01f + fltw);
    if (fltdmp > 0.8f) fltdmp = 0.8f;
    flthp = powf(params.hpfCutoffValue, 2.0f)*0.1f;
    flthpd = 1.0f + params.hpfCutoffSweepValue*0.0003f;

    // Reset vibrato
    vibratoSpeed = powf(params.vibratoSpeedValue, 2.0f)*0.01f;
    vibratoAmplitude = params.vibratoDepthValue*0.5f;

    // Reset envelope
    envelopeLength[0] = (int)(params.attackTimeValue*params.attackTimeValue*100000.0f);
    envelopeLength[1] = (int)(params.sustainTimeValue*params.sustainTimeValue*100000.0f);
    envelopeLength[2] = (int)(params.decayTimeValue*params.decayTimeValue*100000.0f);

    fphase = powf(params.phaserOffsetValue, 2.0f)*1020.0f;
    if (params.phaserOffsetValue < 0.0f) fphase = -fphase;

    fdphase = powf(params.phaserSweepValue, 2.0f)*1.0f;
    if (params.phaserSweepValue < 0.0f) fdphase = -fdphase;

    iphase = abs((int)fphase);

    for (int i = 0; i < 32; i++) noiseBuffer[i] = RFXGEN_RANDF(2.0f) - 1.0f;

    repeatLimit = (int)(powf(1.0f - params.repeatSpeedValue, 2.0f)*20000 + 32);

    if (params.repeatSpeedValue == 0.0f) repeatLimit = 0;
    //----------------------------------------------------------------------------------------

    // NOTE: We reserve enough space for up to 10 seconds of wave audio at given sample rate
    // By default we use float size samples, they are converted to desired sample size at the end
    float *buffer = (float *)RFXGEN_CALLOC(RFXGEN_MAX_GEN_BUFFER_LENGTH*RFXGEN_GEN_SAMPLE_RATE, sizeof(float));
    bool generatingSample = true;
    int sampleCount = 0;

    for (int i = 0; i < RFXGEN_MAX_GEN_BUFFER_LENGTH*RFXGEN_GEN_SAMPLE_RATE; i++)
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
            arpeggioLimit = (int)(powf(1.0f - params.changeSpeedValue, 2.0f)*20000 + 32);

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

        float rfperiod = (float)fperiod;

        if (vibratoAmplitude > 0.0f)
        {
            vibratoPhase += vibratoSpeed;
            rfperiod = (float)(fperiod*(1.0 + sinf(vibratoPhase)*vibratoAmplitude));
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
        if (envelopeStage == 1) envelopeVolume = 1.0f + powf(1.0f - (float)envelopeTime/envelopeLength[1], 1.0f)*2.0f*params.sustainPunchValue;
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
                    for (int i = 0;i < 32; i++) noiseBuffer[i] = RFXGEN_RANDF(2.0f) - 1.0f;
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

    // NOTE: Wave data is generated by default as 32bit float data and 1 channel (mono)

    float *genWaveData = (float *)RFXGEN_CALLOC(sampleCount, sizeof(float));
    RFXGEN_MEMCPY(genWaveData, buffer, sampleCount*sizeof(float));
    RFXGEN_FREE(buffer);
    
    // NOTE: Wave can be converted to desired format after generation

    *frameCount = sampleCount;      // By default generated wave only has 1 channel
    return genWaveData;
}

// Load .rfx (rFXGen) or .sfs (sfxr) sound parameters file
RLAPI WaveParams LoadWaveParams(const char *fileName)
{
    WaveParams params = { 0 };

    if (RFXGEN_ISFILEEXTENSION(fileName, ".rfx"))
    {
        FILE *rfxFile = fopen(fileName, "rb");

        if (rfxFile != NULL)
        {
            // Read .rfx file header
            unsigned char signature[5] = { 0 };
            fread(signature, 4, sizeof(unsigned char), rfxFile);

            // Check for valid .rfx file (FormatCC)
            if ((signature[0] == 'r') &&
                (signature[1] == 'F') &&
                (signature[2] == 'X') &&
                (signature[3] == ' '))
            {
                unsigned short version = 0;
                unsigned short length = 0;
                fread(&version, 1, sizeof(unsigned short), rfxFile);
                fread(&length, 1, sizeof(unsigned short), rfxFile);

                if (version != 200) RFXGEN_LOG("[%s] rFX file version not supported (%i)\n", fileName, version);
                else
                {
                    if (length != sizeof(WaveParams)) RFXGEN_LOG("[%s] Wrong rFX wave parameters size\n", fileName);
                    else fread(&params, 1, sizeof(WaveParams), rfxFile);   // Load wave generation parameters
                }
            }
            else RFXGEN_LOG("[%s] rFX file does not seem to be valid\n", fileName);

            fclose(rfxFile);
        }
    }
    /*
    else if (RFXGEN_ISFILEEXTENSION(fileName, ".sfs"))
    {
        // NOTE: It seem .sfs loading has some issues,
        // I can't see the point to keep supporting this format

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
        else RFXGEN_LOG("[%s] SFS file version not supported\n", fileName);

        fclose(sfsFile);
    }
    */

    return params;
}

// Save .rfx sound parameters file
RLAPI void SaveWaveParams(WaveParams params, const char *fileName)
{
    if (RFXGEN_ISFILEEXTENSION(fileName, ".rfx"))
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
RLAPI WaveParams GenPickupCoin(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.startFrequencyValue = 0.4f + RFXGEN_RANDF(0.5f);
    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = RFXGEN_RANDF(0.1f);
    params.decayTimeValue = 0.1f + RFXGEN_RANDF(0.4f);
    params.sustainPunchValue = 0.3f + RFXGEN_RANDF(0.3f);

    if (RFXGEN_RAND01)
    {
        params.changeSpeedValue = 0.5f + RFXGEN_RANDF(0.2f);
        params.changeAmountValue = 0.2f + RFXGEN_RANDF(0.4f);
    }

    return params;
}

// Generate sound: Laser shoot
RLAPI WaveParams GenLaserShoot(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.waveTypeValue = RFXGEN_RAND(0, 2);

    if ((params.waveTypeValue == 2) && RFXGEN_RAND01) params.waveTypeValue = RFXGEN_RAND01;

    params.startFrequencyValue = 0.5f + RFXGEN_RANDF(0.5f);
    params.minFrequencyValue = params.startFrequencyValue - 0.2f - RFXGEN_RANDF(0.6f);

    if (params.minFrequencyValue < 0.2f) params.minFrequencyValue = 0.2f;

    params.slideValue = -0.15f - RFXGEN_RANDF(0.2f);

    if (RFXGEN_RAND(0, 2) == 0)
    {
        params.startFrequencyValue = 0.3f + RFXGEN_RANDF(0.6f);
        params.minFrequencyValue = RFXGEN_RANDF(0.1f);
        params.slideValue = -0.35f - RFXGEN_RANDF(0.3f);
    }

    if (RFXGEN_RAND01)
    {
        params.squareDutyValue = RFXGEN_RANDF(0.5f);
        params.dutySweepValue = RFXGEN_RANDF(0.2f);
    }
    else
    {
        params.squareDutyValue = 0.4f + RFXGEN_RANDF(0.5f);
        params.dutySweepValue = -RFXGEN_RANDF(0.7f);
    }

    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = 0.1f + RFXGEN_RANDF(0.2f);
    params.decayTimeValue = RFXGEN_RANDF(0.4f);

    if (RFXGEN_RAND01) params.sustainPunchValue = RFXGEN_RANDF(0.3f);

    if (RFXGEN_RAND(0, 2) == 0)
    {
        params.phaserOffsetValue = RFXGEN_RANDF(0.2f);
        params.phaserSweepValue = -RFXGEN_RANDF(0.2f);
    }

    if (RFXGEN_RAND01) params.hpfCutoffValue = RFXGEN_RANDF(0.3f);

    return params;
}

// Generate sound: Explosion
RLAPI WaveParams GenExplosion(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.waveTypeValue = 3;

    if (RFXGEN_RAND01)
    {
        params.startFrequencyValue = 0.1f + RFXGEN_RANDF(0.4f);
        params.slideValue = -0.1f + RFXGEN_RANDF(0.4f);
    }
    else
    {
        params.startFrequencyValue = 0.2f + RFXGEN_RANDF(0.7f);
        params.slideValue = -0.2f - RFXGEN_RANDF(0.2f);
    }

    params.startFrequencyValue *= params.startFrequencyValue;

    if (RFXGEN_RAND(0, 4) == 0) params.slideValue = 0.0f;
    if (RFXGEN_RAND(0, 2) == 0) params.repeatSpeedValue = 0.3f + RFXGEN_RANDF(0.5f);

    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = 0.1f + RFXGEN_RANDF(0.3f);
    params.decayTimeValue = RFXGEN_RANDF(0.5f);

    if (RFXGEN_RAND01 == 0)
    {
        params.phaserOffsetValue = -0.3f + RFXGEN_RANDF(0.9f);
        params.phaserSweepValue = -RFXGEN_RANDF(0.3f);
    }

    params.sustainPunchValue = 0.2f + RFXGEN_RANDF(0.6f);

    if (RFXGEN_RAND01)
    {
        params.vibratoDepthValue = RFXGEN_RANDF(0.7f);
        params.vibratoSpeedValue = RFXGEN_RANDF(0.6f);
    }

    if (RFXGEN_RAND(0, 2) == 0)
    {
        params.changeSpeedValue = 0.6f + RFXGEN_RANDF(0.3f);
        params.changeAmountValue = 0.8f - RFXGEN_RANDF(1.6f);
    }

    return params;
}

// Generate sound: Powerup
RLAPI WaveParams GenPowerup(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    if (RFXGEN_RAND01) params.waveTypeValue = 1;
    else params.squareDutyValue = RFXGEN_RANDF(0.6f);

    if (RFXGEN_RAND01)
    {
        params.startFrequencyValue = 0.2f + RFXGEN_RANDF(0.3f);
        params.slideValue = 0.1f + RFXGEN_RANDF(0.4f);
        params.repeatSpeedValue = 0.4f + RFXGEN_RANDF(0.4f);
    }
    else
    {
        params.startFrequencyValue = 0.2f + RFXGEN_RANDF(0.3f);
        params.slideValue = 0.05f + RFXGEN_RANDF(0.2f);

        if (RFXGEN_RAND01)
        {
            params.vibratoDepthValue = RFXGEN_RANDF(0.7f);
            params.vibratoSpeedValue = RFXGEN_RANDF(0.6f);
        }
    }

    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = RFXGEN_RANDF(0.4f);
    params.decayTimeValue = 0.1f + RFXGEN_RANDF(0.4f);

    return params;
}

// Generate sound: Hit/Hurt
RLAPI WaveParams GenHitHurt(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.waveTypeValue = RFXGEN_RAND(0, 2);
    if (params.waveTypeValue == 2) params.waveTypeValue = 3;
    if (params.waveTypeValue == 0) params.squareDutyValue = RFXGEN_RANDF(0.6f);

    params.startFrequencyValue = 0.2f + RFXGEN_RANDF(0.6f);
    params.slideValue = -0.3f - RFXGEN_RANDF(0.4f);
    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = RFXGEN_RANDF(0.1f);
    params.decayTimeValue = 0.1f + RFXGEN_RANDF(0.2f);

    if (RFXGEN_RAND01) params.hpfCutoffValue = RFXGEN_RANDF(0.3f);

    return params;
}

// Generate sound: Jump
RLAPI WaveParams GenJump(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.waveTypeValue = 0;
    params.squareDutyValue = RFXGEN_RANDF(0.6f);
    params.startFrequencyValue = 0.3f + RFXGEN_RANDF(0.3f);
    params.slideValue = 0.1f + RFXGEN_RANDF(0.2f);
    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = 0.1f + RFXGEN_RANDF(0.3f);
    params.decayTimeValue = 0.1f + RFXGEN_RANDF(0.2f);

    if (RFXGEN_RAND01) params.hpfCutoffValue = RFXGEN_RANDF(0.3f);
    if (RFXGEN_RAND01) params.lpfCutoffValue = 1.0f - RFXGEN_RANDF(0.6f);

    return params;
}

// Generate sound: Blip/Select
RLAPI WaveParams GenBlipSelect(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.waveTypeValue = RFXGEN_RAND01;
    if (params.waveTypeValue == 0) params.squareDutyValue = RFXGEN_RANDF(0.6f);
    params.startFrequencyValue = 0.2f + RFXGEN_RANDF(0.4f);
    params.attackTimeValue = 0.0f;
    params.sustainTimeValue = 0.1f + RFXGEN_RANDF(0.1f);
    params.decayTimeValue = RFXGEN_RANDF(0.2f);
    params.hpfCutoffValue = 0.1f;

    return params;
}

// Generate random sound
RLAPI WaveParams GenRandomize(void)
{
    WaveParams params = { 0 };
    ResetWaveParams(&params);

    params.randSeed = RFXGEN_RAND(0, 0xFFFE);

    params.startFrequencyValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 2.0f);

    if (RFXGEN_RAND01) params.startFrequencyValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 3.0f)+0.5f;

    params.minFrequencyValue = 0.0f;
    params.slideValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 5.0f);

    if ((params.startFrequencyValue > 0.7f) && (params.slideValue > 0.2f)) params.slideValue = -params.slideValue;
    if ((params.startFrequencyValue < 0.2f) && (params.slideValue < -0.05f)) params.slideValue = -params.slideValue;

    params.deltaSlideValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 3.0f);
    params.squareDutyValue = RFXGEN_RANDF(2.0f) - 1.0f;
    params.dutySweepValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 3.0f);
    params.vibratoDepthValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 3.0f);
    params.vibratoSpeedValue = RFXGEN_RANDF(2.0f) - 1.0f;
    //params.vibratoPhaseDelay = RFXGEN_RANDF(2.0f) - 1.0f;
    params.attackTimeValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 3.0f);
    params.sustainTimeValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 2.0f);
    params.decayTimeValue = RFXGEN_RANDF(2.0f)-1.0f;
    params.sustainPunchValue = powf(RFXGEN_RANDF(0.8f), 2.0f);

    if (params.attackTimeValue + params.sustainTimeValue + params.decayTimeValue < 0.2f)
    {
        params.sustainTimeValue += 0.2f + RFXGEN_RANDF(0.3f);
        params.decayTimeValue += 0.2f + RFXGEN_RANDF(0.3f);
    }

    params.lpfResonanceValue = RFXGEN_RANDF(2.0f) - 1.0f;
    params.lpfCutoffValue = 1.0f - powf(RFXGEN_RANDF(1.0f), 3.0f);
    params.lpfCutoffSweepValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 3.0f);

    if (params.lpfCutoffValue < 0.1f && params.lpfCutoffSweepValue < -0.05f) params.lpfCutoffSweepValue = -params.lpfCutoffSweepValue;

    params.hpfCutoffValue = powf(RFXGEN_RANDF(1.0f), 5.0f);
    params.hpfCutoffSweepValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 5.0f);
    params.phaserOffsetValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 3.0f);
    params.phaserSweepValue = powf(RFXGEN_RANDF(2.0f) - 1.0f, 3.0f);
    params.repeatSpeedValue = RFXGEN_RANDF(2.0f) - 1.0f;
    params.changeSpeedValue = RFXGEN_RANDF(2.0f) - 1.0f;
    params.changeAmountValue = RFXGEN_RANDF(2.0f) - 1.0f;

    return params;
}

// Mutate current sound
RLAPI void WaveMutate(WaveParams *params)
{
    RFXGEN_SRAND(time(NULL));       // Refresh seed to avoid converging behaviour
    
    if (RFXGEN_RAND01) params->startFrequencyValue += RFXGEN_RANDF(0.1f) - 0.05f;        
    //if (RFXGEN_RAND01) params.minFrequencyValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->slideValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->deltaSlideValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->squareDutyValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->dutySweepValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->vibratoDepthValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->vibratoSpeedValue += RFXGEN_RANDF(0.1f) - 0.05f;
    //if (RFXGEN_RAND01) params.vibratoPhaseDelay += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->attackTimeValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->sustainTimeValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->decayTimeValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->sustainPunchValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->lpfResonanceValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->lpfCutoffValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->lpfCutoffSweepValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->hpfCutoffValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->hpfCutoffSweepValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->phaserOffsetValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->phaserSweepValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->repeatSpeedValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->changeSpeedValue += RFXGEN_RANDF(0.1f) - 0.05f;
    if (RFXGEN_RAND01) params->changeAmountValue += RFXGEN_RANDF(0.1f) - 0.05f;
}

#endif      // RFXGEN_IMPLEMENTATION
