/*
*	@file	CowardlyEnemyModel.cpp
*	@brief	すばしっこくて卑怯な敵モデルクラス
*/
#include <pch.h>
#include "CowardlyEnemyModel.h"

/*
*	@brief	コンストラクタ
*	@details	すばしっこくて卑怯な敵モデルクラス
*	@param	なし
*/
CowardlyEnemyModel::CowardlyEnemyModel()
	: m_pCommonResources(nullptr)// 共通リソース
	, m_pBodyModel(nullptr)// モデル
	, m_pCreateShader(CreateShader::GetInstance())// シェーダー作成クラスのポインター
	, m_nowState(IState::EnemyState::IDLING)// 現在のステータス

{}
/*
*	@brief	デストラクタ
*	@details	垂直攻撃敵モデルの解放を行う
*	@param	なし
*/
CowardlyEnemyModel::~CowardlyEnemyModel()
{
	// 共通リソースの解放
	m_pCommonResources = nullptr;
	// モデルの解放
	m_pBodyModel = nullptr;
	// 顔モデルマップの解放
	m_pFaceModelMap.clear();
	// ピクセルシェーダーの解放
	m_pPixelShader.Reset();
	// ステータスを初期化
	m_nowState = IState::EnemyState::IDLING;
}
/*
*	@brief	初期化する
*	@details	すばしっこくて卑怯な敵モデルクラスの初期化を行う
*	@param	resources	共通リソース
*	@return	なし
*/
void CowardlyEnemyModel::Initialize(CommonResources* resources)
{
	// 共通リソースを設定
	m_pCommonResources = resources;
	// デバイスを取得
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// シェーダー作成クラスを初期化
	m_pCreateShader->Initialize(device);
	// 影用シェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Shadow/PS_Shadow.cso", m_pPixelShader);
	// モデルマネージャーから垂直攻撃敵のモデルを取得
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("CowardlyEnemy");
	// モデルマネージャーからダメージ顔モデルを取得
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("EnemyDamage");
	// モデルマネージャーから攻撃顔モデルを取得
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("EnemyAttack");
	// モデルマネージャーから攻撃顔モデルを取得
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("EnemyAngry");
	// モデルマネージャーから攻撃顔モデルを取得
	m_pFaceModelMap[IState::EnemyState::IDLING] = m_pCommonResources->GetModelManager()->GetModel("EnemyIdling");
}
/*
*	@brief	描画する
*	@param	context	デバイスコンテキスト
*	@param	states	共通ステート
*	@param	world	ワールド行列
*	@param	view	ビュー行列
*	@param	proj	プロジェクション行列
*	@return なし
*/
void CowardlyEnemyModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	const DirectX::SimpleMath::Matrix& world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 胴体を描画
	m_pBodyModel->Draw(context, *states, world, view, proj);
	// ライトの方向設定
	Vector3 lightDir = Vector3::UnitY;
	// ライトの方向を正規化
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	// 影用行列を作成
	shadowMatrix = world * shadowMatrix;
	// 影を描画
	m_pBodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			// ブレンドステート
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			// 深度ステンシルステート
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			// カリング
			context->RSSetState(states->CullCounterClockwise());
			// ピクセルシェーダー設定
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
		});
	// ステートによって顔のモデルを変更
	auto it = m_pFaceModelMap.find(m_nowState);
	// マップに存在する場合、顔のモデルを描画
	if (it != m_pFaceModelMap.end() && it->second) it->second->Draw(context, *states, world, view, proj);
}