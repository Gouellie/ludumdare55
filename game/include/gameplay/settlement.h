#pragma once

#include <game_object.h>

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
class Settlement : public GameObject
{
public:
    SettlementStatus m_Status{ SettlementStatus::Clear };
    Warrior* m_AssignedWarrior = nullptr;

    // Model
    // Transform

    void Update();
    void UpdateSettlement();

    void GetData(std::string& name, SettlementStatus& status, Warrior* assignedWarrior);

    [[nodiscard]] Warrior* GetAssignedWarrior() const;

private:

};