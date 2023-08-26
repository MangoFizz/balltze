// SPDX-License-Identifier: GPL-3.0-only

/*
 * Loading screen shader
 * Original GLSL shader from SnowyMouse:
 * https://gist.github.com/SnowyMouse/eb7558d2e036e4becbc764501d99a703
*/

sampler2D iChannel0 : register(s0);
float2 iResolution : register(c0);
float iDuration : register(c1);
float iTimeElapsed : register(c2);
float iOpacity : register(c3);

// Width of the sweeper
#define WIDTH 0.09

// Distance from the sweeper we can be to be slightly lit up
#define EXTRA_LUMINOSITY_MAX_DISTANCE 0.2
#define EXTRA_LUMINOSITY_INTENSITY 0.05

// This determines how far to make the beams of light (zoom blur)
#define FOCUS_DETAIL int(8.0 * (float(iResolution.x) / 640.0))
#define FOCUS_POWER 11.0
#define FOCUS_MULTIPLIER 2

float mod(float x, float y) {
  return x - y * floor(x/y);
}

float4 PixelForUV(float2 uv) {
    // Get the actual pixel intensity (just use the red channel since it's supposed to be monochrome)
    float value = tex2Dgrad(iChannel0, uv, 0, 0).r;
    float4 colorOutput = float4(0.2 * value, 0.4 * value, 1.0 * value, 1.0);
    
    // If we're on the sweeper, it's lit up
    float progress = mod(iTimeElapsed, iDuration * 1.5) / iDuration - 0.25;
    float x = uv.x;
    float d = abs(x - progress);
    float a = clamp(WIDTH - d + value * WIDTH * 0.5, 0.0, WIDTH) / WIDTH * 7.0;
    
    // If we're close to the sweeper, light it up slightly even if we're not directly sweeping on it
    float extra_luminosity = (1.0 - clamp(d - (EXTRA_LUMINOSITY_MAX_DISTANCE + WIDTH / 2.0), 0.0, EXTRA_LUMINOSITY_MAX_DISTANCE) / EXTRA_LUMINOSITY_MAX_DISTANCE) * EXTRA_LUMINOSITY_INTENSITY;
    a = a * (1.00 - extra_luminosity) + extra_luminosity;
    
    return saturate(colorOutput * a);
}

float4 LoadScreenPS(float2 PixelPos : VPOS) : COLOR0 {
    // Get our UV as well as the center of the blur
    float2 uv = PixelPos.xy / iResolution.xy;
    float2 center = float2(0.5, 0.58);
    float2 focus = uv - center;

    // Initialize our output color
    float4 outColor = float4(0.0, 0.0, 0.0, 1.0);

    // Do a modified zoom blur
    for (int i = 0; i < FOCUS_DETAIL * FOCUS_MULTIPLIER; i++) {
        float power = 1.0 - FOCUS_POWER * (1.0 / iResolution.x) * float(i);
        float4 addColor = PixelForUV(focus * power + center);

        float reduction = clamp(float(i) / float(FOCUS_DETAIL) - 1.0, 0.0, float(FOCUS_MULTIPLIER)) / float(FOCUS_MULTIPLIER);
        outColor.rgb += clamp(addColor.rgb, 0.0, 1.0 - reduction * 2.0);
    }
    
    outColor.rgb = saturate(outColor.rgb / float(FOCUS_DETAIL));
    outColor.a = iOpacity;

    return outColor;
}
