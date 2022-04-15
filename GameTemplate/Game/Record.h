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
	/// À•W‚ğİ’è
	/// </summary>
	/// <param name="position">À•W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// Šg‘å—¦‚ğİ’è
	/// </summary>
	/// <param name="scale">Šg‘å—¦</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ‰ñ“]‚ğİ’è
	/// </summary>
	/// <param name="rotation">‰ñ“]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
private:
	Vector3					m_position;				// À•W
	Vector3					m_scale;		// Šg‘å—¦
	Quaternion				m_rotation;				// ‰ñ“]
	ModelRender				m_modelRender;			// ƒ‚ƒfƒ‹ƒŒƒ“ƒ_[

};

