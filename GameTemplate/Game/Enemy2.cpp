#include "stdafx.h"
#include "Enemy2.h"
#include "EnemyPath.h"
#include "Player.h"
#include "Bell.h"
#include "GameCamera.h"
#include "Toy.h"
#include "GameOver.h"
#include "GroundFloor.h"

namespace
{
	const float WALK_SPEED = 6.5f * 0.7f;							// ��������
	const float RUN_SPEED = 9.5f * 0.7f;							// ���鑬��
	const float SEARCH_RANGE_TO_BELL = 1000.0f;						// �x���̉�����������͈�
	const float SEARCH_RANGE_TO_TOY = 1300.0f;				
	const float SEARCH_RANGE_TO_FOOTSTEP = 200.0f;					// ��������������͈�
	const float SCREAM_VOLUME = 1.0f;								// ���K�̉���
	const float MAXIMUM_VOLUME = 0.9f;								// �����ő剹��
	const float MINIMUM_VOLUME = 0.00f;								// �����ŏ�����
	const float SCREAM_RANGE = 1300.0f;								// ���K���ɗ֊s�����K�p�����͈�
	const float STEP_RANGE = 450.0f;								// ���s���ɗ֊s�����K�p�����͈�
	const float STEP_SOUNDRANGE = 1350.0f;							// ���s���Ƀv���C���[�ɕ�������͈�
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;					// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;					// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;						// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;						// ���Ԃɂ��e�����̍ŏ��l
	const float MINIMUM_CHASE_TIME = 20.0f;							// �Œ���ǐՂ��鎞��
	const float ENEMY_RADIUS = 30.0f;								// �G�l�~�[�̔��a
	const float ENEMY_HEIGHT = 200.0f;								// �G�l�~�[�̍���
	const float ATTACKING_RANGE = 120.0f;							// �U���\�ȋ���
	const float DISTANCE_TO_TARGET_WHILE_WALKING = 10.0f;			// �������̖ڕW�n�_�܂ł̋���
	const float DISTANCE_TO_TARGET_WHILE_RETURNING = 30.0f;			// ���Ԓ��̖ڕW�n�_�܂ł̋���
	const float TIME_TO_LOSE_SIGHT = 5.0f;							// �v���C���[��������������
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;			// �A�j���[�V�����̕�Ԏ���
	const float TIME_TO_FORCE_STATE_TRANSITION = 6.0f;				// �����I�ɃX�e�[�g�J�ڂ��鎞��
	const float ANIMATION_SPEED = 0.7f;								// �A�j���[�V�����̃X�s�[�h�̔{��
	const float PATH_FINDING_TIMER = 0.7f;							// �i�r���b�V���ɂ��p�X���������鎞�Ԃ̊Ԋu

}

Enemy2::~Enemy2()
{
	g_infoForEdge.SetIsSound(7, 0);
	g_infoForEdge.SetRate(7, 0.00f);
	g_infoForEdge.SetIsSound(8, 0);
	g_infoForEdge.SetRate(8, 0.00f);
	if (m_screamSound != nullptr) {
		DeleteGO(m_screamSound);
	}
	if (m_stepSound != nullptr) {
		DeleteGO(m_stepSound);
	}
}

bool Enemy2::Start()
{

	// �A�j���[�V����
	// �������̃A�j���[�V���������[�h
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	// ���莞�̃A�j���[�V���������[�h
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	// ���ю��̃A�j���[�V���������[�h
	m_animationClips[enAnimationClip_Scream].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Scream].SetLoopFlag(false);
	// ���񂵎��̃A�j���[�V���������[�h
	m_animationClips[enAnimationClip_Survey].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Survey].SetLoopFlag(true);
	// �U�����̃A�j���[�V���������[�h
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(true);


	// ���f���̏�����
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, enAnimationClip_Num, false, enModelUpAxisZ, false, 0, 2);

	// �A�j���[�V�����C�x���g��ǉ�
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		// ����
		OnStepAnimationEvent(clipName, eventName);
	});

	// �G�l�~�[�̃p�X�ړ��̏���������
	std::string filePath;
	filePath = "Assets/enemypath/enemypath";
	filePath += std::to_string(m_enemyNumber);
	filePath += ".tkl";
	// �쐬�����t�@�C���p�X�����Ƃɏ�����
	m_enemyPath.Init(filePath.c_str());
	// �ŏ��̃p�X�ړ��̃|�C���g�����擾
	m_point = m_enemyPath.GetFirstPoint();


	// ���f���̏����̍��W�A��]�A�g�嗦��ݒ�
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	// �v���C���[�����擾
	m_player = FindGO<Player>("player");
	m_bell = FindGO<Bell>("bell");
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	// �����̓o�^
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_caution.wav");
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_warning.wav");
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_danger.wav");
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/enemy/scream/scream.wav");
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/enemy/step/walk1.wav");
	g_soundEngine->ResistWaveFileBank(15, "Assets/sound/enemy/step/walk2.wav");
	g_soundEngine->ResistWaveFileBank(16, "Assets/sound/enemy/step/run1.wav");
	g_soundEngine->ResistWaveFileBank(17, "Assets/sound/enemy/step/run2.wav");

	// �֊s�����̏�����
	g_infoForEdge.InitForSound(7, m_position, SCREAM_RANGE, 0, m_screamRateByTime);
	// �֊s�����̏�����
	g_infoForEdge.InitForSound(8, m_position, STEP_RANGE, 0, m_screamRateByTime);

	// �G�l�~�[�ɂ���ăi�r�Q�[�V�������b�V����ύX
	if (m_enemyNumber == 1) {
		// �i�r�Q�[�V�������b�V���̏�����
		m_nvmMesh.Init("Assets/modelData/enemy/stage2_mesh.tkn");
	}
	else if (m_enemyNumber == 2) {
		m_nvmMesh.Init("Assets/modelData/enemy/stage1_mesh.tkn");
	}


	// �A�j���[�V�����X�s�[�h����
	m_animationSpeed = ANIMATION_SPEED;
	// �A�j���[�V�����̃X�s�[�h���w��
	m_modelRender.SetAnimationSpeed(m_animationSpeed);
	// ��������̌���
	m_toy = FindGO<Toy>("toy");
	return true;
}

void Enemy2::Update()
{

	// ��]
	Rotation();
	// �X�e�[�g�ɂ�鏈��
	ProcessByState();
	// �X�e�[�g�Ǘ�
	ManageState();
	// �A�j���[�V�����Đ�
	PlayAnimation();
	//�����̗֊s���`��
	OutlineByStep();
	// ���W�̍X�V
	m_modelRender.SetPosition(m_position);
	// ���f���̍X�V
	m_modelRender.Update();

	if (m_groundFloor == nullptr) {
		m_groundFloor = FindGO<GroundFloor>("groundfloor");
		return;
	}
	if (m_groundFloor->IsGameClear() == true) {
		m_enemyState = enEnemyState_Survey;
	}
}

void Enemy2::SearchSoundOfPlayer()
{
	// ���G�͈�
	float searchRange = 0.0f;

	Vector3 playerPos = m_player->GetPosition();
	// �v���C���[�Ƃ̋���
	Vector3 distance = m_position - playerPos;

	// �G�l�~�[�̔ԍ���1�Ȃ��
	if (m_enemyNumber == 1) {
		if (m_toy->IsSound()) {
			searchRange = SEARCH_RANGE_TO_TOY;
		}
	}
	// �x�������Ă���Ȃ��
	if (m_bell->IsRing()) {
		// ���G�͈͂��x���p�ɐݒ�
		searchRange = SEARCH_RANGE_TO_BELL;
	}
	// ���������Ă���Ȃ��
	else if (m_gameCamera->IsSound()) {
		// ���G�͈͂𑫉��p�ɐݒ�
		searchRange = SEARCH_RANGE_TO_FOOTSTEP;
	}

	// �v���C���[�Ƃ̋������ݒ肳�ꂽ���G�͈͓��ł����
	if (distance.Length() <= searchRange) {
		// �v���C���[�𔭌�����
		m_isFound = true;
		return;
	}
	// �v���C���[�𔭌��ł��Ȃ�����
	m_isFound = false;
}

void Enemy2::Walk()
{
	// �ړI�n�܂ł̋���
	Vector3 distance = m_point->s_position - m_position;

	// �ړI�n�܂ł̋������߂����
	if (distance.Length() <= DISTANCE_TO_TARGET_WHILE_WALKING) {
		// ���ݍŌ�̃|�C���g�ɂ���Ȃ��
		if (m_point->s_number == m_enemyPath.GetPointListSize() - 1) {
			// �ŏ��̃|�C���g�����擾
			m_point = m_enemyPath.GetFirstPoint();
		}
		// �Ō�ȊO�̃|�C���g�ɂ���Ȃ��
		else {
			// ���̃|�C���g�����擾
			m_point = m_enemyPath.GetNextPoint(m_point->s_number);
		}

	}

	// �ړ����������ݒ�
	m_moveVector = distance;
	m_moveVector.Normalize();

	// �������̈ړ����x��ݒ�
	m_moveSpeed = m_moveVector * WALK_SPEED;
	
	// ���W�Ɉړ����x�����Z
	m_position += m_moveSpeed;
}

void Enemy2::Rotation()
{
	// ��]�p�x��ݒ�
	float angle = atan2(-m_moveVector.x, m_moveVector.z);
	
	// ��]��ݒ�
	m_rotation.SetRotationY(-angle);
	m_modelRender.SetRotation(m_rotation);
}

void Enemy2::ManageState()
{
	switch (m_enemyState) {
	// �������
	case enEnemyState_Walk:
		ProcessWalkStateTransition();
		break;
	// ���K���
	case enEnemyState_Scream:
		ProcessScreamStateTransition();
		break;
	// �ǐՏ��
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	// ���񂵏��
	case enEnemyState_Survey:
		ProcessSurveyStateTransition();
		break;
	// �p�X�ړ��ւ̋A�ҏ��
	case enEnemyState_ReturnToPath:
		ProcessReturnToPathStateTransition();
		break;
	// �U�����
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}

void Enemy2::ProcessByState()
{
	switch (m_enemyState) {
	// �������
	case enEnemyState_Walk:
		Walk();
		SearchSoundOfPlayer();
		break;
	// ���K���
	case enEnemyState_Scream:
		Scream();
		break;
	// �ǐՏ��
	case enEnemyState_Chase:
		Chase();
		SearchSoundOfPlayer();
		break;
	// ���񂵏��
	case enEnemyState_Survey:
		Survey();
		SearchSoundOfPlayer();
		break;
	// �p�X�ړ��ւ̋A�ҏ��
	case enEnemyState_ReturnToPath:
		ReturnToPath();
		SearchSoundOfPlayer();
		break;
	// �U�����
	case enEnemyState_Attack:
		Attack();
		break;
	}
}

void Enemy2::Scream()
{
	// ���K�̉�����炷
	if (m_screamSound == nullptr) {
		m_screamSound = NewGO<SoundSource>(0);
		m_screamSound->Init(7);
		m_screamSound->SetVolume(SCREAM_VOLUME);
		m_screamSound->Play(false);
	}

	// ���K�ɂ��֊s���ւ̉e��
	OutlineByScream();
}

void Enemy2::OutlineByScream()
{
	int check;

	if (m_screamSound != nullptr) {
		// �������Đ���
		if (m_screamSound->IsPlaying() == true)
		{
			check = 1;
			// �e�����𑝂₷
			if (m_screamRateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_screamRateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		// �������Đ�����Ă��Ȃ���
		else {
			check = 0;
			// �e���������炷
			if (m_screamRateByTime > RATE_BY_TIME_MIN_VALUE && check == 0) {
				m_screamRateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				// �e�������Œᐔ�l�ȉ��ɂȂ�Ȃ��悤�ɌŒ�
				if (m_screamRateByTime <= RATE_BY_TIME_MIN_VALUE) {
					m_screamRateByTime = RATE_BY_TIME_MIN_VALUE;
				}
			}
			else {
				// ���K���I��
				m_isEndScream = true;
			}
		}
		g_infoForEdge.SetPosition(7, m_position);
		g_infoForEdge.SetIsSound(7, check);
		g_infoForEdge.SetRate(7, m_screamRateByTime);
	}
}

void Enemy2::Chase()
{
	// �^�[�Q�b�g�ƂȂ�v���C���[�̍��W���擾
	Vector3	playerPos = m_player->GetPosition();

	if (m_pathFindingTimer <= 0.0f) {
		// �v���C���[�܂ł̃p�X������
		m_pathFinding.Execute(
			m_path,
			m_nvmMesh,
			m_position,
			playerPos,
			PhysicsWorld::GetInstance(),
			ENEMY_RADIUS,
			ENEMY_HEIGHT
		);
		// �^�C�}�[���Z�b�g
		m_pathFindingTimer = PATH_FINDING_TIMER;
	}

	m_pathFindingTimer -= g_gameTime->GetFrameDeltaTime();

	// �w�肵�����W�ւ̈ړ�������������
	bool isEnd;
	// �ړ����O�̍��W���L�^
	m_lastPosition = m_position;

	// �Œ�ǐՂ���c�莞��
	// �ǐՂ���c�莞�Ԃ����炷
	m_chaseTime -= g_gameTime->GetFrameDeltaTime();
	// �ǐՂ���c�莞�Ԃ�0�ɂȂ����Ȃ�
	if (m_chaseTime < 0.0f) {
		m_chaseTime = 0.0f;
	}


	// �ݒ肳�ꂽ�p�X�����ƂɈړ�
	m_position = m_path.Move(
		m_position,
		RUN_SPEED,
		isEnd
	);

	// ����
	// ���O�̍��W�ƌ��݂̍��W���r���Ĉړ��������������߂�
	Vector3 momentDist = m_position - m_lastPosition;
	momentDist.Normalize();

	// �ړ����������ݒ�
	m_moveVector = momentDist;
	
	// �v���C���[�Ƃ̋���
	Vector3 distanceToPlayer = m_position - playerPos;
	// �v���C���[�Ƃ̋������߂����
	if (distanceToPlayer.Length() <= ATTACKING_RANGE) {
		// �U�����s��
		m_isAttackable = true;
	}

}

void Enemy2::Survey()
{
	// ���񂵂����Ԃ����Z
	m_surveyTimer += g_gameTime->GetFrameDeltaTime();
}

void Enemy2::ReturnToPath()
{
	bool isEnd;

	// �ړ����O�̍��W���L�^
	m_lastPosition = m_position;

	// �p�X���������Ă��Ȃ��Ȃ��
	if (m_isPathFindingWhileReturning == false) {
		// ���݂̍��W�����ԋ߂����W�̃|�C���g���擾
		m_point = m_enemyPath.GetNearPoint(m_position);
		// �A�ҖڕW�̃p�X�܂ł̃p�X������
		m_pathFinding.Execute(
			m_path,
			m_nvmMesh,
			m_position,
			m_point->s_position,
			PhysicsWorld::GetInstance(),
			ENEMY_RADIUS,
			ENEMY_HEIGHT
		);
		m_isPathFindingWhileReturning = true;
	}

	// �ݒ肳�ꂽ�p�X�����ƂɈړ�
	m_position = m_path.Move(
		m_position,
		WALK_SPEED,
		isEnd
	);

	// ���O�̍��W�ƌ��݂̍��W���r���Ĉړ��������������߂�
	Vector3 distance = m_position - m_lastPosition;
	distance.Normalize();

	// �ړ����������ݒ�
	m_moveVector = distance;
}

void Enemy2::Attack()
{
	// �^�[�Q�b�g�ƂȂ�v���C���[�̍��W���擾
	Vector3	playerPos = m_player->GetPosition();
	
	Vector3 distance = playerPos - m_position;
	distance.Normalize();

	m_moveVector = distance;
}

void Enemy2::ProcessWalkStateTransition()
{
	// �G�𔭌����Ă��Ȃ��Ȃ��
	if (m_isFound == false) {
		return;
	}
	// ���K�����Z�b�g
	m_screamSound = nullptr;
	m_screamRateByTime = 0.0f;
	m_isEndScream = false;
	// �X�e�[�g����K��Ԃɂ���
	m_enemyState = enEnemyState_Scream;
}

void Enemy2::ProcessScreamStateTransition()
{
	// ���K���I�����Ă��Ȃ��Ȃ��
	if (m_isEndScream == false) {
		return;
	}
	// �Œ���ǐՂ��鎞�Ԃ��w��
	m_chaseTime = MINIMUM_CHASE_TIME;
	// �X�e�[�g��ǐՏ�Ԃɂ���
	m_enemyState = enEnemyState_Chase;
}

void Enemy2::ProcessChaseStateTransition()
{
	// �v���C���[���U���\�ȋ����Ȃ��
	if (m_isAttackable == true) {
		m_enemyState = enEnemyState_Attack;
	}
	// �G��ǐՂ����Ԃ��ێ�����Ă���Ȃ��
	if (m_chaseTime > 0.0f || m_isFound == true) {
		return;
	}
	// �v���C���[���������Ă����Ȃ��
	if (m_isFound == false) {
		// ���񂷎��Ԃ����Z�b�g
		m_surveyTimer = 0.0f;
		// �X�e�[�g�����񂵏�Ԃɂ���
		m_enemyState = enEnemyState_Survey;
		// �p�X����������Ԋu�����Z�b�g
		m_pathFindingTimer = PATH_FINDING_TIMER;
		
	}
		
}

void Enemy2::ProcessSurveyStateTransition()
{
	// ��莞�Ԉȓ��Ƀv���C���[�𔭌�
	if (m_isFound == true) {
		// �Œ���ǐՂ��鎞�Ԃ��w��
		m_chaseTime = MINIMUM_CHASE_TIME;
		// �ǐՂ��J�n
		m_enemyState = enEnemyState_Chase;
	}
	// ��莞�ԃv���C���[���������Ă�����
	if (m_surveyTimer > TIME_TO_LOSE_SIGHT) {
		// �A�ҏ�Ԃ̎��Ԃ��w��
		m_timeToReturn = TIME_TO_FORCE_STATE_TRANSITION;
		// �X�e�[�g���p�X�ւ̋A�ҏ�Ԃɂ���
		m_enemyState = enEnemyState_ReturnToPath;
	}

}

void Enemy2::ProcessReturnToPathStateTransition()
{
	// �ڕW�̍��W�ƌ��݂̍��W�̋����𑪂�
	Vector3 distance = m_point->s_position - m_position;

	m_timeToReturn -= g_gameTime->GetFrameDeltaTime();

	// �v���C���[�𔭌�������
	if (m_isFound == true) {
		// ���K�����Z�b�g
		m_screamRateByTime = 0.0f;
		m_isEndScream = false;
		m_screamSound = nullptr;
		// �X�e�[�g����K��Ԃɂ���
		m_enemyState = enEnemyState_Scream;
	}
	// �ڕW�̍��W�ɋ߂��Ȃ�����
	else if (distance.Length() < DISTANCE_TO_TARGET_WHILE_RETURNING) {
		// �X�e�[�g�������Ԃɂ���
		m_enemyState = enEnemyState_Walk;
	}
	// ��莞�Ԉȓ��ɋA�҂ł��Ȃ������ꍇ
	else if (m_timeToReturn <= 0.0f) {
		// �����I�ɃX�e�[�g�������Ԃɂ���
		m_enemyState = enEnemyState_Walk;
	}
	m_isPathFindingWhileReturning = false;
}

void Enemy2::ProcessAttackStateTransition()
{
	// �Q�[���I�[�o�[
	m_isGameOver = true;
	m_attackingTimer -= 1.0f * g_gameTime->GetFrameDeltaTime();
	if (m_attackingTimer <= 0.0f && m_isCaughtPlayer == false) {
		m_isCaughtPlayer = true;
		m_gameOver = NewGO<GameOver>(0, "gameOver");
	}
}

void Enemy2::PlayAnimation()
{
	switch (m_enemyState) {
	// ����
	case enEnemyState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// ���K
	case enEnemyState_Scream:
		m_modelRender.PlayAnimation(enAnimationClip_Scream, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// �ǐ�
	case enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// ����
	case enEnemyState_Survey:
		m_modelRender.PlayAnimation(enAnimationClip_Survey, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// �p�X�ւ̋A��
	case enEnemyState_ReturnToPath:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// �U��
	case enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	}
}

void Enemy2::OnStepAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���uwalk1�v�̎��B
	if (wcscmp(eventName, L"walk1") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(14);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//�L�[�̖��O���uwalk2�v�̎��B
	else if (wcscmp(eventName, L"walk2") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(15);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//�L�[�̖��O���urun1�v�̎��B
	else if (wcscmp(eventName, L"run1") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(16);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//�L�[�̖��O���urun2�v�̎��B
	else if (wcscmp(eventName, L"run2") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(17);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
}

float Enemy2::SoundLevelByDistance(float range)
{
	Vector3 gameCameraPos = m_gameCamera->GetPosition();
	gameCameraPos.y = m_position.y;

	//�v���C���[�ƃG�l�~�[�̋���
	Vector3 diff = m_position - gameCameraPos;

	// ���̑傫��
	float soundLevel = MAXIMUM_VOLUME - (diff.Length() / range * MAXIMUM_VOLUME);
	// �ŏ����ʂ�菬�����Ȃ�����Œ肷��
	if (soundLevel <= MINIMUM_VOLUME) {
		soundLevel = MINIMUM_VOLUME;
	}

	return soundLevel;
}

void Enemy2::StepVolumeControl()
{
	if (m_stepSound != nullptr)
	{
		m_stepSound->SetVolume(SoundLevelByDistance(STEP_SOUNDRANGE));
	}
}

void Enemy2::OutlineByStep()
{
	int check;

	if (m_stepSound != nullptr) {
		// �������Đ���
		if (m_stepSound->IsPlaying() == true)
		{
			check = 1;
			// �e�����𑝂₷
			if (m_stepRateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_stepRateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		// �������Đ�����Ă��Ȃ���
		else {
			check = 0;
			if (m_isGameOver == false) {
				// �e���������炷
				if (m_stepRateByTime > RATE_BY_TIME_MIN_VALUE && check == 0) {
					m_stepRateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
					// �e�������Œᐔ�l�ȉ��ɂȂ�Ȃ��悤�ɌŒ�
					if (m_stepRateByTime <= RATE_BY_TIME_MIN_VALUE) {
						m_stepRateByTime = RATE_BY_TIME_MIN_VALUE;
						m_stepSound = nullptr;
					}
				}
			}

		}
		g_infoForEdge.SetPosition(8, m_position);
		g_infoForEdge.SetIsSound(8, check);
		g_infoForEdge.SetRate(8, m_stepRateByTime);
	}
}

void Enemy2::Render(RenderContext& rc)
{
	// �`��
	m_modelRender.Draw(rc);
}
