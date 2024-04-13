#pragma once

#include <simple_components.h>

#include <string>

enum class WarriorStatus
{
    Waiting = 0,
    Dispatched = 1,
    Healing = 2,
    Dead = 3
};

class WarriorComponent : public Component
{
public:
    DEFINE_COMPONENT(WarriorComponent)
    std::string m_Name;
    unsigned int m_Health{ 0 };
    WarriorStatus m_Status{ WarriorStatus::Waiting };

    void UpdateWarrior();

    void GetData();

private:

};