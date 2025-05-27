/*
*	@file SpecialBullet.h
*	@brief 敵弾クラス(特殊弾)
*/
#pragma once
// 自作ヘッダーファイル
#include "Game/CommonResources.h"
#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/Interface/IEnemyBullet.h"
#include "Game/KumachiLib/KumachiLib.h"
class SpecialBullet : public IEnemyBullet
{
public:
	// public関数
	SpecialBullet();// コンストラクタ
	~SpecialBullet();// デストラクタ
	void Initialize() override;// 初期化関数
	void Update(float elapsedTime) override;// 更新関数
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }// 敵弾ポインターをセットする関数
	void SetCommonResources(CommonResources* commonResources) override { m_pCommonResources = commonResources; }// 共通リソースをセットする関数
	void SetSEVolume(float volume) override { m_seVolume = volume; } // SE音量をセットする関数
private:
	// private関数
	void Expand();		// 回転弾を展開する
	void Shot();		// 回転弾を発射する
	void StopExpand();	// 回転弾の展開をやめる
	void ComeBack();	// 回転弾を自分の周りに戻す
private:
	// privateメンバ変数	
	CommonResources* m_pCommonResources;				// 共通リソース
	EnemyBullet* m_pEnemyBullet;					// 敵弾ポインター
	DirectX::SimpleMath::Vector3 m_position;		// 弾の座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 弾の速さ
	DirectX::SimpleMath::Vector3 m_direction;		// 弾が飛ぶ方向
	DirectX::SimpleMath::Vector3 m_target;			// 弾発射時のターゲットの位置
	DirectX::SimpleMath::Vector3 m_currentTarget;	// 現在のターゲットの位置
	DirectX::BoundingSphere m_boundingSphere;		// 「弾」境界球
	float m_spiralAngle;							// 回転弾の角度
	float m_elapsedTime;							// 経過時間
	float m_time;									// 時間
	DirectX::SimpleMath::Vector3 m_basePos;			// 弾の基準位置
	float m_rotationSpeed;							// 弾の回転速度
	float m_distance;								// 弾の敵との距離
	float m_height;									// 弾の高さ
	DirectX::SimpleMath::Vector3 m_positionOffSet;	// 弾の位置オフセット
	float m_seVolume;								// SE音量
	bool m_isPlayChargeSE;							// チャージSE再生フラグ
	bool m_isPlayShotSE;							// 発射SE再生フラグ
};