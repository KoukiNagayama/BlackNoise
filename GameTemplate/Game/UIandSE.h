#pragma once
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

class GameCamera;
class Bell;
class UIandSE : public IGameObject
{
public:
	UIandSE() {};
	~UIandSE();
	/// <summary>
	/// ����������
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g</param>
	void Render(RenderContext& rc);

	void DrawSprite();
	
private:
	
	
	Vector3 m_disToPlayer;					//�v���C���[�Ƃ̋���
	SpriteRender m_spriteRender;			//�摜
	FontRender m_fontRender;				//����
	SoundSource* m_sound1;					//���ʉ�

	GameCamera* m_gamecam = nullptr;		//�Q�[���J����
	Bell* m_bell = nullptr;					//�x��

	bool m_drawSprite = false;
};

