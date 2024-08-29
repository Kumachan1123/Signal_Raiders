// C++側から設定されるデータ
cbuffer ConstBuffer : register(b0)
{
    
    float power; // 強度
    float fadeAmount; // フェードの進行度（0.0～1.0）
    float padding; // パディング
    int num; // テクスチャ番号 
};

// テクスチャとサンプラー
Texture2D fadeTex : register(t0); // フェード
Texture2D readyTex : register(t1); // 待機テクスチャ
Texture2D goTex : register(t2); // ゴー
Texture2D BlackTex : register(t3); // 黒塗り
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
    float fadeValue = fadeTex.Sample(samLinear, input.Tex).r;
    float4 mainValue;
    // テクスチャ番号に応じて取得するテクスチャが変わる
    switch (num)
    {
        case 0:
           
            mainValue = BlackTex.Sample(samLinear, input.Tex); //黒塗り
            break;
        case 1:
            mainValue = readyTex.Sample(samLinear, input.Tex); //待機テクスチャ
            break;
        case 2:
            mainValue = goTex.Sample(samLinear, input.Tex); //ゴー
            break;
    }
    // 指定された番号に対応する画像がなかったら
    if (mainValue.a == 0)
    {
        return float4(0, 0, 0, 0);
    }
    
   
    // フェード処理の強度を元にブレンドファクターを計算
    float alpha = smoothstep(fadeAmount - power, fadeAmount + power, fadeValue);

    // 黒塗りのテクスチャとフェード処理を合成
    return float4(mainValue.r, mainValue.g, mainValue.b, alpha);
}
