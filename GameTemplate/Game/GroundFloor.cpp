#include "stdafx.h"
#include "GroundFloor.h"
#include "Player.h"
#include "GameCamera.h"
#include "Bell.h"
#include "Enemy2.h"
#include "Enemy.h"
#include "Door.h"
#include "Piece.h"
#include "Key.h"
#include "WhiteBoard.h"
#include "EntranceDoor.h"
#include "GameClear.h"

namespace
{
	const float DISTANCE = 400.0f; //プレイヤーとの距離
}

GroundFloor::~GroundFloor()
{
	DeleteGO(m_gamecam);
	DeleteGO(m_player);
	DeleteGO(m_bell);
	DeleteGO(m_enemy);
	DeleteGO(m_whiteBoard);
	DeleteGO(m_entranceDoor);
}

bool GroundFloor::Start()
{
	//各クラスを生成。
	m_levelRender.Init("Assets/level3D/stage1_4.tkl", [&](LevelObjectData& objData) {
		//ステージ
		if (objData.EqualObjectName(L"floor1") == true) {

			m_bgRender.Init("Assets/modelData/stage1/floor1.tkm");
			//配置座標、スケール、回転を取得する。
			m_bgRender.SetPosition(objData.position);
			m_physicsStaticObject.CreateFromModel(m_bgRender.GetModel(), m_bgRender.GetModel().GetWorldMatrix());
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
		// エネミー
		if (objData.ForwardMatchName(L"enemy") == true)
		{
			m_enemy = NewGO<Enemy2>(0, "enemy");
			m_enemy->SetPosition(objData.position);
			m_enemy->SetNumber(objData.number);
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
		/*if (objData.EqualObjectName(L"piece") == true) {
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
		}*/
		if (objData.EqualObjectName(L"gameclear") == true) {
			m_clearPos = objData.position;
			return true;
		}
		if (objData.EqualObjectName(L"whiteBoard") == true) {
			m_whiteBoard = NewGO<WhiteBoard>(0, "whiteBoard");
			m_whiteBoard->SetPosition(objData.position);
			m_whiteBoard->SetScale(objData.scale);
			m_whiteBoard->SetRotation(objData.rotation);
			return true;
		}
		if (objData.EqualObjectName(L"EntranceDoor") == true) {
			m_entranceDoor = NewGO<EntranceDoor>(0, "entranceDoor");
			m_entranceDoor->SetPosition(objData.position);
			m_entranceDoor->SetRotation(objData.rotation);
			m_entranceDoor->SetScale(objData.scale);
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
	ReachGameClear();
	m_bgRender.Update();
}

void GroundFloor::ReachGameClear()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_clearPos.y;
	Vector3 disToPlayer = cameraPos - m_clearPos;
	if (disToPlayer.Length() <= DISTANCE)
	{
		if (m_isGameClear == false) {
			m_isGameClear = true;
			m_gameClear = NewGO<GameClear>(0, "gameClear");
			
		}
	}
}

void GroundFloor::Render(RenderContext& rc)
{
	m_bgRender.Draw(rc);

}
