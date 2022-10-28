#include "Arduino.h"
#include "NeoPixelBus.h"
#include <NeoPixelAnimator.h>
#include "LedStripCustom.h"

const uint16_t PixelCount = 22;          // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 4;             // make sure to set this to the correct pin, ignored for Esp8266
const uint8_t AnimationChannels = 1;     // we only need one as all the pixels are animated at once
unsigned long startingMillis = millis(); // some global variables available anywhere in the program
float intensity = 0.25;
int flag1=1;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip1(PixelCount, PixelPin);
NeoPixelAnimator animations(AnimationChannels); // NeoPixel animation management object

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



void TurnOnStrip(float luminance, float longevity)
{
    RgbColor target = HslColor(120 / 360.0f, 1.0f, luminance);
    uint16_t time = longevity;

    animationState[0].StartingColor = strip1.GetPixelColor(0);
    animationState[0].EndingColor = target;

    animations.StartAnimation(0, time, BlendAnimationUpdate);
    strip1.Show();
    flag1=1;
}

void TurnOffStrip(float luminance, float longevity)
{
    // fade to black
    uint16_t time = longevity;

    animationState[0].StartingColor = strip1.GetPixelColor(0);
    animationState[0].EndingColor = RgbColor(0);

    animations.StartAnimation(0, time, BlendAnimationUpdate);
    strip1.Show();
    flag1=0;
}

void TurnOffMasterStrip()
{
    for (byte Led = 0; Led <= PixelCount; Led++)
    {
        RgbColor black(0);
        strip1.SetPixelColor(Led, black);
    }
    strip1.Show();
}

void LedStripCustom_setup()
{
    strip1.Begin();
    strip1.Show();
    TurnOffMasterStrip();
}

void LedStripCustomUpdate(float light,float time)
{
    // the normal loop just needs these two to run the active animations
    if (animations.IsAnimating())
    {
    animations.UpdateAnimations();
    strip1.Show();
    }

    else
    {
    if(flag1 == 1)
    {
    TurnOffStrip(light,time);
    }
    }
}

void LedStripCustom_loop()
{
    
LedStripCustomUpdate(0.25,500);

}