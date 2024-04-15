#pragma once

#include <behavior.h>

class BarrackController : public GameObjectBehavior
{
private:
    float m_fHeight = 40;
    bool m_bExpanded = false;
    Texture2D m_SummonWarriorSprite = { 0 };
    Texture2D m_BarracksSprite = { 0 };
    Texture2D m_WarriorSprite = { 0 };
    Texture2D m_WarriorPanelSprite = { 0 };

public:
    DEFINE_BEHAVIOR(BarrackController)

    void SetSprite(const Texture2D& summonWarriorSprite, const Texture2D& barracksSprite, const Texture2D& warriorSprite, const Texture2D& warriorPanelSprite);

    void OnRenderUI() override;
};