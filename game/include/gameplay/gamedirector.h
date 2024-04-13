#pragma once

#include <gameplay/warrior.h>

#include <vector>

class Scene;

class GameDirector
{
public:
    std::vector<Warrior*> m_AvailableWarriors;

    static GameDirector& GetInstance()
    {
        static GameDirector* directorInstance = new GameDirector();
        return *directorInstance;
    }

    void ResolveTurn(const Scene& scene);
    unsigned int m_CurrentTurn{ 0 };

    void AddWarrior();
    Warrior* GetWarrior(std::size_t index) { return m_AvailableWarriors.at(index); }
    const Warrior* GetWarrior(std::size_t index) const { return m_AvailableWarriors.at(index); }

private:
    GameDirector() {}

    static constexpr unsigned int MAX_TURNS{ 30 };
};