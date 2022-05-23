#include "stdafx.h"
#include "UI.h"
#include "sound/SoundEngine.h"
#include "GameCamera.h"
#include "Crowbar.h"
#include "Gramophone.h"
#include "Hammer.h"
#include "Record.h"


namespace
{
	const Vector3	ITEM_NAME_POSITION = Vector3(200.0f, -80.0f, 0.0f);		// �A�C�e������\��������W
	const Vector3	SUPPLEMENT_POSITION = Vector3(200.0f, -120.0f, 0.0f);	// �⑫��\��������W
	const float		DISTANCE_TO_ITEM = 400.0f;								// �A�C�e���Ƃ̋���
	const float		VECTOR_CONSISTENCY = 0.8f;								// �x�N�g������v���Ă��邩��r����l
	const float		TIME_TO_DISPLAY = 2.5f;
}

bool UI::Start()
{
	//m_spriteRender.Init("Assets/sprite/Abutton.dds", 128, 128);
	//m_spriteRender.SetPosition(Vector3(0.0f, -130.0f, 0.0f));
	
	//�j�󉹂̓ǂݍ���
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/se/item_get.wav");

	m_gameCamera = FindGO<GameCamera>("gamecamera");

	m_crowbar = FindGO<Crowbar>("crowbar");


	return true;
}

void UI::Update()
{

	// �o�[��������
	m_crowbar = FindGO<Crowbar>("crowbar");
	// �v���C���[�̍��W���擾
	m_playerPos = m_gameCamera->GetPosition();
	// �v���C���[�̑O�������擾
	m_playerForward = g_camera3D->GetForward();


	FindLocateOfCrowbar();
	
	// ������̕\�����Ԃ����炷
	if (m_timeToDisplay >= 0.0f) {
		m_timeToDisplay -= g_gameTime->GetFrameDeltaTime();
	}
}

void UI::FindLocateOfCrowbar()
{
	if (m_crowbar == nullptr) {
		return;
	}
	// �o�[���̍��W
	Vector3 crowbarPos = m_crowbar->GetPosition();
	// �v���C���[�ƃA�C�e���̍��W�̍�
	Vector3 diffToItem = crowbarPos - m_playerPos;

	// �v���C���[�ƃo�[������苗���ȉ��Ȃ��
	if (diffToItem.Length() <= DISTANCE_TO_ITEM) {
		// �v���C���[�����Ă�������ɃA�C�e�������邩���ׂ�
		if (CheckPlayerOrientation(diffToItem)) {
			// �\�����镶������w��
			SpecifyStringToDisplay("crowbar");
			return;
		}
	}

}

bool UI::CheckPlayerOrientation(Vector3 diffToItem)
{
	// �v���C���[���猩���A�C�e���̕���
	diffToItem.Normalize();
	// ���ς����߂�
	float dot = m_playerForward.Dot(diffToItem);
	// �v���C���[���A�C�e���ɋ߂������������Ă�����
	if (dot >= VECTOR_CONSISTENCY) {
		m_isDraw = true;
		return true;
	}
	else {
		m_isDraw = false;
		return false;
	}
}

void UI::SpecifyStringToDisplay(std::string item)
{
	wchar_t itemName[256];
	wchar_t supplement[256];
	// �o�[���̏ꍇ
	if (item == "crowbar") {
		// �o�[���p�������ݒ�
		swprintf_s(itemName, 256, L"�o�[����");
		swprintf_s(supplement, 256, L"�����Ɏg���邩��");
		// �A�C�e������ݒ�
		m_itemNameFont.SetText(itemName);
		m_itemNameFont.SetPosition(ITEM_NAME_POSITION);
		// �⑫����ݒ�
		m_supplementFont.SetText(supplement);
		m_supplementFont.SetPosition(SUPPLEMENT_POSITION);

		// �������\������悤�ɐݒ肷��
		if (m_isCrowbarDescript == false) {
			m_isCrowbarDescript = true;
			m_timeToDisplay = TIME_TO_DISPLAY;
		}
		return;
	}
}

void UI::MakeGetSound()
{
	m_se = NewGO<SoundSource>(10);
	m_se->Init(10);
	m_se->SetVolume(1.0f);
	m_se->Play(false);
}

void  UI::Render(RenderContext& rc)
{
	if (m_isDraw) {
		if(m_timeToDisplay >= 0.0f){
			// �`��
			m_itemNameFont.Draw(rc);
			m_supplementFont.Draw(rc);
		}
	}
}