#pragma once

#include "Weapon.h"
#define MINIGUN_DAMAGE 3
class MiniGun : public Weapon
{
public:
	MiniGun(GameObject* _gameObject);

	bool AttackTick() override;

	void Attack(Miner* _miner) override;
};