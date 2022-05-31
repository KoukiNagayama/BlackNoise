#pragma once
class Enemy2;
class GameStart;
class GroundFloor;
class Title;
class GameOver : public IGameObject
{
public:
	GameOver() {};
	~GameOver();
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
	void Render(RenderContext& rc);
private:
	/// <summary>
	/// �I�u�W�F�N�g���폜
	/// </summary>
	void DeleteObject();
	/// <summary>
	/// �t�F�[�h�C��
	/// </summary>
	void FadeIn();
	/// <summary>
	/// �t�F�[�h�A�E�g
	/// </summary>
	void FadeOut();
private:
	Enemy2*					m_enemy;						// �G�l�~�[
	SpriteRender			m_gameOverSprite;					// �Q�[���I�[�o�[���̃X�v���C�g
	float					m_currentAlpha = 0.0f;			// ���݂̃��l
	float					m_timerToFadeOut = 0.0f;
	Vector3					m_gameOverPosition;
	Vector3					m_gameOverScale;
	GameStart*				m_gameStart = nullptr;
	GroundFloor*			m_groundFloor = nullptr;
	Title*					m_title = nullptr;
	bool					m_isFadeOut = false;
};

