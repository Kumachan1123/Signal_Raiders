// C++������ݒ肳���f�[�^
cbuffer ConstBuffer : register(b0)
{
    float power; // ���x
    float fadeAmount; // �t�F�[�h�̐i�s�x�i0.0�`1.0�j
    float padding; // �p�f�B���O
    int num; // �e�N�X�`���ԍ�
};

// �e�N�X�`���ƃT���v���[
Texture2D fadeTex : register(t0); // �t�F�[�h
Texture2D readyTex : register(t1); // �ҋ@�e�N�X�`��
Texture2D goTex : register(t2); // �S�[
Texture2D BlackTex : register(t3); // ���h��
SamplerState samLinear : register(s0);

// �V�F�[�_�[����
struct PS_INPUT
{
    float4 pos : SV_POSITION; // �ʒu
    float2 Tex : TEXCOORD; // UV���W
};

// �f�B�]���u�G�t�F�N�g�p�s�N�Z���V�F�[�_�[
float4 main(PS_INPUT input) : SV_TARGET
{
    // �t�F�[�h�e�N�X�`���̖��x���擾
    float fadeValue = fadeTex.Sample(samLinear, input.Tex).r;

    // �e�e�N�X�`�����T���v�����O
    float4 blackSample = BlackTex.Sample(samLinear, input.Tex);
    float4 readySample = readyTex.Sample(samLinear, input.Tex);
    float4 goSample = goTex.Sample(samLinear, input.Tex);

    // num �Ɋ�Â��ăe�N�X�`����I���i���`��Ԃ܂��̓X�e�b�v�֐����g�p�j
    float4 mainValue = lerp(blackSample, readySample, step(1, num));
    mainValue = lerp(mainValue, goSample, step(2, num));

    // �t�F�[�h�����̋��x�����Ƀu�����h�t�@�N�^�[���v�Z
    float alpha = smoothstep(fadeAmount - power, fadeAmount + power, fadeValue);

    // �e�N�X�`���ƃt�F�[�h����������
    return float4(mainValue.rgb, alpha);
}
