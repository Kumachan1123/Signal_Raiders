/*
	@file	SettingBar.h
	@brief	�ݒ�o�[�N���X
*/
#pragma once
#include "Game/Scene/SettingScene/SettingMenu/SettingMenu.h"
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"

class CommonResources;
class SettingBar
{
public:

	//	�ϐ�
private:
	SettingMenu* m_pSettingMenu;// �Z�b�e�B���O���j���[
	DX::DeviceResources* m_pDR;// �f�o�C�X���\�[�X
	CommonResources* m_commonResources;// ���ʃ��\�[�X
	std::vector<std::unique_ptr<SettingUI>> m_pBar;// �ݒ�o�[
	std::vector<std::unique_ptr<SettingUI>> m_pBarPointer;// �ݒ�o�[�̋�
	const wchar_t* m_pSettingBarTexturePath;// �ݒ�o�[�e�N�X�`���p�X
	const wchar_t* m_pSettingBarPointerTexturePath;// �ݒ�o�[�̋ʃe�N�X�`���p�X
	int m_windowWidth, m_windowHeight;// �E�B���h�E�̕��ƍ���
	DirectX::Keyboard::KeyboardStateTracker m_tracker;// �g���b�J�[
	SettingMenu::StateID m_num;// ���ID
	float m_time = 0;// ����
	//	�֐�
public:
	SettingBar(SettingMenu* pSettingMenu);
	~SettingBar();
	void Initialize(CommonResources* resources, int width, int height);
	void Update(float elapsedTime);
	void Render();
	void Add(const wchar_t* path
			 , DirectX::SimpleMath::Vector2 position
			 , DirectX::SimpleMath::Vector2 scale
			 , kumachi::ANCHOR anchor);
	void SetStateIDNum(SettingMenu::StateID num) { m_num = num; }
};