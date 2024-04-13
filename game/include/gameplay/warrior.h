#pragma once

#include <game_object.h>

#include <string>

enum class WarriorStatus
{
    Waiting = 0,
    Dispatched = 1,
    Healing = 2,
    Dead = 3
};

class Warrior : public GameObject
{
public:
    unsigned int m_Health{ 0 };
    WarriorStatus m_Status{ WarriorStatus::Waiting };

    Warrior();
    Warrior(unsigned int health, WarriorStatus status);

    void Update();
    void UpdateWarrior();

    void GetData();

private:

};