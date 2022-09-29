#pragma once
#include "ComponentBase.h"
#include "AnimationClip.h"
#include "Vector2D.h"

#define MINER_SCALE 60
#define MINER_DEVIATION 35

using namespace Engine;

enum class EMinerState
{
	None = 0,
	Move = 1,
	Dig = 2,
	Die = 3,
	Create=4
};

class Miner : public ComponentBase
{
public:
	Miner(GameObject* _gameObject);
	~Miner();
	void Tick() override;

	void Initialize(Vector2D _initialPosition, float _moveSpeed, float _digtime, int _hp);
	void MinerState();

	void Move();
	void Dig(float _curtime);
	void OnDie(float _curtime);
	void Damage(int damage);
	void SetRectanglePos();
	bool CheckMiner(Vector2D curpos);
	Vector2D DesrtoryWindow();

private:
	int m_Mineridx;
	int m_CurDigX;
	int m_CurDigY;
	Sprite* m_Zero;
	Vector2D m_Scaler;
	Vector2D m_LeftBottomPos;
	Vector2D m_RightTopPos;
	Vector2D m_DigPosition;
	AnimationClip* m_MoveAnimationClip;
	AnimationClip* m_DigAnimationClip;
	AnimationClip* m_DieAnimationClip;
	float m_DigTime;
	float m_HitTime;
	float m_PreviusDigTime;
	float m_MoveSpeed;
	int m_Hp;
	EMinerState m_State;
	EMinerState m_PreviousState;

public:
	inline void SetMinerIdx(int _value) { m_Mineridx = _value; }
};
