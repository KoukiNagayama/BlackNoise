#pragma once
class Title;
class TitleSprite : public IGameObject
{
public:
	TitleSprite() {};
	~TitleSprite() {};
	/// <summary>
	/// ����������
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc">�����_�����O�R���e�L�X�g</param>
	void Render(RenderContext& rc);
private:
	SpriteRender			m_bellSprite;						// �x���̃X�v���C�g
	SpriteRender			m_textSprite;						// �e�L�X�g�̃X�v���C�g
	SpriteRender			m_bButtonSprite;					// B�{�^���̃X�v���C�g
	SpriteRender			m_pressSprite;						// PRESS�̃X�v���C�g
	Vector3					m_bellPos;							// �x���̍��W
	Vector3					m_textPos;							// �e�L�X�g�̍��W
	Vector3					m_bButtonPos;						// B�{�^���̍��W
	Vector3					m_pressPos;							// PRESS�̍��W
	Vector3					m_bellScale;						// �x���̊g�嗦
	Vector3					m_textScale;						// �e�L�X�g�̊g�嗦
	Vector3					m_bButtonScale;						// B�{�^���̊g�嗦
	Vector3					m_pressScale;						// PRESS�̊g�嗦
	Title*					m_title = nullptr;					// �^�C�g��
	float					m_currentAlpha = 1.0f;				// ���݂̃��l
};

