/*
*	@file Stage.h
*	@brief �X�e�[�W�N���X
*/
#pragma once

// �O���錾
class CommonResources;
class Stage
{
public:// �萔
	static const float STAGE_SIZE;	// �X�e�[�W�̍L��
	static const float STAGE_HEIGHT;	// �X�e�[�W�̍���
private:

	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	// ���f���̕`��Ŏg�p����
	std::unique_ptr<DirectX::Model> m_model;	// ���f��

	// �e�N�X�`���p�X
	wchar_t m_texturePath[256];
public:
	Stage();
	~Stage();

	void Initialize(CommonResources* resources);

	void Render(DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Vector3 pos
	);
	//	�֐�
};