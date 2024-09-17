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
    // ���ԂɊ�Â��Đn�̃O���f�[�V�����G�t�F�N�g�𐶐�
    float gradient = exp(-abs(sin(time * 1.0 + input.Tex.y * 5.0)));
    
    // �e�N�X�`�����T���v�����O
    float4 output = tex.Sample(samLinear, input.Tex);
    
    // �O���f�[�V�����G�t�F�N�g��������
    // �G�t�F�N�g�̐F�͐n
    output.rgba += gradient * float4(0.25, 0.5, 1.0, 0.0);
    
    // ���ʂ�Ԃ�
    return output * color;
}

