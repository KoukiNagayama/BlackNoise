#pragma once
#include "EnemyPath.h"
#include "AI/PathFinding/PathFinding.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"

class Player;
class Bell;
class GameCamera;
/// <summary>
/// エネミークラス
/// </summary>
class Enemy2 : public IGameObject
{
public:
	Enemy2() {};
	~Enemy2() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダリングコンテキスト</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 拡大率を設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// エネミーの番号を設定
	/// </summary>
	/// <param name="number">番号</param>
	void SetNumber(const int& number)
	{
		m_enemyNumber = number;
	}
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
private:
	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// ステートによる処理
	/// </summary>
	void ProcessByState();
	/// <summary>
	/// プレイヤーの音を探索
	/// </summary>
	void SearchSoundOfPlayer();
	/// <summary>
	/// 通常移動
	/// </summary>
	void Walk();
	/// <summary>
	/// 咆哮
	/// </summary>
	void Scream();
	/// <summary>
	/// 咆哮による輪郭線
	/// </summary>
	void OutlineByScream();
	/// <summary>
	/// 追跡
	/// </summary>
	void Chase();
	/// <summary>
	/// 見回し
	/// </summary>
	void Survey();
	/// <summary>
	/// パス移動への帰還
	/// </summary>
	void ReturnToPath();
	/// <summary>
	/// 歩き時のステート遷移
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// 追跡時のステート遷移
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// 攻撃時のステート遷移
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 叫び時のステート遷移
	/// </summary>
	void ProcessScreamStateTransition();
	/// <summary>
	/// 見回し時のステート遷移
	/// </summary>
	void ProcessSurveyStateTransition();
	/// <summary>
	/// パス移動への帰還時のステート遷移
	/// </summary>
	void ProcessReturnToPathStateTransition();
	/// <summary>
	/// アニメーション再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// 足音の輪郭線描画
	/// </summary>
	void OutlineByStep();
	/// <summary>
	/// プレイヤーと音源の距離により音の大きさを調整する
	/// </summary>
	/// <param name="range">音源の影響範囲</param>
	float SoundLevelByDistance(float range);
	/// <summary>
	/// 音量調整
	/// </summary>
	void StepVolumeControl();
	// アニメーションイベント用の関数。
	void OnStepAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

private:
	// ステート
	enum EnEnemyState {
		enEnemyState_Walk,				// 歩き	
		enEnemyState_Scream,			// 咆哮
		enEnemyState_Chase,				// 追跡
		enEnemyState_Survey,			// 見回し
		enEnemyState_ReturnToPath,		// パス移動に戻る
		enEnemyState_Attack,			// 攻撃
		enEnemyState_Num,				// ステートの数
	};

	// アニメーション
	enum EnAnimationClip {
		enAnimationClip_Walk,			// 歩き
		enAnimationClip_Run,			// 走り
		enAnimationClip_Scream,			// 咆哮
		enAnimationClip_Survey,			// 見回し
		enAnimationClip_Attack,			// 攻撃
		enAnimationClip_Num,			// アニメーションの数
	};

	ModelRender			m_modelRender;								// モデルレンダー
	Vector3				m_position;									// 座標
	Vector3				m_scale = Vector3::One * 5.0f;				// 拡大率
	Quaternion			m_rotation = Quaternion::Identity;			// 回転
	int					m_enemyNumber;								// レベル上で登録されたエネミーの番号
	AnimationClip		m_animationClips[enAnimationClip_Num];		// アニメーションクリップ
	Vector3				m_moveVector;								// 移動する方向
	Vector3				m_moveSpeed;								// 移動する速さ
	EnemyPath			m_enemyPath;								// パス
	Point*				m_point;									// パス移動のポイント
	Player*				m_player;									// プレイヤー情報
	EnEnemyState		m_enemyState = enEnemyState_Walk;			// ステート
	Bell*				m_bell = nullptr;							// ベル
	SoundSource*		m_screamSound = nullptr;					// 咆哮時の音源
	SoundSource*		m_stepSound = nullptr;						// 足音
	float				m_screamRateByTime = 0.0f;					// 咆哮の時間による影響率
	float				m_stepRateByTime = 0.0f;					// 足音による影響率
	float				m_chaseTime;								// 追いかけ続ける時間
	bool				m_isFound = false;							// プレイヤーを見つけているか
	bool				m_isAttackable = false;						// プレイヤーへの攻撃は可能か
	nsAI::NaviMesh		m_nvmMesh;									// ナビゲーションメッシュ
	nsAI::Path			m_path;										// ナビゲーションメッシュ用パス
	nsAI::PathFinding	m_pathFinding;								// パス検索
	Vector3				m_lastPosition;								// 直前の座標
	float				m_surveyTimer = 0.0f;						// 見回す時間
	bool				m_isEndScream = false;						// 咆哮が終了しているか
	bool				m_isGameOver = false;						// ゲームオーバーになっているか
	GameCamera*			m_gameCamera = nullptr;						// ゲームカメラ
	float				m_timeToReturn = 0.0f;						// 帰還する時間
	float				m_animationSpeed = 1.0f;					// アニメーションのスピード
	float				m_pathFindingTimer = 0.0f;					// 目的地までのパスを検索する間の時間
	bool				m_isPathFindingWhileReturning = false;		// パス移動への帰還時にパス検索をしたか
};

