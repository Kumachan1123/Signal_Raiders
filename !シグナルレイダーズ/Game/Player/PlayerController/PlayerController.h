/*
	@file	PlayerController.h
	@brief	プレイヤーコントローラークラス
	作成者：くまち
*/
#pragma once
//前方宣言
class CommonResources;
class Mouse;

class PlayerController
{
			//変数
public:
	DirectX::SimpleMath::Vector3 m_position;		// 一人称視点
	DirectX::SimpleMath::Vector3 m_velocity;		// 移動ベクトル
	float m_yawX, m_pitchY;

	float m_lastMouseX;
	float m_lastMouseY;
	HWND m_hWnd;
	POINT m_point;
	// カメラ左右回転
	float yawX = 0.0f;
	float m_beforeYawX;
	// カメラ上下回転
	float yawY = 0.0f;
	// ダッシュ
	float m_dash;
			//関数
private:
	// 共通リソース
	CommonResources* m_commonResources; float DmouseX = 0;
public:
	PlayerController();
	~PlayerController();
	void Initialize(CommonResources* resources);
	void MoveStop();
	void Update(const std::unique_ptr < DirectX::Keyboard::KeyboardStateTracker>& kb, DirectX::SimpleMath::Vector3& Direction, float elapsedTime);
	
	//Getter
	float GetYawX()const { return m_yawX; }
	float GetYawY()const { return m_pitchY; }
	DirectX::SimpleMath::Vector3 GetPlayerPosition()const { return m_position; }
	float GetPlayerPositionY()const { return m_position.y; }
	//Setter
	void SetPlayetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
};

template <typename T>
T clamp(const T& value, const T& min, const T& max)
{
    return (value < min) ? min : (value > max) ? max : value;
}
