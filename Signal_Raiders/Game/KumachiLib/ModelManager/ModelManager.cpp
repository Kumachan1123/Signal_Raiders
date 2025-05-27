/*
*	@file ModelManager.cpp
*	@brief モデルマネージャークラス
*/
#include "pch.h"
#include "ModelManager.h"

/*
*	@brief コンストラクタ
*	@details モデルマネージャークラスのコンストラクタ
*/
ModelManager::ModelManager()
	: m_pCommonResources(nullptr) // 共通リソース
	, m_device(nullptr) // デバイス
	, m_modelMap() // モデルのマップ
	, m_effectFactory(nullptr) // エフェクトファクトリー
{
}
/*
*	@brief デストラクタ
*	@details モデルマネージャークラスのデストラクタ
*	@return なし
*/
ModelManager::~ModelManager()
{
	m_modelMap.clear(); // モデルのマップをクリア
	m_effectFactory.reset(); // エフェクトファクトリーをリセット
	m_device = nullptr; // デバイスをnullptrに設定
	m_pCommonResources = nullptr; // 共通リソースをnullptrに設定
}
/*
*	@brief モデルの初期化
*	@details 各種モデルのロードとエフェクトの設定を行う
*	@return なし
*/
void ModelManager::Initialize()
{
	m_device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();// デバイスを取得
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_device);// エフェクトファクトリーの作成
	CreateBulletModels(); // 弾モデルの作成
	CreateEnemyModels(); // 敵モデルの作成
	CreateVerticalAttackerModels(); // 垂直攻撃敵モデルの作成
	CreateBossModels(); // ボスモデルの作成
	CreateLastBossModels(); // ラスボスモデルの作成
}
/*
*	@brief 弾モデルの作成
*	@details プレイヤーと敵の弾モデルを作成し、エフェクトを設定する
*	@param なし
*	@return なし
*/
void ModelManager::CreateBulletModels()
{
	m_effectFactory->ReleaseCache();// キャッシュを解放する
	m_effectFactory->SetDirectory(L"Resources/Models");// モデルのディレクトリを指定
	m_modelMap["PlayerBullet"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Bullet.cmo", *m_effectFactory);// 弾モデルを読み込む
	m_modelMap["PlayerBullet"]->UpdateEffects([&](DirectX::IEffect* effect)// エフェクトの更新
		{
			auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			basicEffect->SetDiffuseColor(DirectX::Colors::SkyBlue);
			basicEffect->SetEmissiveColor(DirectX::Colors::Cyan);
		});// 自弾モデルのエフェクトを設定する
	m_effectFactory->ReleaseCache();// キャッシュを解放する
	m_modelMap["EnemyBullet"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Bullet.cmo", *m_effectFactory);// 弾モデルを読み込む
	m_modelMap["EnemyBullet"]->UpdateEffects([&](DirectX::IEffect* effect)	// モデルのエフェクトを設定する
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
	m_effectFactory->ReleaseCache(); // キャッシュを解放する
	m_effectFactory->SetDirectory(L"Resources/Models/Enemy");// モデルのディレクトリを指定
	m_modelMap["EnemyHead"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_Head.cmo", *m_effectFactory);// 敵の頭モデルを読み込む
	m_modelMap["EnemyAntenna"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_Antenna.cmo", *m_effectFactory);// 敵のアンテナモデルを読み込む
	m_modelMap["EnemyHand"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_Hand.cmo", *m_effectFactory);// 敵の手モデルを読み込む
	m_modelMap["EnemyShadow"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy.cmo", *m_effectFactory);// 敵の影用モデルを読み込む
	m_modelMap["EnemyDamage"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_DamageFace.cmo", *m_effectFactory);// 敵のダメージ顔モデルを読み込む
	m_modelMap["EnemyAttack"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_AttackFace.cmo", *m_effectFactory);// 敵の攻撃顔モデルを読み込む
	m_modelMap["EnemyIdling"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Enemy/Enemy_IdlingHead.cmo", *m_effectFactory);// 敵の普段の顔モデルを読み込む
}
/*
*	@brief 垂直攻撃敵モデルの作成
*	@details 垂直攻撃敵のモデルを作成する
*	@param なし
*	@return なし
*/
void ModelManager::CreateVerticalAttackerModels()
{
	m_effectFactory->ReleaseCache(); // キャッシュを解放する
	m_effectFactory->SetDirectory(L"Resources/Models/VerticalAttacker");// モデルのディレクトリを指定
	m_modelMap["VerticalAttacker"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/VerticalAttacker/VerticalAttacker.cmo", *m_effectFactory);// 垂直攻撃敵のモデルを読み込む
}
/*
*	@brief ボスモデルの作成
*	@details ボスのモデルを作成する
*	@param なし
*	@return なし
*/
void ModelManager::CreateBossModels()
{
	m_effectFactory->ReleaseCache(); // キャッシュを解放する
	m_effectFactory->SetDirectory(L"Resources/Models/Boss");// モデルのディレクトリを指定
	m_modelMap["BossBody"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Boss/Boss.cmo", *m_effectFactory);// ボスの胴体モデルを読み込む
	m_modelMap["BossFaceDamage"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Boss/Boss_Face_Damage.cmo", *m_effectFactory);// ボスのダメージ顔モデルを読み込む
	m_modelMap["BossFaceAttack"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Boss/Boss_Face_Attack.cmo", *m_effectFactory);// ボスの攻撃顔モデルを読み込む
	m_modelMap["BossFaceAngry"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Boss/Boss_Face_Angry.cmo", *m_effectFactory);// 怒り状態の顔
}
/*
*	@brief ラスボスモデルの作成
*	@details ラスボスのモデルを作成する
*	@param なし
*	@return なし
*/
void ModelManager::CreateLastBossModels()
{
	m_effectFactory->ReleaseCache(); // キャッシュを解放する
	m_effectFactory->SetDirectory(L"Resources/Models/LastBoss");// モデルのディレクトリを指定
	m_modelMap["LastBossBody"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/LastBoss/LastBoss_Body.cmo", *m_effectFactory);// ラスボスの胴体モデルを読み込む
	m_modelMap["LastBossFaceDamage"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/LastBoss/LastBoss_Face.cmo", *m_effectFactory);// ラスボスのダメージ顔モデルを読み込む
	m_modelMap["LastBossFaceAttack"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/LastBoss/LastBoss_AttackFace.cmo", *m_effectFactory);// ラスボスの攻撃顔モデルを読み込む
	m_modelMap["LastBossFaceAngry"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/LastBoss/LastBoss_AngryFace.cmo", *m_effectFactory);// ラスボスの怒り顔モデルを読み込む
}
/*
*	@brief バリアモデルの作成
*	@details バリアのモデルを作成する
*	@param なし
*	@return なし
*/
void ModelManager::CreateBarrierModels()
{
	m_effectFactory->ReleaseCache(); // キャッシュを解放する
	m_effectFactory->SetDirectory(L"Resources/Models/Boss");// モデルのディレクトリを指定
	m_modelMap["Barrier"] = DirectX::Model::CreateFromCMO(m_device, L"Resources/Models/Boss/Boss_Barrier.cmo", *m_effectFactory);// ボスのバリアモデルを読み込む
}

DirectX::Model* ModelManager::GetModel(const std::string& key)
{
	auto it = m_modelMap.find(key);
	if (it != m_modelMap.end())	return it->second.get();
	return nullptr;
}
