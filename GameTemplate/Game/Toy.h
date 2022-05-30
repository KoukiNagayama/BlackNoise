#pragma once
#include "sound/SoundSource.h"

class GameCamera;
class Toy : public IGameObject
{
public:
	enum enToyState {
		enToyState_usual,		// �ʏ펞
		enToyState_collapse		// ����
	};
	Toy() {};
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
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc">�����_�����O�R���e�L�X�g</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	bool IsSound() const
	{
		return m_isSound;
	}
private:
	/// <summary>
	/// ����炷
	/// </summary>
	void MakeSound();
	/// <summary>
	/// �e�����𒲂ׂ�
	/// </summary>
	void CheckRate();
private:
	ModelRender			m_modelRender;					// ���f�������_�[
	Vector3				m_position;						// ���W
	GameCamera*			m_gameCamera;					// �Q�[���J����
	Vector3				m_gameCameraPos;				// �Q�[���J�����̍��W
	enToyState			m_toyState = enToyState_usual;
	float				m_rateByTime;
	SoundSource*		m_sound = nullptr;
	bool				m_isSound = false;

};

