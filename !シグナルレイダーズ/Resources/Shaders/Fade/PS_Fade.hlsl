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
    float4 mainValue;
    // �e�N�X�`���ԍ��ɉ����Ď擾����e�N�X�`�����ς��
    switch (num)
    {
        case 0:
           
            mainValue = BlackTex.Sample(samLinear, input.Tex); //���h��
            break;
        case 1:
            mainValue = readyTex.Sample(samLinear, input.Tex); //�ҋ@�e�N�X�`��
            break;
        case 2:
            mainValue = goTex.Sample(samLinear, input.Tex); //�S�[
            break;
    }
    // �w�肳�ꂽ�ԍ��ɑΉ�����摜���Ȃ�������
    if (mainValue.a == 0)
    {
        return float4(0, 0, 0, 0);
    }
    
   
    // �t�F�[�h�����̋��x�����Ƀu�����h�t�@�N�^�[���v�Z
    float alpha = smoothstep(fadeAmount - power, fadeAmount + power, fadeValue);

    // ���h��̃e�N�X�`���ƃt�F�[�h����������
    return float4(mainValue.r, mainValue.g, mainValue.b, alpha);
}
