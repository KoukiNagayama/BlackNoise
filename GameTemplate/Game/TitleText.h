#pragma once
class TitleText : public IGameObject
{
public:
	TitleText() {};
	~TitleText() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	ModelRender			m_modelRender;			// ���f�������_�[
	Vector3				m_position;				// ���W
	Vector3				m_scale;				// �g�嗦
	Quaternion			m_rotation = Quaternion::Identity;				// ��]
};

