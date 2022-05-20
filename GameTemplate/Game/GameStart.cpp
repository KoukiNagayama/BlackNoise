#include "stdafx.h"
#include "GameStart.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Bell.h"
#include "Toy.h"
#include "Enemy.h"
#include "InfoForEdge.h"
#include "Enemy.h"
#include "Gramophone.h"
#include "Record.h"
#include "Item.h"
#include "Door.h"
#include "BrokenDoor.h"
#include "Hammer.h"
#include "GroundFloor.h"
#include "Crowbar.h"
#include "FloorGimmick.h"
#include "UI.h"
#include "Enemy2.h"
namespace
{
	const float DISTANCE = 100.0f;
}

GameStart::GameStart()
{
}

GameStart::~GameStart()
{
	DeleteGO(m_bg);
	DeleteGO(m_gamecam);
	DeleteGO(m_bell);
	DeleteGO(m_toy);
	DeleteGO(m_brokendoor);
	DeleteGO(m_hammer);
	DeleteGO(m_enemy); 
	for (auto door : m_door)
	{
		DeleteGO(door);
	}
	for (auto gramophone : m_gramophone)
	{
		DeleteGO(gramophone);
	}
	for (auto record : m_record)
	{
		DeleteGO(record);
	}
	DeleteGO(m_floorgimmick);
	DeleteGO(m_item);
	DeleteGO(m_crowbar);
	DeleteGO(m_ui);
}

bool GameStart::Start()
{
	Vector3 position = Vector3::Zero;
	//各クラスを生成。

	m_levelRender.Init("Assets/level3D/stage2.tkl", [&](LevelObjectData& objData) {
		//ステージ
		if (objData.EqualObjectName(L"floor2") == true) {

			m_bg = NewGO<BackGround>(0);
			//配置座標、スケール、回転を取得する。
			m_bg->SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		//プレイヤー(カメラ)
		if (objData.EqualObjectName(L"player") == true) {

			m_gamecam = NewGO<GameCamera>(0, "gamecamera");
			//配置座標、スケール、回転を取得する。
			m_gamecam->SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		//つみき
		if (objData.EqualObjectName(L"crashtoy") == true) {

			m_toy = NewGO<Toy>(0, "toy");
			//配置座標、スケール、回転を取得する。
			m_toy->SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		//蓄音機
		if (objData.ForwardMatchName(L"gramophone") == true)
		{
			auto gramophone = NewGO<Gramophone>(0, "gramophone");
			gramophone->SetPosition(objData.position);
			gramophone->SetScale(objData.scale);
			gramophone->SetRotation(objData.rotation);
			gramophone->SetNumber(objData.number);
			m_gramophone.push_back(gramophone);
			return true;
		}

		//レコード
		if (objData.ForwardMatchName(L"record") == true)
		{
			auto record = NewGO<Record>(0, "record");
			record->SetPosition(objData.position);
			record->SetScale(objData.scale);
			record->SetRotation(objData.rotation);
			record->SetNumber(objData.number);
			m_record.push_back(record);
			return true;
		}
		if(objData.ForwardMatchName(L"door") == true)
		{
			auto door = NewGO<Door>(0, "door");
			door->SetPosition(objData.position);
			door->SetScale(objData.scale);
			door->SetRotation(objData.rotation);
			m_door.push_back(door);
			return true;
		}
		/*if (objData.ForwardMatchName(L"brokendoor") == true)
		{
			m_brokendoor = NewGO<BrokenDoor>(0, "brokendoor");
			m_brokendoor->SetPosition(objData.position);
			m_brokendoor->SetRotation(objData.rotation);
			return true;
		}*/
		if (objData.ForwardMatchName(L"hammer") == true)
		{
			m_hammer = NewGO<Hammer>(0, "hammer");
			m_hammer->SetPosition(objData.position);
			return true;
		}

		if (objData.ForwardMatchName(L"switchfloor") == true)
		{
			m_switchPos = objData.position;
			return true;
		}
		if (objData.ForwardMatchName(L"gimmick") == true)
		{
			m_floorgimmick = NewGO<FloorGimmick>(0, "floorgimmick");
			m_floorgimmick->SetPosition(objData.position);
			m_floorgimmick->SetScale(objData.scale);
			m_floorgimmick->SetRotation(objData.rotation);

			return true;
		}
		if (objData.ForwardMatchName(L"crowbar") == true)
		{
			m_crowbar = NewGO<Crowbar>(0, "crowbar");
			m_crowbar->SetPosition(objData.position);
			m_crowbar->SetScale(objData.scale);
			m_crowbar->SetRotation(objData.rotation);

			return true;
		}

		return false;
	});

	m_levelRender2.Init("Assets/level3D/stage2.tkl", [&](LevelObjectData& objData) {
		if (objData.ForwardMatchName(L"enemy") == true)
		{
			m_enemy = NewGO<Enemy2>(0, "enemy");
			m_enemy->SetPosition(objData.position);
			m_enemy->SetNumber(objData.number);
			return true;
		}
		});
	m_player = NewGO<Player>(0, "player");
	m_bell = NewGO<Bell>(0, "bell");
	//m_enemy = NewGO<Enemy>(0, "enemy");
	m_item = NewGO<Item>(0, "item");
	m_ui = NewGO<UI>(0, "ui");
	return true;
}

void GameStart::Update()
{
	SwitchFirstFloor();
}

void GameStart::SwitchFirstFloor()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_switchPos.y;

	Vector3 disToPlayer = cameraPos - m_switchPos;
	//プレイヤーとの距離が100.0fだったら。
	if (disToPlayer.Length() <= DISTANCE)
	{
		//ゲームスタートクラスを削除
		DeleteGO(this);
		//一階の表示
		m_groundfloor = NewGO<GroundFloor>(0, "groundfloor");
	}
}

