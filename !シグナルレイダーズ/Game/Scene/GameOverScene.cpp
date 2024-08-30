/*
	@file	GameOverScene.cpp
	@brief	クリアシーンクラス
*/
#include "pch.h"
#include "GameOverScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
// FMODのインクルード
#include "Libraries/FMOD/inc/fmod.hpp"
#include "Libraries/FMOD/inc/fmod_errors.h"
#include <Libraries/Microsoft/DebugDraw.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
GameOverScene::GameOverScene()
	:
	m_commonResources{},
	m_spriteBatch{},
	m_spriteFont{},
	m_gameoverTexture{},
	m_pressKeyTexture{},
	m_pressKeyTexCenter{},
	m_gameoverTexCenter{},
	m_isChangeScene{},
	m_system{ nullptr },
	m_soundSE{ nullptr },
	m_soundBGM{ nullptr },
	m_channelSE{ nullptr },
	m_channelBGM{ nullptr },
	m_isFade{},
	m_volume{},
	m_counter{},
	m_time{ },
	m_size{},
	m_pressKeySize{},
	m_fade{},
	m_backGround{ nullptr }
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
GameOverScene::~GameOverScene()
{
	// do nothing.
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void GameOverScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;

	auto DR = m_commonResources->GetDeviceResources();
	auto device = DR->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// スプライトバッチを作成する
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	// スプライトフォントを作成する
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(
		device,
		L"Resources/Fonts/SegoeUI_18.spritefont"
	);
	// フェードの初期化
	m_fade = std::make_unique<Fade>(m_commonResources);
	m_fade->Create(DR);
	m_fade->SetState(Fade::FadeState::FadeIn);
	m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	// 背景を作成する
	m_backGround = std::make_unique<BackGround>(m_commonResources);
	m_backGround->Create(DR);

	// 画像をロードする
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/Space.png",
			nullptr,
			m_pressKeyTexture.ReleaseAndGetAddressOf()
		)
	);

	// 画像をロードする
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			L"Resources/Textures/GameOver.png",
			nullptr,
			m_gameoverTexture.ReleaseAndGetAddressOf()
		)
	);


	/*
		以下、テクスチャの大きさを求める→テクスチャの中心座標を計算する
	*/
	// 一時的な変数の宣言
	Microsoft::WRL::ComPtr<ID3D11Resource> resource{};
	Microsoft::WRL::ComPtr<ID3D11Resource> resource2{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D2{};
	D3D11_TEXTURE2D_DESC desc{};
	D3D11_TEXTURE2D_DESC desc2{};
	Vector2 texSize{};
	Vector2 texSize2{};

	// テクスチャの情報を取得する================================
	// テクスチャをID3D11Resourceとして見る
	m_gameoverTexture->GetResource(resource.GetAddressOf());
	m_pressKeyTexture->GetResource(resource2.GetAddressOf());
	// ID3D11ResourceをID3D11Texture2Dとして見る
	resource.As(&tex2D);
	resource2.As(&tex2D2);

	// テクスチャ情報を取得する
	tex2D->GetDesc(&desc);

	// テクスチャサイズを取得し、float型に変換する
	texSize.x = static_cast<float>(desc.Width);
	texSize.y = static_cast<float>(desc.Height);

	// テクスチャの中心位置を計算する
	m_gameoverTexCenter = texSize / 2.0f;
	tex2D2->GetDesc(&desc2);

	// テクスチャサイズを取得し、float型に変換する
	texSize2.x = static_cast<float>(desc2.Width);
	texSize2.y = static_cast<float>(desc2.Height);
	m_pressKeyTexCenter = texSize2 / 2.0f;

	// シーン変更フラグを初期化する
	m_isChangeScene = false;

	// Sound用のオブジェクトを初期化する
	InitializeFMOD();

	// フェードに関する準備
	m_isFade = false;
	m_volume = 1.0f;
	m_counter = 0;
}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void GameOverScene::Update(float elapsedTime)
{
	// 宣言をしたが、実際は使用していない変数
	UNREFERENCED_PARAMETER(elapsedTime);
	FMOD_RESULT result;

	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	// スペースキーが押されたら
	if (m_fade->GetState() == Fade::FadeState::FadeInEnd && kbTracker->pressed.Space)
	{
		result = m_system->playSound(m_soundSE, nullptr, false, &m_channelSE);
		assert(result == FMOD_OK);
		// フェードアウトに移行

		m_fade->SetState(Fade::FadeState::FadeOut);
		m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	}
	// フェードアウトが終了したら
	if (m_fade->GetState() == Fade::FadeState::FadeOutEnd)
	{
		m_isChangeScene = true;
	}
	// 二重再生しない
	if (m_channelBGM == nullptr)
	{
		result = m_system->playSound(m_soundBGM, nullptr, false, &m_channelBGM);
		assert(result == FMOD_OK);
	}
	// フェードに関する準備
	m_time += elapsedTime; // 時間をカウント
	m_size = (sin(m_time) + 1.0f) * 0.3f + 0.75f; // sin波で0.5〜1.5の間を変動させる
	m_pressKeySize = (cos(m_time) + 1.0f) * 0.3f + 0.75f; // sin波で0.5〜1.5の間を変動させる
	// 背景の更新
	m_backGround->Update(elapsedTime);

	// フェードの更新
	m_fade->Update(elapsedTime);
}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void GameOverScene::Render()
{
	// 背景の描画
	m_backGround->Render();

	// スペースキー押してってやつ描画
	if (m_fade->GetState() == Fade::FadeState::FadeInEnd)DrawSpace();

	// フェードの描画
	m_fade->Render();

}

//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void GameOverScene::Finalize()
{
	// do nothing.
		// Sound用のオブジェクトを解放する
	m_soundSE->release();
	m_soundBGM->release();
	m_system->release();
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID GameOverScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		m_channelBGM->stop();
		return IScene::SceneID::TITLE;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

//---------------------------------------------------------
// FMODのシステムの初期化と音声データのロード
//---------------------------------------------------------
void GameOverScene::InitializeFMOD()
{
	// システムをインスタンス化する
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	assert(result == FMOD_OK);

	// システムを初期化する
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	assert(result == FMOD_OK);

	// SEをロードする
	result = m_system->createSound("Resources/Sounds/select.mp3", FMOD_DEFAULT, nullptr, &m_soundSE);
	assert(result == FMOD_OK);

	// BGMをロードする
	result = m_system->createSound("Resources/Sounds/result.mp3", FMOD_LOOP_NORMAL, nullptr, &m_soundBGM);
	assert(result == FMOD_OK);
}

//---------------------------------------------------------
// スペースキー押してってやつ描画
//---------------------------------------------------------
void GameOverScene::DrawSpace()
{
	auto states = m_commonResources->GetCommonStates();

	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	// タイトルロゴの描画位置を決める
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };
	// 画像の中心を計算する
	Vector2 titlePos{ rect.right / 2.0f, rect.bottom / 2.0f };

	// ゲームオーバーを描画する
	m_spriteBatch->Draw(
		m_gameoverTexture.Get(),	// テクスチャ(SRV)
		titlePos,				// スクリーンの表示位置(originの描画位置)
		nullptr,			// 矩形(RECT)
		Colors::White,		// 背景色
		0.0f,				// 回転角(ラジアン)
		m_gameoverTexCenter,		// テクスチャの基準になる表示位置(描画中心)(origin)
		Vector2::One,				// スケール(scale)
		SpriteEffects_None,	// エフェクト(effects)
		0.0f				// レイヤ深度(画像のソートで必要)(layerDepth)
	);
	// 画像の中心を計算する
	Vector2 spacePos{ rect.right / 2.0f, rect.bottom / 1.2f };

	// 指示を描画する
	m_spriteBatch->Draw(
		m_pressKeyTexture.Get(),	// テクスチャ(SRV)
		spacePos,				// スクリーンの表示位置(originの描画位置)
		nullptr,			// 矩形(RECT)
		Colors::White,		// 背景色
		0.0f,				// 回転角(ラジアン)
		m_pressKeyTexCenter,		// テクスチャの基準になる表示位置(描画中心)(origin)
		m_pressKeySize,				// スケール(scale)
		SpriteEffects_None,	// エフェクト(effects)
		0.0f				// レイヤ深度(画像のソートで必要)(layerDepth)
	);
#ifdef _DEBUG
	// 純粋にスプライトフォントで文字列を描画する方法
	m_spriteFont->DrawString(m_spriteBatch.get(), L"GameOver Scene", Vector2(10, 40));

	wchar_t buf[32];
	swprintf_s(buf, 32, L"right : %d, bottom : %d", rect.right, rect.bottom);
	m_spriteFont->DrawString(m_spriteBatch.get(), buf, Vector2(10, 70));
#endif
	// スプライトバッチの終わり
	m_spriteBatch->End();
}