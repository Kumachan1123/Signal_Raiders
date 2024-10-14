/*
	@file	EnemyBullet.h
	@brief	敵の弾クラス
	作成者：くまち
*/

#pragma once
#ifndef ENEMY_BULLET_DEFINED
#define ENEMY_BULLET_DEFINED
#include "Game/CommonResources.h"
#include "Game/BulletTrail/BulletTrail.h"

class CommonResources;

class EnemyBullet
{	//変数
public:
	//const float SIZE = 0.15f;
	DirectX::SimpleMath::Vector3 m_position;		// 弾の座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 弾の速さ
	float m_size;			// 弾の大きさ
	DirectX::SimpleMath::Vector3 m_direction;		// 弾が飛ぶ方向
	float m_time;									// 生存時間
	const int DAMAGE = 10;						// 敵に与えるダメージ
	// ジオメトリックプリミティブ弾
	std::unique_ptr<DirectX::Model> m_model;
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 「弾」境界ボックス
	DirectX::BoundingSphere m_boundingSphere;
	DirectX::SimpleMath::Vector3 m_target;
	// カメラ
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 弾の自転
	float m_angle;

	// 弾の軌道
	std::unique_ptr<BulletTrail> m_bulletTrail;
	// 	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

public:
	EnemyBullet(float size);
	~EnemyBullet();
	void Initialize(CommonResources* resources);
	void Update(DirectX::SimpleMath::Vector3& pos, float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir, DirectX::SimpleMath::Vector3& target);
	//Getter
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	DirectX::SimpleMath::Vector3 GetBulletDirection()const { return m_direction; }
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	float GetPlayerPositionY()const { return m_position.y; }
	float GetTime()const { return m_time; }
	//Setter
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }

	// 弾が生成されてからの経過時間が寿命を超えたかどうかを判定する
	bool IsExpired() const
	{
		const float BULLET_LIFETIME = 30.0f;				// 寿命

		return GetTime() >= BULLET_LIFETIME;
	}

};
#endif //ENEMY_BULLET_DEFINED

