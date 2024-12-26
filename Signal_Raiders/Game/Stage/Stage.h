/*
	@file	Sky.h
	@brief	�X�J�C�N���X
*/
#pragma once

// �O���錾
class CommonResources;
class Stage
{
private:

	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	// ���f���̕`��Ŏg�p����
	std::unique_ptr<DirectX::Model> m_model;	// ���f��

	// �e�N�X�`���p�X
	wchar_t m_texturePath[256];

	// �X�e�[�WID
	int m_stageID;
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