#include "EnemyHP.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // wčłę˝ćĚ\Ś
    float4 output = tex.Sample(samLinear, input.Tex);

    // ĄĚHP
    float nowHP = HP.x;
    // ĹĺHP
    float maxHP = HP.y;
    // HPĚ
    float hpRatio = nowHP / maxHP;

    // HPĚF
    // Î ¨ ŠF (0.5f`1.0f)
    float greenToYellow = saturate((hpRatio - 0.5f) * 2.0f);

    // ŠF ¨ ňF (0.25f`0.5f)
    float yellowToOrange = saturate((hpRatio - 0.25f) * 4.0f) * (1.0f - greenToYellow);

    // ňF ¨ ÔF (0.0f`0.25f)
    float orangeToRed = saturate(hpRatio * 4.0f) * (1.0f - yellowToOrange - greenToYellow);

    // eFđŹ
    float4 HPcolor =
        greenToYellow * float4(0.0f, 1.0f, 0.0f, 1.0f) + // Î ¨ ŠF
        yellowToOrange * float4(1.0f, 1.0f, 0.0f, 1.0f) + // ŠF ¨ ňF
        orangeToRed * float4(1.0f, 0.5f, 0.0f, 1.0f) + // ňF ¨ ÔF
        (1.0f - greenToYellow - yellowToOrange - orangeToRed) * float4(1.0f, 0.0f, 0.0f, 1.0f); // Ô

    // oÍĚF
    return output * HPcolor * color;
}
