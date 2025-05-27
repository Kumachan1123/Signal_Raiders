/*
*	@file ModelManager.h
*	@brief モデルマネージャークラス
*/
#pragma once
// 標準ライブラリ
#include <memory>
#include <unordered_map>
#include <string>
// DirectX
#include <DeviceResources.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
// 前方宣言
class CommonResources;
class ModelManager
{
public:// アクセサ
	DirectX::Model* GetModel(const std::string& key);// モデルの取得
	void SetCommonResources(CommonResources* commonResources) { m_pCommonResources = commonResources; } // 共通リソースを設定する
public:// public関数
	ModelManager();	// コンストラクタ
	~ModelManager();// デストラクタ
	void Initialize();// モデルの初期化
private:
	void CreateSkyModels();// 空モデルの作成
	void CreateBulletModels(); // 弾モデルの作成
	void CreateEnemyModels(); // 敵モデルの作成
	void CreateVerticalAttackerModels(); // 垂直攻撃敵モデルの作成
	void CreateBossModels(); // ボスモデルの作成
	void CreateLastBossModels(); // ラスボスモデルの作成
	void CreateBarrierModels(); // バリアモデルの作成
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイス
	ID3D11Device1* m_pDevice;
	// モデルのマップ
	std::unordered_map<std::string, std::shared_ptr<DirectX::Model>> m_pModelMap;
	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
};
