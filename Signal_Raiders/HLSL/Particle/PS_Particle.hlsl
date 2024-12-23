#include "Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);
float4 main(PS_INPUT input) : SV_TARGET
{
	//	�w�肳�ꂽ�摜��������擾
    float4 output = tex.Sample(samLinear, input.Tex);
   // float3 rainbowColor = GetRainbow(time.x);
    output *= input.Color;
	//	C++����w�肳�ꂽ�F�������l�����āA�\���F������
    
  
   
    // �A���t�@�l��0�̏ꍇ�̓s�N�Z�����\���ɂ���
    float alphaMask = step(0.0f, output.a); // �A���t�@�l��0�Ȃ�0�A����ȊO��1�ɂȂ�
    output *= alphaMask; // �A���t�@��0�̃s�N�Z�������S�ɖ�����

  
    return output;
}