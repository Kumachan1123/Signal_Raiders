/*
	@file	PressKey.h
	@brief	�w���N���X
*/
#pragma once
#include <DeviceResources.h>
// �O���錾
class CommonResources;
class PressKey final
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pressKeyTexture;//�w���摜
	// �e�N�X�`���̔����̑傫��
	DirectX::SimpleMath::Vector2 m_pressKeyTexCenter;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	float m_time = 0.0f;// �g�k�Ɏg������
	float m_size = 0.0f;// �摜�T�C�Y
public:
	PressKey(CommonResources* commonResources);//�R���X�g���N�^
	~PressKey();//�f�X�g���N�^
	void Initialize();//������
	void Update(float elapsedTime);//�X�V
	void Render();//�`��
private:
	void CreateTexture();//�e�N�X�`���̍쐬
};
