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
	const float WALK_SPEED = 6.5f * 0.7f;							// 歩く速さ
	const float RUN_SPEED = 9.5f * 0.7f;							// 走る速さ
	const float SEARCH_RANGE_TO_BELL = 1000.0f;						// ベルの音が聞こえる範囲
	const float SEARCH_RANGE_TO_TOY = 1300.0f;				
	const float SEARCH_RANGE_TO_FOOTSTEP = 200.0f;					// 足音が聞こえる範囲
	const float SCREAM_VOLUME = 1.0f;								// 咆哮の音量
	const float MAXIMUM_VOLUME = 0.9f;								// 足音最大音量
	const float MINIMUM_VOLUME = 0.00f;								// 足音最小音量
	const float SCREAM_RANGE = 1300.0f;								// 咆哮時に輪郭線が適用される範囲
	const float STEP_RANGE = 450.0f;								// 歩行時に輪郭線が適用される範囲
	const float STEP_SOUNDRANGE = 1350.0f;							// 歩行時にプレイヤーに聞こえる範囲
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;					// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;					// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;						// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;						// 時間による影響率の最小値
	const float MINIMUM_CHASE_TIME = 20.0f;							// 最低限追跡する時間
	const float ENEMY_RADIUS = 30.0f;								// エネミーの半径
	const float ENEMY_HEIGHT = 200.0f;								// エネミーの高さ
	const float ATTACKING_RANGE = 120.0f;							// 攻撃可能な距離
	const float DISTANCE_TO_TARGET_WHILE_WALKING = 10.0f;			// 歩き中の目標地点までの距離
	const float DISTANCE_TO_TARGET_WHILE_RETURNING = 30.0f;			// 期間中の目標地点までの距離
	const float TIME_TO_LOSE_SIGHT = 5.0f;							// プレイヤーを見失った時間
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;			// アニメーションの補間時間
	const float TIME_TO_FORCE_STATE_TRANSITION = 6.0f;				// 強制的にステート遷移する時間
	const float ANIMATION_SPEED = 0.7f;								// アニメーションのスピードの倍率
	const float PATH_FINDING_TIMER = 0.7f;							// ナビメッシュによるパスを検索する時間の間隔

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

	// アニメーション
	// 歩き時のアニメーションをロード
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	// 走り時のアニメーションをロード
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	// 叫び時のアニメーションをロード
	m_animationClips[enAnimationClip_Scream].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Scream].SetLoopFlag(false);
	// 見回し時のアニメーションをロード
	m_animationClips[enAnimationClip_Survey].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Survey].SetLoopFlag(true);
	// 攻撃時のアニメーションをロード
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(true);


	// モデルの初期化
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, enAnimationClip_Num, false, enModelUpAxisZ, false, 0, 2);

	// アニメーションイベントを追加
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		// 足音
		OnStepAnimationEvent(clipName, eventName);
	});

	// エネミーのパス移動の情報を初期化
	std::string filePath;
	filePath = "Assets/enemypath/enemypath";
	filePath += std::to_string(m_enemyNumber);
	filePath += ".tkl";
	// 作成したファイルパスをもとに初期化
	m_enemyPath.Init(filePath.c_str());
	// 最初のパス移動のポイント情報を取得
	m_point = m_enemyPath.GetFirstPoint();


	// モデルの初期の座標、回転、拡大率を設定
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	// プレイヤー情報を取得
	m_player = FindGO<Player>("player");
	m_bell = FindGO<Bell>("bell");
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	// 音源の登録
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_caution.wav");
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_warning.wav");
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_danger.wav");
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/enemy/scream/scream.wav");
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/enemy/step/walk1.wav");
	g_soundEngine->ResistWaveFileBank(15, "Assets/sound/enemy/step/walk2.wav");
	g_soundEngine->ResistWaveFileBank(16, "Assets/sound/enemy/step/run1.wav");
	g_soundEngine->ResistWaveFileBank(17, "Assets/sound/enemy/step/run2.wav");

	// 輪郭線情報の初期化
	g_infoForEdge.InitForSound(7, m_position, SCREAM_RANGE, 0, m_screamRateByTime);
	// 輪郭線情報の初期化
	g_infoForEdge.InitForSound(8, m_position, STEP_RANGE, 0, m_screamRateByTime);

	// エネミーによってナビゲーションメッシュを変更
	if (m_enemyNumber == 1) {
		// ナビゲーションメッシュの初期化
		m_nvmMesh.Init("Assets/modelData/enemy/stage2_mesh.tkn");
	}
	else if (m_enemyNumber == 2) {
		m_nvmMesh.Init("Assets/modelData/enemy/stage1_mesh.tkn");
	}


	// アニメーションスピードを代入
	m_animationSpeed = ANIMATION_SPEED;
	// アニメーションのスピードを指定
	m_modelRender.SetAnimationSpeed(m_animationSpeed);
	// おもちゃの検索
	m_toy = FindGO<Toy>("toy");
	return true;
}

void Enemy2::Update()
{

	// 回転
	Rotation();
	// ステートによる処理
	ProcessByState();
	// ステート管理
	ManageState();
	// アニメーション再生
	PlayAnimation();
	//足音の輪郭線描画
	OutlineByStep();
	// 座標の更新
	m_modelRender.SetPosition(m_position);
	// モデルの更新
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
	// 索敵範囲
	float searchRange = 0.0f;

	Vector3 playerPos = m_player->GetPosition();
	// プレイヤーとの距離
	Vector3 distance = m_position - playerPos;

	// エネミーの番号が1ならば
	if (m_enemyNumber == 1) {
		if (m_toy->IsSound()) {
			searchRange = SEARCH_RANGE_TO_TOY;
		}
	}
	// ベルが鳴っているならば
	if (m_bell->IsRing()) {
		// 索敵範囲をベル用に設定
		searchRange = SEARCH_RANGE_TO_BELL;
	}
	// 足音が鳴っているならば
	else if (m_gameCamera->IsSound()) {
		// 索敵範囲を足音用に設定
		searchRange = SEARCH_RANGE_TO_FOOTSTEP;
	}

	// プレイヤーとの距離が設定された索敵範囲内であれば
	if (distance.Length() <= searchRange) {
		// プレイヤーを発見した
		m_isFound = true;
		return;
	}
	// プレイヤーを発見できなかった
	m_isFound = false;
}

void Enemy2::Walk()
{
	// 目的地までの距離
	Vector3 distance = m_point->s_position - m_position;

	// 目的地までの距離が近ければ
	if (distance.Length() <= DISTANCE_TO_TARGET_WHILE_WALKING) {
		// 現在最後のポイントにいるならば
		if (m_point->s_number == m_enemyPath.GetPointListSize() - 1) {
			// 最初のポイント情報を取得
			m_point = m_enemyPath.GetFirstPoint();
		}
		// 最後以外のポイントにいるならば
		else {
			// 次のポイント情報を取得
			m_point = m_enemyPath.GetNextPoint(m_point->s_number);
		}

	}

	// 移動する方向を設定
	m_moveVector = distance;
	m_moveVector.Normalize();

	// 歩き時の移動速度を設定
	m_moveSpeed = m_moveVector * WALK_SPEED;
	
	// 座標に移動速度を加算
	m_position += m_moveSpeed;
}

void Enemy2::Rotation()
{
	// 回転角度を設定
	float angle = atan2(-m_moveVector.x, m_moveVector.z);
	
	// 回転を設定
	m_rotation.SetRotationY(-angle);
	m_modelRender.SetRotation(m_rotation);
}

void Enemy2::ManageState()
{
	switch (m_enemyState) {
	// 歩き状態
	case enEnemyState_Walk:
		ProcessWalkStateTransition();
		break;
	// 咆哮状態
	case enEnemyState_Scream:
		ProcessScreamStateTransition();
		break;
	// 追跡状態
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	// 見回し状態
	case enEnemyState_Survey:
		ProcessSurveyStateTransition();
		break;
	// パス移動への帰還状態
	case enEnemyState_ReturnToPath:
		ProcessReturnToPathStateTransition();
		break;
	// 攻撃状態
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}

void Enemy2::ProcessByState()
{
	switch (m_enemyState) {
	// 歩き状態
	case enEnemyState_Walk:
		Walk();
		SearchSoundOfPlayer();
		break;
	// 咆哮状態
	case enEnemyState_Scream:
		Scream();
		break;
	// 追跡状態
	case enEnemyState_Chase:
		Chase();
		SearchSoundOfPlayer();
		break;
	// 見回し状態
	case enEnemyState_Survey:
		Survey();
		SearchSoundOfPlayer();
		break;
	// パス移動への帰還状態
	case enEnemyState_ReturnToPath:
		ReturnToPath();
		SearchSoundOfPlayer();
		break;
	// 攻撃状態
	case enEnemyState_Attack:
		Attack();
		break;
	}
}

void Enemy2::Scream()
{
	// 咆哮の音源を鳴らす
	if (m_screamSound == nullptr) {
		m_screamSound = NewGO<SoundSource>(0);
		m_screamSound->Init(7);
		m_screamSound->SetVolume(SCREAM_VOLUME);
		m_screamSound->Play(false);
	}

	// 咆哮による輪郭線への影響
	OutlineByScream();
}

void Enemy2::OutlineByScream()
{
	int check;

	if (m_screamSound != nullptr) {
		// 音源が再生中
		if (m_screamSound->IsPlaying() == true)
		{
			check = 1;
			// 影響率を増やす
			if (m_screamRateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_screamRateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		// 音源が再生されていない時
		else {
			check = 0;
			// 影響率を減らす
			if (m_screamRateByTime > RATE_BY_TIME_MIN_VALUE && check == 0) {
				m_screamRateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				// 影響率を最低数値以下にならないように固定
				if (m_screamRateByTime <= RATE_BY_TIME_MIN_VALUE) {
					m_screamRateByTime = RATE_BY_TIME_MIN_VALUE;
				}
			}
			else {
				// 咆哮が終了
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
	// ターゲットとなるプレイヤーの座標を取得
	Vector3	playerPos = m_player->GetPosition();

	if (m_pathFindingTimer <= 0.0f) {
		// プレイヤーまでのパスを検索
		m_pathFinding.Execute(
			m_path,
			m_nvmMesh,
			m_position,
			playerPos,
			PhysicsWorld::GetInstance(),
			ENEMY_RADIUS,
			ENEMY_HEIGHT
		);
		// タイマーリセット
		m_pathFindingTimer = PATH_FINDING_TIMER;
	}

	m_pathFindingTimer -= g_gameTime->GetFrameDeltaTime();

	// 指定した座標への移動が完了したか
	bool isEnd;
	// 移動直前の座標を記録
	m_lastPosition = m_position;

	// 最低追跡する残り時間
	// 追跡する残り時間を減らす
	m_chaseTime -= g_gameTime->GetFrameDeltaTime();
	// 追跡する残り時間が0になったなら
	if (m_chaseTime < 0.0f) {
		m_chaseTime = 0.0f;
	}


	// 設定されたパスをもとに移動
	m_position = m_path.Move(
		m_position,
		RUN_SPEED,
		isEnd
	);

	// 方向
	// 直前の座標と現在の座標を比較して移動した方向を求める
	Vector3 momentDist = m_position - m_lastPosition;
	momentDist.Normalize();

	// 移動する方向を設定
	m_moveVector = momentDist;
	
	// プレイヤーとの距離
	Vector3 distanceToPlayer = m_position - playerPos;
	// プレイヤーとの距離が近ければ
	if (distanceToPlayer.Length() <= ATTACKING_RANGE) {
		// 攻撃を行う
		m_isAttackable = true;
	}

}

void Enemy2::Survey()
{
	// 見回した時間を加算
	m_surveyTimer += g_gameTime->GetFrameDeltaTime();
}

void Enemy2::ReturnToPath()
{
	bool isEnd;

	// 移動直前の座標を記録
	m_lastPosition = m_position;

	// パスを検索していないならば
	if (m_isPathFindingWhileReturning == false) {
		// 現在の座標から一番近い座標のポイントを取得
		m_point = m_enemyPath.GetNearPoint(m_position);
		// 帰還目標のパスまでのパスを検索
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

	// 設定されたパスをもとに移動
	m_position = m_path.Move(
		m_position,
		WALK_SPEED,
		isEnd
	);

	// 直前の座標と現在の座標を比較して移動した方向を求める
	Vector3 distance = m_position - m_lastPosition;
	distance.Normalize();

	// 移動する方向を設定
	m_moveVector = distance;
}

void Enemy2::Attack()
{
	// ターゲットとなるプレイヤーの座標を取得
	Vector3	playerPos = m_player->GetPosition();
	
	Vector3 distance = playerPos - m_position;
	distance.Normalize();

	m_moveVector = distance;
}

void Enemy2::ProcessWalkStateTransition()
{
	// 敵を発見していないならば
	if (m_isFound == false) {
		return;
	}
	// 咆哮をリセット
	m_screamSound = nullptr;
	m_screamRateByTime = 0.0f;
	m_isEndScream = false;
	// ステートを咆哮状態にする
	m_enemyState = enEnemyState_Scream;
}

void Enemy2::ProcessScreamStateTransition()
{
	// 咆哮が終了していないならば
	if (m_isEndScream == false) {
		return;
	}
	// 最低限追跡する時間を指定
	m_chaseTime = MINIMUM_CHASE_TIME;
	// ステートを追跡状態にする
	m_enemyState = enEnemyState_Chase;
}

void Enemy2::ProcessChaseStateTransition()
{
	// プレイヤーを攻撃可能な距離ならば
	if (m_isAttackable == true) {
		m_enemyState = enEnemyState_Attack;
	}
	// 敵を追跡する状態が維持されているならば
	if (m_chaseTime > 0.0f || m_isFound == true) {
		return;
	}
	// プレイヤーを見失っていたならば
	if (m_isFound == false) {
		// 見回す時間をリセット
		m_surveyTimer = 0.0f;
		// ステートを見回し状態にする
		m_enemyState = enEnemyState_Survey;
		// パスを検索する間隔をリセット
		m_pathFindingTimer = PATH_FINDING_TIMER;
		
	}
		
}

void Enemy2::ProcessSurveyStateTransition()
{
	// 一定時間以内にプレイヤーを発見
	if (m_isFound == true) {
		// 最低限追跡する時間を指定
		m_chaseTime = MINIMUM_CHASE_TIME;
		// 追跡を開始
		m_enemyState = enEnemyState_Chase;
	}
	// 一定時間プレイヤーを見失っていたら
	if (m_surveyTimer > TIME_TO_LOSE_SIGHT) {
		// 帰還状態の時間を指定
		m_timeToReturn = TIME_TO_FORCE_STATE_TRANSITION;
		// ステートをパスへの帰還状態にする
		m_enemyState = enEnemyState_ReturnToPath;
	}

}

void Enemy2::ProcessReturnToPathStateTransition()
{
	// 目標の座標と現在の座標の距離を測る
	Vector3 distance = m_point->s_position - m_position;

	m_timeToReturn -= g_gameTime->GetFrameDeltaTime();

	// プレイヤーを発見したら
	if (m_isFound == true) {
		// 咆哮をリセット
		m_screamRateByTime = 0.0f;
		m_isEndScream = false;
		m_screamSound = nullptr;
		// ステートを咆哮状態にする
		m_enemyState = enEnemyState_Scream;
	}
	// 目標の座標に近くなったら
	else if (distance.Length() < DISTANCE_TO_TARGET_WHILE_RETURNING) {
		// ステートを歩き状態にする
		m_enemyState = enEnemyState_Walk;
	}
	// 一定時間以内に帰還できなかった場合
	else if (m_timeToReturn <= 0.0f) {
		// 強制的にステートを歩き状態にする
		m_enemyState = enEnemyState_Walk;
	}
	m_isPathFindingWhileReturning = false;
}

void Enemy2::ProcessAttackStateTransition()
{
	// ゲームオーバー
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
	// 歩き
	case enEnemyState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// 咆哮
	case enEnemyState_Scream:
		m_modelRender.PlayAnimation(enAnimationClip_Scream, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// 追跡
	case enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// 見回し
	case enEnemyState_Survey:
		m_modelRender.PlayAnimation(enAnimationClip_Survey, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// パスへの帰還
	case enEnemyState_ReturnToPath:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// 攻撃
	case enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	}
}

void Enemy2::OnStepAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が「walk1」の時。
	if (wcscmp(eventName, L"walk1") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(14);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//キーの名前が「walk2」の時。
	else if (wcscmp(eventName, L"walk2") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(15);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//キーの名前が「run1」の時。
	else if (wcscmp(eventName, L"run1") == 0) {
		m_stepSound = NewGO<SoundSource>(0);
		m_stepSound->Init(16);
		m_stepSound->Play(false);
		StepVolumeControl();
	}
	//キーの名前が「run2」の時。
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

	//プレイヤーとエネミーの距離
	Vector3 diff = m_position - gameCameraPos;

	// 音の大きさ
	float soundLevel = MAXIMUM_VOLUME - (diff.Length() / range * MAXIMUM_VOLUME);
	// 最小音量より小さくなったら固定する
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
		// 音源が再生中
		if (m_stepSound->IsPlaying() == true)
		{
			check = 1;
			// 影響率を増やす
			if (m_stepRateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_stepRateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		// 音源が再生されていない時
		else {
			check = 0;
			if (m_isGameOver == false) {
				// 影響率を減らす
				if (m_stepRateByTime > RATE_BY_TIME_MIN_VALUE && check == 0) {
					m_stepRateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
					// 影響率を最低数値以下にならないように固定
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
	// 描画
	m_modelRender.Draw(rc);
}
