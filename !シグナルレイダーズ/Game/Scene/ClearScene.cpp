/*
	@file	ClearScene.cpp
	@brief	クリアシーンクラス
*/
#include "pch.h"
#include "ClearScene.h"
#include "Game/Screen.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
#include <cassert>
#include <Libraries/Microsoft/DebugDraw.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;

//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ClearScene::ClearScene()
	:
	m_commonResources{},
	m_spriteBatch{},
	m_spriteFont{},
	m_clearTexture{},
	m_pressKeyTexture{},
	m_pressKeyTexCenter{},
	m_clearTexCenter{},
	m_isChangeScene{},
	m_isFade{},
	m_volume{},
	m_counter{},
	m_time{},
	m_fade{},
	m_size{},
	m_pressKeySize{},
	m_backGround{ nullptr }
{
}

//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
ClearScene::~ClearScene()
{
	// do nothing.
	Finalize();
}

//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void ClearScene::Initialize(CommonResources* resources)
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
	// 背景の初期化
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
			L"Resources/Textures/Clear.png",
			nullptr,
			m_clearTexture.ReleaseAndGetAddressOf()
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
	m_clearTexture->GetResource(resource.GetAddressOf());
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
	m_clearTexCenter = texSize / 2.0f;
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
void ClearScene::Update(float elapsedTime)
{
	// 宣言をしたが、実際は使用していない変数
	UNREFERENCED_PARAMETER(elapsedTime);
	// オーディオマネージャーのインスタンスを取得
	auto audioManager = AudioManager::GetInstance();
	// キーボードステートトラッカーを取得する
	const auto& kbTracker = m_commonResources->GetInputManager()->GetKeyboardTracker();

	// スペースキーが押されたら
	if (m_fade->GetState() == Fade::FadeState::FadeInEnd && kbTracker->pressed.Space)
	{
		// SEの再生
		audioManager->PlaySound("SE", .3);
		// フェードアウトに移行
		m_fade->SetState(Fade::FadeState::FadeOut);
		m_fade->SetTextureNum((int)(Fade::TextureNum::BLACK));
	}
	// フェードアウトが終了したら
	if (m_fade->GetState() == Fade::FadeState::FadeOutEnd)
	{
		m_isChangeScene = true;
	}
	// BGMの再生
	audioManager->PlaySound("BGM", 0.3);

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
void ClearScene::Render()
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
void ClearScene::Finalize()
{
	// オーディオマネージャーのインスタンスを取得
	auto audioManager = AudioManager::GetInstance();
	audioManager->Shutdown();
}

//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID ClearScene::GetNextSceneID() const
{
	auto audioManager = AudioManager::GetInstance();
	// シーン変更がある場合
	if (m_isChangeScene)
	{
		// BGMとSEの停止
		audioManager->StopSound("BGM");
		audioManager->StopSound("SE");

		return IScene::SceneID::TITLE;
	}

	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

//---------------------------------------------------------
// FMODのシステムの初期化と音声データのロード
//---------------------------------------------------------
void ClearScene::InitializeFMOD()
{
	// シングルトンのオーディオマネージャー
// AudioManagerのシングルトンインスタンスを取得
	AudioManager* audioManager = AudioManager::GetInstance();

	// FMODシステムの初期化
	audioManager->Initialize();

	// 音声データのロード
	// ここで必要な音声データをAudioManagerにロードさせる
	audioManager->LoadSound("Resources/Sounds/select.mp3", "SE");
	audioManager->LoadSound("Resources/Sounds/result.mp3", "BGM");
}

// スペースキー押してってやつ描画
void ClearScene::DrawSpace()
{
	auto states = m_commonResources->GetCommonStates();

	// スプライトバッチの開始：オプションでソートモード、ブレンドステートを指定する
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());

	// タイトルロゴの描画位置を決める
	RECT rect{ m_commonResources->GetDeviceResources()->GetOutputSize() };
	// 画像の中心を計算する
	Vector2 titlePos{ rect.right / 2.0f, rect.bottom / 2.0f };

	// タイトルロゴを描画する
	m_spriteBatch->Draw(
		m_clearTexture.Get(),	// テクスチャ(SRV)
		titlePos,				// スクリーンの表示位置(originの描画位置)
		nullptr,			// 矩形(RECT)
		Colors::White,		// 背景色
		0.0f,				// 回転角(ラジアン)
		m_clearTexCenter,		// テクスチャの基準になる表示位置(描画中心)(origin)
		Vector2::One,				// スケール(scale)
		SpriteEffects_None,	// エフェクト(effects)
		0.0f				// レイヤ深度(画像のソートで必要)(layerDepth)
	);
	// 画像の中心を計算する
	Vector2 spacePos{ rect.right / 2.0f, rect.bottom / 1.2f };

	// タイトルロゴを描画する
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
	m_spriteFont->DrawString(m_spriteBatch.get(), L"Title Scene", Vector2(10, 40));

	wchar_t buf[32];
	swprintf_s(buf, 32, L"right : %d, bottom : %d", rect.right, rect.bottom);
	m_spriteFont->DrawString(m_spriteBatch.get(), buf, Vector2(10, 70));
#endif
	// スプライトバッチの終わり
	m_spriteBatch->End();
}