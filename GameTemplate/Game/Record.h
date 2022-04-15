#pragma once
class Gramophone;
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
private:
	Vector3					m_position;				// ���W
	Vector3					m_scale;		// �g�嗦
	Quaternion				m_rotation;				// ��]
	ModelRender				m_modelRender;			// ���f�������_�[

};

