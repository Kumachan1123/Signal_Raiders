// C++側から設定されるデータ①
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; // ワールド行列
    matrix matView; // ビュー行列
    matrix matProj; // プロジェクション行列
    float4 color; // 色
    float time; // 時間
    float2 uv; // uv座標
    float padding; // パディング
};


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
    // UV座標の中心を(0.5, 0.5)に設定する
    float2 center = uv;
    
    // 現在のUV座標と中心との距離を計算する
    float2 uvOffset = input.Tex - center;
    float distance = length(uvOffset); // 距離

    // 光の弧の描画範囲を調整する
    float innerRadius = 0.03; // 光の弧の内側の半径
    float outerRadius = 0.3; // 光の弧の外側の半径

    // 光の弧の範囲内でのグラデーションを計算する
    float corona = smoothstep(innerRadius, outerRadius, distance);

    // 時間に基づいてパッと点いてやんわり消えるアニメーションを実現する
    //float pulse = 0.5 * (sin(time * 5.0) + 1.0); // サイン波で0から1の範囲の値を繰り返す

    // 赤単色を設定（R=1.0, G=0.0, B=0.0）
    float3 rgb = float3(1.0, 0.0, 0.0);

    // アルファ値を時間に応じて調整する（光がやんわり消えるようにする）
    float alpha = (1.0 - corona) /** pulse*/;

    // 色を返す（光の弧の範囲内で赤色を適用）
    return float4(rgb, alpha);
}