#include "Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);
float4 main(PS_INPUT input) : SV_TARGET
{
	//	指定された画像から情報を取得
    float4 output = tex.Sample(samLinear, input.Tex);
   // float3 rainbowColor = GetRainbow(time.x);
    output *= input.Color;
	//	C++から指定された色割合を考慮して、表示色を決定
    
  
   
    // アルファ値が0の場合はピクセルを非表示にする
    float alphaMask = step(0.0f, output.a); // アルファ値が0なら0、それ以外は1になる
    output *= alphaMask; // アルファが0のピクセルを完全に無効化

  
    return output;
}