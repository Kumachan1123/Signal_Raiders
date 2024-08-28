// C++������ݒ肳���f�[�^
cbuffer ConstBuffer : register(b0)
{
    
    float power; // ���x
    float fadeAmount; // �t�F�[�h�̐i�s�x�i0.0�`1.0�j
    float2 padding; // �p�f�B���O
    
};

// �e�N�X�`���ƃT���v���[
Texture2D dissolveTex : register(t0);
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
    float fadeValue = dissolveTex.Sample(samLinear, input.Tex).r;

    // �t�F�[�h�����̋��x�����Ƀu�����h�t�@�N�^�[���v�Z
    float alpha = smoothstep(fadeAmount - power, fadeAmount + power, fadeValue);

    // ���h��̃e�N�X�`���ƃt�F�[�h����������
    return float4(0, 0, 0, alpha);
}
