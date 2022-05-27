#include "stdafx.h"
#include "TitleSprite.h"
#include "Title.h"

namespace
{
	const Vector3 SPRITE_POSITION1 = Vector3(-7.7f, -10.0f, 0.0f);
	const Vector3 SPRITE_POSITION2 = Vector3(7.0f, -20.8f, 0.0f);
	const float SPRITE_SCALE = 0.9f;
}
bool TitleSprite::Start()
{
	// �e�X�v���C�g��������
	m_spriteRender1.Init("Assets/sprite/PRESS_A_TO_PLAY_MUSIC.dds", 1920, 1080);
	m_spriteRender2.Init("Assets/sprite/PRESS_A_TO_START.dds", 1920, 1080);

	// �X�v���C�g�̍��W���w��
	m_spritePos1 = SPRITE_POSITION1;
	m_spritePos2 = SPRITE_POSITION2;

	// �X�v���C�g�̊g�嗦���w��
	m_spriteScale = Vector3(SPRITE_SCALE, SPRITE_SCALE, SPRITE_SCALE);

	// �w�肵�����W�A�g�嗦��ݒ�
	m_spriteRender1.SetPosition(m_spritePos1);
	m_spriteRender1.SetScale(m_spriteScale);
	m_spriteRender2.SetPosition(m_spritePos2);
	m_spriteRender2.SetScale(m_spriteScale);

	// �^�C�g���I�u�W�F�N�g������
	m_title = FindGO<Title>("title");

	return true;
}

void TitleSprite::Update()
{
	// �e�X�v���C�g���X�V
	m_spriteRender1.Update();
	m_spriteRender2.Update();
}

void TitleSprite::Render(RenderContext& rc)
{
	// �X�e�[�g���ҋ@���Ȃ��
	if (m_title->IsIdleStateNow() == true) {
		m_spriteRender1.Draw(rc);
	}
	// �X�e�[�g���ҋ@���ȊO�Ȃ��
	else {
		m_spriteRender2.Draw(rc);
	}
}
