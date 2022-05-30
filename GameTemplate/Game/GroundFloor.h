#pragma once
#include "LevelRender.h"

class Player;
class GameCamera;
class Bell;
class Door;
class Enemy2;
class Piece;
class Key;

class GroundFloor : public IGameObject
{
public:
	GroundFloor() {};
	~GroundFloor();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// 鍵を拾った
	/// </summary>
	void PickKey()
	{
		m_pickKey = true;
	}
private:
	/// <summary>
	/// 鍵を持ってゴールに到着した。
	/// </summary>
	void GameClear();
	LevelRender m_levelRender;
	Player* m_player;
	GameCamera* m_gamecam;
	Bell* m_bell;
	Enemy2* m_enemy;
	std::vector<Door*>	m_door;
	std::vector<Piece*> m_piece;
	Key* m_key;
	ModelRender m_bgRender;
	SpriteRender m_spriteRender;
	PhysicsStaticObject m_physicsStaticObject;
	Vector3 m_bgPos;
	Vector3 m_clearPos;

	bool m_pickKey = false; //鍵を拾った？
	bool m_clear = false; //クリアした？
};

