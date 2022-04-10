#include "stdafx.h"
#include "GameStart.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Bell.h"
#include "Toy.h"
#include "Key.h"
#include "Skey.h"
#include "InfoForEdge.h"


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
	g_infoForEdge.InitForSound(0, position, 200.0f, 1, 0.00f);
	//各クラスを生成。

	m_levelRender.Init("Assets/modelData/stage/stage_2.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"stage") == true) {

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
		if (objData.EqualObjectName(L"key") == true) {

			m_key = NewGO<Key>(0, "key");
			//配置座標、スケール、回転を取得する。
			m_key->SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
		if (objData.EqualObjectName(L"skey") == true) {
			m_skey = NewGO<Skey>(0, "skey");
			//配置座標、スケール、回転を取得する。
			m_skey->SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
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
		if (objData.EqualObjectName(L"toy") == true) {

			m_toy = NewGO<Toy>(0, "toy");
			//配置座標、スケール、回転を取得する。
			m_toy->SetPosition(objData.position);
			//trueにすると、レベルの方でモデルが読み込まれない。
			return true;
		}
	});

	//m_bg = NewGO<BackGround> (0, "background");
	//m_gamecam = NewGO<GameCamera>(0, "gamecamera");
	m_player = NewGO<Player>(0, "player");
	m_bell = NewGO<Bell>(0, "bell");
	return true;
}

void GameStart::Update()
{
	//g_infoForEdge.SetIsSound(1, true);
}

void GameStart::Render(RenderContext& rc)
{
}
