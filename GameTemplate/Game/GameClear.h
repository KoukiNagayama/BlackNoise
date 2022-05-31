#pragma once
class GroundFloor;
class Title;

class GameClear : public IGameObject
{
public:
	GameClear() {};
	~GameClear();
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
	/// <summary>
	/// �Q�[���N���A���Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsGameClear() const
	{
		return m_isGameClear;
	}
private:
	SpriteRender				m_gameClearSprite;					// �Q�[���N���A���̃X�v���C�g
	GroundFloor*				m_groundFloor = nullptr;
	float						m_timerToDrawSprite;
	bool						m_isDraw = false;
	bool						m_isGameClear = true;
	Title* m_title = nullptr;
};

