#include "stdafx.h"
#include "GameStart.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Bell.h"
#include "Toy.h"
#include "Key.h"
#include "Skey.h"
#include "Enemy.h"
#include "InfoForEdge.h"
#include "Enemy.h"
#include "Gramophone.h"
#include "Record.h"
#include "Item.h"

GameStart::GameStart()
{
}

GameStart::~GameStart()
{

}

bool GameStart::Start()
{
	Vector3 position = Vector3::Zero;
	//g_infoForEdge.InitForSound(0, position, 200.0f, 0);
	//g_infoForEdge.InitForSound(0, position, 200.0f, 1, 0.00f);
	//各クラスを生成。

	m_levelRender.Init("Assets/modelData/stage/level.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"Floor2") == true) {

			m_bg = NewGO<BackGround>(0);
			//配置座標、スケール、回転を取得する。
			m_bg->SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		if (objData.EqualObjectName(L"player") == true) {

			m_gamecam = NewGO<GameCamera>(0, "gamecamera");
			//配置座標、スケール、回転を取得する。
			m_gamecam->SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		//if (objData.EqualObjectName(L"key") == true) {

		//	m_key = NewGO<Key>(0, "key");
		//	//配置座標、スケール、回転を取得する。
		//	m_key->SetPosition(objData.position);
		//	//trueにすると、レベルの方でモデルが読み込まれない。
		//	return true;
		//}
		//if (objData.EqualObjectName(L"skey") == true) {
		//	m_skey = NewGO<Skey>(0, "skey");
		//	//配置座標、スケール、回転を取得する。
		//	m_skey->SetPosition(objData.position);
		//	//trueにすると、レベルの方でモデルが読み込まれない。
		//	return true;
		//}
		//if (objData.EqualObjectName(L"keydoor1") == true) {

		//	m_gamecam = NewGO<GameCamera>(0, "gamecamera");
		//	//配置座標、スケール、回転を取得する。
		//	m_gamecam->SetPosition(objData.position);
		//	//trueにすると、レベルの方でモデルが読み込まれない。
		//	return true;
		//}
		//if (objData.EqualObjectName(L"keydoor2") == true) {

		//	m_gamecam = NewGO<GameCamera>(0, "gamecamera");
		//	//配置座標、スケール、回転を取得する。
		//	m_gamecam->SetPosition(objData.position);
		//	//trueにすると、レベルの方でモデルが読み込まれない。
		//	return true;
		//}
		if (objData.EqualObjectName(L"crashtoy") == true) {

			m_toy = NewGO<Toy>(0, "toy");
			//配置座標、スケール、回転を取得する。
			m_toy->SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		/*if (objData.EqualObjectName(L"gramophone@1") == true) {
			m_gramophone1 = NewGO<Gramophone>(0, "gramophone");
			m_gramophone1->SetPosition(objData.position);
			m_gramophone1->SetScale(objData.scale);
			m_gramophone1->SetRotation(objData.rotation);
			m_gramophone1->SetNumber(objData.number);
			return true;
		}*/
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
		if (objData.ForwardMatchName(L"record") == true)
		{
			auto record = NewGO<Record>(0, "record");
			//record->SetPosition(objData.position);
			record->SetScale(objData.scale);
			record->SetRotation(objData.rotation);
			record->SetNumber(objData.number);
			m_record.push_back(record);
			return true;
		}
	});

	//m_bg = NewGO<BackGround> (0, "background");
	//m_gamecam = NewGO<GameCamera>(0, "gamecamera");
	m_key = NewGO<Key>(0, "key");
	m_player = NewGO<Player>(0, "player");
	m_bell = NewGO<Bell>(0, "bell");
	//m_enemy = NewGO<Enemy>(0, "enemy");
	m_item = NewGO<Item>(0, "item");
	
	return true;
}

void GameStart::Update()
{
	//g_infoForEdge.SetIsSound(1, true);
}

void GameStart::Render(RenderContext& rc)
{
}
