#pragma once

#include "game_object.h"

#include <gameplay/warrior.h>

#include <vector>

class Scene;
class ModelComponent;

class GameDirector
{
public:
    std::vector<Warrior> m_AvailableWarriors;
    ModelComponent* m_PickedModel = nullptr;
    unsigned int m_CurrentTurn{ 0 };
    unsigned int m_Cash{ 100 };
    int m_PickedWarriorIndex{ -1 };

    [[nodiscard]] static GameDirector& GetInstance()
    {
        static GameDirector* directorInstance = new GameDirector();
        directorInstance->m_AvailableWarriors.reserve(MAX_WARRIORS);
        return *directorInstance;
    }

    void ResetDirector();

    void ResolveTurn(const Scene& scene);
    void ResolveSettlementEvent(GameObject& child);
    void AddCash(GameObject& child);
    [[nodiscard]] bool AreAllSettlementsDestroyed(const Scene& scene);

    void SetPickedModel(ModelComponent* picked);
    [[nodiscard]] ModelComponent* GetPickedModel() { return m_PickedModel; }

    bool AddWarrior(std::string name, unsigned int health, unsigned int power);
    [[nodiscard]] Warrior* GetWarrior(std::size_t index) { return &m_AvailableWarriors.at(index); }
    [[nodiscard]] const Warrior* GetWarrior(std::size_t index) const { return &m_AvailableWarriors.at(index); }

    void HealWarriors();

    [[nodiscard]] int GetPickedWarriorIndex() { return m_PickedWarriorIndex; }
    void SetPickedWarriorIndex(int index) { m_PickedWarriorIndex = index; }

    void SetGameOver(bool state) { m_GameOver = state; }
    [[nodiscard]] bool GetGameOver() { return m_GameOver; }

private:
    GameDirector() {}

    bool m_GameOver{ false };
    static constexpr unsigned int MAX_TURNS{ 30 };
    static constexpr int MAX_WARRIORS{ 20 };
};