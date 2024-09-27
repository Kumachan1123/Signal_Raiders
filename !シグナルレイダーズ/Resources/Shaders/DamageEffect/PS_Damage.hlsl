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


// �Ԃ��甒�ւ̃O���f�[�V�������쐬����s�N�Z���V�F�[�_�[��main�֐�
float4 main(PS_INPUT input) : SV_TARGET
{
  // UV���W�̒��S��(0.5, 0.5)�ɐݒ肷��
    float2 center = uv;
    
    // ���݂�UV���W�ƒ��S�Ƃ̋������v�Z����
    float2 uvOffset = input.Tex - center;
    float distance = length(uvOffset); // ����

    // ���̌ʂ̕`��͈͂𒲐�����
    float innerRadius = 0.00001; // ���̖ʐς̔��a
    float outerRadius = 0.1; // �Ԃ̖ʐς̔��a

    // ���̌ʂ͈͓̔��ł̃O���f�[�V�������v�Z����
    float corona = smoothstep(innerRadius, outerRadius, distance);

    // ��Ԃ���
    float3 rgb = lerp(float3(1.0, 0.5, 0.0), float3(3.0, 0.0, 0.0), corona);

    // �A���t�@�l�����̌ʂɊ�Â��Ē�������
    float alpha = (1.0 - corona);

    // �F��Ԃ��i���S�ɋ߂Â��قǔ����Ȃ�悤�ɐԐF�Ɣ��F��K�p�j
    return float4(rgb, alpha);
}
