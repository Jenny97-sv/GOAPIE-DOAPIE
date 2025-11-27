#pragma once
#include <vector>
#include <memory>
#include <string>
#include "GameObject.h"

class Scene
{
public:
    virtual ~Scene();

    virtual bool Init() = 0;
    virtual void Update(float dt);
    virtual void Render();
    virtual void Unload();

    // Universal GameObject management
    GameObject* CreateObject(GameObject* obj);
    void RemoveObject(GameObject* obj);
    GameObject* FindObjectByName(const std::string& name);
    template <typename T>
    T* FindObjectOfType()
    {
        for (auto& go : gameObjects)
            if (auto casted = dynamic_cast<T*>(go.get()))
                return casted;
        for (auto& co : currentObjects)
            if (auto casted = dynamic_cast<T*>(co))
                return casted;
        
        return nullptr;
    }


protected:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    std::vector<GameObject*> currentObjects{};
};
