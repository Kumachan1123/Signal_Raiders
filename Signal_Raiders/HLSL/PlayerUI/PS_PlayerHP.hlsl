#include "PlayerUI.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//	wè³ê½æÌ\¦
    float4 output = tex.Sample(samLinear, input.Tex);

	// æÌ§ßxðÏX
    float smoothValue = renderRatio;
    output.a *= lerp(1.0f, 0.0f, smoothstep(smoothValue, smoothValue + 0.00000001f, input.Tex.x));
     // HPÌF
    // Î ¨ ©F (0.5f`1.0f)
    float greenToYellow = saturate((renderRatio - 0.5f) * 2.0f);

    // ©F ¨ òF (0.25f`0.5f)
    float yellowToOrange = saturate((renderRatio - 0.25f) * 4.0f) * (1.0f - greenToYellow);

    // òF ¨ ÔF (0.0f`0.25f)
    float orangeToRed = saturate(renderRatio * 4.0f) * (1.0f - yellowToOrange - greenToYellow);

    // eFð¬
    float4 HPcolor =
        greenToYellow * float4(0.0f, 1.0f, 0.0f, 1.0f) + // Î ¨ ©F
        yellowToOrange * float4(1.0f, 1.0f, 0.0f, 1.0f) + // ©F ¨ òF
        orangeToRed * float4(1.0f, 0.5f, 0.0f, 1.0f) + // òF ¨ ÔF
        (1.0f - greenToYellow - yellowToOrange - orangeToRed) * float4(1.0f, 0.0f, 0.0f, 1.0f); // Ô
    
    return output * HPcolor;
}