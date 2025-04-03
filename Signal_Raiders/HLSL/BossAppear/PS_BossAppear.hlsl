#include "Header.hlsli"

// C++側から設定されるデータ
Texture2D tex : register(t0); // テクスチャ
SamplerState samLinear : register(s0); // サンプラーステート

// ピクセルシェーダーのメイン関数
float4 main(PS_INPUT input) : SV_TARGET
{
   // UV座標を取得
    float2 uv = input.Tex;
    float scrollSpeed = 0.5; // スクロール速度

    // UVのY座標に応じたスクロール方向を決定（if なし）
    float upMask = step(2.0 / 3.0, uv.y); // 上エリアなら 1.0、他は 0.0
    float downMask = step(uv.y, 1.0 / 3.0); // 下エリアなら 1.0、他は 0.0

    // スクロール方向を決定（上は +1.0、中央は 0.0、下は -1.0）
    float scrollDir = upMask - downMask;
  //  float scrollDir = sign(uv.y - (2.0 / 3.0)) - sign(uv.y - (1.0 / 3.0));
    // UVのX座標をスクロール（中間部分は動かない）
    uv.x += frac(time.x * scrollSpeed) * scrollDir;


    // 2秒後からフェードアウト開始（5秒で完全に消える）
    float fadeStartTime = 2.0; // フェード開始時間
    float fadeDuration = 0.30; // フェードにかかる時間
    float alpha = saturate(1.0 - max(0.0, (time.x - fadeStartTime) / fadeDuration));

    // テクスチャをサンプリングしてアルファ値を適用
    float4 color = tex.Sample(samLinear, uv);
    color.a *= alpha; // 徐々に透明にする

    return color;
}
