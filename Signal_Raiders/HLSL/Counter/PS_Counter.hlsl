#include "Counter.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.Tex;
    float w = width.x;
    float h = height.x;
    uv.x /= w;
    uv.y /= h;
    float count2 = floor(count.x);
    
    uv.x += (1.0f / w) * (count2 % w);
    uv.y += (1.0f / h) * (int) (count2 / w);
    
    return tex.Sample(samLinear, uv);
}
