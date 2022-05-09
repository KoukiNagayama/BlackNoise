#include "stdafx.h"
#include "GroundFloor.h"
#include "Player.h"
#include "GameCamera.h"
#include "Bell.h"
#include "Enemy.h"
#include "Enemy.h"
#include "Door.h"
#include "Piece.h"
#include "Key.h"

namespace
{
	const float DISTANCE = 100.0f; //プレイヤーとの距離
}

GroundFloor::~GroundFloor()
{
	DeleteGO(m_gamecam);
	DeleteGO(m_player);
	DeleteGO(m_bell);
}

bool GroundFloor::Start()
{
	//各クラスを生成。
	m_levelRender.Init("Assets/modelData/stage1/stage1.tkl", [&](LevelObjectData& objData) {
		//ステージ
		if (objData.EqualObjectName(L"floor1") == true) {

			m_bgRender.Init("Assets/modelData/stage1/floor1.tkm");
			//配置座標、スケール、回転を取得する。
			m_bgRender.SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		//プレイヤー(カメラ)
		if (objData.EqualObjectName(L"player") == true) {

			//配置座標、スケール、回転を取得する。
			m_gamecam = NewGO<GameCamera>(0, "gamecamera");
			m_gamecam->SetPosition(objData.position);
			m_gamecam->SetRotation(objData.rotation);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		/*if(objData.ForwardMatchName(L"door") == true)
		{
			auto door = NewGO<Door>(0, "door");
				door->SetPosition(objData.position);
				door->SetScale(objData.scale);
				door->SetRotation(objData.rotation);
				return true;
		}*/
		if (objData.EqualObjectName(L"piece") == true) {
			auto piece = NewGO<Piece>(0, "piece");
			//配置座標、スケール、回転を取得する。
			piece->SetPosition(objData.position);
			piece->SetRotation(objData.rotation);
			piece->SetScale(objData.scale);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		if (objData.EqualObjectName(L"key") == true) {
			m_key = NewGO<Key>(0, "key");
			//配置座標、スケール、回転を取得する。
			m_key->SetPosition(objData.position);
			m_key->SetRotation(objData.rotation);
			m_key->SetScale(objData.scale);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		if (objData.EqualObjectName(L"gameclear") == true) {
			m_clearPos = objData.position;
			return true;
		}
	});
	
	m_player = NewGO<Player>(0, "player");
	m_bell = NewGO<Bell>(0, "bell");
	//m_enemy = NewGO<Enemy>(0, "enemy");

	//画像の読み込み
	m_spriteRender.Init("Assets/sprite/gameover.dds",1457,728);
	//モデルの更新
	m_bgRender.Update();

	return true;
}

void GroundFloor::Update()
{
	GameClear();
	m_bgRender.Update();
}

void GroundFloor::GameClear()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_clearPos.y;
	Vector3 disToPlayer = cameraPos - m_clearPos;
	if (m_pickKey != false && disToPlayer.Length() <= DISTANCE)
	{
		m_clear = true;
	}
}

void GroundFloor::Render(RenderContext& rc)
{
	m_bgRender.Draw(rc);
	if (m_clear != false)
	{
		m_spriteRender.Draw(rc);
	}
}
