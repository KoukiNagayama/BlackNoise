#pragma once
#include "EnemyPath.h"
#include "AI/PathFinding/PathFinding.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"

class Player;
class Bell;
class GameCamera;
/// <summary>
/// �G�l�~�[�N���X
/// </summary>
class Enemy2 : public IGameObject
{
public:
	Enemy2() {};
	~Enemy2() {};

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
	/// <param name="rc">�����_�����O�R���e�L�X�g</param>
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
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
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
	/// �G�l�~�[�̔ԍ���ݒ�
	/// </summary>
	/// <param name="number">�ԍ�</param>
	void SetNumber(const int& number)
	{
		m_enemyNumber = number;
	}
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
private:
	/// <summary>
	/// ��]
	/// </summary>
	void Rotation();
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �X�e�[�g�ɂ�鏈��
	/// </summary>
	void ProcessByState();
	/// <summary>
	/// �v���C���[�̉���T��
	/// </summary>
	void SearchSoundOfPlayer();
	/// <summary>
	/// �ʏ�ړ�
	/// </summary>
	void Walk();
	/// <summary>
	/// ���K
	/// </summary>
	void Scream();
	/// <summary>
	/// ���K�ɂ��֊s��
	/// </summary>
	void OutlineByScream();
	/// <summary>
	/// �ǐ�
	/// </summary>
	void Chase();
	/// <summary>
	/// ����
	/// </summary>
	void Survey();
	/// <summary>
	/// �p�X�ړ��ւ̋A��
	/// </summary>
	void ReturnToPath();
	/// <summary>
	/// �������̃X�e�[�g�J��
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// �ǐՎ��̃X�e�[�g�J��
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// �U�����̃X�e�[�g�J��
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// ���ю��̃X�e�[�g�J��
	/// </summary>
	void ProcessScreamStateTransition();
	/// <summary>
	/// ���񂵎��̃X�e�[�g�J��
	/// </summary>
	void ProcessSurveyStateTransition();
	/// <summary>
	/// �p�X�ړ��ւ̋A�Ҏ��̃X�e�[�g�J��
	/// </summary>
	void ProcessReturnToPathStateTransition();
	/// <summary>
	/// �A�j���[�V�����Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �����̗֊s���`��
	/// </summary>
	void OutlineByStep();
	/// <summary>
	/// �v���C���[�Ɖ����̋����ɂ�艹�̑傫���𒲐�����
	/// </summary>
	/// <param name="range">�����̉e���͈�</param>
	float SoundLevelByDistance(float range);
	/// <summary>
	/// ���ʒ���
	/// </summary>
	void StepVolumeControl();
	// �A�j���[�V�����C�x���g�p�̊֐��B
	void OnStepAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

private:
	// �X�e�[�g
	enum EnEnemyState {
		enEnemyState_Walk,				// ����	
		enEnemyState_Scream,			// ���K
		enEnemyState_Chase,				// �ǐ�
		enEnemyState_Survey,			// ����
		enEnemyState_ReturnToPath,		// �p�X�ړ��ɖ߂�
		enEnemyState_Attack,			// �U��
		enEnemyState_Num,				// �X�e�[�g�̐�
	};

	// �A�j���[�V����
	enum EnAnimationClip {
		enAnimationClip_Walk,			// ����
		enAnimationClip_Run,			// ����
		enAnimationClip_Scream,			// ���K
		enAnimationClip_Survey,			// ����
		enAnimationClip_Attack,			// �U��
		enAnimationClip_Num,			// �A�j���[�V�����̐�
	};

	ModelRender			m_modelRender;								// ���f�������_�[
	Vector3				m_position;									// ���W
	Vector3				m_scale = Vector3::One * 5.0f;				// �g�嗦
	Quaternion			m_rotation = Quaternion::Identity;			// ��]
	int					m_enemyNumber;								// ���x����œo�^���ꂽ�G�l�~�[�̔ԍ�
	AnimationClip		m_animationClips[enAnimationClip_Num];		// �A�j���[�V�����N���b�v
	Vector3				m_moveVector;								// �ړ��������
	Vector3				m_moveSpeed;								// �ړ����鑬��
	EnemyPath			m_enemyPath;								// �p�X
	Point*				m_point;									// �p�X�ړ��̃|�C���g
	Player*				m_player;									// �v���C���[���
	EnEnemyState		m_enemyState = enEnemyState_Walk;			// �X�e�[�g
	Bell*				m_bell = nullptr;							// �x��
	SoundSource*		m_screamSound = nullptr;					// ���K���̉���
	SoundSource*		m_stepSound = nullptr;						// ����
	float				m_screamRateByTime = 0.0f;					// ���K�̎��Ԃɂ��e����
	float				m_stepRateByTime = 0.0f;					// �����ɂ��e����
	float				m_chaseTime;								// �ǂ����������鎞��
	bool				m_isFound = false;							// �v���C���[�������Ă��邩
	bool				m_isAttackable = false;						// �v���C���[�ւ̍U���͉\��
	nsAI::NaviMesh		m_nvmMesh;									// �i�r�Q�[�V�������b�V��
	nsAI::Path			m_path;										// �i�r�Q�[�V�������b�V���p�p�X
	nsAI::PathFinding	m_pathFinding;								// �p�X����
	Vector3				m_lastPosition;								// ���O�̍��W
	float				m_surveyTimer = 0.0f;						// ���񂷎���
	bool				m_isEndScream = false;						// ���K���I�����Ă��邩
	bool				m_isGameOver = false;						// �Q�[���I�[�o�[�ɂȂ��Ă��邩
	GameCamera*			m_gameCamera = nullptr;						// �Q�[���J����
	float				m_timeToReturn = 0.0f;						// �A�҂��鎞��
	float				m_animationSpeed = 1.0f;					// �A�j���[�V�����̃X�s�[�h
	float				m_pathFindingTimer = 0.0f;					// �ړI�n�܂ł̃p�X����������Ԃ̎���
	bool				m_isPathFindingWhileReturning = false;		// �p�X�ړ��ւ̋A�Ҏ��Ƀp�X������������
};

