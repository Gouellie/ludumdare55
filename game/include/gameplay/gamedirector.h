#pragma once

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

    void ResolveTurn(const Scene& scene);

    void SetPickedModel(ModelComponent* picked);
    [[nodiscard]] ModelComponent* GetPickedModel() { return m_PickedModel; }

    void AddWarrior(std::string name, unsigned int health, unsigned int power);
    [[nodiscard]] Warrior* GetWarrior(std::size_t index) { return &m_AvailableWarriors.at(index); }
    [[nodiscard]] const Warrior* GetWarrior(std::size_t index) const { return &m_AvailableWarriors.at(index); }

    [[nodiscard]] bool AreAllSettlementsDestroyed(const Scene& scene);

    void AddCash(const Scene& scene);
    void HealWarriors();

    [[nodiscard]] int GetPickedWarriorIndex() { return m_PickedWarriorIndex; }

private:
    GameDirector() {}

    static constexpr unsigned int MAX_TURNS{ 30 };
    static constexpr int MAX_WARRIORS{ 20 };
};