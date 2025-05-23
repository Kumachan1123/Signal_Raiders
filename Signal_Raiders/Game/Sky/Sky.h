/*
*	@brief	�X�J�C�N���X
*	@file	Sky.h
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <memory>
#include <locale> 
#include <codecvt>
#include <unordered_map>
#include <string>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include "DeviceResources.h"
// �O�����C�u����
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources.h"
// �O���錾
class CommonResources;
class Sky
{
public:// public�֐�
	Sky(int StageID);// �R���X�g���N�^
	~Sky();// �f�X�g���N�^
	void Initialize(CommonResources* resources);// ������
	void Render(DirectX::SimpleMath::Matrix view,// �`��
		DirectX::SimpleMath::Matrix proj,
		DirectX::SimpleMath::Matrix world,
		DirectX::SimpleMath::Vector3 pos
	);
private:
	std::wstring ConvertToWString(const std::string& str);// ������ϊ�
private:// private�萔
	const std::unordered_map<int, const std::string> m_texturePathMap =// �X�e�[�W�ԍ��ɉ�������̃��f���̃p�X
	{
		{0, "Resources/models/sky/sky.cmo"},// �X�e�[�W�P
		{1, "Resources/models/sky/CloudySky.cmo"},// �X�e�[�W�Q
		{2, "Resources/models/sky/EveningSky.cmo"},// �X�e�[�W�R
		{3, "Resources/models/sky/NightSky.cmo"},// �X�e�[�W�S
		{4, "Resources/models/sky/MidNightSky.cmo"}// �X�e�[�W�T
	};
private:// private�ϐ�
	CommonResources* m_commonResources;// ���ʃ��\�[�X
	std::unique_ptr<DirectX::Model> m_model;// ���f��
	wchar_t m_texturePath[256];// �e�N�X�`���p�X
	int m_stageID;// �X�e�[�WID
};