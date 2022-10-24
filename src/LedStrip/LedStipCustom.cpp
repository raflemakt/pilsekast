#include "Arduino.h"
#include "NeoPixelBus.h"
#include <NeoPixelAnimator.h>
#include <ezButton.h>

const uint16_t PixelCount = 22;      // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 4;         // make sure to set this to the correct pin, ignored for Esp8266
const uint8_t AnimationChannels = 1; // we only need one as all the pixels are animated at once
boolean fading = true;               // general purpose variable used to store effect state
unsigned long startingMillis;        // some global variables available anywhere in the program
float longevity = 700;
float intensity = 0.25;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip1(PixelCount, PixelPin);
NeoPixelAnimator animations(AnimationChannels); // NeoPixel animation management object
ezButton button(21);

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

void TurnOnOffStrip(float luminance, float longevity)
{

    if (fading)
    {
        // Fade upto a random color
        // we use HslColor object as it allows us to easily pick a hue
        // with the same saturation and luminance so the colors picked
        // will have similiar overall brightness
        RgbColor target = HslColor(120 / 360.0f, 1.0f, luminance);
        uint16_t time = longevity;

        animationState[0].StartingColor = strip1.GetPixelColor(0);
        animationState[0].EndingColor = target;

        animations.StartAnimation(0, time, BlendAnimationUpdate);
    }
    else
    {
        // fade to black
        uint16_t time = longevity;

        animationState[0].StartingColor = strip1.GetPixelColor(0);
        animationState[0].EndingColor = RgbColor(0);

        animations.StartAnimation(0, time, BlendAnimationUpdate);
    }

    // toggle to the next effect state
    fading = !fading;
}

void LedStripCustom_setup()
{
    strip1.Begin();
    strip1.Show();
    Serial.begin(9800);
    button.setDebounceTime(50);
    for (byte Led = 0; Led <= PixelCount; Led++)
    {
        RgbColor black(0);
        strip1.SetPixelColor(Led, black);
    }
    strip1.Show();
}

void LedStripCustom_loop()
{
    button.loop(); // MUST call the loop() function first
    unsigned long currentlyMillis = millis();
    while (millis() - currentlyMillis <= longevity)
    {
        Serial.println("The button is pressed");
        if (animations.IsAnimating())
        {
            // the normal loop just needs these two to run the active animations
            animations.UpdateAnimations();
            strip1.Show();
        }
        else
        {
            // no animation runnning, start some
            //
            TurnOnOffStrip(intensity, longevity); // 0.0 = black, 0.25 is normal, 0.5 is bright
        }
        strip1.Show();
    }
}