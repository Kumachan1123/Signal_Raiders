// C++側から設定されるデータ①
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; // ワールド行列
    matrix matView; // ビュー行列
    matrix matProj; // プロジェクション行列
    float4 color; // 色
    float time; // 時間
    float3 padding; // パディング
};

// C++側から設定されるデータ②
Texture2D tex : register(t0); // テクスチャ1
SamplerState samLinear : register(s0); // サンプラーステート

// このシェーダが引数として受け取るデータ
struct PS_INPUT
{
    float4 pos : SV_POSITION; // 位置
    float2 Tex : TEXCOORD; // UV座標
};

// HSVからRGBに変換する関数
float3 HSVtoRGB(float3 hsv)
{
    float3 rgb = clamp(abs(fmod(hsv.x * 6.0 + float3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return hsv.z * lerp(float3(1.0, 1.0, 1.0), rgb, hsv.y);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャをサンプリング
    float4 output = tex.Sample(samLinear, input.Tex);

    // 縦線の強度を計算
    float lineIntensity = sin(input.Tex.y * 400.0 + time * 10.0) * 0.1;

    // ノイズの強度を計算
    float noise = (frac(sin(dot(input.Tex.xy * float2(12.9898, 78.233), float2(12.9898, 78.233))) * 43758.5453) - 0.5) * 0.2;

    // ブラウン管効果を適用
    output.rgb += lineIntensity + noise;

    // 時間に基づいて青系のグラデーションエフェクトを生成
    float gradient = exp(-abs(sin(time * 1.0 - input.Tex.y * 3.0)));
    
    // 青系のグラデーションエフェクトを加える
    output.rgb += gradient * float3(.5, .5, .5);
   
    // 結果を返す
    return output * color;
}
