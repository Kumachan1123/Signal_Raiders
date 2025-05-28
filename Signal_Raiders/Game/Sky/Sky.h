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
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
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
private:// private�萔
	const std::unordered_map<int, const std::string> m_keyMap =// �X�e�[�W�ԍ��ɉ�������̃��f����Key
	{
		{0, "Stage1"},// �X�e�[�W�P
		{1, "Stage2"},// �X�e�[�W�Q
		{2, "Stage3"},// �X�e�[�W�R
		{3, "Stage4"},// �X�e�[�W�S
		{4, "Stage5"} // �X�e�[�W�T
	};
private:// private�ϐ�
	CommonResources* m_pCommonResources;// ���ʃ��\�[�X
	DirectX::Model* m_pModel;// ���f��
	wchar_t m_texturePath[256];// �e�N�X�`���p�X
	int m_stageID;// �X�e�[�WID
	std::string Path;
};