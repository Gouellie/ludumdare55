#include <gameplay/settlement.h>

#include <gameplay/warrior.h>

void SettlementComponent::AddEvent(char* name, int damagePerTurn, unsigned int requiredPower, unsigned int penalty, unsigned int cost)
{
    m_CurrentEvent = new Event(name, damagePerTurn, requiredPower, penalty, cost);
}

void SettlementComponent::ClearEvent()
{
    delete m_CurrentEvent;
    m_CurrentEvent = nullptr;
}

void SettlementComponent::GetWarriors(std::vector<Warrior*>& warriors)
{
    warriors = m_AssignedWarriors;
}

void SettlementComponent::AddWarrior(Warrior* warrior)
{
    m_AssignedWarriors.push_back(warrior);
    warrior->SetStatus(WarriorStatus::Dispatched);
}

void SettlementComponent::RemoveWarrior(std::size_t index)
{
    m_AssignedWarriors.at(index)->SetStatus(WarriorStatus::Waiting);
    auto it = m_AssignedWarriors.begin();
    it = std::next(it, index);
    m_AssignedWarriors.erase(it);
}

const unsigned int SettlementComponent::GetWarriorPower() const
{
    unsigned int totalPower = 0;
    for (const Warrior* warrior : m_AssignedWarriors)
    {
        totalPower += warrior->GetPowerLevel();
    }
    return totalPower;
}

void SettlementComponent::ClearWarriors()
{
    for (Warrior* warrior : m_AssignedWarriors)
    {
        warrior->SetStatus(WarriorStatus::Waiting);
    }
    m_AssignedWarriors.clear();
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