#pragma once
class TitleText : public IGameObject
{
public:
	TitleText() {};
	~TitleText() {};
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
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3 GetPosition() const
	{
		return m_position;
	}
private:
	ModelRender			m_modelRender;			// ���f�������_�[
	Vector3				m_position;				// ���W
	Vector3				m_scale;				// �g�嗦
	Quaternion			m_rotation;				// ��]
};
