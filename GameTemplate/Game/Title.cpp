#include "stdafx.h"
#include "Title.h"
#include "TitleCamera.h"
#include "TitleText.h"
#include "BackGround.h"
#include "TitleGramophone.h"
#include "TitleSprite.h"
#include "TitlePiano.h"

Title::~Title()
{
	// 登録された音源データのクリア
	g_infoForEdge.Clear();


}

bool Title::Start()
{
	// 各インスタンスを生成
	m_levelRender1.Init("Assets/level3D/title.tkl", [&](LevelObjectData& objData)
	{
		if (objData.EqualObjectName(L"titleText") == true) {
			m_titleText = NewGO<TitleText>(0, "titleText");
			m_titleText->SetPosition(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"titleCamera") == true) {
			m_titleCamera = NewGO<TitleCamera>(0, "titleCamera");
			m_titleCamera->SetPosition(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"piano") == true) {
			m_titlePiano = NewGO<TitlePiano>(0, "titlePiano");
			m_titlePiano->SetPosition(objData.position);
			return true;
		}
	});
	m_levelRender2.Init("Assets/level3D/stage2.tkl", [&](LevelObjectData& objData)
	{
		if (objData.EqualObjectName(L"floor2") == true) {
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"gramophone@3") == true) {
			m_titleGramophone = NewGO<TitleGramophone>(0, "titleGramophone");
			m_titleGramophone->SetPosition(objData.position);
			m_titleGramophone->SetRotation(objData.rotation);
			return true;
		}
	});
	m_titleSprite = NewGO<TitleSprite>(0, "titleSprite");

	return true;
}

void Title::Update()
{
	if (m_titleState == enTitleState_Idle) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_titleState = enTitleState_PlayPiano;
		}
	}
	else if (m_titleState == enTitleState_PlayGramophone) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			DeleteGO(this);
		}
	}
}

void Title::Render(RenderContext& rc)
{
	
}
