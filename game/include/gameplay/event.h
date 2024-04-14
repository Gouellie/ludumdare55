#pragma once

#include <string>

class Event
{
public:

    Event();
    Event(char* name, unsigned int damagePerTurn, unsigned int requiredPower, unsigned int penalty, unsigned int damageToWarriors)
        : m_Name(name), m_Damage{ damagePerTurn }, m_RequiredPower{ requiredPower }, m_Penalty{penalty}, m_DamageToWarriors{ damageToWarriors } {}
    Event(const Event& other);

    void SetName(char* name) { m_Name = name; }
    char* GetName() { return m_Name; }
    const char* GetName() const { return m_Name; }

    void SetDamage(unsigned int newDmg) { m_Damage = newDmg; }
    unsigned int GetDamage() { return m_Damage; }
    const unsigned int GetDamage() const { return m_Damage; }

    void SetRequiredPower(unsigned int newRequiredPower) { m_RequiredPower = newRequiredPower; }
    unsigned int GetRequiredPower() { return m_RequiredPower; }
    const unsigned int GetRequiredPower() const { return m_RequiredPower; }

    void SetPenalty(unsigned int newPenalty) { m_Penalty = newPenalty; }
    unsigned int GetPenalty() { return m_Penalty; }
    const unsigned int GetPenalty() const { return m_Penalty; }

    void SetDamageToWarriors(unsigned int newDmg) { m_DamageToWarriors = newDmg; }
    unsigned int GetDamageToWarriors() { return m_DamageToWarriors; }
    const unsigned int GetDamageToWarriors() const { return m_DamageToWarriors; }

private:
    char* m_Name;
    unsigned int m_Damage{ 0 };
    unsigned int m_RequiredPower{ 0 };
    unsigned int m_Penalty{ 0 }; // Cost in cash if Event destroys settlement
    unsigned int m_DamageToWarriors{ 0 }; // Damage inflicted to warriors upon solving the event
};