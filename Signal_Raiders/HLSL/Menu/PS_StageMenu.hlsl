#include "Menu.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);
float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}
float4 oldTV(float2 inUV)
{
    float2 uv = inUV;
	//	���S�����炷
    uv = uv - 0.5f; // uv - float2 (0.5f, 0.5f);

    float vignette = length(uv);
    uv /= 1 - vignette * 0.2f;
	//	���S��߂�
    float2 texUV = uv + 0.5f;
    
    
    // sin�g��0.025f����0�̊Ԃ���������l�𐶐�
    float warp = cos(time.x) * 0.25f;
    warp = lerp(warp, 0.000001f, step(1.5f, time.x)); // step���g�p���ď�����؂�ւ�
    warp = 0.00001f;
    texUV.x += (random(floor(texUV.y * 1500) + time.x * 0.001f) - 0.5f) * warp;
    texUV.y += (random(floor(texUV.x * 1500) + time.x * 0.001f) - 0.5f) * warp;
    //texUV.x += floor(uv.y * 5) * 0.1f;
    //texUV.y += floor(uv.x * 5) * 0.1f;
    
    float4 base = tex.Sample(samLinear, texUV);;
       // --- UV���W�̒����i�F�����p�j ---
    float2 uvR = texUV + float2(0.00325, 0.0) * length(texUV - 0.0125); // �ԃ`�����l���������E�ɂ��炷
    float2 uvB = texUV - float2(0.00325, 0.0) * length(texUV - 0.0125); // �`�����l�����������ɂ��炷

    float3 col;

    col.r = tex.Sample(samLinear, uvR).r; // �ԃ`�����l�������炵�ăT���v��
    col.g = tex.Sample(samLinear, texUV).g; // �΃`�����l���͂��̂܂�
    col.b = tex.Sample(samLinear, uvB).b; // �`�����l�������炵�ăT���v��
 
    col = lerp(col, float3(
                random(uv + float2(123 + time.x, 0)),
                random(uv + float2(123 + time.x, 1)),
                random(uv + float2(123 + time.x, 2))),
                step(random(floor(texUV.y * 500) + time.x * 0.005f), 0.005f));
    
 //   col *= 1 - vignette * 0.5f;
    
        
// --- �X�L�������C���̌��ʌv�Z�i�s�K���j ---
    float scanlineRandom = random(float2(floor(texUV.y * 50000.0), time.x * 10000.0));
    float scanline = sin(texUV.y * 500.0 + scanlineRandom * 500.0) * 0.025f;
    col *= 1.0 - scanline;

// --- �m�C�Y�̒ǉ� ---
    float noise = (random(texUV * time.x * 5000.0f) - 0.05f) * 0.00000006f;
    col += noise;
    //// �r�l�b�g���ʁi��ʒ[���Â�����)
    //float2 uvOffset = texUV - 0.75f;
    //float vignette2 = length(uvOffset) * 0.5f;
    //col *= 1.0 - vignette2 * 0.5;
    
    
    base = float4(col, base.a);
    // base�̐F�𖾂�߂ɂ���
    //base.rgb *= 1.5f;
    // �F���]����̃O���[���X�P�[��
    base = lerp(base, float4(0, 0, 0, 1), step(0, max(abs(uv.y) - 0.5f, abs(uv.x) - 0.5f)));
    return base;
}

float4 main(PS_INPUT input) : SV_TARGET
{
  
    // ���ʂ�Ԃ�
    return oldTV(input.Tex);
}

