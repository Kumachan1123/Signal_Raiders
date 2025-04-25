//--------------------------------------------------------------------------------------
// File: WifiLoading.h
//
// Wi-FiローディングUIクラス
//
//-------------------------------------------------------------------------------------
#pragma once
#include "StepTimer.h"
#include "Game/Player/PlayerUI/PlayerUI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include <memory>
#include "Keyboard.h"
#include "Game/Screen.h"
#include "Game/Interface/IUI.h"


class WifiLoading : public IUI
{

	//変数
private:

	DX::DeviceResources* m_pDR;


	std::unique_ptr<PlayerUI> m_loading;// Wi-FiローディングUI

	std::unique_ptr<PlayerUI> m_loadgingText;// ローディングテキスト


	const wchar_t* m_baseTexturePath;


	int m_windowWidth, m_windowHeight;
	// フレームの行数と列数
	int m_frameRows;
	int m_frameCols;
	// フレーム数
	int m_anim;
	// アニメーションスピード
	float m_animSpeed;
	// アニメーション時間
	float m_animTime;
	DirectX::Keyboard::KeyboardStateTracker m_tracker;


	//関数
public:
	WifiLoading();
	~WifiLoading();

	void Initialize(CommonResources* resources, int width, int height) override;
	void Update(const UpdateContext& data)override
	{
		Update(data.elapsedTime);
	};
	void Render()override;

	void Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor);
private:
	void Update(float elapsedTime);

};