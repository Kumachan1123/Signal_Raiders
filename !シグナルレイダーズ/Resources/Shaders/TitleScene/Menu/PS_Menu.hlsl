#include "Menu.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//トムの画像表示
    float4 output = tex.Sample(samLinear, input.tex);
     // 時間に基づいて青系のグラデーションエフェクトを生成
    float gradient = exp(-abs(sin(time * 1.0 + input.tex.y * 5.0)));
   
	 
// グラデーションエフェクトを加える
    // エフェクトの色は青系
    output.rgba += gradient * float4(0.5, 0.5, 0.5, 0.0);
    
    // 結果を返す    
    return output;
}