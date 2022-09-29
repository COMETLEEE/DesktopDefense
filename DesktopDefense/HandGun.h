#pragma once

#include "Weapon.h"
#define HANDGUN_DAMAGE 5

class HandGun : public Weapon
{
public:
	HandGun(GameObject* _gameObject);
	
	bool AttackTick() override;

	void Attack(Miner* _miner) override;
};