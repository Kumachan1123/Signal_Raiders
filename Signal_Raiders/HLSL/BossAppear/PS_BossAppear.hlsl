#include "Header.hlsli"

// C++������ݒ肳���f�[�^
Texture2D tex : register(t0); // �e�N�X�`��
SamplerState samLinear : register(s0); // �T���v���[�X�e�[�g

// �s�N�Z���V�F�[�_�[�̃��C���֐�
float4 main(PS_INPUT input) : SV_TARGET
{
   // UV���W���擾
    float2 uv = input.Tex;
    float scrollSpeed = 0.5; // �X�N���[�����x

    // UV��Y���W�ɉ������X�N���[������������iif �Ȃ��j
    float upMask = step(2.0 / 3.0, uv.y); // ��G���A�Ȃ� 1.0�A���� 0.0
    float downMask = step(uv.y, 1.0 / 3.0); // ���G���A�Ȃ� 1.0�A���� 0.0

    // �X�N���[������������i��� +1.0�A������ 0.0�A���� -1.0�j
    float scrollDir = upMask - downMask;
  //  float scrollDir = sign(uv.y - (2.0 / 3.0)) - sign(uv.y - (1.0 / 3.0));
    // UV��X���W���X�N���[���i���ԕ����͓����Ȃ��j
    uv.x += frac(time.x * scrollSpeed) * scrollDir;


    // 2�b�ォ��t�F�[�h�A�E�g�J�n�i5�b�Ŋ��S�ɏ�����j
    float fadeStartTime = 2.0; // �t�F�[�h�J�n����
    float fadeDuration = 0.30; // �t�F�[�h�ɂ����鎞��
    float alpha = saturate(1.0 - max(0.0, (time.x - fadeStartTime) / fadeDuration));

    // �e�N�X�`�����T���v�����O���ăA���t�@�l��K�p
    float4 color = tex.Sample(samLinear, uv);
    color.a *= alpha; // ���X�ɓ����ɂ���

    return color;
}
