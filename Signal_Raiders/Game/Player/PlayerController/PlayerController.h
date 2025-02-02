/*
	@file	PlayerController.h
	@brief	プレイヤーコントローラークラス
*/
#pragma once
#include "Game/Player/Player.h"
//前方宣言
class CommonResources;
class Mouse;
class Player;
class PlayerController
{
	//変数
public:
	DirectX::SimpleMath::Vector3 m_position;		// 一人称視点
	DirectX::SimpleMath::Vector3 m_velocity;		// 移動ベクトル
	float m_yawX, m_pitchY;						// カメラ回転
	float m_rotate;								// プレイヤー回転(ミニマップに渡す用)
	float m_sensitive;							// マウス感度
	float m_lastMouseX;
	float m_lastMouseY;
	HWND m_hWnd;
	POINT m_point;
	// プレイヤー
	Player* m_pPlayer;

	// ダッシュ
	float m_dash;

private:
	// 共通リソース
	CommonResources* m_commonResources; float DmouseX = 0;
public:
	PlayerController(Player* pPlayer);
	~PlayerController();
	void Initialize(CommonResources* resources);
	void MoveStop();
	void Update(float elapsedTime);// 更新
	void DebugCommand();// デバッグコマンド
	void Shoot();// プレイヤーが弾を撃つ
	//Getter
	float GetYawX()const { return m_yawX; }
	float GetPitch()const { return m_pitchY; }
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const { return m_position; }
	float GetRotate()const { return m_rotate; }
	//Setter
	void SetPlayetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetMouseSensitive(float sensitive) { m_sensitive = sensitive; }
};

