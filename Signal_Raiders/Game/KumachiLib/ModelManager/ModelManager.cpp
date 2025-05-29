/*
*	@file ModelManager.cpp
*	@brief モデルマネージャークラス
*/
#include <pch.h>
#include "ModelManager.h"

/*
*	@brief コンストラクタ
*	@details モデルマネージャークラスのコンストラクタ
*	@param なし
*	@return なし
*/
ModelManager::ModelManager()
	: m_pCommonResources(nullptr) // 共通リソース
	, m_pDevice(nullptr) // デバイス
	, m_pModelMap() // モデルのマップ
	, m_pEffectFactory(nullptr) // エフェクトファクトリー
{
}
/*
*	@brief デストラクタ
*	@details モデルマネージャークラスのデストラクタ
*	@param なし
*	@return なし
*/
ModelManager::~ModelManager()
{
	m_pModelMap.clear(); // モデルのマップをクリア
	m_pEffectFactory.reset(); // エフェクトファクトリーをリセット
	m_pDevice = nullptr; // デバイスをnullptrに設定
	m_pCommonResources = nullptr; // 共通リソースをnullptrに設定
}
/*
*	@brief モデルの初期化
*	@details 各種モデルのロードとエフェクトの設定を行う
*	@param なし
*	@return なし
*/
void ModelManager::Initialize()
{
	m_pDevice = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// デバイスを取得
	m_pEffectFactory = std::make_unique<DirectX::EffectFactory>(m_pDevice);// エフェクトファクトリーの作成
	m_pEffectFactory->SetSharing(false);// エフェクトの共有を無効にする
	CreateBulletModels(); // 弾モデルの作成
	CreateEnemyModels(); // 敵モデルの作成
	CreateVerticalAttackerModels(); // 垂直攻撃敵モデルの作成
	CreateBossModels(); // ボスモデルの作成
	CreateLastBossModels(); // ラスボスモデルの作成
	CreateBarrierModels(); // バリアモデルの作成
	CreateStageModels(); // ステージモデルの作成
}

/*
*	@brief 弾モデルの作成
*	@details プレイヤーと敵の弾モデルを作成し、エフェクトを設定する
*	@param なし
*	@return なし
*/
void ModelManager::CreateBulletModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models");// モデルのディレクトリを指定
	m_pModelMap["PlayerBullet"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Bullet.cmo", *m_pEffectFactory);// 弾モデルを読み込む
	m_pModelMap["PlayerBullet"]->UpdateEffects([&](DirectX::IEffect* effect)// エフェクトの更新
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			basicEffect->SetDiffuseColor(DirectX::Colors::SkyBlue);
			basicEffect->SetEmissiveColor(DirectX::Colors::Cyan);
		});// 自弾モデルのエフェクトを設定する
	m_pModelMap["EnemyBullet"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Bullet.cmo", *m_pEffectFactory);// 弾モデルを読み込む
	m_pModelMap["EnemyBullet"]->UpdateEffects([&](DirectX::IEffect* effect)	// モデルのエフェクトを設定する
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			basicEffect->SetDiffuseColor(DirectX::SimpleMath::Vector4(1, 0.2f, 0, 1));
			basicEffect->SetAmbientLightColor(DirectX::Colors::Red);
			basicEffect->SetEmissiveColor(DirectX::Colors::Tomato);
		});// 敵弾モデルのエフェクトを設定する
}
/*
*	@brief 敵モデルの作成
*	@details 敵のモデルを作成する
*	@param なし
*	@return なし
*/
void ModelManager::CreateEnemyModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models/Enemy");// モデルのディレクトリを指定
	m_pModelMap["EnemyHead"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_Head.cmo", *m_pEffectFactory);// 敵の頭モデルを読み込む
	m_pModelMap["EnemyAntenna"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_Antenna.cmo", *m_pEffectFactory);// 敵のアンテナモデルを読み込む
	m_pModelMap["EnemyHand"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_Hand.cmo", *m_pEffectFactory);// 敵の手モデルを読み込む
	m_pModelMap["EnemyShadow"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy.cmo", *m_pEffectFactory);// 敵の影用モデルを読み込む
	m_pModelMap["EnemyDamage"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *m_pEffectFactory);// 敵のダメージ顔モデルを読み込む
	m_pModelMap["EnemyAttack"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *m_pEffectFactory);// 敵の攻撃顔モデルを読み込む
	m_pModelMap["EnemyIdling"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *m_pEffectFactory);// 敵の普段の顔モデルを読み込む
}
/*
*	@brief 垂直攻撃敵モデルの作成
*	@details 垂直攻撃敵のモデルを作成する
*	@param なし
*	@return なし
*/
void ModelManager::CreateVerticalAttackerModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models/VerticalAttacker");// モデルのディレクトリを指定
	m_pModelMap["VerticalAttacker"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/VerticalAttacker/VerticalAttacker.cmo", *m_pEffectFactory);// 垂直攻撃敵のモデルを読み込む
}
/*
*	@brief ボスモデルの作成
*	@details ボスのモデルを作成する
*	@param なし
*	@return なし
*/
void ModelManager::CreateBossModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models/Boss");// モデルのディレクトリを指定
	m_pModelMap["BossBody"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/Boss.cmo", *m_pEffectFactory);// ボスの胴体モデルを読み込む
	m_pModelMap["BossFaceDamage"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/Boss_Face_Damage.cmo", *m_pEffectFactory);// ボスのダメージ顔モデルを読み込む
	m_pModelMap["BossFaceAttack"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/Boss_Face_Attack.cmo", *m_pEffectFactory);// ボスの攻撃顔モデルを読み込む
	m_pModelMap["BossFaceAngry"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/Boss_Face_Angry.cmo", *m_pEffectFactory);// 怒り状態の顔
}
/*
*	@brief ラスボスモデルの作成
*	@details ラスボスのモデルを作成する
*	@param なし
*	@return なし
*/
void ModelManager::CreateLastBossModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models/Boss");// モデルのディレクトリを指定
	m_pModelMap["LastBossBody"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/LastBoss_Body.cmo", *m_pEffectFactory);// ラスボスの胴体モデルを読み込む
	m_pModelMap["LastBossFaceDamage"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/LastBoss_DamageFace.cmo", *m_pEffectFactory);// ラスボスのダメージ顔モデルを読み込む
	m_pModelMap["LastBossFaceAttack"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/LastBoss_Face.cmo", *m_pEffectFactory);// ラスボスの攻撃顔モデルを読み込む
	m_pModelMap["LastBossFaceAngry"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/LastBoss_AngryFace.cmo", *m_pEffectFactory);// ラスボスの怒り顔モデルを読み込む
}
/*
*	@brief バリアモデルの作成
*	@details バリアのモデルを作成する
*	@param なし
*	@return なし
*/
void ModelManager::CreateBarrierModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/Models/Boss");// モデルのディレクトリを指定
	m_pModelMap["Barrier"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/Models/Boss/Boss_Barrier.cmo", *m_pEffectFactory);// ボスのバリアモデルを読み込む
}
/*
*	@brief ステージモデルの作成
*	@details ステージのモデルを作成する
*	@param なし
*	@return なし
*/
void ModelManager::CreateStageModels()
{
	m_pEffectFactory->SetDirectory(L"Resources/models/Stage");// モデルのディレクトリを設定
	m_pModelMap["Stage"] = DirectX::Model::CreateFromCMO(m_pDevice, L"Resources/models/Stage/Stage.cmo", *m_pEffectFactory);// ステージモデルを読み込む
	m_pModelMap["Stage"]->UpdateEffects([](DirectX::IEffect* effect)	// モデルのエフェクト情報を更新する
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (!basicEffect)return;// エフェクトがnullptrの場合は処理を終える
			basicEffect->SetLightEnabled(0, false);// ライトを無効にする
			basicEffect->SetLightEnabled(1, false);// ライトを無効にする
			basicEffect->SetLightEnabled(2, false);// ライトを無効にする
			basicEffect->SetEmissiveColor(DirectX::Colors::White);// モデルを自発光させる
		}
	);
}
/*
*	@brief モデルを取得する
*	@details 指定されたキーに対応するモデルを取得する
*	@param key モデルのキー
*	@return 指定されたキーに対応するモデルのポインタ。見つからない場合はnullptrを返す
*/
DirectX::Model* ModelManager::GetModel(const std::string& key)
{
	auto it = m_pModelMap.find(key);// キーに対応するモデルを検索
	if (it != m_pModelMap.end())	return it->second.get();// 見つかった場合はモデルを返す
	return nullptr;// 見つからなかった場合はnullptrを返す
}
/*
*	@brief 指定されたステージの空のモデルを作成して返す
*	@details ステージIDに応じた空のモデルを取得する
*	@param stageID ステージID
*	@return 指定されたステージの空のモデルのポインタ。見つからない場合はnullptrを返す
*/
DirectX::Model* ModelManager::GetSkyModel(const std::string& stageID)
{
	auto it = m_skyModelPaths.find(stageID); // ステージIDに対応する空のモデルのパスを検索
	if (it == m_skyModelPaths.end())return nullptr; // 見つからなかった場合はnullptrを返す
	m_pEffectFactory->SetDirectory(L"Resources/Models/sky");// モデルのディレクトリを指定
	std::wstring wpath = ConvertToWString(it->second); // 文字列をワイド文字列に変換
	m_pModelMap["Sky"] = DirectX::Model::CreateFromCMO(m_pDevice, wpath.c_str(), *m_pEffectFactory); // モデルを保持
	return m_pModelMap["Sky"].get(); // モデルを返す
}
/*
*	@brief 文字列をワイド文字列に変換する
*	@details std::stringをstd::wstringに変換する
*	@param str 変換する文字列
*	@return 変換後のワイド文字列
*/
std::wstring ModelManager::ConvertToWString(const std::string& str) { return std::wstring(str.begin(), str.end()); }