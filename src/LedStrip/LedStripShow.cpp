#include "Arduino.h"
#include "NeoPixelBus.h"

#include "LedStrip/LedStripShow.h"

#include <NeoPixelAnimator.h>
const uint16_t PixelCount = 22; // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 2;    // make sure to set this to the correct pin, ignored for Esp8266
const RgbColor CylonEyeColor(HtmlColor(0x7f0000));
byte ColorR;
byte ColorG;
byte ColorB;
float Intensity;
float duration;
bool animationMode = false;
int EnvAttack;
int EnvDecay;
int EnvSustain;
int EnvRelease;
int period = 20000;
bool flag = true;
unsigned long startMillis; // some global variables available anywhere in the program
unsigned long currentMillis;

// include voids from other default animations in NeoPixelLibrary

void write(byte colorR, byte colorG, byte colorB, float intensity, float duration, byte animationMode, int EnvAttack, int EnvDecay, int EnvSustain, int EnvRelease)
{
}

// Loop parameters
const uint16_t AnimCount = PixelCount / 5 * 2 + 1; // we only need enough animations for the tail and one extra

const uint16_t PixelFadeDuration = 300; // third of a second
// one second divide by the number of pixels = loop once a second
const uint16_t NextPixelMoveDuration = 1000 / PixelCount; // how fast we move through the pixels
NeoGamma<NeoGammaTableMethod> colorGamma;                 // for any fade animations, best to correct gamma

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
struct MyAnimation1State

{
    RgbColor StartingColor1;
    RgbColor EndingColor1;
    uint16_t IndexPixel; // which pixel this animation is effecting
};

NeoPixelAnimator animations1(AnimCount); // NeoPixel animation management object
MyAnimation1State animation1State[AnimCount];
uint16_t frontPixel = 0; // the front of the loop
RgbColor frontColor;     // the color at the front of the loop

void SetRandomSeed()
{
    uint32_t seed;

    // random works best with a seed that can use 31 bits
    // analogRead on a unconnected pin tends toward less than four bits
    seed = analogRead(0);
    delay(1);

    for (int shifts = 3; shifts < 31; shifts += 3)
    {
        seed ^= analogRead(0) << shifts;
        delay(1);
    }

    // Serial.println(seed);
    randomSeed(seed);
}

void FadeOutAnimUpdate(const AnimationParam &param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animation1State[param.index].StartingColor1,
        animation1State[param.index].EndingColor1,
        param.progress);
    // apply the color to the strip
    strip.SetPixelColor(animation1State[param.index].IndexPixel,
                        colorGamma.Correct(updatedColor));
}

void LoopAnimUpdate(const AnimationParam &param)
{
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        animations1.RestartAnimation(param.index);

        // pick the next pixel inline to start animating
        //
        frontPixel = (frontPixel + 1) % PixelCount; // increment and wrap
        if (frontPixel == 0)
        {
            // we looped, lets pick a new front color
            frontColor = HslColor(random(360) / 360.0f, 1.0f, 0.25f);
        }

        uint16_t indexAnim;
        // do we have an animation available to use to animate the next front pixel?
        // if you see skipping, then either you are going to fast or need to increase
        // the number of animation channels
        if (animations1.NextAvailableAnimation(&indexAnim, 1))
        {
            animation1State[indexAnim].StartingColor1 = frontColor;
            animation1State[indexAnim].EndingColor1 = RgbColor(0, 0, 0);
            animation1State[indexAnim].IndexPixel = frontPixel;

            animations1.StartAnimation(indexAnim, PixelFadeDuration, FadeOutAnimUpdate);
        }
    }
}
// Loop end

// FadeInOut
const uint8_t AnimationChannels = 1; // we only need one as all the pixels are animated at once

// For Esp8266, the Pin is omitted and it uses GPIO3 due to DMA hardware use.
// There are other Esp8266 alternative methods that provide more pin options, but also have
// other side effects.
// for details see wiki linked here https://github.com/Makuna/NeoPixelBus/wiki/ESP8266-NeoMethods

NeoPixelAnimator animations2(AnimationChannels); // NeoPixel animation management object

boolean fadeToColor = true; // general purpose variable used to store effect state

// what is stored for state is specific to the need, in this case, the colors.
// basically what ever you need inside the animation update function
struct MyAnimation2State
{
    RgbColor StartingColor2;
    RgbColor EndingColor2;
};

// one entry per pixel to match the animation timing manager
MyAnimation2State animation2State[AnimationChannels];

// simple blend function
void BlendAnimUpdate(const AnimationParam &param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animation2State[param.index].StartingColor2,
        animation2State[param.index].EndingColor2,
        param.progress);

    // apply the color to the strip
    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
    {
        strip.SetPixelColor(pixel, updatedColor);
    }
}

void FadeInFadeOutRinseRepeat(float luminance)
{
    if (fadeToColor)
    {
        // Fade upto a random color
        // we use HslColor object as it allows us to easily pick a hue
        // with the same saturation and luminance so the colors picked
        // will have similiar overall brightness
        RgbColor target = HslColor(random(360) / 360.0f, 1.0f, luminance);
        uint16_t time = random(800, 2000);

        animation2State[0].StartingColor2 = strip.GetPixelColor(0);
        animation2State[0].EndingColor2 = target;

        animations2.StartAnimation(0, time, BlendAnimUpdate);
    }
    else
    {
        // fade to black
        uint16_t time = random(600, 700);

        animation2State[0].StartingColor2 = strip.GetPixelColor(0);
        animation2State[0].EndingColor2 = RgbColor(0);

        animations2.StartAnimation(0, time, BlendAnimUpdate);
    }

    // toggle to the next effect state
    fadeToColor = !fadeToColor;
}

// Cyclon

NeoPixelAnimator animations3(2); // only ever need 2 animations

uint16_t lastPixel = 0; // track the eye position
int8_t moveDir = 1;     // track the direction of movement

// uncomment one of the lines below to see the effects of
// changing the ease function on the movement animation
AnimEaseFunction moveEase =
    //      NeoEase::Linear;
    //      NeoEase::QuadraticInOut;
    //      NeoEase::CubicInOut;
    NeoEase::QuarticInOut;
//      NeoEase::QuinticInOut;
//      NeoEase::SinusoidalInOut;
//      NeoEase::ExponentialInOut;
//      NeoEase::CircularInOut;

void FadeAll(uint8_t darkenBy)
{
    RgbColor color;
    for (uint16_t indexPixel = 0; indexPixel < strip.PixelCount(); indexPixel++)
    {
        color = strip.GetPixelColor(indexPixel);
        color.Darken(darkenBy);
        strip.SetPixelColor(indexPixel, color);
    }
}

void FadeAnimUpdate(const AnimationParam &param)
{
    if (param.state == AnimationState_Completed)
    {
        FadeAll(10);
        animations3.RestartAnimation(param.index);
    }
}

void MoveAnimUpdate(const AnimationParam &param)
{
    // apply the movement animation curve
    float progress = moveEase(param.progress);

    // use the curved progress to calculate the pixel to effect
    uint16_t nextPixel;
    if (moveDir > 0)
    {
        nextPixel = progress * PixelCount;
    }
    else
    {
        nextPixel = (1.0f - progress) * PixelCount;
    }

    // if progress moves fast enough, we may move more than
    // one pixel, so we update all between the calculated and
    // the last
    if (lastPixel != nextPixel)
    {
        for (uint16_t i = lastPixel + moveDir; i != nextPixel; i += moveDir)
        {
            strip.SetPixelColor(i, CylonEyeColor);
        }
    }
    strip.SetPixelColor(nextPixel, CylonEyeColor);

    lastPixel = nextPixel;

    if (param.state == AnimationState_Completed)
    {
        // reverse direction of movement
        moveDir *= -1;

        // done, time to restart this position tracking animation/timer
        animations3.RestartAnimation(param.index);
    }
}

void SetupAnimations()
{
    // fade all pixels providing a tail that is longer the faster
    // the pixel moves.
    animations3.StartAnimation(0, 5, FadeAnimUpdate);

    // take several seconds to move eye fron one side to the other
    animations3.StartAnimation(1, 2000, MoveAnimUpdate);
}

void UserChosenOption()
{
}

void LedStrip_setup()
{
    // put your setup code here, to run once:

    strip.Begin();
    strip.Show();

    SetRandomSeed();
    startMillis = millis(); // initial start time
    Serial.begin(9800);
}

void LedStrip_loop()
{
    // put your main code here, to run repeatedly:
    {
        currentMillis = millis();                  // get the current "time" (actually the number of milliseconds since the program started)
        if (currentMillis - startMillis >= period) // test whether the period has elapsed
        {
            animationMode=!animationMode;
            for (byte Led = 0; Led <= PixelCount; Led++)
            {
                RgbColor black(0);
                strip.SetPixelColor(Led, black);
            }
            Serial.println("Lights off");
            strip.Show();

            startMillis = currentMillis; // IMPORTANT to save the start time of the current LED state.
        }
    }
    switch (animationMode)
    {

    case false:

        if (flag == true)
        {
            animations1.StartAnimation(0, NextPixelMoveDuration, LoopAnimUpdate);
            flag = false;
            Serial.println("Animation1 start");
        }

        animations1.UpdateAnimations();
        strip.Show();

        break;

    case true:
        if (flag == false)
        {
            flag = true;
            Serial.println("Animation2Start");
        }

        if (animations2.IsAnimating())
        {
            // the normal loop just needs these two to run the active animations
            animations2.UpdateAnimations();
            strip.Show();
        }
        else
        {
            // no animation runnning, start some
            //
            FadeInFadeOutRinseRepeat(0.2f); // 0.0 = black, 0.25 is normal, 0.5 is bright
        }

        break;
    }
}
