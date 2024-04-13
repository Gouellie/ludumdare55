#include <gameplay/settlement.h>

#include <gameplay/warrior.h>

using super = GameObject;

void Settlement::Update()
{
    super::Update();

    UpdateSettlement();
}

void Settlement::UpdateSettlement()
{

}

Warrior* Settlement::GetAssignedWarrior() const
{
    return m_AssignedWarrior;
}

void Settlement::GetData(std::string& name, SettlementStatus& status, Warrior* assignedWarrior)
{
    name = GetName();
    status = m_Status;
    assignedWarrior = GetAssignedWarrior();
}