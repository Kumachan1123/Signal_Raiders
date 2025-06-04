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
	// �R���X�g���N�^
	Sky(int StageID);
	// �f�X�g���N�^
	~Sky();
	// ������
	void Initialize(CommonResources* resources);
	// �`��
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		const DirectX::SimpleMath::Matrix& world
	);
private:// private�萔
	// �X�e�[�W�ԍ��ɉ�������̃��f����Key
	const std::unordered_map<int, const std::string> m_keyMap =
	{
		{0, "Stage1"},// �X�e�[�W�P
		{1, "Stage2"},// �X�e�[�W�Q
		{2, "Stage3"},// �X�e�[�W�R
		{3, "Stage4"},// �X�e�[�W�S
		{4, "Stage5"} // �X�e�[�W�T
	};
private:// private�ϐ�
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// ���f��
	DirectX::Model* m_pModel;
	// �e�N�X�`���p�X
	wchar_t m_texturePath[256];
	// �X�e�[�WID
	int m_stageID;
};