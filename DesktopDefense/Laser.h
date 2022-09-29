#pragma once

#include "Weapon.h"
#define LASER_DAMAGE 3

class Laser : public Weapon
{
public:
	Laser(GameObject* _gameObject);

	bool AttackTick() override;

	void Attack(Miner* _miner) override;

	inline void SetOption() { m_PenOption.PenStyle = EPenStyle::Solid, m_PenOption.PenColor = RGB(255, 0, 0), m_PenOption.PenWidth = 3; }

	Vector2D m_StartPosition;
	Vector2D m_EndPosition;

private:
	PaintShapeOption m_PenOption;
};
