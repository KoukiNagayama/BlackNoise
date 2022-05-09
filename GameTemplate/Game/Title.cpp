#include "stdafx.h"
#include "Title.h"
#include "TitleCamera.h"
#include "TitleText.h"
#include "BackGround.h"
#include "Gramophone.h"

bool Title::Start()
{
	// 各インスタンスを生成
	m_tCamera = NewGO<TitleCamera>(0, "titleCamera");
	m_tText = NewGO<TitleText>(0, "titleText");
	//m_backGround = NewGO<BackGround>(0, "backGround");
	//m_gramophone = NewGO<Gramophone>(0, "gramophone");


	return true;
}

void Title::Update()
{

}

void Title::Render(RenderContext& rc)
{

}
