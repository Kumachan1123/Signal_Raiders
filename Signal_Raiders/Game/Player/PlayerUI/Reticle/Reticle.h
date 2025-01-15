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

class Reticle
{
public:

	//�ϐ�
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;


	std::unique_ptr<PlayerUI> m_reticle;


	const wchar_t* m_baseTexturePath;

	std::unique_ptr<PlayerUI> m_baseWindow;

	int m_windowWidth, m_windowHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;


	//�֐�
public:
	Reticle();
	~Reticle();

	void Initialize(DX::DeviceResources* pDR, int width, int height);
	void Update();
	void Render();

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
};
