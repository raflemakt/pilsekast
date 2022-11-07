#include "Arduino.h"
#include "NeoPixelBus.h"
#include <NeoPixelAnimator.h>
#include "LedStripCustom.h"
#include "lightController.h"

const uint16_t PixelCount = 22;      // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 4;         // make sure to set this to the correct pin, ignored for Esp8266
const uint8_t AnimationChannels = 1; // we only need one as all the pixels are animated at once
int flag2 = 1;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip2(PixelCount, PixelPin);
NeoPixelAnimator animationsSimple(AnimationChannels); // NeoPixel animation management object

struct MyAnimationState
{
    RgbColor StartingColor;
    RgbColor EndingColor;
};

MyAnimationState animationSimpleState[AnimationChannels];

void BlendingAnimationUpdate(const AnimationParam &param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationSimpleState[param.index].StartingColor,
        animationSimpleState[param.index].EndingColor,
        param.progress);

    // apply the color to the strip
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        strip2.SetPixelColor(pixel, updatedColor);
    }
}

void TurnOnSimpleStrip(float luminance, float Hue)
{
    RgbColor target = HslColor(Hue / 360.0f, 1.0f, luminance);
    uint16_t time = 10;

    animationSimpleState[0].StartingColor = strip2.GetPixelColor(0);
    animationSimpleState[0].EndingColor = target;

    animationsSimple.StartAnimation(0, time, BlendingAnimationUpdate);
    strip2.Show();
    flag2 = 1;
    Chosen_Option = 3;
}

void TurnOffSimpleStrip(float luminance)
{
    // fade to black
    uint16_t time = 10;

    animationSimpleState[0].StartingColor = strip2.GetPixelColor(0);
    animationSimpleState[0].EndingColor = RgbColor(0);

    animationsSimple.StartAnimation(0, time, BlendingAnimationUpdate);
    strip2.Show();
    flag2 = 0;
}

void TurnOffSimpleMasterStrip()
{
    for (byte Led = 0; Led <= PixelCount; Led++)
    {
        RgbColor black(0);
        strip2.SetPixelColor(Led, black);
    }
    strip2.Show();
}

void LedStripSimple_setup()
{
    strip2.Begin();
    strip2.Show();
    TurnOffSimpleMasterStrip();
}

void LedStripSimpleUpdate(float light)
{
    // the normal loop just needs these two to run the active animations
    if (animationsSimple.IsAnimating())
    {
        animationsSimple.UpdateAnimations();
        strip2.Show();
    }

    else
    {

        if (flag2 == 1)
        {
            TurnOffSimpleStrip(light);
        }
    }
}

void LedStripSimple_loop()
{

    LedStripSimpleUpdate(0.25);
}