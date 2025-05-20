/*
	@file	Sky.h
	@brief	�X�J�C�N���X
*/
#pragma once
#include <unordered_map>
#include <string>
// �O���錾
class CommonResources;
class Sky
{
private:

	// ���ʃ��\�[�X
	CommonResources* m_commonResources;

	// ���f���̕`��Ŏg�p����
	std::unique_ptr<DirectX::Model> m_model;	// ���f��
	const std::unordered_map<int, const std::string> m_texturePathMap =
	{
		{0, "Resources/models/sky/sky.cmo"},
		{1, "Resources/models/sky/CloudySky.cmo"},
		{2, "Resources/models/sky/EveningSky.cmo"},
		{3, "Resources/models/sky/NightSky.cmo"},
		{4, "Resources/models/sky/MidNightSky.cmo"}
	};
	// �e�N�X�`���p�X
	wchar_t m_texturePath[256];

	// �X�e�[�WID
	int m_stageID;
public:
	Sky(int StageID);
	~Sky();

	void Initialize(CommonResources* resources);

	void Render(DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix proj,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Vector3 pos
	);
	//	�֐�
private:
	std::wstring ConvertToWString(const std::string& str);
};