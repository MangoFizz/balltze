// SPDX-License-Identifier: GPL-3.0-only

sampler2D iChannel0 : register(s0);
float4 iColorMask : register(c0);

float4 SpriteColorMaskPS(float2 TextCoord : TEXCOORD0) : COLOR0 {
    return tex2D(iChannel0, TextCoord.xy) * iColorMask;
}
