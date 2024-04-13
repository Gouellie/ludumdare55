#include <gameplay/settlement.h>

#include <gameplay/warrior.h>

using super = GameObject;


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
    }
}