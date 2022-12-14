#include "Arduino.h"
#include "NeoPixelBus.h"
#include <NeoPixelAnimator.h>
#include "LedStripCustom.h"
#include "lightController.h"
#include "configuration.h"

const uint16_t PixelCount = LED_STRIP_LED_AMOUNT; // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 4;                      // make sure to set this to the correct pin, ignored for Esp8266
const uint8_t AnimationChannels = 1;              // we only need one as all the pixels are animated at once
int flag1 = 1;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip1(PixelCount, PixelPin); // method of writing to strip
NeoPixelAnimator animations(AnimationChannels);                            // NeoPixel animation management object

struct MyAnimationState
{
    RgbColor StartingColor;
    RgbColor EndingColor;
};

MyAnimationState animationState[AnimationChannels];

void BlendAnimationUpdate(const AnimationParam &param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);

    // apply the color to the strip
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        strip1.SetPixelColor(pixel, updatedColor);
    }
}

// fade to chosen intensity
// Chosen_option is used in main case in lightcontroller
void TurnOnStrip(float luminance, float longevity)
{
    RgbColor target = HslColor(120 / 360.0f, 1.0f, luminance);
    uint16_t time = longevity;

    animationState[0].StartingColor = strip1.GetPixelColor(0);
    animationState[0].EndingColor = target;

    animations.StartAnimation(0, time, BlendAnimationUpdate);
    strip1.Show();
    flag1 = 1;
    Chosen_Option = 2;
}

void TurnOffStrip(float luminance, float longevity)
{
    // fade to black
    uint16_t time = longevity;

    animationState[0].StartingColor = strip1.GetPixelColor(0);
    animationState[0].EndingColor = RgbColor(0);

    animations.StartAnimation(0, time, BlendAnimationUpdate);
    strip1.Show();
    flag1 = 0;
}

// Function to turn off all lights
void TurnOffMasterStrip()
{
    for (byte Led = 0; Led <= PixelCount; Led++)
    {
        RgbColor black(0);
        strip1.SetPixelColor(Led, black);
    }
    strip1.Show();
}

// universal setup for every function using LED-strip turning of and readying
void LedStripCustom_setup()
{
    strip1.Begin();
    strip1.Show();
    TurnOffMasterStrip();
}

// Function to run in lightController loop
void LedStripCustomUpdate(float light, float time)
{
    // the normal loop just needs these two to run the active animations
    if (animations.IsAnimating())
    {
        animations.UpdateAnimations();
        strip1.Show();
    }

    else
    {

        if (flag1 == 1)
        {
            TurnOffStrip(light, time);
        }
    }
}

void LedStripCustom_loop()
{

    LedStripCustomUpdate(0.25, 500);
}