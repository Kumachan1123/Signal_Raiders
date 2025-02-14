/*
	@file	BulletParameters.h
	@brief	弾パラメータークラス
*/
#pragma once
class BulletParameters
{
public:
	//======================================
	// プレイヤー用の弾の定数
	//======================================
	static const float PLAYER_BULLET_SIZE;// 弾の大きさ
	static const float COLLISION_RADIUS;// 弾の当たり判定の半径
	static const float ADJUST_MOVE;// 移動量の補正値
	static const float ADJUST_DIRECTION;	// Directionを若干上方向にするための補正値
	static const DirectX::SimpleMath::Vector3 INITIAL_POSITION;	// 発射位置
	static const float PLAYER_BULLET_LIFETIME;				// 寿命
	static const int DAMAGE;						// 敵に与えるダメージ
	static const float RELOAD_INTERVAL;				// リロード間隔

	//======================================
	// 敵用の弾の定数
	//======================================
	static const float ENEMY_BULLET_LIFETIME;			// 寿命
	static const float STRAIGHT_BULLET_SPEED;			// 直線弾の速度
	static const float STRAIGHT_ADJUST_DIRECTION;	// Directionを若干下にするための補正値
	static const float VERTICAL_BULLET_SPEED;			// 垂直弾の速度
	static const DirectX::SimpleMath::Vector3 VERTICAL_BULLET_LANDING_VELOCITY;	// 着弾前の速度
	static const float VERTICAL_BULLET_LANDING_POSITION;	// 垂直弾の着弾位置
	static const float SPIRAL_BULLET_SPEED;						// 螺旋弾の速度
	static const float SPIRAL_BULLET_ROTATION_SPEED;						// 螺旋弾の回転速度
	static const float SPIRAL_RADIUS;						// 螺旋弾の半径
	static const float SPIRAL_ROTATION_FREQUENCY;	// 螺旋弾の周波数
	static const float SPIRAL_RADIUS_SCALE;	// 螺旋弾の半径のスケール
	static const float SPIRAL_DEPTH_SCALE;	// 螺旋弾の深さのスケール
	static const float SPIRAL_BULLET_SPEED_DECAY;	// 螺旋弾の速度を徐々に遅くするための値
	//======================================
	// 共通の定数（プレイヤー・敵共通で使うもの）
	//======================================
	static const float DELETE_BULLET_POSITION;// 弾の削除位置
	static const float BULLET_ROTATION_SPEED;// 弾の自転速度(度)
	static const DirectX::SimpleMath::Plane SHADOW_PLANE;// 影の平面
	static const float ADJUST_HIT_VOLUME;// ヒットSEボリューム調整
	static const float ANGLE_MIN;// 角度の最小値
	static const float ANGLE_MAX;// 角度の最大値
	static const float DELETE_BULLET_POSITION;// 弾の削除位置
	static const int MAX_PLAYER_BULLET_COUNT;// プレイヤーの弾の最大数
	static const float HIT_VOLUME;// ヒット音のボリューム
};