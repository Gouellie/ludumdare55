#pragma once

class Event
{
public:

    Event();
    Event(const char* name, int damagePerTurn, int requiredPower, int penalty, int damageToWarriors)
        : m_Name(name), m_Damage{ damagePerTurn }, m_RequiredPower{ requiredPower }, m_Penalty{penalty}, m_DamageToWarriors{ damageToWarriors } {}
    Event(const Event& other);

    void SetName(const char* name) { m_Name = name; }
    const char* GetName() { return m_Name; }
    const char* GetName() const { return m_Name; }

    void SetDamage(int newDmg) { m_Damage = newDmg; }
    int GetDamage() { return m_Damage; }
    const int GetDamage() const { return m_Damage; }

    void SetRequiredPower(int newRequiredPower) { m_RequiredPower = newRequiredPower; }
    int GetRequiredPower() { return m_RequiredPower; }
    const int GetRequiredPower() const { return m_RequiredPower; }

    void SetPenalty(int newPenalty) { m_Penalty = newPenalty; }
    int GetPenalty() { return m_Penalty; }
    const int GetPenalty() const { return m_Penalty; }

    void SetDamageToWarriors(int newDmg) { m_DamageToWarriors = newDmg; }
    int GetDamageToWarriors() { return m_DamageToWarriors; }
    const int GetDamageToWarriors() const { return m_DamageToWarriors; }

    void ApplyDifficultyScale(const int currentTurn);

private:
    const char* m_Name;
    int m_Damage{ 0 };
    int m_RequiredPower{ 0 };
    int m_Penalty{ 0 }; // Cost in cash if Event destroys settlement
    int m_DamageToWarriors{ 0 }; // Damage inflicted to warriors upon solving the event
    static constexpr int BASE_DIFFICULTY_BONUS{ 5 };
};