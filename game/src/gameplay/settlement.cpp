#include <gameplay/settlement.h>

#include <gameplay/warrior.h>

void SettlementComponent::AddEvent(unsigned int timeToSolve, unsigned int requiredPower, unsigned int penalty, unsigned int cost)
{
    m_CurrentEvent = new Event(timeToSolve, requiredPower, penalty, cost);
}

void SettlementComponent::GetWarriors(std::vector<Warrior*>& warriors)
{
    warriors = m_AssignedWarriors;
}

void SettlementComponent::GetData(std::string& name, SettlementStatus& status, std::vector<Warrior*>& assignedWarriors)
{
    name = GetName();
    status = m_Status;
    GetWarriors(assignedWarriors);
}

Color SettlementComponent::GetColor()
{
    switch (GetStatus())
    {
    case SettlementStatus::Clear:
        return BLUE;
        break;
    case SettlementStatus::Attacked:
        return RED;
        break;
    case SettlementStatus::Destroyed:
        return GREEN;
        break;
    default:
        return WHITE;
        break;
    }
}