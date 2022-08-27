#version 330

precision highp float;
precision highp int;
precision highp sampler2D;
precision highp samplerCube;
precision highp isampler2D;
precision highp sampler2DArray;

out vec4 color;
in vec2 TexCoords;

uniform sampler2D pathTraceTexture;
uniform float invSampleCounter;

vec4 ToneMap(in vec4 c, float limit)
{
	float luminance = 0.3*c.x + 0.6*c.y + 0.1*c.z;

	return c * 1.0 / (1.0 + luminance / limit);
}

vec4 Tonemap(in vec4 c, float limit)
{
    float luminance = 0.3 * c.x + 0.6 * c.y + 0.1 * c.z;

    return c * 1.0 / (1.0 + luminance / limit);
}

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
mat3 ACESInputMat = mat3
(
    vec3(0.59719, 0.35458, 0.04823),
    vec3(0.07600, 0.90834, 0.01566),
    vec3(0.02840, 0.13383, 0.83777)
);

// ODT_SAT => XYZ => D60_2_D65 => sRGB
mat3 ACESOutputMat = mat3
(
    vec3(1.60475, -0.53108, -0.07367),
    vec3(-0.10208, 1.10813, -0.00605),
    vec3(-0.00327, -0.07276, 1.07602)
);

vec3 RRTAndODTFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

vec3 ACESFitted(vec3 color)
{
    color = color * ACESInputMat;

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = color * ACESOutputMat;

    // Clamp to [0, 1]
    color = clamp(color, 0.0, 1.0);

    return color;
}

vec3 ACES(in vec3 c)
{
    float a = 2.51f;
    float b = 0.03f;
    float y = 2.43f;
    float d = 0.59f;
    float e = 0.14f;

    return clamp((c * (a * c + b)) / (c * (y * c + d) + e), 0.0, 1.0);
}

void main()
{
    bool enableTonemap = true;
    bool enableAces = true;
    bool simpleAcesFit = true;
	vec4 col = texture(pathTraceTexture, TexCoords) * invSampleCounter;
	//color = pow(ToneMap(color, 1.5), vec4(1.0 / 2.2));
    vec3 clr = col.rgb;
    float alpha = col.a;
    if (enableTonemap)
    {
        if (enableAces)
        {
            if (simpleAcesFit)
                clr = ACES(clr);
            else
                clr = ACESFitted(clr);
        }
        else
            clr = ToneMap(vec4(clr, 1.0f), 1.5).rgb;
    }
    clr = pow(clr, vec3(1.0 / 2.2));
    color = vec4(clr, 1.0);
}
