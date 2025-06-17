/*
*	@file	BossModel.cpp
*	@brief	ボスモデルクラス
*/
#include <pch.h>
#include "BossModel.h"
/*
*	@breif	コンストラクタ
*	@details ボスモデルクラスのコンストラクタ
*	@param なし
*	@return	なし
*/
BossModel::BossModel()
	: m_pCommonResources{}// 共通リソース
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラスのポインター
	, m_pBodyModel{}// 胴体
	, m_pPixelShader{}// 影用のピクセルシェーダー
	, m_nowState(IState::EnemyState::IDLING)// 現在のステート
{}

/*
*	@breif	デストラクタ
*	@details ボスモデルクラスのデストラクタ
*	@param なし
*	@return	なし
*/
BossModel::~BossModel() {/*do nothing*/ }

/*
*	@breif	初期化
*	@details ボスモデルクラスの初期化
*	@param	CommonResources* resources 共通リソース
*	@return	なし
*/
void BossModel::Initialize(CommonResources* resources)
{
	// 共通リソースを設定
	m_pCommonResources = resources;
	// デバイスリソースからデバイスを取得する
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// シェーダー作成クラスを初期化	
	m_pCreateShader->Initialize(device);
	// 影用のピクセルシェーダーを作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Shadow/PS_Shadow.cso", m_pPixelShader);
	// 胴体モデルをマネージャーから取得
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("BossBody");
	// ダメージ顔モデルをマネージャーから取得
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("BossFaceDamage");
	// 攻撃顔モデルをマネージャーから取得
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("BossFaceAttack");
	// 怒り顔モデルをマネージャーから取得
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("BossFaceAngry");
}
/*
*	@brief	描画
*	@details ボスモデルの描画
*	@param	context  デバイスコンテキスト
*	@param	states	コモンステート
* 　@param	world	ワールド行列
*	@param	view	ビュー行列
*	@param	proj	プロジェクション行列
*	@return	なし
*/
void BossModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	const DirectX::SimpleMath::Matrix& world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// ライトの方向を設定
	Vector3 lightDir = Vector3::UnitY;
	// ライトの方向を正規化
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	// 影行列にワールド行列を掛ける
	shadowMatrix = world * shadowMatrix;
	// 影を描画する
	m_pBodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);// ブレンドステート
			context->OMSetDepthStencilState(states->DepthNone(), 0);// 深度ステンシルステート
			context->RSSetState(states->CullClockwise());// ラスタライザーステート
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);// ピクセルシェーダー
		});
	// 胴体を描画する
	m_pBodyModel->Draw(context, *states, world, view, proj);
	// ステートによって顔のモデルを変更
	auto it = m_pFaceModelMap.find(m_nowState);
	// マップに存在する場合、顔を描画する
	if (it != m_pFaceModelMap.end() && it->second)it->second->Draw(context, *states, world, view, proj);
}
