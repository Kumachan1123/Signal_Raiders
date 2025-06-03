/*
*  @file LastBoss.h
*  @brief ラスボスクラス
*/
#pragma once
// 標準ライブラリ
#include <SimpleMath.h>
// 外部ライブラリ
#include <DeviceResources.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// 自作ヘッダーファイル
#include "Game/KumachiLib/AudioManager/AudioManager.h"
#include "Game/Interface/IEnemy.h"
#include "Game/Interface/IBossLogic.h"
#include "Game/Enemy/BossBase/BossBase.h"
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
public:// public関数

	// コンストラクタ
	LastBoss(BossBase* pBoss, CommonResources* commonResources);
	// デストラクタ
	~LastBoss()override;
	// 初期化
	void Initialize()override;
	// 状態を変更
	void ChangeState()override;
	// 描画
	void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)override;
	// 弾の発射位置を決定
	void BulletPositioning()override;
	// 弾を生成
	void CreateBullet()override;
private:// private変数
	// 共通リソース
	CommonResources* m_pCommonResources;
	//モデル
	std::unique_ptr<LastBossModel>		m_pBossModel;
	// HPバー
	std::unique_ptr<EnemyHPBar>		m_pHPBar;
	// 敵の座標
	DirectX::SimpleMath::Vector3 m_position;
	// 敵の速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 敵の回転
	DirectX::SimpleMath::Vector3 m_rotate;
	//敵の境界球
	DirectX::BoundingSphere m_bossBS;
	// マトリクス
	DirectX::SimpleMath::Matrix m_matrix;
	// 弾の発射位置:右上
	DirectX::SimpleMath::Vector3 m_bulletPosRightUp;
	// 弾の発射位置:右下
	DirectX::SimpleMath::Vector3 m_bulletPosRightDown;
	// 弾の発射位置:左上
	DirectX::SimpleMath::Vector3 m_bulletPosLeftUp;
	// 弾の発射位置:左下
	DirectX::SimpleMath::Vector3 m_bulletPosLeftDown;
	// 発射位置を回転させるためのクォータニオン
	DirectX::SimpleMath::Quaternion m_bulletQuaternion;
	// EnemyBulletクラスに送る弾のタイプ
	BulletType m_bulletType;
	// 弾の飛ぶ方向
	DirectX::SimpleMath::Vector3 m_bulletDirection;
	// 時間
	float m_time;
	// カメラの視点 
	DirectX::SimpleMath::Vector3 m_cameraEye;
	// カメラのターゲット
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// カメラの上ベクトル
	DirectX::SimpleMath::Vector3 m_cameraUp;
	// ボスのベースクラス
	BossBase* m_pBossBase;
};