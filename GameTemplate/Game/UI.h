#pragma once
#include "sound/SoundSource.h"
class UI:public IGameObject
{
public:
	UI() {};
	~UI();
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
	/// A�{�^����\���E��\���ɂ���
	/// </summary>
	/// <param name="isDraw">�\��->1</param>
	void SetUIRender(bool isDraw)
	{
		m_isDraw_A = isDraw;
	}

private:
	//���E���̐���
	void MakeSound();

	SpriteRender m_spriteRender;	//A�{�^���̉摜
	SoundSource* m_se;				//�A�C�e�����E��
	FontRender m_recordFont;		//���R�[�h���E����
	FontRender m_hammerFont;		//�n���}�[���E����
	FontRender m_crowbarFont;		//�o�[�����E����
	bool m_isDraw_A = false;		//A�{�^����UI��\�����邩
};

