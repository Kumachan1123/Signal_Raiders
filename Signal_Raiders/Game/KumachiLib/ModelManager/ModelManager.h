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
	DirectX::Model* GetSkyModel(const std::string& stageID);// 指定されたステージの空のモデルを作成して返す
	void SetCommonResources(CommonResources* commonResources) { m_pCommonResources = commonResources; } // 共通リソースを設定する
public:// public関数
	ModelManager();	// コンストラクタ
	~ModelManager();// デストラクタ
	void Initialize();// モデルの初期化
private:
	std::wstring ConvertToWString(const std::string& str);// 文字列変換
	void CreateBulletModels(); // 弾モデルの作成
	void CreateEnemyModels(); // 敵モデルの作成
	void CreateVerticalAttackerModels(); // 垂直攻撃敵モデルの作成
	void CreateBossModels(); // ボスモデルの作成
	void CreateLastBossModels(); // ラスボスモデルの作成
	void CreateBarrierModels(); // バリアモデルの作成
	void CreateStageModels(); // ステージモデルの作成
private:// 定数
	const std::unordered_map<std::string, std::string> m_skyModelPaths = // 空モデルのパス
	{
		{"Stage1", "Resources/Models/sky/sky.cmo"}, // ステージ1の空モデル
		{"Stage2", "Resources/Models/sky/CloudySky.cmo"}, // ステージ2の空モデル
		{"Stage3", "Resources/Models/sky/EveningSky.cmo"}, // ステージ3の空モデル
		{"Stage4", "Resources/Models/sky/NightSky.cmo"}, // ステージ4の空モデル
		{"Stage5", "Resources/Models/sky/MidNightSky.cmo"} // ステージ5の空モデル
	};
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// デバイス
	ID3D11Device1* m_pDevice;
	// モデルのマップ
	std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_pModelMap;
	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
};
