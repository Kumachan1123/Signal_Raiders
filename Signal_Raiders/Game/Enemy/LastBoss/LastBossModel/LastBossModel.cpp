/*
*	@file LastBossModel.cpp
*	@brief ラスボスモデルクラス
*/
#include <pch.h>
#include "LastBossModel.h"
/*
*	@breif	コンストラクタ
*	@details ラスボスモデルクラスのコンストラクタ
*	@param なし
*	@return	なし
*/
LastBossModel::LastBossModel()
	: m_pCommonResources{}// 共通リソース
	, m_pCreateShader{ CreateShader::GetInstance() }// シェーダー作成クラスのポインター
	, m_pBodyModel{}// 胴体
	, m_pPixelShader{}// 影用のピクセルシェーダー
	, m_nowState(IState::EnemyState::IDLING)// 現在のステート
{}

/*
*	@breif	デストラクタ
*	@details 各種ポインターをnullptrに設定
*	@param なし
*   @return	なし
*/
LastBossModel::~LastBossModel()
{
	// 共通リソースを解放
	m_pCommonResources = nullptr;
	// 胴体モデルを解放
	m_pBodyModel = nullptr;
	// ピクセルシェーダーをリセット
	m_pPixelShader.Reset();
	// 顔のモデルマップをクリア
	m_pFaceModelMap.clear();
}
/*
*	@breif	初期化
*	@details ラスボスモデルクラスの初期化
*	@param	CommonResources* resources 共通リソース
*   @return	なし
*/
void LastBossModel::Initialize(CommonResources* resources)
{
	// 共通リソースを取得
	m_pCommonResources = resources;
	// デバイスを取得
	auto device = resources->GetDeviceResources()->GetD3DDevice();
	// シェーダー作成クラスを初期化
	m_pCreateShader->Initialize(device);
	// 影用のピクセルシェーダーを読み込み
	m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Shadow/PS_Shadow.cso", m_pPixelShader);
	// 胴体モデルをマネージャーから取得
	m_pBodyModel = m_pCommonResources->GetModelManager()->GetModel("LastBossBody");
	// ダメージ顔モデルをマネージャーから取得
	m_pFaceModelMap[IState::EnemyState::HIT] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceDamage");
	// 攻撃顔モデルをマネージャーから取得
	m_pFaceModelMap[IState::EnemyState::ATTACK] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceAttack");
	// 怒り顔モデルをマネージャーから取得
	m_pFaceModelMap[IState::EnemyState::ANGRY] = m_pCommonResources->GetModelManager()->GetModel("LastBossFaceAngry");
}
/*
*	@breif	描画
*	@details ラスボスモデルの描画
*	@param	ID3D11DeviceContext1* context デバイスコンテキスト
*	@param	DirectX::DX11::CommonStates* states コモンステート
*	@param	DirectX::SimpleMath::Matrix world ワールド行列
*	@param	DirectX::SimpleMath::Matrix view ビュー行列
*	@param	DirectX::SimpleMath::Matrix proj プロジェクション行列
*	@return	なし
*/
void LastBossModel::Render(ID3D11DeviceContext1* context,
	DirectX::DX11::CommonStates* states,
	const DirectX::SimpleMath::Matrix& world,
	const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ライトの方向を設定
	Vector3 lightDir = Vector3::UnitY;
	// ライトの方向を正規化
	lightDir.Normalize();
	// 影行列の元を作る
	Matrix shadowMatrix = Matrix::CreateShadow(Vector3::UnitY, Plane(0.0f, 1.0f, 0.0f, 0.01f));
	// 影行列をワールド行列に適用
	shadowMatrix = world * shadowMatrix;
	// 影wを描画
	m_pBodyModel->Draw(context, *states, shadowMatrix * Matrix::Identity, view, proj, true, [&]()
		{
			// ブレンドステート
			context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
			// 深度ステンシルステート
			context->OMSetDepthStencilState(states->DepthNone(), 0);
			// ラスタライザーステート
			context->RSSetState(states->CullClockwise());
			// ピクセルシェーダー
			context->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
		});
	// 胴体を描画
	m_pBodyModel->Draw(context, *states, world, view, proj);
	// ステートによって顔のモデルを変更
	auto it = m_pFaceModelMap.find(m_nowState);
	// マップに存在する場合
	if (it != m_pFaceModelMap.end() && it->second)
	{
		// 顔のモデルを描画
		it->second->Draw(context, *states, world, view, proj);
	}
}
