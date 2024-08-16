// C++������ݒ肳���f�[�^�@
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; // ���[���h�s��
    matrix matView; // �r���[�s��
    matrix matProj; // �v���W�F�N�V�����s��
    float4 color; // �F
    float time; // ����
    float3 padding; // �p�f�B���O
};

// C++������ݒ肳���f�[�^�A
Texture2D tex : register(t0); // �e�N�X�`��1
Texture2D tex2 : register(t1); // �e�N�X�`��2
SamplerState samLinear : register(s0); // �T���v���[�X�e�[�g

// ���̃V�F�[�_�������Ƃ��Ď󂯎��f�[�^
struct PS_INPUT
{
    float4 pos : SV_POSITION; // �ʒu
    float2 Tex : TEXCOORD; // UV���W
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // ���ԂɊ�Â��ăg���C�����ʂ𐶐�
    float trail = exp(-abs(sin(time * 1.0 + input.Tex.xy * 1.0)));
    
    // �e�N�X�`�����T���v�����O
    float4 output = tex.Sample(samLinear, input.Tex);
    
    // �g���C�����ʂ�������
    output.rgb += trail * float3(0.70, 0.7, 1.0);
    
    // ���ʂ�Ԃ�
    return output * color;
}
