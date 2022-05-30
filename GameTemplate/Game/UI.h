#pragma once
#include "sound/SoundSource.h"
class Hammer;
class Record;
class Gramophone;
class Crowbar;
class GameCamera;
class Item;
class FloorGimmick;


class UI:public IGameObject
{
public:
	UI() {};
	~UI() {};
	/// <summary>
	/// ����������
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// �{�^���̕\���A��\��
	/// </summary>
	/// <param name="isDraw">0->��\��,1->�\��</param>
	/// <param name="number">0->A�{�^��,1->X�{�^��</param>
	void SetButtonRender(bool isDraw,int number)
	{
		switch (number)
		{
		case 0:
			m_spriteRender.Init("Assets/sprite/Abutton.dds", 128, 128);
			break;
		case 1:
			m_spriteRender.Init("Assets/sprite/Xbutton.dds", 128, 128);
			break;
		default:
			break;
		}
		m_isDraw = isDraw;
	}

	//���E���̐���
	void MakeGetSound();
private:
	/// <summary>
	/// �o�[���̈ʒu������
	/// </summary>
	bool FindLocateOfCrowbar();
	/// <summary>
	/// ���R�[�h�̈ʒu������
	/// </summary>
	bool FindLocateOfRecord();
	/// <summary>
	/// ��������؂̔̈ʒu������
	/// </summary>
	bool FindLocateOfFloorGimmick();
	/// <summary>
	/// �v���C���[���A�C�e���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="diffToItem">�A�C�e���ƃv���C���[�̍��W�̍�</param> 
	bool CheckPlayerOrientation(Vector3 diffToItem);
	/// <summary>
	/// �\�����镶������w��
	/// </summary>
	void SpecifyStringToDisplay(std::string item);

private:
	SpriteRender		m_spriteRender;						// A�{�^���̉摜
	SoundSource*		m_se = nullptr;								// �A�C�e�����E��
	FontRender			m_itemNameFont;						// �A�C�e�����p�t�H���g
	FontRender			m_supplementFont;					// �⑫�p�t�H���g
	bool				m_isDraw = false;					// A�{�^����UI��\�����邩
	Hammer*				m_hammer = nullptr;					// �n���}�[
	Record*				m_record = nullptr;					// ���R�[�h
	Gramophone*			m_gramophone = nullptr;				// �~���@
	Crowbar*			m_crowbar = nullptr;				// �o�[��
	GameCamera*			m_gameCamera = nullptr;				// �Q�[���J����
	FloorGimmick*		m_floorGimmick = nullptr;			// �t���A�����p�M�~�b�N
	Item*				m_item = nullptr;					// �A�C�e���N���X
	Vector3				m_playerForward;					// �v���C���[�̐���
	Vector3				m_playerPos;						// �v���C���[�̍��W
	Vector3				m_diffToItem;						// �A�C�e���Ƃ̍��W�̍�
	bool				m_isCrowbarDescript = false;		// �o�[���̐�����\�����邩
	bool				m_isHammerDescript = false;			// �n���}�[�̐�����\�����邩
	bool				m_isRecordDescript = false;			// ���R�[�h�̐�����\�����邩
	bool				m_isGramophoneDescript = false;		// �~���@�̐�����\�����邩
	bool				m_isFloorGimmickDescript = false;	// �t���A�����p�M�~�b�N�̐�����\�����邩
	float				m_timeToDisplay = 0.0f;				// �e�L�X�g��\�����鎞��
};

