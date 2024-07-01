/*
	@file	SkyBox.h
	@brief	�X�J�C�{�b�N�X�N���X
	�쐬�ҁF���܂�
*/
#pragma once

// �O���錾
class CommonResources;
class SkyBox
{
private:
	 
	// ���ʃ��\�[�X
	CommonResources* m_commonResources;
 
	// ���f���̕`��Ŏg�p����
	std::unique_ptr<DirectX::Model> m_model;	// ���f��
public:
	SkyBox();
	~SkyBox();

	void Initialize(CommonResources* resources) ;

	void Render(DirectX::SimpleMath::Matrix view,
				DirectX::SimpleMath::Matrix proj,
				DirectX::SimpleMath::Matrix world,
				DirectX::SimpleMath::Vector3 pos
	);
	//	�֐�
};