#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <irrKlang.h>
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

const int bpm = 140;                      // beats per minute
const double quarterNote = 60000.0 / bpm; // in ms
const double quarterNoteHz = 1000 / quarterNote;

void ControlEffects(int key, ISoundEffectControl *fx)
{
    switch (key)
    {
    case '1':
        fx->isI3DL2ReverbSoundEffectEnabled()
            ? fx->disableI3DL2ReverbSoundEffect()
            : fx->enableI3DL2ReverbSoundEffect();
        break;
    case '2':
        fx->isChorusSoundEffectEnabled()
            ? fx->disableChorusSoundEffect()
            : fx->enableChorusSoundEffect(50, 25, 25, quarterNoteHz / 2);
        break;
    case '3':
        fx->isEchoSoundEffectEnabled()
            ? fx->disableEchoSoundEffect()
            : fx->enableEchoSoundEffect(50, 30, quarterNote * 2, quarterNote);
        break;
    case '4':
        fx->isFlangerSoundEffectEnabled()
            ? fx->disableFlangerSoundEffect()
            : fx->enableFlangerSoundEffect(100, 70, 80, 2.f, true, 2, 180);
        break;
    case '5':
        fx->isCompressorSoundEffectEnabled()
            ? fx->disableCompressorSoundEffect()
            : fx->enableCompressorSoundEffect(5, 1, 200, -20, 10);
        break;
    case '6':
        fx->isGargleSoundEffectEnabled()
            ? fx->disableGargleSoundEffect()
            : fx->enableGargleSoundEffect(quarterNoteHz * 4);
        break;
    case '7':
        fx->isDistortionSoundEffectEnabled()
            ? fx->disableDistortionSoundEffect()
            : fx->enableDistortionSoundEffect();
        break;
    case '8':
        fx->isChorusSoundEffectEnabled()
            ? fx->disableChorusSoundEffect()
            : fx->enableChorusSoundEffect(50, 25, 25, quarterNoteHz * 4);
        break;
    case '0':
        fx->disableAllEffects();
        break;
    }
}

void DisplayMenu()
{
    printf("\nSound effects by 156324. Keys:\n");
    printf("\nESCAPE: quit\n");
    printf("Q/W => -1/+1 Low Eq\n");
    printf("E/R => -1/+1 Mid Eq\n");
    printf("T/Y => -1/+1 High Eq\n");
    printf("1: enable/disable reverb\n");
    printf("2: enable/disable chorus\n");
    printf("3: enable/disable echo\n");
    printf("4: enable/disable flanger\n");
    printf("5: enable/disable compressor\n");
    printf("6: enable/disable gargle\n");
    printf("7: enable/disable distortion\n");
    printf("8: enable/disable czwarta gestosc\n");
    printf("0: disable all effects\n");
}

int lowEq, midEq, highEq;
const int lowFreq = 300, midFreq = 1000, highFreq = 8000;
const int lowWidth = 36, midWidth = 24, highWidth = 36;

void ControlEq(int key, ISoundEffectControl *fx)
{
    if (key < 'a') // make key lower
        key += 'a' - 'A';

    auto between = [](int min, int v, int max) { return v < min ? min : v > max ? max : v; };
    auto change = [between](int &val, int by) { val = between(-15, val + by, 15); };

    switch (key)
    {
    case 'q':
        change(lowEq, -1);
        fx->enableParamEqSoundEffect(lowFreq, lowWidth, lowEq);
        break;
    case 'w':
        change(lowEq, +1);
        fx->enableParamEqSoundEffect(lowFreq, lowWidth, lowEq);
        break;
    case 'e':
        change(midEq, -1);
        fx->enableParamEqSoundEffect(midFreq, midWidth, midEq);
        break;
    case 'r':
        change(midEq, +1);
        fx->enableParamEqSoundEffect(midFreq, midWidth, midEq);
        break;
    case 't':
        change(highEq, -1);
        fx->enableParamEqSoundEffect(highFreq, highWidth, highEq);
        break;
    case 'y':
        change(highEq, +1);
        fx->enableParamEqSoundEffect(highFreq, highWidth, highEq);
        break;
    }
}

int main(int argc, const char **argv)
{
    // start the sound engine with default parameters
    auto engine = createIrrKlangDevice();

    if (!engine)
        return -1; // error starting up the engine

    const auto filename = "../../media/MF-W-90.XM";

    auto music = engine->play2D(filename, true, false, true, ESM_AUTO_DETECT, true);

    DisplayMenu();

    while (true) // endless loop until user exits
    {
        auto key = getch();
        if (key == 27) //esc key
            break;

        ISoundEffectControl *fx = nullptr;
        if (music)
            fx = music->getSoundEffectControl();
        if (!fx)
        {
            printf("This device or sound does not support sound effects.\n");
            continue;
        }

        ControlEffects(key, fx);
        ControlEq(key, fx);
    }

    // don't forget to release the resources
    if (music)
        music->drop(); // release music stream.
    engine->drop();    // delete Engine
    return 0;
}
