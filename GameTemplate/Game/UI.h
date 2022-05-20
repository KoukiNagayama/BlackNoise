#pragma once
#include "sound/SoundSource.h"
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

	SpriteRender m_spriteRender;	//A�{�^���̉摜
	SoundSource* m_se;				//�A�C�e�����E��
	FontRender m_recordFont;		//���R�[�h���E����
	FontRender m_hammerFont;		//�n���}�[���E����
	FontRender m_crowbarFont;		//�o�[�����E����
	bool m_isDraw = false;		//A�{�^����UI��\�����邩
};

