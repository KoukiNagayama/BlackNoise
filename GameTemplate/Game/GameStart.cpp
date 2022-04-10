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
	//�e�N���X�𐶐��B

	m_levelRender.Init("Assets/modelData/stage/stage_2.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"stage") == true) {

			m_bg = NewGO<BackGround>(0);
			//�z�u���W�A�X�P�[���A��]���擾����B
			m_bg->SetPosition(objData.position);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;
		}
		if (objData.EqualObjectName(L"player") == true) {

			m_gamecam = NewGO<GameCamera>(0, "gamecamera");
			//�z�u���W�A�X�P�[���A��]���擾����B
			m_gamecam->SetPosition(objData.position);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;
		}
		if (objData.EqualObjectName(L"key") == true) {

			m_key = NewGO<Key>(0, "key");
			//�z�u���W�A�X�P�[���A��]���擾����B
			m_key->SetPosition(objData.position);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;
		}
		if (objData.EqualObjectName(L"skey") == true) {
			m_skey = NewGO<Skey>(0, "skey");
			//�z�u���W�A�X�P�[���A��]���擾����B
			m_skey->SetPosition(objData.position);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;
		}
		//if (objData.EqualObjectName(L"keydoor1") == true) {

		//	m_gamecam = NewGO<GameCamera>(0, "gamecamera");
		//	//�z�u���W�A�X�P�[���A��]���擾����B
		//	m_gamecam->SetPosition(objData.position);
		//	//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
		//	return true;
		//}
		//if (objData.EqualObjectName(L"keydoor2") == true) {

		//	m_gamecam = NewGO<GameCamera>(0, "gamecamera");
		//	//�z�u���W�A�X�P�[���A��]���擾����B
		//	m_gamecam->SetPosition(objData.position);
		//	//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
		//	return true;
		//}
		if (objData.EqualObjectName(L"toy") == true) {

			m_toy = NewGO<Toy>(0, "toy");
			//�z�u���W�A�X�P�[���A��]���擾����B
			m_toy->SetPosition(objData.position);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
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
