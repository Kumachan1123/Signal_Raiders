/*
*	@file ModelManager.h
*	@brief モデルマネージャークラス
*/
#pragma once
// 標準ライブラリ
#include <memory>
#include <unordered_map>
#include <string>
#include <fstream>
// DirectX
#include <DeviceResources.h>

class ModelManager
{
public:// アクセサ
	DirectX::Model* GetModel(const std::string& key);// モデルの取得
	DirectX::Model* GetSkyModel(const std::string& stageID);// 指定されたステージの空のモデルを作成して返す
public:// public関数
	ModelManager();	// コンストラクタ
	~ModelManager();// デストラクタ
	void Initialize(ID3D11Device1* pDevice);// モデルの初期化
private:
	//std::wstring ConvertToWString(const std::string& str);// 文字列変換
	void SetupBulletModelEffects();   // 弾モデルにエフェクト設定
	void SetupStageModelEffects();   // ステージモデルにエフェクト設定
	void LoadJsonFile();// JSONファイルの読み込み
	void LoadSkyModelsJson();//天球モデルのパスをJSONファイルから読み込む
private:// 定数
	const std::unordered_map<std::string, std::string> m_skyModelPaths = // 空モデルのパス
	{
		{"Stage1", "Resources/Models/sky.cmo"			}, // ステージ1の空モデル
		{"Stage2", "Resources/Models/CloudySky.cmo"		}, // ステージ2の空モデル
		{"Stage3", "Resources/Models/EveningSky.cmo"	}, // ステージ3の空モデル
		{"Stage4", "Resources/Models/NightSky.cmo"		}, // ステージ4の空モデル
		{"Stage5", "Resources/Models/MidNightSky.cmo"	} // ステージ5の空モデル
	};
private:// private変数
	// デバイス
	ID3D11Device1* m_pDevice;
	// モデルのマップ
	std::unordered_map<std::string, std::unique_ptr<DirectX::Model>> m_pModelMap;
	// モデルパスのマップ
	std::unordered_map<std::string, std::wstring> m_skyModelPathMap;
	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_pEffectFactory;
};
