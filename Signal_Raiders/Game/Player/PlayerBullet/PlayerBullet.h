/*
	@file	PlayerBullet.h
	@brief	プレイヤーの弾クラス
*/
#pragma once
//前方宣言
class CommonResources;
#include "Game/CommonResources.h"
#include "Game/Particle/Particle.h"
#include "Game/ParticleUtility/ParticleUtility.h"
class PlayerBullet
{
	//変数
public:
	const float SIZE = 0.1f;// 弾の大きさ

	DirectX::SimpleMath::Vector3 m_position;		// 弾の座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 弾の速さ
	DirectX::SimpleMath::Vector3 m_direction;		// 弾が飛ぶ方向
	float m_time;									// 生存時間
	const float BULLET_LIFETIME = 1;				// 寿命
	const int DAMAGE = 10;						// 敵に与えるダメージ
	// ジオメトリックプリミティブ弾
	std::unique_ptr<DirectX::Model> m_model;
private:
	// 軌跡用の履歴を保存する配列
	std::vector<DirectX::SimpleMath::Vector3> m_trailPositions;
	const size_t MAX_TRAIL_LENGTH = 50;  // 軌跡の長さ
	// 共通リソース
	CommonResources* m_commonResources;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 「弾」境界ボックス
	DirectX::BoundingSphere m_boundingSphere;
	// モデルの影
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// 弾の自転
	float m_angle;
	// 弾の軌跡ポインター
	std::unique_ptr<Particle> m_bulletTrail;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// 	//デバッグ用
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_trailTexture;
	// カメラ
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;
	//関数
public:
	PlayerBullet();
	~PlayerBullet();

	void MoveStop();// 弾の移動を止める
	void Initialize(CommonResources* resources);// 初期化
	void Update(DirectX::SimpleMath::Vector3& Direction, float elapsedTime);// 更新
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// 描画
	void RenderShadow(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// 影の描画
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);// 当たり判定の描画
	void MakeBall(const DirectX::SimpleMath::Vector3& pos, DirectX::SimpleMath::Vector3& dir);// 弾を生成
	//Getter
	DirectX::SimpleMath::Vector3 GetBulletPosition()const { return m_position; }
	DirectX::BoundingSphere& GetBoundingSphere() { return m_boundingSphere; }
	float GetPlayerPositionY()const { return m_position.y; }
	float GetTime()const { return m_time; }
	//Setter
	void SetBulletPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye) { m_bulletTrail->SetCameraPosition(eye); m_cameraEye = eye; }
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target) { m_bulletTrail->SetCameraTarget(target); m_cameraTarget = target; }
	void SetCameraUp(DirectX::SimpleMath::Vector3 up) { m_bulletTrail->SetCameraUp(up); m_cameraUp = up; }
	// 弾が生成されてからの経過時間が寿命を超えたかどうかを判定する
	bool IsExpired() const { return m_time >= BULLET_LIFETIME; }
	// 敵にダメージを与える
	int Damage()const { return DAMAGE; }
};