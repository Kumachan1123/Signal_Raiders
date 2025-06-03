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
public:// public関数
	// コンストラクタ
	SpecialBullet();
	// デストラクタ
	~SpecialBullet();
	// 初期化関数
	void Initialize() override;
	// 更新関数
	void Update(float elapsedTime) override;
	// 敵弾ポインターをセットする関数
	void SetEnemyBullet(EnemyBullet* pEnemyBullet) override { m_pEnemyBullet = pEnemyBullet; }
	// 共通リソースをセットする関数
	void SetCommonResources(CommonResources* commonResources) override { m_pCommonResources = commonResources; }
	// SE音量をセットする関数
	void SetSEVolume(float volume) override { m_seVolume = volume; }
private:// private関数
	// 回転弾を展開する
	void Expand();
	// 回転弾を発射する
	void Shot();
	// 回転弾の展開をやめる
	void StopExpand();
	// 回転弾を自分の周りに戻す
	void ComeBack();
private:// privateメンバ変数	
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 敵弾ポインター
	EnemyBullet* m_pEnemyBullet;
	// 弾の座標
	DirectX::SimpleMath::Vector3 m_position;
	// 弾の速さ
	DirectX::SimpleMath::Vector3 m_velocity;
	// 弾が飛ぶ方向
	DirectX::SimpleMath::Vector3 m_direction;
	// 弾発射時のターゲットの位置
	DirectX::SimpleMath::Vector3 m_target;
	// 現在のターゲットの位置
	DirectX::SimpleMath::Vector3 m_currentTarget;
	// 「弾」境界球
	DirectX::BoundingSphere m_boundingSphere;
	// 回転弾の角度
	float m_spiralAngle;
	// 経過時間
	float m_elapsedTime;
	// 時間
	float m_time;
	// 弾の基準位置
	DirectX::SimpleMath::Vector3 m_basePos;
	// 弾の回転速度
	float m_rotationSpeed;
	// 弾の敵との距離
	float m_distance;
	// 弾の高さ
	float m_height;
	// 弾の位置オフセット
	DirectX::SimpleMath::Vector3 m_positionOffSet;
	// SE音量
	float m_seVolume;
	// チャージSE再生フラグ
	bool m_isPlayChargeSE;
	// 発射SE再生フラグ
	bool m_isPlayShotSE;
};