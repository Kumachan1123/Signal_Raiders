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
SamplerState samLinear : register(s0); // �T���v���[�X�e�[�g

// ���̃V�F�[�_�������Ƃ��Ď󂯎��f�[�^
struct PS_INPUT
{
    float4 pos : SV_POSITION; // �ʒu
    float2 Tex : TEXCOORD; // UV���W
};

// HSV����RGB�ɕϊ�����֐�
float3 HSVtoRGB(float3 hsv)
{
    float3 rgb = clamp(abs(fmod(hsv.x * 6.0 + float3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    return hsv.z * lerp(float3(1.0, 1.0, 1.0), rgb, hsv.y);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    // �e�N�X�`�����T���v�����O
    float4 output = tex.Sample(samLinear, input.Tex);

    // �c���̋��x���v�Z
    float lineIntensity = sin(input.Tex.y * 400.0 + time * 10.0) * 0.1;

    // �m�C�Y�̋��x���v�Z
    float noise = (frac(sin(dot(input.Tex.xy * float2(12.9898, 78.233), float2(12.9898, 78.233))) * 43758.5453) - 0.5) * 0.2;

    // �u���E���ǌ��ʂ�K�p
    output.rgb += lineIntensity + noise;

    // ���ԂɊ�Â��Đn�̃O���f�[�V�����G�t�F�N�g�𐶐�
    float gradient = exp(-abs(sin(time * 1.0 - input.Tex.y * 3.0)));
    
    // �n�̃O���f�[�V�����G�t�F�N�g��������
    output.rgb += gradient * float3(.5, .5, .5);
   
    // ���ʂ�Ԃ�
    return output * color;
}
