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
Texture2D tex2 : register(t1); // テクスチャ2
SamplerState samLinear : register(s0); // サンプラーステート

// このシェーダが引数として受け取るデータ
struct PS_INPUT
{
    float4 pos : SV_POSITION; // 位置
    float2 Tex : TEXCOORD; // UV座標
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // 時間に基づいて青系のグラデーションエフェクトを生成
    float gradient = exp(-abs(sin(time * 1.0 + input.Tex.y * 5.0)));
    
    // テクスチャをサンプリング
    float4 output = tex.Sample(samLinear, input.Tex);
    
    // グラデーションエフェクトを加える
    // エフェクトの色は青系
    output.rgba += gradient * float4(0.25, 0.5, 1.0, 0.0);
    
    // 結果を返す
    return output * color;
}

