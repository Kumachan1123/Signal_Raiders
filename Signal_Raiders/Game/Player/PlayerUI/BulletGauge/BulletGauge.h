//--------------------------------------------------------------------------------------
// File: BulletGauge.h
//
// �e�Q�[�W�N���X
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

class BulletGauge
{

	//�ϐ�
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;

	std::unique_ptr<PlayerUI> m_gauge;


	const wchar_t* m_baseTexturePath;



	int m_windowWidth, m_windowHeight;

	//�֐�
public:
	BulletGauge();
	~BulletGauge();

	void Initialize(DX::DeviceResources* pDR, int width, int height);
	void Update(float bulletPoint);
	void Render();

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);


};
