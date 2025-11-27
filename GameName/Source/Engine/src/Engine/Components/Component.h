#pragma once
//#include "Component.h"

class GameObject;

class Component
{
public:
    virtual ~Component() = default;

    virtual void Init() {}
    virtual void Update(float dt) {}
    virtual void Render() {}

    void SetOwner(GameObject* go) { owner = go; }
//protected:
    GameObject* owner = nullptr;
};