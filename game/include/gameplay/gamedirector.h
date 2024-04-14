#pragma once

#include "game_object.h"

#include <gameplay/event.h>
#include <gameplay/warrior.h>

#include <vector>

class Scene;
class ModelComponent;
class SettlementComponent;

class GameDirector
{
public:
    ModelComponent* m_PickedModel = nullptr;
    unsigned int m_CurrentTurn{ 0 };
    int m_Cash{ 100 };
    int m_PickedWarriorIndex{ -1 };

    [[nodiscard]] static GameDirector& GetInstance()
    {
        static GameDirector* directorInstance = new GameDirector();
        directorInstance->m_AvailableWarriors.reserve(MAX_WARRIORS);
        directorInstance->m_EventList.reserve(NB_EVENTS);
        return *directorInstance;
    }

    void ResetDirector();

    void ResolveTurn(const Scene& scene);
    void ResolveSettlementEvent(GameObject& child);
    void AddCash(GameObject& child);
    [[nodiscard]] bool AreAllSettlementsDestroyed(const Scene& scene);

    void SetPickedModel(ModelComponent* picked);
    [[nodiscard]] ModelComponent* GetPickedModel() { return m_PickedModel; }
    [[nodiscard]] SettlementComponent* GetPickedSettlement();

    bool AddWarrior(std::string name, unsigned int health, unsigned int power);
    void AddStartingWarriors();
    [[nodiscard]] Warrior* GetWarrior(std::size_t index) { return &m_AvailableWarriors.at(index); }
    [[nodiscard]] const Warrior* GetWarrior(std::size_t index) const { return &m_AvailableWarriors.at(index); }
    [[nodiscard]] std::vector<Warrior>& GetWarriors() { return m_AvailableWarriors; }

    void HealWarriors();

    [[nodiscard]] int GetPickedWarriorIndex() { return m_PickedWarriorIndex; }
    void SetPickedWarriorIndex(int index) { m_PickedWarriorIndex = index; }

    void SetGameOver(bool state) { m_GameOver = state; }
    [[nodiscard]] bool GetGameOver() { return m_GameOver; }

    void PopulateEventList();
    Event& GetRandomEvent();

    void TakePenalty(int penalty) { m_Cash - penalty; }

private:
    GameDirector() {}
    std::vector<Warrior> m_AvailableWarriors;
    std::vector<Event> m_EventList;

    bool m_GameOver{ false };
    static constexpr unsigned int MAX_TURNS{ 30 };
    static constexpr int MAX_WARRIORS{ 20 };
    static constexpr int NB_EVENTS{ 5 };
    static constexpr int WARRIOR_COST{ 50 };
};