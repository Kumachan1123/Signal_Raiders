/*
*	@file	GameEndChecker.h
*	@brief	ゲーム終了前に再確認するUIクラス
*/
#pragma once
#ifndef GAMEENDCHECKER_DEFINED
#define GAMEENDCHECKER_DEFINED
// 標準ライブラリ
#include <vector>
#include <string>
// DirectX
#include <DeviceResources.h>
#include <Mouse.h>
#include <WICTextureLoader.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"

// 前方宣言
class CommonResources;

// メニュークラス
class GameEndChecker : public IMenuUI
{
public:
	// 構造体
	// UIの変形
	struct UITransForm
	{
		DirectX::SimpleMath::Vector2 position;// 位置
		DirectX::SimpleMath::Vector2 scale;// スケール
	};

public:
	// アクセサ
	// ゲームをやめるかどうか取得
	bool GetIsEndGame() const { return m_isEndGame; }
	// ゲームをやめるかどうか設定
	void SetIsEndGame(bool isEndGame) { m_isEndGame = isEndGame; }
	// このクラスの処理を行うか取得
	bool GetIsGameEndCheck() const { return m_isGameEndCheck; }
	// このクラスの処理を行うか設定
	void SetIsGameEndCheck(bool isGameEndCheck) { m_isGameEndCheck = isGameEndCheck; }
	// ステージを選びなおすかどうか取得
	bool GetIsStageSelect() const { return m_isStageSelect; }
	// ステージを選びなおすかどうか設定
	void SetIsStageSelect(bool isStageSelect) { m_isStageSelect = isStageSelect; }
	// UIにヒットしたかどうか取得
	bool GetIsHit()const { return m_hit; }
	// UIにヒットしたかどうか設定
	void SetIsHit(bool hit) { m_hit = hit; }
	// SEの音量設定
	void SetSEVolume(float volume) { m_SEVolume = volume; }
public:
	// public関数
	// コンストラクタ
	GameEndChecker();
	// デストラクタ
	~GameEndChecker();
	// 初期化
	void Initialize(CommonResources* resources, int width, int height);
	// 更新
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }
	// 描画
	void Render();
	// UI追加
	void Add(const std::string& key
		, const DirectX::SimpleMath::Vector2& position
		, const DirectX::SimpleMath::Vector2& scale
		, KumachiLib::ANCHOR anchor
		, IMenuUI::UIType type);
private:
	// private関数
	// 更新
	void Update(float elapsedTime);
private:
	// private定数
	// 無効なメニューインデックス
	static const int INVALID_MENU_INDEX;
private:
	// private変数
	// メニューのインデックス
	unsigned int m_menuIndex;
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 選択可能UI
	std::vector<std::unique_ptr<UI>> m_pUI;
	// 選択不可能UI
	std::vector<std::unique_ptr<UI>> m_pNonSelectUI;
	// 選択中のUIに表示する画像
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// 選択中のUIに表示する画像のパス
	const wchar_t* m_pSelectTexturePath;
	// ウィンドウの幅・高さ 
	int m_windowWidth, m_windowHeight;
	// UIの座標などの情報を格納する配列
	std::vector<UITransForm> m_transforms;
	// ゲームをやめるかどうか
	bool m_isEndGame;
	// 時間
	float m_time;
	// 当たったかどうか
	bool m_hit;
	// SEの音量
	float m_SEVolume;
	// 再生フラグ
	bool m_isSEPlay;
	// このクラスの処理を行うかのフラグ
	bool m_isGameEndCheck;
	// ステージを選びなおすかどうかのフラグ
	bool m_isStageSelect;
};
#endif // GAMEENDCHECKER_DEFINED_DEFINED