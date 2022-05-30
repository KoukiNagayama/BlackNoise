#include "stdafx.h"
#include "Title.h"
#include "TitleCamera.h"
#include "TitleText.h"
#include "BackGround.h"
#include "TitleSprite.h"
#include "GameStart.h"

#include "sound/SoundEngine.h"


namespace {
	const float RANGE = 800.0f;						// �x���̉��ɂ��֊s�����`�悳���͈�
	const float EDGE_FADE_IN_DELTA_VALUE = 0.01f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.005f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
}

Title::~Title()
{
	DeleteGO(m_titleCamera);
	DeleteGO(m_titleSprite);
	DeleteGO(m_titleText);
	DeleteGO(m_backGround);
	g_infoForEdge.Clear();
}

bool Title::Start()
{
	// ������o�^
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/item/bell_low.wav");
	

	// �e�I�u�W�F�N�g�𐶐�
	m_levelRender1.Init("Assets/level3D/title.tkl", [&](LevelObjectData& objData)
	{
		// �^�C�g�����f��
		if (objData.EqualObjectName(L"titleText") == true) {
			m_titleText = NewGO<TitleText>(0, "titleText");
			m_titleText->SetPosition(objData.position);
			return true;
		}
		// �^�C�g���p�J����
		if (objData.EqualObjectName(L"titleCamera") == true) {
			m_titleCamera = NewGO<TitleCamera>(0, "titleCamera");
			m_titleCamera->SetPosition(objData.position);
			return true;
		}
		// �w�i
		if (objData.EqualObjectName(L"stage2") == true) {
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			return true;
		}
	});
	// �^�C�g���p�摜
	m_titleSprite = NewGO<TitleSprite>(0, "titleSprite");

	// �^�C�g���p�J����������
	m_titleCamera = FindGO<TitleCamera>("titleCamera");
	// �^�C�g���p�J�����̍��W���擾
	m_position = m_titleCamera->GetPosition();

	// �����f�[�^��������
	g_infoForEdge.InitForSound(0, m_position, RANGE, 0, 1.0f);
	return true;
}

void Title::Update()
{
	ManageState();

	ChangeRate();

	g_infoForEdge.SetInfoForSound(0, m_position, RANGE, m_isRateChange, m_rateByTime);
}

void Title::ManageState()
{
	switch (m_titleState) {
	// �ҋ@���Ȃ��
	case enTitleState_Idle:
		ProcessIdleStateTransition();
		break;
	// �x����炵�Ă����ԂȂ��
	case enTitleState_RingBell:
		ProcessRingBellStateTransition();
		break;
	case enTitleState_FadeOut:
		ProcessFromFadeOutStateToStartingGame();
		break;
	}
}

void Title::ProcessIdleStateTransition()
{
	// B�{�^���������ꂽ�Ȃ��
	if (g_pad[0]->IsTrigger(enButtonB)) {
		// �X�e�[�g���x����炵����ԂɑJ��
		m_titleState = enTitleState_RingBell;
		// ����炷
		MakeSound();
	}
}

void Title::ProcessRingBellStateTransition()
{
	if (m_isEndFadeIn == false) {
		return;
	}
	// A�{�^���������ꂽ�Ȃ��
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_titleState = enTitleState_FadeOut;
	}
}

void Title::ProcessFromFadeOutStateToStartingGame()
{
	if (m_isEndStanging == true) {
		// �֊s���̓o�^�����N���A����
		g_infoForEdge.Clear();
		// �Q�[���N���X�̃I�u�W�F�N�g�𐶐�����
		m_gameStart = NewGO<GameStart>(0, "gamestart");
		// �폜
		DeleteGO(this);
	}
}

void Title::MakeSound()
{
	m_bellSound = NewGO<SoundSource>(0);
	m_bellSound->Init(10);
	m_bellSound->Play(false);
	g_infoForEdge.SetIsSound(0, 1);
}

void Title::ChangeRate()
{
	switch (m_titleState) {
	case enTitleState_RingBell:
		IncreaseRate();
		break;
	case enTitleState_FadeOut:
		ReduceRate();
		break;
	}
}

void Title::IncreaseRate()
{
	if (m_bellSound != nullptr) {
		if (m_bellSound->IsPlaying() == true)
		{
			m_isRateChange = 1;
			if (m_rateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_rateByTime += EDGE_FADE_IN_DELTA_VALUE;

			}
			if (m_rateByTime >= RATE_BY_TIME_MAX_VALUE) {
				m_isEndFadeIn = true;
			}
		}
	}
}

void Title::ReduceRate()
{
	m_isRateChange = 0;
	if (m_rateByTime > RATE_BY_TIME_MIN_VALUE && m_isRateChange == 0) {
		m_rateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTime <= RATE_BY_TIME_MIN_VALUE) {
			m_isEndStanging = true;
		}
	}
}

void Title::Render(RenderContext& rc)
{
	
}
