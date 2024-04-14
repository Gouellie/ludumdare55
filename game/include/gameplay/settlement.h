#pragma once

#include <simple_components.h>

#include <gameplay/event.h>

#include <string>
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

    void AddEvent(char* name, int damagePerTurn, unsigned int requiredPower, unsigned int penalty, unsigned int cost);
    void AddEvent(const Event& event);
    Event* GetEvent() { return m_CurrentEvent; }
    void ClearEvent();

    void HandleEventEnd(const bool success);

    void SetStatus(SettlementStatus newStatus) { m_Status = newStatus; }
    SettlementStatus GetStatus() { return m_Status; }

    const char* GetName() const { return m_Name; }
    void GetData(SettlementStatus& status, std::vector<Warrior*>& assignedWarriors);

    void TakeDamage(int dmg) { m_Health -= dmg; }

    void GetWarriors(std::vector<Warrior*>& warriors);
    void AddWarrior(Warrior* warrior);
    void RemoveWarrior(std::size_t index);
    const unsigned int GetWarriorPower() const;
    void DealDamageToWarriors();
    void ClearWarriors();
    Color GetColor();

    [[nodiscard]] const int GetHealth() const { return m_Health; }

    const unsigned int GetIncome() const { return m_Income; }
    void InflictPenalty();

private:
    SettlementStatus m_Status{ SettlementStatus::Clear };
    std::vector<Warrior*> m_AssignedWarriors;
    char* m_Name;
    Event* m_CurrentEvent = nullptr;
    int m_Health{ 100 };
    unsigned int m_Income{ 5 };
    static constexpr int MAX_ASSIGNED_WARRIORS{ 3 };
};