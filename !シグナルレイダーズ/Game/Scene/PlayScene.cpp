/*
	@file	PlayScene.cpp
	@brief	プレイシーンクラス
*/
#include "pch.h"
#include "Game/Scene/PlayScene.h"
#include "Game/CommonResources.h"
#include "DeviceResources.h"
#include "Game/FPS_Camera/FPS_Camera.h"
#include "Libraries/MyLib/DebugString.h"
#include "Libraries/MyLib/GridFloor.h"
#include "Libraries/MyLib/InputManager.h"
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include <vector>
#include <cassert>
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>

#include <Libraries/Microsoft/DebugDraw.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
PlayScene::PlayScene()
	:
	m_commonResources{},
	m_gridFloor{},
	m_projection{},
	m_isChangeScene{},
	m_model{},
	m_angle{},
	m_camera{},
	m_wifi{ nullptr }
{
}
//---------------------------------------------------------
// デストラクタ
//---------------------------------------------------------
PlayScene::~PlayScene()
{
	// do nothing.
}
//---------------------------------------------------------
// 初期化する
//---------------------------------------------------------
void PlayScene::Initialize(CommonResources* resources)
{
	assert(resources);
	m_commonResources = resources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();


	m_wifi = std::make_unique<Wifi>();
	m_wifi->Initialize();
	// グリッド床を作成する
	m_gridFloor = std::make_unique<mylib::GridFloor>(device, context, states);
	m_gridFloor->SetColor(DirectX::Colors::Yellow);
	// モデルを読み込む準備
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	// モデルを読み込む
	m_model = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Enemy/Enemy.cmo", *fx);
	// FPSカメラを作成する
	m_camera = std::make_unique<FPS_Camera>();
	// コントローラー生成
	m_playerController = std::make_unique<PlayerController>();
	m_playerController->Initialize(resources);
	m_playerController->SetPlayetPosition(m_camera->GetEyePosition());
	// 回転角を初期化する（度）
	m_angle = 0;
	// シーン変更フラグを初期化する
	m_isChangeScene = false;
	// スカイボックス生成
	m_skybox = std::make_unique<SkyBox>();
	m_skybox->Initialize(resources);
	// プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<DX11::PrimitiveBatch<DX11::VertexPositionColor>>(context);
	m_inPlayerArea.Radius = 20.0f;
	m_PlayerSphere.Radius = 2.0f;

	/*
		デバッグドローの表示用オブジェクトを生成する
	*/
	// ベーシックエフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	// 入力レイアウトを作成する
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);


}

//---------------------------------------------------------
// 更新する
//---------------------------------------------------------
void PlayScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	// キーボードステートトラッカーを取得する
	const auto& kb = m_commonResources->GetInputManager()->GetKeyboardTracker();
	auto& mstate = m_commonResources->GetInputManager()->GetMouseState();
	auto& mtracker = m_commonResources->GetInputManager()->GetMouseTracker();


	// カメラが向いている方向を取得する
	DirectX::SimpleMath::Vector3 cameraDirection = m_camera->GetDirection();
	m_playerController->Update(kb, cameraDirection, elapsedTime);
	// シーンチェンジ
	//m_isChangeScene = true;
	// 〇〇を回転する
	m_angle += 0.025f;
	if (m_angle > 360)m_angle = 0;
	// FPSカメラ位置を更新する
	m_camera->Update(m_playerController->GetPlayerPosition(), m_playerController->GetYawX());
	m_camera->SetTargetPositionY(m_playerController->GetYawY());


	// 左クリックで弾発射
	if (mtracker->GetLastState().leftButton && !m_isBullet)
	{
		auto bullet = std::make_unique<PlayerBullet>();
		bullet->Initialize(m_commonResources);
		bullet->MakeBall(m_playerController->GetPlayerPosition(), cameraDirection);
		m_playerBullets.push_back(std::move(bullet));
		m_isBullet = true;
	}
	if (!mtracker->GetLastState().leftButton)
	{
		m_isBullet = false;
	}

	UpdateBullets(elapsedTime);
	UpdateEnemies(elapsedTime);
	m_wifi->Update(elapsedTime);
	if (!m_isEnemyBorn && m_wifi->GetPreWifiLevels().empty())m_isEnemyBorn = true;//生成可能にする
	// 生成可能なら
	if (m_isEnemyBorn && !m_isBorned)
	{
		for (int it = 0; it < 1; it++)// m_wifi->GetWifiLevels().size()
		{
			auto enemy = std::make_unique<Enemy>();// 敵を生成
			enemy->Initialize(m_commonResources, m_wifi->GetWifiLevels()[it]);  // 初期化
			m_enemy.push_back(std::move(enemy));   // 敵配列に登録
		}
		// 生成不可能にする
		m_isEnemyBorn = false;
		m_isBorned = true;
	}

}

//---------------------------------------------------------
// 描画する
//---------------------------------------------------------
void PlayScene::Render()
{


	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// カメラからビュー行列と射影行列を取得する
	Matrix view = m_camera->GetViewMatrix();
	Matrix projection = m_camera->GetProjectionMatrix();
	Matrix skyWorld = Matrix::CreateRotationY(XMConvertToRadians(m_angle));
	skyWorld *= Matrix::CreateScale(10);
	// 格子床を描画する
	m_gridFloor->Render(context, view, projection);
	// スカイボックス描画
	m_skybox->Render(view, projection, skyWorld, m_playerController->GetPlayerPosition());
	// 各パラメータを設定する
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_inputLayout.Get());
	//** デバッグドローでは、ワールド変換いらない
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->Apply(context);

	m_inPlayerArea.Center = m_playerController->GetPlayerPosition();
	m_PlayerSphere.Center = m_playerController->GetPlayerPosition();
	m_primitiveBatch->Begin();
	DX::Draw(m_primitiveBatch.get(), m_PlayerSphere, DirectX::Colors::PeachPuff);
	m_primitiveBatch->End();

	//m_model->Draw(context, *states, skyWorld, view, projection);
	// 弾を描画する
	for (const auto& bullet : m_playerBullets)bullet->Render(view, projection);

	if (m_enemy.size() > 0)
	{
		for (const auto& enemy : m_enemy)
		{

			enemy->Render(view, projection);


		}
	}
	// デバッグ情報を「DebugString」で表示する
	auto debugString = m_commonResources->GetDebugString();

	debugString->AddString("Play Scene");
	debugString->AddString("X:%f", m_playerController->GetPlayerPosition().x);
	debugString->AddString("Z:%f", m_playerController->GetPlayerPosition().z);
	m_wifi->Render(debugString);
}
//---------------------------------------------------------
// 後始末する
//---------------------------------------------------------
void PlayScene::Finalize()
{
	// do nothing.
	m_playerBullets.clear();
	m_enemy.clear();
	m_wifi.reset();
	m_gridFloor.reset();
	m_model.reset();
	m_camera.reset();
	m_playerController.reset();
	m_skybox.reset();
}
//---------------------------------------------------------
// 次のシーンIDを取得する
//---------------------------------------------------------
IScene::SceneID PlayScene::GetNextSceneID() const
{
	// シーン変更がある場合
	if (m_isChangeScene)return IScene::SceneID::TITLE;
	// シーン変更がない場合
	return IScene::SceneID::NONE;
}

void PlayScene::UpdateBullets(float elapsedTime)
{
	auto debugString = m_commonResources->GetDebugString();
	DirectX::SimpleMath::Vector3 dir = m_camera->GetDirection();
	debugString->AddString("HP:%i", m_playerHP);
	for (auto it = m_playerBullets.begin(); it != m_playerBullets.end(); )
	{
		(*it)->Update(dir, elapsedTime);

		if ((*it)->IsExpired())
		{
			it = m_playerBullets.erase(it);
		}
		else
		{
			bool isHit = false;
			for (auto& enemy : m_enemy)
			{
				if ((*it)->GetBoundingSphere().Intersects(enemy->GetBoundingSphere()))
				{
					isHit = true;
					m_count++;//debug
					enemy->SetEnemyHP(enemy->GetHP() - (*it)->Damage());

					break;
				}
			}
			if (isHit)
			{

				it = m_playerBullets.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void PlayScene::UpdateEnemies(float elapsedTime)
{
	if (m_enemy.size() <= 0)
	{
		m_isBorned = false;
	}

	// 敵同士の当たり判定
	// 敵同士が重ならないようにする
	for (size_t i = 0; i < m_enemy.size(); ++i)
	{
		for (size_t j = i + 1; j < m_enemy.size(); ++j)
		{
			bool hit = m_enemy[i]->GetBoundingSphere().Intersects(m_enemy[j]->GetBoundingSphere());
			m_enemy[i]->SetHitToOtherEnemy(hit);
			m_enemy[j]->SetHitToOtherEnemy(hit);
			if (hit)
			{
				m_enemy[i]->CheckHitOtherEnemy(m_enemy[i]->GetBoundingSphere(),
											   m_enemy[j]->GetBoundingSphere());
			}
		}
	}

	// 敵とプレイヤーの一定範囲との当たり判定
	for (auto& enemy : m_enemy)
	{
		m_isHitPlayerToEnemy = false;
		enemy->Update(elapsedTime, m_playerController->GetPlayerPosition());

		bool hit = enemy->GetBulletHitToPlayer();
		if (hit)
		{
			enemy->SetPlayerHP(m_playerHP);

		}

		if (enemy->GetBoundingSphere().Intersects(m_inPlayerArea))	m_isHitPlayerToEnemy = true;


		enemy->SetHitToPlayer(m_isHitPlayerToEnemy);
		enemy->SetPlayerBoundingSphere(m_PlayerSphere);
	}

	// 削除対象を保持するベクター
	std::vector<std::unique_ptr<Enemy>> enemiesToRemove;

	// 削除対象を収集する
	for (auto it = m_enemy.begin(); it != m_enemy.end(); )
	{
		if ((*it)->GetEnemyIsDead())
		{
			// ここで削除時の特別な処理を行う
			// 例えば、敵の消滅エフェクトを再生するなど
			//HandleEnemyDeath(it->get());

			// 削除対象に追加
			enemiesToRemove.push_back(std::move(*it));

			it = m_enemy.erase(it);  // 削除してイテレータを更新
		}
		else
		{
			++it;  // 次の要素へ
		}
	}

	// 削除対象に対して特別な処理を実行
	for (auto& enemy : enemiesToRemove)
	{
		// 特別な処理を実行
		//HandleEnemyDeath(enemy.get());
	}
}