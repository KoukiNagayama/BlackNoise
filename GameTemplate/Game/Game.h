#pragma once
class Player;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	// �A�j���[�V�����N���b�v
	enum EnAnimationClip {
		enAnimClip_Idle,					// �ҋ@�A�j���[�V����
		enAnimClip_Run,						// ����A�j���[�V����
		enAnimClip_Num,						// �A�j���[�V�����N���b�v�̐�
	};

	Light m_light;
	ModelRender m_modelRender;				// ���f�������_�[
	ModelRender m_modelRender2;
	ModelRender m_bgModelRender;		
	Vector3		m_position;					// ���W
	Vector3     m_position2;
	Vector3		m_scale = g_vec3One;		// �g�嗦
	Quaternion	m_rotation;					// ��]
	Animation   m_animation;				// �A�j���[�V����
	AnimationClip m_animationClipArray[enAnimClip_Num];	// �A�j���[�V�����N���b�v
	Vector3		m_spPosition = {0.0f,100.0f,-10.0f};
	Quaternion	m_qRotY;
	Quaternion  m_qRotX;
	SpriteRender m_spriteRender1;
	SpriteRender m_spriteRender2;
};

