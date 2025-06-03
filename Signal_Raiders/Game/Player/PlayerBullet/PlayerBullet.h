/*
*	@file	PlayerBullet.h
*	@brief	プレイヤーの弾クラス
*/
#pragma once
// 標準ライブラリ
#include <cassert>
// DirectXライブラリ
#include <SimpleMath.h>
#include <DeviceResources.h>
// 外部ライブラリ
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/DebugDraw.h>
#include <Libraries/Microsoft/ReadData.h>
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/KumachiLib//DrawCollision/DrawCollision.h"
#include "Game/KumachiLib/KumachiLib.h"
#include "Game/Particle/Particle.h"
#include "Game/ParticleUtility/ParticleUtility.h"
#include "Game/BulletParameters/BulletParameters.h"
#include "Game/Interface/IBullet.h"
//前方宣言
class CommonResources;

class PlayerBullet : public IBullet
{
public:// アクセサ
	// 弾の座標を取得する
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	// 弾の座標を設定する
	void SetBulletPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
	// 弾の境界球を取得する
	const DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	// プレイヤーのY座標を取得する
	float GetPlayerPositionY()const { return m_position.y; }
	// 時間を取得する
	float GetTime()const { return m_time; }
	// カメラの視点を設定する
	void SetCameraEye(const DirectX::SimpleMath::Vector3& eye) { m_pBulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	// カメラの注視点を設定する
	void SetCameraTarget(const DirectX::SimpleMath::Vector3& target) { m_pBulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	// カメラの上方向を設定する
	void SetCameraUp(const DirectX::SimpleMath::Vector3& up) { m_pBulletTrail->SetCameraUp(up); m_cameraUp = up; }
	// 追加ダメージを取得する
	int GetAdditionalDamage()const { return m_additionalDamage; }
	// 追加ダメージを設定する
	void SetAdditionalDamage(int damage) { m_additionalDamage = damage; }
public:// public関数
	// コンストラクタ
	PlayerBullet();
	// デストラクタ
	~PlayerBullet()override;
	// 初期化
	void Initialize(CommonResources* resources)override;
	// 更新
	void Update(float elapsedTime)override;
	// 描画
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// 影の描画
	void RenderShadow(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// 当たり判定の描画
	void DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 弾を生成
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir);
	// 弾が生成されてからの経過時間が寿命を超えたかどうかを判定する
	bool IsExpired() const { return m_time >= BulletParameters::PLAYER_BULLET_LIFETIME; }
	// 敵にダメージを与える
	int Damage()const { return BulletParameters::DAMAGE + m_additionalDamage; }
private:// private変数
	// 軌跡用の履歴を保存する配列
	std::vector<DirectX::SimpleMath::Vector3> m_trailPositions;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 「弾」境界ボックス
	DirectX::BoundingSphere m_boundingSphere;
	// モデルの影
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	// 弾の自転
	float m_angle;
	// 弾の軌跡ポインター
	std::unique_ptr<Particle> m_pBulletTrail;
	// 追加ダメージ
	int m_additionalDamage;
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// カメラの向き
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// カメラの上方向
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// 弾の座標
	DirectX::SimpleMath::Vector3 m_position;
	// 弾の速さ
	DirectX::SimpleMath::Vector3 m_velocity;
	// 弾が飛ぶ方向
	DirectX::SimpleMath::Vector3 m_direction;
	// 生存時間
	float m_time;
	// 弾モデル
	DirectX::Model* m_pModel;
};