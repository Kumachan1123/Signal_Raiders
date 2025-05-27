/*
*	@file	Sky.cpp
*	@brief	スカイクラス
*/
#include "pch.h"
#include "Sky.h"
/*
*	@brief	コンストラクタ
*	@details スカイのコンストラクタ
*	@param StageID ステージID
*	@return なし
*/
Sky::Sky(int StageID)
	: m_pCommonResources{}// 共通リソース
	, m_model{}// モデル
	, m_texturePath{}// テクスチャパス
	, m_stageID{ StageID }// ステージID
{
}
/*
*	@brief	デストラクタ
*	@details スカイのデストラクタ
*	@param なし
*	@return なし
*/
Sky::~Sky() {/*/do nothing.*/ }
/*
*	@brief	初期化
*	@details 空の初期化
*	@param resources 共通リソース
*	@return なし
*/
void Sky::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(resources);// リソースがnullptrでないことを確認
	m_pCommonResources = resources;// リソースを保存
	auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// デバイスを取得
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);// モデルを読み込む準備
	fx->SetDirectory(L"Resources/models/sky");// モデルのディレクトリを設定
	auto it = m_texturePathMap.find(m_stageID);// ステージIDに応じた空のモデルのパスを取得
	if (it != m_texturePathMap.end()) // マップの要素が見つかった場合
	{
		std::wstring wpath = ConvertToWString(it->second);// ステージIDに応じた空のモデルのパスを取得
		m_model = Model::CreateFromCMO(device, wpath.c_str(), *fx);// モデルを読み込む
	}
	m_model->UpdateEffects([](DirectX::IEffect* effect)	// モデルのエフェクト情報を更新する
		{
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);// ベーシックエフェクトを設定する
			if (!basicEffect)return;// エフェクトがnullptrの場合は処理を終える
			basicEffect->SetLightEnabled(0, false);// ライトを無効にする
			basicEffect->SetLightEnabled(1, false);// ライトを無効にする
			basicEffect->SetLightEnabled(2, false);// ライトを無効にする
			basicEffect->SetEmissiveColor(Colors::White);// エミッシブカラーを設定する
		}
	);
}
/*
*	@brief	描画
*	@details スカイの描画
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@param world ワールド行列
*	@param pos スカイの位置
*	@return なし
*/
void Sky::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
	DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Vector3 pos)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();// デバイスコンテキストを取得
	auto states = m_pCommonResources->GetCommonStates();// 共通ステートを取得
	world *= Matrix::CreateTranslation(pos);// ワールド行列を更新
	m_model->Draw(context, *states, world, view, proj);// モデルを描画する
}
/*
*	@brief	文字列変換
*	@details 文字列をワイド文字列に変換する
*	@param str 変換する文字列
*	@return ワイド文字列
*/
std::wstring Sky::ConvertToWString(const std::string& str) { return std::wstring(str.begin(), str.end()); }