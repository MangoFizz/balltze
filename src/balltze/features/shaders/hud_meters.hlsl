// SPDX-License-Identifier: GPL-3.0-only

sampler2D sampler_2d : register(s0);
float4 tint_color : register(c0);
float4 empty_color	: register(c1);
float4 background_color	: register(c2);

float4 main(float2 texCoord : TEXCOORD0) : COLOR0 {
	float4 tex = tex2D(sampler_2d, texCoord);

	// clip by non interpolated alpha
	clip(dot(tex.rgb, 1) - 0.001);

	float4 r0;
	if(tex.a <= tint_color.a) {
		r0.rgb = tint_color.rgb;
		r0.a = (1.0 - background_color.y);
	}
	else {
		r0.rgb = empty_color.rgb;
		r0.a = (1.0 - background_color.y) * empty_color.a / 2;
	}

    float4 res;
    res.rgb = tex.rgb * r0.rgb * (1.0 - background_color.x);
    res.a = r0.a;

	return res;
};
