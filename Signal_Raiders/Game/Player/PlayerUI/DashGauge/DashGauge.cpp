/*
*	@file DashGauge.cpp
*	@brief ダッシュゲージクラス
*/
#include <pch.h>
#include "DashGauge.h"
/*
*	@brief コンストラクタ
*	@details ダッシュゲージクラスのコンストラクタ
*	@param なし
*	@return なし
*/
DashGauge::DashGauge()
	: m_windowHeight(0)	// ウィンドウの高さ
	, m_windowWidth(0)	// ウィンドウの幅
	, m_pDR(nullptr)	// デバイスリソースへのポインタ
	, m_pCommonResources(nullptr)	// 共通リソースへのポインタ
	, m_pGauge(nullptr)	// ゲージ本体
	, m_pFrame(nullptr)	// ゲージの枠
	, m_pBack(nullptr)	// ゲージの背景
	, m_pDashIcon(nullptr)	// ダッシュアイコン
{
}
/*
*	@brief デストラクタ
*	@details ダッシュゲージクラスのデストラクタ(ここでは何もしない)
*	@param なし
*	@return なし
*/
DashGauge::~DashGauge() {/*do nothing*/ }
/*
*	@brief 初期化
*	@details ダッシュゲージの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@param width ウィンドウの幅
*	@param height ウィンドウの高さ
*	@return なし
*/
void DashGauge::Initialize(CommonResources* resources, int width, int height)
{
	using namespace DirectX::SimpleMath;
	m_pCommonResources = resources; // 共通リソースを設定
	m_pDR = resources->GetDeviceResources();// デバイスリソースを取得
	m_windowWidth = width;// ウィンドウの幅を設定
	m_windowHeight = height;// ウィンドウの高さを設定
	Add(m_pBack,// ゲージの背景
		"HPBarBase",
		Vector2(210, 100),
		Vector2(.250f, .250f),
		KumachiLib::ANCHOR::MIDDLE_CENTER);
	Add(m_pFrame,// ゲージの枠
		"HPBarFrame",
		Vector2(210, 100),
		Vector2(.250f, .250f),
		KumachiLib::ANCHOR::MIDDLE_CENTER);
	Add(m_pGauge,// ゲージ本体
		"HPBar",
		Vector2(210, 100),
		Vector2(.250f, .250f),
		KumachiLib::ANCHOR::MIDDLE_CENTER);
	m_pGauge->SetRenderRatioOffset(0);
	Add(m_pDashIcon,// ダッシュアイコン
		"Dash",
		Vector2(-5, 75),
		Vector2(1.0f, 1.0f),
		KumachiLib::ANCHOR::TOP_LEFT);
}
/*
*	@brief 更新
*	@details ダッシュゲージの更新を行う
*	@param dashStamina ダッシュスタミナの値
*	@return なし
*/
void DashGauge::Update(float dashStamina)
{
	float stamina = dashStamina * 0.1f;// ダッシュスタミナを0.1倍して正規化
	stamina = std::max(0.f, stamina); // 0未満にならないようにチェック
	m_pGauge->SetRenderRatio(stamina);// ゲージの表示比率を設定
}
/*
*	@brief 描画
*	@details ダッシュゲージの描画を行う
*	@param なし
*	@return なし
*/
void DashGauge::Render()
{
	//シェーダーの種類を設定して描画
	m_pBack->SetShaderType(PlayerUI::ShaderType::OTHER), m_pBack->Render();// 背景を描画
	m_pGauge->SetShaderType(PlayerUI::ShaderType::HP), m_pGauge->Render(); // ゲージ本体を描画
	m_pFrame->SetShaderType(PlayerUI::ShaderType::OTHER), m_pFrame->Render(); // 枠を描画
	m_pDashIcon->SetShaderType(PlayerUI::ShaderType::OTHER), m_pDashIcon->Render();// ダッシュアイコンを描画
}
/*
*	@brief UI追加
*	@details ダッシュゲージにUIを追加する
*	@param pPlayerUI 追加するUIのスマートポインタ
*	@param key UIのテクスチャキー
*	@param position UIの位置
*	@param scale UIのスケール
*	@param anchor UIのアンカー
*	@return なし
*/
void DashGauge::Add(std::unique_ptr<PlayerUI>& pPlayerUI, std::string key, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, KumachiLib::ANCHOR anchor)
{
	pPlayerUI = std::make_unique<PlayerUI>(m_pCommonResources);// 新しいPlayerUIを作成
	// PlayerUIを作成し、指定されたパス、位置、スケール、アンカーで初期化
	pPlayerUI->Create(m_pDR
		, key
		, position
		, scale
		, anchor);
	pPlayerUI->SetWindowSize(m_windowWidth, m_windowHeight);
}