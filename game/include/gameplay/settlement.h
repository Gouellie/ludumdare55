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
    SettlementStatus m_Status{ SettlementStatus::Clear };
    std::vector<Warrior*> m_AssignedWarriors;
    std::string m_Name;
    Event* m_CurrentEvent = nullptr;

    void AddEvent(unsigned int timeToSolve, unsigned int requiredPower, unsigned int penalty, unsigned int cost);
    Event* GetEvent() { return m_CurrentEvent; }

    void SetStatus(SettlementStatus newStatus) { m_Status = newStatus; }
    SettlementStatus GetStatus() { return m_Status; }

    std::string GetName() { return m_Name; }
    void GetData(std::string& name, SettlementStatus& status, std::vector<Warrior*>& assignedWarriors);

    void GetWarriors(std::vector<Warrior*>& warriors);

    Color GetColor();

private:

};