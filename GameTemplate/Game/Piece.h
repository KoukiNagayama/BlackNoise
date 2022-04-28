#pragma once
class GameCamera;
class Key;
class Piece:public IGameObject
{
public:
	Piece() {};
	~Piece() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPosition(Vector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �g�嗦�̐ݒ�
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rot">�N�H�[�^�j�I��</param>
	void SetRotation(Quaternion& rot)
	{
		m_rotation = rot;
	}

private:
	Vector3 m_position; //���W
	Vector3 m_scale; //�g�嗦
	Quaternion m_rotation; //��]
	GameCamera* m_gamecam = nullptr; //�Q�[���J����
	Key* m_key = nullptr; //��
	ModelRender m_modelRender; //���f��
};

