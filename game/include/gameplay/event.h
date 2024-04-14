#pragma once

#include <string>

class Event
{
public:
    char* m_Name;
    unsigned int m_Damage{ 0 };
    unsigned int m_RequiredPower{ 0 };
    unsigned int m_Penalty{ 0 }; // Cost in cash if Event destroys settlement
    unsigned int m_DamageToWarriors{ 0 }; // Damage inflicted to warriors upon solving the event

    Event();
    Event(char* name, unsigned int damagePerTurn, unsigned int requiredPower, unsigned int penalty, unsigned int damageToWarriors)
        : m_Name(name), m_Damage{ damagePerTurn }, m_RequiredPower{ requiredPower }, m_Penalty{penalty}, m_DamageToWarriors{ damageToWarriors } {}

    void SetName(char* name) { m_Name = name; }
    char* GetName() { return m_Name; }
};