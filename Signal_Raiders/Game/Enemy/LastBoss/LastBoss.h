/*
*  @file LastBoss.h
*  @brief ラスボスクラス
*/
#pragma once
// 標準ライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include "DeviceResources.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
// 自作ヘッダーファイル
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Interface/IBossLogic.h"
#include "Game/Enemy/BossBase/BossBase.h"
//#include "Game/Enemy/EnemyBullet/EnemyBullet.h"
#include "Game/KumachiLib/DrawCollision/DrawCollision.h"
#include "Game/Enemy/Parameters/EnemyParameters.h"
#include "Game/Enemy/EnemyHPBar/EnemyHPBar.h"
#include "Game/Enemy/LastBoss/LastBossModel/LastBossModel.h"

//前方宣言
class CommonResources;
class BossAI;
class LastBossModel;
class BossBase;
class LastBoss : public IBossLogic
{
public:
	// 初期ステータスを設定
	LastBoss(BossBase* pBoss, CommonResources* commonResources);
	~LastBoss()override;
	void Initialize()override;                                             // Boss, LastBossにてモデルを生成
	void ChangeState()override;                                // Boss,LastBossにて状態を変更
	void Draw(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)override;        // Boss, LastBossにて描画
	void BulletPositioning()override;	// 弾の発射位置を決定
	void CreateBullet()override;// 弾を生成

	void CreateLeftBullet(float angleOffset, EnemyBullet::BulletType type);// 左の弾を生成
	void CreateRightBullet(float angleOffset, EnemyBullet::BulletType type);// 右の弾を生成

private:
	// 共通リソース
	CommonResources* m_commonResources;
	std::unique_ptr<LastBossModel>		m_pBossModel;//モデル
	std::unique_ptr<EnemyHPBar>		m_pHPBar;// HPバー
	// 敵の情報
	DirectX::SimpleMath::Vector3 m_position;		// 座標
	DirectX::SimpleMath::Vector3 m_velocity;		// 速度
	DirectX::SimpleMath::Vector3 m_rotate;		// 回転
	DirectX::BoundingSphere m_bossBS;	//敵の境界球
	DirectX::SimpleMath::Matrix m_matrix;// マトリクス
	// 弾の発射位置
	// 右上
	DirectX::SimpleMath::Vector3 m_bulletPosRightUp;
	// 右下
	DirectX::SimpleMath::Vector3 m_bulletPosRightDown;
	// 左上
	DirectX::SimpleMath::Vector3 m_bulletPosLeftUp;
	// 左下
	DirectX::SimpleMath::Vector3 m_bulletPosLeftDown;

	// 発射位置を回転させるためのクォータニオン
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// 弾のタイプ
	EnemyBullet::BulletType m_bulletType;// EnemyBulletクラスに送る
	// 弾の飛ぶ方向
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// 時間
	float m_time;
	// プレイヤーのカメラの情報
	DirectX::SimpleMath::Vector3 m_cameraEye;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	DirectX::SimpleMath::Vector3 m_cameraUp;
	BossBase* m_pBossBase;

};