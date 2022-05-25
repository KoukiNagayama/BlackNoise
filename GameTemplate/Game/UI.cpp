#include "stdafx.h"
#include "UI.h"
#include "sound/SoundEngine.h"
#include "GameCamera.h"
#include "Crowbar.h"
#include "Gramophone.h"
#include "Hammer.h"
#include "Record.h"
#include "FloorGimmick.h"
#include "Item.h"


namespace
{
	const Vector3	ITEM_NAME_POSITION = Vector3(200.0f, -80.0f, 0.0f);		// �A�C�e������\��������W
	const Vector3	SUPPLEMENT_POSITION = Vector3(200.0f, -120.0f, 0.0f);	// �⑫��\��������W
	const float		DISTANCE_TO_ITEM = 150.0f;								// �A�C�e���Ƃ̋���
	const float		DISTANCE_TO_FLOORGIMMICK = 350.0f;						// �t���A�𕕍�����M�~�b�N�Ƃ̋���
	const float		VECTOR_CONSISTENCY = 0.7f;								// �x�N�g������v���Ă��邩��r����l		
	const float		TIME_TO_DISPLAY = 3.0f;									// �A�C�e���ɑ΂��锽���̃e�L�X�g��\�������鎞��
}

bool UI::Start()
{

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

	m_item = FindGO<Item>("item");

	// �o�[���������Ă��邩
	bool isFindCrowbar = FindLocateOfCrowbar();
	// ���R�[�h�������Ă��邩
	bool isFindRecord = FindLocateOfRecord();
	// �t���A�̕����p�M�~�b�N�������Ă��邩
	bool isFindFloorGimmick = FindLocateOfFloorGimmick();

	// �S�Ă������Ă��Ȃ��Ȃ��
	if (isFindCrowbar == false
		&& isFindRecord == false
		&& isFindFloorGimmick == false)
	{
		// �e�L�X�g��`�悵�Ȃ�
		m_isDraw = false;
	}
	
	// ������̕\�����Ԃ����炷
	/*if (m_timeToDisplay >= 0.0f) {
		m_timeToDisplay -= g_gameTime->GetFrameDeltaTime();
	}*/
}

bool UI::FindLocateOfCrowbar()
{
	if (m_crowbar == nullptr) {
		return false;
	}
	// �o�[���̍��W
	Vector3 crowbarPos = m_crowbar->GetPosition();

	// �v���C���[�ƃo�[���̍��W�̍�
	Vector3 diffToItem = crowbarPos - m_playerPos;

	// �v���C���[�ƃo�[������苗���ȉ��Ȃ��
	if (m_item->GetHaveCrowbar() == false 
		&& diffToItem.Length() <= DISTANCE_TO_ITEM
	) {
		// �v���C���[�����Ă�������ɃA�C�e�������邩���ׂ�
		if (CheckPlayerOrientation(diffToItem)) {
			// �\�����镶������w��
			SpecifyStringToDisplay("crowbar");
			return true;
		}
	}
	return false;
}

bool UI::FindLocateOfRecord()
{
	// ���R�[�h���������Ĕz��Ƃ���
	const auto& records = FindGOs<Record>("record");
	// ���R�[�h�z��̃T�C�Y
	const int recordSize = records.size();
	for (int i = 0; i < recordSize; i++) {
		m_record = records[i];
		if (m_record == nullptr) {
			return false;
		}
		// ���R�[�h�̍��W
		Vector3 recordPos = m_record->GetPosition();

		// �v���C���[�ƃ��R�[�h�̍��W�̍�
		Vector3 diffToItem = recordPos - m_playerPos;

		// �v���C���[�ƃ��R�[�h����苗���ȉ��Ȃ��
		if (m_item->GetHaveRecord() == 0
			&& diffToItem.Length() <= DISTANCE_TO_ITEM) 
		{
			// �v���C���[�����Ă�������ɃA�C�e�������邩���ׂ�
			if (CheckPlayerOrientation(diffToItem)) {
				// �\�����镶������w��
				SpecifyStringToDisplay("record");
				return true;
			}
		}

	}
	return false;
}

bool UI::FindLocateOfFloorGimmick()
{
	// ���R�[�h���������Ĕz��Ƃ���
	const auto& gimmicks = FindGOs<FloorGimmick>("floorgimmick");
	// ���R�[�h�z��̃T�C�Y
	const int gimmickSize = gimmicks.size();
	for (int i = 0; i < gimmickSize; i++) {
		m_floorGimmick = gimmicks[i];
		if (m_floorGimmick == nullptr) {
			return false;
		}

		// ���R�[�h�̍��W
		Vector3 gimmickPos = m_floorGimmick->GetPosition();

		// �v���C���[�ƃ��R�[�h�̍��W�̍�
		Vector3 diffToItem = gimmickPos - m_playerPos;

		// �v���C���[�ƃ��R�[�h����苗���ȉ��Ȃ��
		if (m_floorGimmick->GetAttackCount() != 0 
			&& diffToItem.Length() <= DISTANCE_TO_FLOORGIMMICK) {
			// �v���C���[�����Ă�������ɃA�C�e�������邩���ׂ�
			if (CheckPlayerOrientation(diffToItem)) {
				// �\�����镶������w��
				SpecifyStringToDisplay("floorGimmick");
				return true;
			}
		}
	}
	return false;
}

bool UI::CheckPlayerOrientation(Vector3 diffToItem)
{
	// �v���C���[���猩���A�C�e���̕���
	diffToItem.Normalize();
	// ���ς����߂�
	float dot = m_playerForward.Dot(diffToItem);
	// �v���C���[���A�C�e���ɋ߂������������Ă�����
	if (dot >= VECTOR_CONSISTENCY) {
		// �e�L�X�g��`�悷��
		m_isDraw = true;
		return true;
	}
	else {
		// �e�L�X�g��`�悵�Ȃ�
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
		/*if (m_isCrowbarDescript == false) {
			m_isCrowbarDescript = true;
			m_timeToDisplay = TIME_TO_DISPLAY;
		}*/
		return;
	}
	// ���R�[�h�̏ꍇ
	else if (item == "record") {
		// ���R�[�h�p�������ݒ�
		swprintf_s(itemName, 256, L"���R�[�h��");
		swprintf_s(supplement, 256, L"�ǂ����Ŏg���邩��");
		// �A�C�e������ݒ�
		m_itemNameFont.SetText(itemName);
		m_itemNameFont.SetPosition(ITEM_NAME_POSITION);
		// �⑫����ݒ�
		m_supplementFont.SetText(supplement);
		m_supplementFont.SetPosition(SUPPLEMENT_POSITION);

		// �������\������悤�ɐݒ肷��
		if (m_isRecordDescript == false) {
			m_isRecordDescript = true;
			m_timeToDisplay = TIME_TO_DISPLAY;
		}
		return;
	}
	// 
	else if (item == "floorGimmick") {
		// ���R�[�h�p�������ݒ�
		swprintf_s(itemName, 256, L"�؂̔ŕ�������Ă���");
		swprintf_s(supplement, 256, L"�ǂ��ɂ����ĉ󂹂Ȃ����낤��");
		// �A�C�e������ݒ�
		m_itemNameFont.SetText(itemName);
		m_itemNameFont.SetPosition(ITEM_NAME_POSITION);
		// �⑫����ݒ�
		m_supplementFont.SetText(supplement);
		m_supplementFont.SetPosition(SUPPLEMENT_POSITION);

		// �������\������悤�ɐݒ肷��
		if (m_isFloorGimmickDescript == false) {
			m_isFloorGimmickDescript = true;
			m_timeToDisplay = TIME_TO_DISPLAY;
		}
		return;
	}
}

void UI::MakeGetSound()
{
	/*m_se = NewGO<SoundSource>(10);
	m_se->Init(10);
	m_se->SetVolume(1.0f);
	m_se->Play(false);*/
}

void  UI::Render(RenderContext& rc)
{
	if (m_isDraw) {
		// �`��
		m_itemNameFont.Draw(rc);
		m_supplementFont.Draw(rc);
	}
}