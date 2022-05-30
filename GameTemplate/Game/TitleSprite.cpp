#include "stdafx.h"
#include "TitleSprite.h"
#include "Title.h"

namespace
{
	const Vector3 BELL_POSITION = Vector3::Zero;
	const Vector3 TEXT_POSITION = Vector3(7.0f, -20.8f, 0.0f);
	const Vector3 B_POSITION = Vector3(0.0f, 30.0f, 0.0f);
	const Vector3 PRESS_POSITION = Vector3(0.0f, -180.0f, 0.0f);
	const Vector3 BELL_SCALE = Vector3(0.3f, 0.3f, 0.3f);
	const Vector3 TEXT_SCALE = Vector3(0.9f, 0.9f, 0.9f);
	const Vector3 B_SCALE = Vector3(0.35f, 0.35f, 0.35f);
	const Vector3 PRESS_SCALE = Vector3::One;
}
bool TitleSprite::Start()
{
	// �e�X�v���C�g��������
	m_bellSprite.Init("Assets/sprite/Bell.DDS", 500, 862);
	m_textSprite.Init("Assets/sprite/PRESS_A_TO_START.dds", 1920, 1080);
	m_bButtonSprite.Init("Assets/sprite/B.DDS", 512, 512);
	m_pressSprite.Init("Assets/sprite/PRESS.DDS", 512, 512);

	// �X�v���C�g�̍��W���w��
	m_bellPos = BELL_POSITION;
	m_textPos = TEXT_POSITION;
	m_bButtonPos = B_POSITION;
	m_pressPos = PRESS_POSITION;

	// �X�v���C�g�̊g�嗦���w��
	m_bellScale = BELL_SCALE;
	m_textScale = TEXT_SCALE;
	m_bButtonScale = B_SCALE;
	m_pressScale = PRESS_SCALE;

	// �w�肵�����W�A�g�嗦��ݒ�
	m_bellSprite.SetPosition(m_bellPos);
	m_bellSprite.SetScale(m_bellScale);
	m_textSprite.SetPosition(m_textPos);
	m_textSprite.SetScale(m_textScale);
	m_bButtonSprite.SetPosition(m_bButtonPos);
	m_bButtonSprite.SetScale(m_bButtonScale);
	m_pressSprite.SetPosition(m_pressPos);
	m_pressSprite.SetScale(m_pressScale);

	// �^�C�g���I�u�W�F�N�g������
	m_title = FindGO<Title>("title");

	return true;
}

void TitleSprite::Update()
{
	// �t�F�[�h�A�E�g���̓X�v���C�g���t�F�[�h�A�E�g������
	if (m_title->IsFadeOutStateNow()) {
		m_currentAlpha -= 0.3f * g_gameTime->GetFrameDeltaTime();
		// ���݂̃��l��0�ȉ��Ȃ��
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
		}
	}
	// �e�X�v���C�g���X�V
	m_bellSprite.Update();
	m_textSprite.Update();
	m_bButtonSprite.Update();
	m_pressSprite.Update();
}

void TitleSprite::Render(RenderContext& rc)
{
	// �X�e�[�g���ҋ@���Ȃ��
	if (m_title->IsIdleStateNow() == true) {
		m_bellSprite.Draw(rc);
		m_bButtonSprite.Draw(rc);
		m_pressSprite.Draw(rc);
	}
	// �X�e�[�g���ҋ@���ȊO�Ȃ��
	else{
		if (m_currentAlpha > 0.0f) {
			m_textSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
			m_textSprite.Draw(rc);
		}
	}
}
