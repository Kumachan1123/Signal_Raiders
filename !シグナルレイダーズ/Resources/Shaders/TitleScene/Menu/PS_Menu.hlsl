#include "Menu.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//�g���̉摜�\��
    float4 output = tex.Sample(samLinear, input.tex);
     // ���ԂɊ�Â��Đn�̃O���f�[�V�����G�t�F�N�g�𐶐�
    float gradient = exp(-abs(sin(time * 1.0 + input.tex.y * 5.0)));
   
	 
// �O���f�[�V�����G�t�F�N�g��������
    // �G�t�F�N�g�̐F�͐n
    output.rgba += gradient * float4(0.5, 0.5, 0.5, 0.0);
    
    // ���ʂ�Ԃ�    
    return output;
}