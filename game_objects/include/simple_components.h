/*
-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you
--  wrote the original software. If you use this software in a product, an acknowledgment
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "component.h"
#include "raylib.h"

#include <vector>
#include <string>
#include <unordered_map>

class TransformComponent : public Component
{
private:
    Vector2 Translation = { 0 };
    float Rotation = 0;

public:
    DEFINE_COMPONENT(TransformComponent)

    void SetPosition(const Vector2& pos);
    Vector2 GetPosition() const;

    void SetRotation(float rotation);
    float GetRotation() const;

    void PushMatrix();
    void PopMatrix();
};

class Transform3DComponent : public Component {
private:
    Vector3 Translation = { 0 };
    Vector3 RotationAxis = { 0.f,1.f,0.f };
    float Rotation = 0; // Angle

public:
    DEFINE_COMPONENT(Transform3DComponent)

    Transform3DComponent* SetPosition(const Vector3& pos);
    Vector3 GetPosition() const;

    void SetRotationAxis(const Vector3& axis);
    Vector3 GetRotationAxis() const;

    void SetRotation(float rotation);
    float GetRotation() const;

    void SetTransform(const Vector3& pos, const Vector3& rotationAxis, float rotation);

    Matrix GetMatrix();
    
    void PushMatrix();
    void PopMatrix();
};

class SpriteComponent : public Component
{
private:
    Texture2D Sprite = { 0 };
    Rectangle SourceRect = { 0,0,-1,-1 };
    Color Tint = WHITE;
    float Scale = 1.0f;

public:
    DEFINE_COMPONENT(SpriteComponent)

    void OnRender() override;

    void SetSprite(const Texture2D& texture);
    void SetSprite(const Texture2D& texture, const Rectangle& sourceRect);
    void SetSpriteRect(const Rectangle& sourceRect);

    Rectangle GetSourceRect(void) const { return SourceRect; }

    void SetTint(Color tint);
    Color GetTint() const;

    void SetScale(float scale);
    float GetScale() const;
};

struct AnimationSequence
{
    std::vector<Rectangle> Frames;
    float FPS = 15;
    bool Loop = true;

    void FlipFrames(bool flipX, bool flipY);

    void FromSpriteSheet(Texture2D& texture, int frameWidth, int frameHeight);
};

class SpriteAnimationComponent : public Component
{
private:
    std::unordered_map<std::string, AnimationSequence> Sequences;
    std::string CurrentSequence;

    int CurrentFrame = 0;
    float LastFrameTime = 0;

public: 
    DEFINE_COMPONENT(SpriteAnimationComponent)

    void OnUpdate() override;

    AnimationSequence& AddSequence(std::string_view name);

    void SetCurrentSequence(std::string_view sequenceName);
    void ResetSequence();

    bool IsAnimating() const;
};

class ModelComponent : public Component 
{
private:
    Model m_Model = { 0 };
    Color m_Tint = WHITE;
    BoundingBox m_BoundingBox = { 0 };
    bool m_IsPicked = false;
public:
    DEFINE_COMPONENT(ModelComponent)
    void OnRender3D() override;

    void SetPicked(bool value) { m_IsPicked = value; }
    bool GetPicked() const { return m_IsPicked; }

    void SetModel(const Model& model);
    void SetTint(Color color) { m_Tint = color; }
    BoundingBox GetBoundingBox(Matrix* mat) const;
};

class ModelAnimationComponent : public Component
{
private:
    ModelAnimation* m_Animations = nullptr;
    int m_AnimCount = -1;
    int m_CurrentAnim = 0;

    int m_CurrentFrame = 0;
    float m_LastFrameTime = 0;

public:
    DEFINE_COMPONENT(ModelAnimationComponent)

    void OnUpdate() override;

    void AddAnimation(ModelAnimation* animations, int animCount);

    void SetCurrentAnim(int currentAnim);
    void ResetSequence();

    bool IsAnimating() const;
};
