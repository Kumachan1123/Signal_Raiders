/*
*	@file	EnemyHPBar.cpp
*	@brief	敵HPBarクラス
*/
#include <pch.h>
#include "EnemyHPBar.h"


/*
*	@brief	インプットレイアウト
*	@return 	なし
*/
const std::vector<D3D11_INPUT_ELEMENT_DESC>  EnemyHPBar::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	コンストラクタ
*	@details 敵HPBarクラスのコンストラクタ
*	@param	なし
*	@return なし
*/
EnemyHPBar::EnemyHPBar()
	: m_pCommonResources{}// 共通リソース
	, m_maxHP(100)// 敵の最大HP
	, m_currentHP(100)// 敵の現在HP
	, m_displayedHP(100)// 敵の表示HP
	, m_lerpSpeed(5.0f)// 補間速度
	, m_scale(1.5f)// スケール
	, m_time{ 0.0f }// 経過時間
	, m_isDead(false)// 死亡フラグ
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// 板ポリゴン描画クラス
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラス
{
	using namespace DirectX::SimpleMath;
	// HPバーの頂点情報を設定する
	m_hpbarVert[0] = { Vector3(HPBAR_X_MIN,HPBAR_Y_MAX,0.0f),Vector2(0.0f,0.0f) };	// 左上
	m_hpbarVert[1] = { Vector3(HPBAR_X_MAX,HPBAR_Y_MAX,0.0f),Vector2(1.0f,0.0f) };	// 右上
	m_hpbarVert[2] = { Vector3(HPBAR_X_MAX,HPBAR_Y_MIN,0.0f),Vector2(1.0f,1.0f) };	// 右下
	m_hpbarVert[3] = { Vector3(HPBAR_X_MIN,HPBAR_Y_MIN,0.0f),Vector2(0.0f,1.0f) };	// 左下
	// HPバーの背景の頂点情報を設定する
	m_hpbarBackVert[0] = { Vector3(HPBARBACK_X_MIN,HPBARBACK_Y_MAX,0.0f),Vector2(0.0f,0.0f) };// 左上
	m_hpbarBackVert[1] = { Vector3(HPBARBACK_X_MAX,HPBARBACK_Y_MAX,0.0f),Vector2(1.0f,0.0f) };	// 右上
	m_hpbarBackVert[2] = { Vector3(HPBARBACK_X_MAX,HPBARBACK_Y_MIN,0.0f),Vector2(1.0f,1.0f) };	// 右下
	m_hpbarBackVert[3] = { Vector3(HPBARBACK_X_MIN,HPBARBACK_Y_MIN,0.0f),Vector2(0.0f,1.0f) };// 左下

}

/*
*	@brief	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param	なし
*	@return なし
*/
EnemyHPBar::~EnemyHPBar()
{
	// 共通リソースをnullptrに設定
	m_pCommonResources = nullptr;
}
/*
*	@brief	シェーダーを作成する
*	@details	敵HPバーのシェーダーを作成する
*	@param	なし
*	@return なし
*/
void EnemyHPBar::CreateShaders()
{
	// シェーダー作成クラスの初期化
	m_pCreateShader->Initialize(m_pCommonResources->GetDeviceResources()->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// 頂点シェーダー作成
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/EnemyHP/VS_EnemyHP.cso", m_pVertexShader);
	// ピクセルシェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/EnemyHP/PS_EnemyHP.cso", m_pPixelShader);
	// インプットレイアウトを受け取る
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// シェーダーにデータを渡すためのコンスタントバッファ生成
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ConstBuffer));
	// シェーダーの構造体に頂点シェーダーを渡す
	m_shaders.vs = m_pVertexShader.Get();
	// シェーダーの構造体にピクセルシェーダーを渡す
	m_shaders.ps = m_pPixelShader.Get();
	// シェーダーの構造体にジオメトリシェーダーを渡す（使わないのでnullptr）
	m_shaders.gs = nullptr;
}
/*
*	@brief	初期化
*	@details	敵HPバーの初期化
*	@param	CommonResources* resources	共通リソース
*	@return なし
*/
void EnemyHPBar::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースを取得
	m_pCommonResources = resources;
	// 板ポリゴン描画用クラスの初期化
	m_pDrawPolygon->InitializePositionTexture(m_pCommonResources->GetDeviceResources());
	// シェーダーの作成
	CreateShaders();
	// テクスチャの取得
	m_pGaugeTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("EnemyHPBar"));
	// 初期HPを設定
	m_displayedHP = (float)(m_maxHP);
}
/*
*	@brief	更新
*	@details	敵HPバーの更新
*	@param	float elapsedTime	経過時間
*	@return なし
*/
void EnemyHPBar::Update(float elapsedTime)
{
	// 経過時間を加算
	m_time += elapsedTime;
	// HPを滑らかに更新するための線形補間
	m_displayedHP = Lerp(m_displayedHP, static_cast<float>(m_currentHP), elapsedTime * m_lerpSpeed);
	// 表示HPに基づいてHP割合を計算
	float hpPercentage = m_displayedHP / static_cast<float>(m_maxHP);
	// HPバーの最大時の幅を設定 
	float maxBarWidth = 3.0f;
	// HPバーの幅を計算
	float currentBarWidth = maxBarWidth * hpPercentage;
	// HPバーの頂点を設定 
	m_hpbarVert[0].position.x = BAR_LEFT;                    // 左上
	m_hpbarVert[1].position.x = BAR_LEFT + currentBarWidth;  // 右上
	m_hpbarVert[2].position.x = BAR_LEFT + currentBarWidth;  // 右下
	m_hpbarVert[3].position.x = BAR_LEFT;                    // 左下
	// HPバーの幅が0以下になったら死亡
	if (m_hpbarVert[1].position.x <= BAR_LEFT)m_isDead = true;
}
/*
*	@brief	描画
*	@details	敵HPバーの描画
*	@param	DirectX::SimpleMath::Matrix view	ビュー行列
*	@param	DirectX::SimpleMath::Matrix proj	プロジェクション行列
*	@param	DirectX::SimpleMath::Vector3 pos	座標
*	@param	DirectX::SimpleMath::Vector3 rot	回転
*	@return なし
*/
void EnemyHPBar::Render(
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj,
	const DirectX::SimpleMath::Vector3& pos,
	const DirectX::SimpleMath::Vector3& rot)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ビルボード行列を作成
	Matrix billboardMatrix = view.Invert();
	billboardMatrix._41 = 0.0f;
	billboardMatrix._42 = 0.0f;
	billboardMatrix._43 = 0.0f;
	// ビルボードをアフィン変換
	Matrix worldBillboard{};
	// ビルボード行列を代入
	worldBillboard = billboardMatrix;
	// スケールをかける
	worldBillboard *= Matrix::CreateScale(m_scale);
	// 回転をかける
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(-rot.y * 2.0f));
	// ビルボードの位置を設定
	worldBillboard *= Matrix::CreateTranslation(pos);
	// 回転をかける
	worldBillboard *= Matrix::CreateRotationY(XMConvertToRadians(rot.y * 2.0f));
	// ビュー設定
	m_constBuffer.matView = view.Transpose();
	// プロジェクション設定
	m_constBuffer.matProj = proj.Transpose();
	// ワールド設定
	m_constBuffer.matWorld = worldBillboard.Transpose();
	// 描画前設定
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// サンプラーステート
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// ブレンドステート
		DrawPolygon::RasterizerStates::CULL_NONE,// ラスタライザーステート
		DrawPolygon::DepthStencilStates::DEPTH_NONE);	// 深度ステンシルステート
	// HPbar(背景)描画///////////////////////////////////////////////////////////////////////////////
	// 色設定 
	m_constBuffer.colors = SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	// シェーダーに渡すバッファを作成
	CreateBuffer();
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pGaugeTexture);
	// ポリゴンを描画
	m_pDrawPolygon->DrawTexture(m_hpbarBackVert);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
	// HPbar(緑)描画///////////////////////////////////////////////////////////////////////////////
	// 色設定
	m_constBuffer.colors = SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	// シェーダーに渡すバッファを作成
	CreateBuffer();
	// 描画準備
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pGaugeTexture);
	// ポリゴンを描画
	m_pDrawPolygon->DrawTexture(m_hpbarVert);
	// シェーダの登録を解除しておく
	m_pDrawPolygon->ReleaseShader();
}

/*
*	@brief	シェーダーに渡すバッファを作成
*	@details	敵HPバーのシェーダーに渡すバッファを作成する
*	@param	なし
*	@return なし
*/
void EnemyHPBar::CreateBuffer()
{
	using namespace DirectX::SimpleMath;
	// 時間設定
	m_constBuffer.hp = Vector4(float(m_currentHP), float(m_maxHP), 0, 0);
	// 受け渡し用バッファの内容更新
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constBuffer);
	// シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// 頂点シェーダもピクセルシェーダも、同じ値を渡す
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// シェーダをセットする
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
}