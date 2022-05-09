#pragma once
class Gramophone;
class GameCamera;
class Item;

class Record : public IGameObject
{
public:
	Record() {};
	~Record() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// �g�嗦��ݒ�
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �I�u�W�F�N�g�̔ԍ���ݒ�
	/// </summary>
	/// <param name="number">�I�u�W�F�N�g�̔ԍ�</param>
	void SetNumber(const int& number)
	{
		m_number = number;
	}
private:
	Vector3					m_position;							// ���W
	Vector3					m_scale = Vector3::Zero;			// �g�嗦
	Quaternion				m_rotation = Quaternion::Identity;	// ��]
	ModelRender				m_modelRender;						// ���f�������_�[
	int						m_number;
	GameCamera*				m_gameCamera;
	Vector3					m_gameCameraPos;
	Item*					m_item;

};

