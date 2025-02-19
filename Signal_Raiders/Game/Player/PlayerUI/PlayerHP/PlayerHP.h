//--------------------------------------------------------------------------------------
// File: PlayerHP.h
//
// HP�Q�[�W�N���X
//
//-------------------------------------------------------------------------------------

#pragma once

#include "StepTimer.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"

class PlayerHP
{


	//�ϐ�
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;

	std::unique_ptr<PlayerUI> m_heart;
	std::unique_ptr<PlayerUI> m_gauge;
	std::unique_ptr<PlayerUI> m_frame;
	std::unique_ptr<PlayerUI> m_base;


	const wchar_t* m_baseTexturePath;

	std::unique_ptr<PlayerUI> m_baseWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;
	float m_maxHP;// �ő�HP(�r���ŕϓ�����j
	float m_currentHP;// ���݂�HP

	//�֐�
public:
	PlayerHP();
	~PlayerHP();

	void Initialize(DX::DeviceResources* pDR, int width, int height);
	void Update(float playerHP);
	void Render();
	void SetMaxHP(float maxHP) { m_maxHP = maxHP; }
	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
};
