#include "stdafx.h"
#include "Title.h"
#include "TitleCamera.h"
#include "TitleText.h"
#include "BackGround.h"
#include "Gramophone.h"

Title::~Title()
{
	// 登録された音源データのクリア
	g_infoForEdge.Clear();
}

bool Title::Start()
{
	// 各インスタンスを生成
	//m_tCamera = NewGO<TitleCamera>(0, "titleCamera");
	//m_tText = NewGO<TitleText>(0, "titleText");
	//m_backGround = NewGO<BackGround>(0, "backGround");
	//m_gramophone = NewGO<Gramophone>(0, "gramophone");
	m_levelRender1.Init("Assets/level3D/title.tkl", [&](LevelObjectData& objData)
	{
		if (objData.EqualObjectName(L"titleText") == true) {
			m_tText = NewGO<TitleText>(0, "titleText");
			m_tText->SetPosition(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"titleCamera") == true) {
			m_tCamera = NewGO<TitleCamera>(0, "titleCamera");
			m_tCamera->SetPosition(objData.position);
			return true;
		}
	});

	m_levelRender2.Init("Assets/modelData/stage/stage2.tkl", [&](LevelObjectData& objData)
	{
		if (objData.EqualObjectName(L"floor2") == true) {
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			return true;
		}
		if (objData.EqualObjectName(L"gramophone") == true) {
			m_gramophone = NewGO<Gramophone>(0, "gramophone");
			m_gramophone->SetPosition(objData.position);
			m_gramophone->SetRotation(objData.rotation);
			return true;
		}
	});

	return true;
}

void Title::Update()
{
	
}

void Title::Render(RenderContext& rc)
{

}
