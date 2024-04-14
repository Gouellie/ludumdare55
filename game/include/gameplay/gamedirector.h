#pragma once

#include "game_object.h"

#include <gameplay/event.h>
#include <gameplay/warrior.h>

#include <vector>

class Buyable;
class Scene;
class ModelComponent;
class SettlementComponent;

enum class GameState
{
    Running = 0,
    Victory = 1,
    Fail = 2
};

class GameDirector
{
public:
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
    [[nodiscard]] const int GetCash() const { return m_Cash; }
    [[nodiscard]] bool AreAllSettlementsDestroyed(const Scene& scene);

    [[nodiscard]] const bool CanBuy(const Buyable& item) const;

    void SetPickedModel(ModelComponent* picked);
    [[nodiscard]] ModelComponent* GetPickedModel() { return m_PickedModel; }
    [[nodiscard]] SettlementComponent* GetPickedSettlement();

    bool AddWarrior(Warrior warrior);
    void AddStartingWarriors();
    [[nodiscard]] Warrior* GetWarrior(std::size_t index) { return &m_AvailableWarriors.at(index); }
    [[nodiscard]] const Warrior* GetWarrior(std::size_t index) const { return &m_AvailableWarriors.at(index); }
    [[nodiscard]] std::vector<Warrior>& GetWarriors() { return m_AvailableWarriors; }

    void HealWarriors();

    [[nodiscard]] int GetPickedWarriorIndex() { return m_PickedWarriorIndex; }
    void SetPickedWarriorIndex(int index) { m_PickedWarriorIndex = index; }

    [[nodiscard]] const int GetCurrentTurn() const { return m_CurrentTurn; }
    void IncrementTurn() { ++m_CurrentTurn; }

    void SetGameOver(bool state) { m_GameOver = state; }
    [[nodiscard]] bool GetGameOver() { return m_GameOver; }
    void SetGameState(GameState newState) { m_CurrentState = newState; }
    void EvaluateGameOver(const Scene& scene);

    void PopulateEventList();
    Event& GetRandomEvent();

    void TakePenalty(int penalty) { m_Cash -= penalty; }

private:
    GameDirector() {}
    GameState m_CurrentState{ GameState::Running };
    std::vector<Warrior> m_AvailableWarriors;
    std::vector<Event> m_EventList;
    ModelComponent* m_PickedModel = nullptr;
    int m_Cash{ 100 };
    int m_PickedWarriorIndex{ -1 };
    int m_CurrentTurn{ 0 };
    bool m_GameOver{ false };
    static constexpr int MAX_TURNS{ 30 };
    static constexpr int MAX_WARRIORS{ 20 };
    static constexpr int NB_EVENTS{ 5 };
};