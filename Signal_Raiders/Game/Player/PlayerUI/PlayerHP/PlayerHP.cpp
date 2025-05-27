/*
*	@file PlayerHP.cpp
*	@brief HPゲージクラス
*/
#include <pch.h>
#include "PlayerHP.h"
/*
*	@brief コンストラクタ
*	@details 初期化
*	@param なし
*	@return なし
*/
PlayerHP::PlayerHP()
	: m_windowHeight(0)		// ウィンドウの高さ
	, m_windowWidth(0)		// ウィンドウの幅
	, m_maxHP(100.0f)		// 最大HP
	, m_currentHP(100.0f)	// 現在のHP
	, m_pDR(nullptr)		// デバイスリソース
	, m_pGauge(nullptr)		// HPゲージ
	, m_pFrame(nullptr)		// HPゲージの枠
	, m_pBack(nullptr)		// HPゲージの背景
	, m_pHeartIcon(nullptr)	// ハートアイコン
{
}
/*
*	@brief デストラクタ
*	@details ここでは何もしない
*	@param なし
*	@return なし
*/
PlayerHP::~PlayerHP() {/*do nothing*/ }
/*
*	@brief 初期化関数
*	@details UIの初期化を行う
*	@param resources デバイスリソース
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void PlayerHP::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	m_pDR = resources->GetDeviceResources();// デバイスリソースを取得
	m_windowWidth = width;// ウィンドウの幅を設定
	m_windowHeight = height;// ウィンドウの高さを設定
	// HPバー
	Add(m_pGauge, L"Resources/Textures/HPBar.png"
		, Vector2(328, 40)
		, Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);
	m_pGauge->SetRenderRatioOffset(0);// ゲージのオフセットを0に設定
	// HPバーの背景
	Add(m_pBack, L"Resources/Textures/HPBarBase.png"
		, Vector2(328, 40)
		, Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);
	// HPバーの枠
	Add(m_pFrame, L"Resources/Textures/HPBarFrame.png"
		, Vector2(328, 40)
		, Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER);
	// ハートアイコン
	Add(m_pHeartIcon, L"Resources/Textures/HP.png"
		, Vector2(0, -5)
		, Vector2(.50f, .50f)
		, KumachiLib::ANCHOR::TOP_LEFT);
}
/*
*	@brief HP更新関数
*	@details プレイヤーのHPを更新する
*	@param PlayerHP プレイヤーのHP
*	@return なし
*/
void PlayerHP::Update(float PlayerHP)
{
	if (m_maxHP <= 0.0f) return; // 最大HPが0以下にならないようチェック
	float hp = PlayerHP / m_maxHP; // 最大HPを考慮して正規化
	hp = Clamp(hp, 0.0f, 1.0f); // 0.0～1.0の範囲に収める
	m_pGauge->SetRenderRatio(hp);// ゲージの比率を設定
}
/*
*	@brief 描画関数
*	@details UIを描画する
*	@param なし
*	@return なし
*/
void PlayerHP::Render()
{
	//シェーダーの種類を設定して描画
	m_pBack->SetShaderType(PlayerUI::ShaderType::OTHER), m_pBack->Render();// 背景を描画
	m_pGauge->SetShaderType(PlayerUI::ShaderType::HP), m_pGauge->Render();// ゲージを描画
	m_pFrame->SetShaderType(PlayerUI::ShaderType::OTHER), m_pFrame->Render(); // 枠を描画
	m_pHeartIcon->SetShaderType(PlayerUI::ShaderType::OTHER), m_pHeartIcon->Render();// ハートアイコンを描画
}
/*
*	@brief UIの追加関数
*	@details PlayerUIを追加する
*	@param pPlayerUI 追加するPlayerUIのポインタ
*	@param path テクスチャのパス
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void PlayerHP::Add(std::unique_ptr<PlayerUI>& pPlayerUI, const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>();// PlayerUIのインスタンスを生成
	// UIの初期化
	pPlayerUI->Create(m_pDR	// デバイスリソース
		, path				// テクスチャのパス
		, position			// UIの位置
		, scale				// UIのスケール
		, anchor);			// UIのアンカーを設定
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);// ウィンドウのサイズを設定
}