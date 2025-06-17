/*
*	@file	EnemyModel.cpp
*	@brief	敵モデルクラス
*/
#include <pch.h>
#include "EnemyModel.h"
/*
*	@brief	コンストラクタ
*	@details 敵モデルクラスのコンストラクタ
*	@param	なし
*	@return	なし
*/
EnemyModel::EnemyModel()
	: m_pCommonResources{}// 共通リソース
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラスのポインター
	, m_pBodyModel{}// 胴体モデル
	, m_pAntennaModel{}// アンテナモデル
	, m_pHandModel{}// 手モデル
	, m_pShadowModel{}// 影用のモデル
	, m_nowState{ IState::EnemyState::IDLING }// 現在の状態
{}
/*
*	@brief	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param	なし
*	@return	なし
*/
EnemyModel::~EnemyModel()
{
	// 共通リソースを解放
	m_pCommonResources = nullptr;
	// 胴体モデルを解放
	m_pBodyModel = nullptr;
	// アンテナモデルを解放
	m_pAntennaModel = nullptr;
	// 手モデルを解放
	m_pHandModel = nullptr;
	// 影用のモデルを解放
	m_pShadowModel = nullptr;
	// ピクセルシェーダーをリセット
	m_pPixelShader.Reset();
	// 顔のモデルマップをクリア
	m_pFaceModelMap.clear();
}


/*
*	@brief	初期化
*	@details 敵モデルクラスの初期化
*	@param	CommonResources* resources 共通リソース
*	@return	なし
*/
void EnemyModel::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソース取得
	m_pCommonResources = resources;
	// デバイス取得
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// シェーダー作成クラスを初期化
	m_pCreateShader->Initialize(device);
	// 影用シェーダー作成
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Shadow/PS_Shadow.cso", m_pPixelShader);
	// モデルマネージャーから頭モデルを取得
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("EnemyHead");
	// モデルマネージャーからアンテナモデルを取得
	m_pAntennaModel = m_pCommonResources->GetModelManager()->GetModel("EnemyAntenna");
	// モデルマネージャーから手モデルを取得
	m_pHandModel = m_pCommonResources->GetModelManager()->GetModel("EnemyHand");
	// モデルマネージャーから影用モデルを取得
	m_pShadowModel = m_pCommonResources->GetModelManager()->GetModel("EnemyShadow");
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
*	@brief	描画
*	@details 敵モデルの描画を行う
*	@param	ID3D11DeviceContext1* context コンテキスト
*	@param	DirectX::DX11::CommonStates* states ステート
*	@param	DirectX::SimpleMath::Matrix world ワールド行列
*	@param	DirectX::SimpleMath::Matrix view ビュー行列
*	@param	DirectX::SimpleMath::Matrix proj プロジェクション行列
*	@return	なし
*/
void EnemyModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	const DirectX::SimpleMath::Matrix& world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 体の描画
	m_pBodyModel->Draw(context, *states, world, view, proj);
	// アンテナの描画
	m_pAntennaModel->Draw(context, *states, world, view, proj);
	// 手の描画
	m_pHandModel->Draw(context, *states, world, view, proj);
	// ライトの方向を設定
	Vector3 lightDir = Vector3::UnitY;
	// ライトの方向を正規化
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	// 影の行列をワールド行列にかける
	shadowMatrix = world * shadowMatrix;
	// 影の描画
	m_pShadowModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			// ブレンドステートを不透明にする
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			// 深度ステンシルステートを無効にする
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			// カリングを無効にする
			context->RSSetState(states->CullNone());
			// ピクセルシェーダーをセット
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
		});
	// ステートによって顔のモデルを変更
	auto it = m_pFaceModelMap.find(m_nowState);
	// マップに存在する場合、顔のモデルを描画
	if (it != m_pFaceModelMap.end() && it->second)it->second->Draw(context, *states, world, view, proj);
}