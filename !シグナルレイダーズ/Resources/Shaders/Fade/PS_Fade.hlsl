// C++側から設定されるデータ
cbuffer ConstBuffer : register(b0)
{
    
    float power; // 強度
    float fadeAmount; // フェードの進行度（0.0～1.0）
    float2 padding; // パディング
    
};

// テクスチャとサンプラー
Texture2D dissolveTex : register(t0);
SamplerState samLinear : register(s0);

// シェーダー入力
struct PS_INPUT
{
    float4 pos : SV_POSITION; // 位置
    float2 Tex : TEXCOORD; // UV座標
};

// ディゾルブエフェクト用ピクセルシェーダー
float4 main(PS_INPUT input) : SV_TARGET
{
   
      // フェードテクスチャの明度を取得
    float fadeValue = dissolveTex.Sample(samLinear, input.Tex).r;

    // フェード処理の強度を元にブレンドファクターを計算
    float alpha = smoothstep(fadeAmount - power, fadeAmount + power, fadeValue);

    // 黒塗りのテクスチャとフェード処理を合成
    return float4(0, 0, 0, alpha);
}
