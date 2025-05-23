/*
*	@file	StageSelectMenu.h
*	@brief	リザルトメニュークラス
*/
#pragma once
// 標準ライブラリ
#include <vector>
// DirectX
#include <DeviceResources.h>
#include "Game/CommonResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <Mouse.h>
// 外部ライブラリ
#include "Libraries/MyLib/InputManager.h"
// 自作ヘッダーファイル
#include "Game/UI/UI.h"
#include "Game/Interface/IMenuUI.h"
#include "Game/Screen.h"
#include "Game/UI/UI.h"
#include "Game/KumachiLib/BinaryFile/BinaryFile.h"
// 前方宣言
class CommonResources;
class StageSelectMenu : public IMenuUI
{
public:// 列挙型
	enum SceneID// シーンID
	{
		PLAY = 0,// プレイシーン
		END// タイトルに戻る
	};
public:// アクセサ
	SceneID GetSceneNum() const { return m_num; }// シーン番号取得
	void SetSceneNum(SceneID num) { m_num = num; }// シーン番号設定
	int GetMenuIndex() const { return m_menuIndex; }// メニューインデックス取得
	void SetMenuIndex(int index) { m_menuIndex = index; }// メニューインデックス設定
	bool GetIsHit()const { return m_hit; }// UIにヒットしたかどうか取得
	void SetIsHit(bool hit) { m_hit = hit; }// UIにヒットしたかどうか設定
	void SetSEVolume(float volume) { m_SEVolume = volume; }// SEの音量設定
public:// public関数
	StageSelectMenu();// コンストラクタ
	~StageSelectMenu();// デストラクタ
	void Initialize(CommonResources* resources, int width, int height)override;// 初期化
	void Update(const UpdateContext& context)override { Update(context.elapsedTime); }// 更新(contextから時間を取得して更新)
	void Render()override;// 描画
	void Add(const wchar_t* path// メニューアイテムを追加
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, KumachiLib::ANCHOR anchor
		, UIType type);
private:// private関数
	void Update(float elapsedTime);// 更新
private:// private定数
	static const int INVALID_MENU_INDEX;// 無効なメニューインデックス
private:// private変数
	// デバイスリソース
	DX::DeviceResources* m_pDR;
	// 共通リソース
	CommonResources* m_commonResources;
	// 選択可能UI
	std::vector<std::unique_ptr<UI>> m_pUI;
	// 選択不可能UI
	std::vector<std::unique_ptr<UI>> m_pGuide;
	// 選択中UI
	std::vector<std::unique_ptr<UI>> m_pSelect;
	// 選択中UIのテクスチャパス
	const wchar_t* m_pSelectTexturePath;
	// ウィンドウの幅と高さ
	int m_windowWidth, m_windowHeight;
	// 選択可能UIの数
	SceneID m_num;
	// 時間
	float m_time;
	// ヒットフラグ
	bool m_hit;
	// 選択可能UIのインデックス
	unsigned int m_menuIndex;
	// SEの音量
	float m_SEVolume;
	// 再生フラグ
	bool m_isSEPlay;
};