#include "stdafx.h"
#include "GameOver.h"
#include "Enemy2.h"
#include "GameStart.h"
#include "GroundFloor.h"
#include "Title.h"
namespace
{
	const Vector3 DIE_POSITION = Vector3::Zero;
	const Vector3 DIE_SCALE = Vector3::One; 
	const float   FADE_OUT_TIMER_MAX_VALUE = 1.5f;
}
GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	// �G�l�~�[�̌���
	m_enemy = FindGO<Enemy2>("enemy");

	// �X�v���C�g�̏�����
	m_gameOverSprite.Init("Assets/sprite/Game_Over.DDS", 1920, 1080);
	m_gameOverPosition = DIE_POSITION;
	m_gameOverScale = DIE_SCALE;

	m_gameOverSprite.SetPosition(m_gameOverPosition);
	m_gameOverSprite.SetScale(m_gameOverScale);

	m_gameStart = FindGO<GameStart>("gameStart");
	m_groundFloor = FindGO<GroundFloor>("groundfloor");

	m_timerToFadeOut = FADE_OUT_TIMER_MAX_VALUE;

	g_infoForEdge.Clear();

	return true;
}

void GameOver::Update()
{

	// ���l��1.0���Ⴂ�Ȃ��
	if (m_isFadeOut == false) {
		FadeIn();
	}
	else{
		// �t�F�[�h�A�E�g����܂ł̎��Ԃ��v�Z
		m_timerToFadeOut -= 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_timerToFadeOut <= 0.0f) {
			m_timerToFadeOut = 0.0f;
			FadeOut();
		}
	}
	
	m_gameOverSprite.Update();
}

void GameOver::FadeIn()
{
	// ���l�𑝂₷
	m_currentAlpha += 0.5f * g_gameTime->GetFrameDeltaTime();
	// ���l�����O�̎���1.0��荂���Ȃ����Ȃ��
	if (m_currentAlpha >= 1.0f) {
		m_isFadeOut = true;
		DeleteObject();
	}
}

void GameOver::FadeOut()
{
	// ���l�����炷
	m_currentAlpha -= 0.2f * g_gameTime->GetFrameDeltaTime();
	// ���l�����O�̎���0.0���Ⴍ�Ȃ����Ȃ��
	if (m_currentAlpha <= 0.0f) {
		m_title = NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

void GameOver::DeleteObject()
{
	if (m_gameStart != nullptr) {
		DeleteGO(m_gameStart);
	}
	if (m_groundFloor != nullptr) {
		DeleteGO(m_groundFloor);
	}
	g_infoForEdge.Clear();
}

void GameOver::Render(RenderContext& rc)
{
	if (m_currentAlpha >= 0.0f) {
		m_gameOverSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_gameOverSprite.Draw(rc);
	}
}
