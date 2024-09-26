// C++������ݒ肳���f�[�^�@
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld; // ���[���h�s��
    matrix matView; // �r���[�s��
    matrix matProj; // �v���W�F�N�V�����s��
    float4 color; // �F
    float time; // ����
    float2 uv; // uv���W
    float padding; // �p�f�B���O
};


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
    // UV���W�̒��S��(0.5, 0.5)�ɐݒ肷��
    float2 center = uv;
    
    // ���݂�UV���W�ƒ��S�Ƃ̋������v�Z����
    float2 uvOffset = input.Tex - center;
    float distance = length(uvOffset); // ����

    // ���̌ʂ̕`��͈͂𒲐�����
    float innerRadius = 0.03; // ���̌ʂ̓����̔��a
    float outerRadius = 0.3; // ���̌ʂ̊O���̔��a

    // ���̌ʂ͈͓̔��ł̃O���f�[�V�������v�Z����
    float corona = smoothstep(innerRadius, outerRadius, distance);

    // ���ԂɊ�Â��ăp�b�Ɠ_���Ă���������A�j���[�V��������������
    //float pulse = 0.5 * (sin(time * 5.0) + 1.0); // �T�C���g��0����1�͈̔͂̒l���J��Ԃ�

    // �ԒP�F��ݒ�iR=1.0, G=0.0, B=0.0�j
    float3 rgb = float3(1.0, 0.0, 0.0);

    // �A���t�@�l�����Ԃɉ����Ē�������i��������������悤�ɂ���j
    float alpha = (1.0 - corona) /** pulse*/;

    // �F��Ԃ��i���̌ʂ͈͓̔��ŐԐF��K�p�j
    return float4(rgb, alpha);
}