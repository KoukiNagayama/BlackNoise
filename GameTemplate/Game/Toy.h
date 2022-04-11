#pragma once
class GameCamera;
class Toy : public IGameObject
{
public:
	enum enToyState {
		enToyState_usual,
		enToyState_collapse
	};
	Toy();
	~Toy();
	/// <summary>
	/// �������֐��B
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void Render(RenderContext& rc);
	void Update();
private:
	ModelRender			m_modelRender;		// ���f�������_�[
	Vector3				m_position;			// ���W
	GameCamera*			m_gameCamera;		// �Q�[���J����
	Vector3				m_gameCameraPos;	// �Q�[���J�����̍��W
	enToyState			m_toyState = enToyState_usual;
};

