#pragma once

#include <simple_components.h>

#include <gameplay/event.h>

#include <vector>

class Warrior;

enum class SettlementStatus
{
    Clear = 0,
    Attacked = 1,
    Destroyed = 2
};

// Contain in a SettlementsScene : scene
class SettlementComponent : public Component
{
public:
    DEFINE_COMPONENT(SettlementComponent)

    void AddEvent(const char* name, int damagePerTurn, int requiredPower, int penalty, int cost);
    void AddEvent(const Event& event);
    Event* GetEvent() { return m_CurrentEvent; }
    void ClearEvent();

    void HandleEventEnd(const bool success);

    void SetStatus(SettlementStatus newStatus) { m_Status = newStatus; }
    SettlementStatus GetStatus() { return m_Status; }

    const char* GetName() const { return m_Name; }
    void SetName(const char* newName) { m_Name = newName; }
    void GetData(SettlementStatus& status, std::vector<Warrior*>& assignedWarriors);

    void TakeDamage(int dmg) { m_Health -= dmg; }

    void GetWarriors(std::vector<Warrior*>& warriors);
    void AddWarrior(Warrior* warrior);
    void RemoveWarrior(std::size_t index);
    const int GetWarriorPower() const;
    void DealDamageToWarriors();
    void ClearWarriors();
    Color GetColor();

    void InitHealth(int val);
    void SetHealth(int newHealth) { m_Health = newHealth; }
    void SetMaxHealth(int newMaxHealth) { m_MaxHealth = newMaxHealth; }
    [[nodiscard]] const int GetHealth() const { return m_Health; }
    [[nodiscard]] const int GetMaxHealth() const { return m_MaxHealth; }

    const int GetIncome() const { return m_Income; }
    void InflictPenalty();

private:
    SettlementStatus m_Status{ SettlementStatus::Clear };
    std::vector<Warrior*> m_AssignedWarriors;
    const char* m_Name;
    Event* m_CurrentEvent = nullptr;
    int m_Health{ 100 };
    int m_MaxHealth{ 100 };
    int m_Income{ 5 };
    static constexpr int MAX_ASSIGNED_WARRIORS{ 3 };
};