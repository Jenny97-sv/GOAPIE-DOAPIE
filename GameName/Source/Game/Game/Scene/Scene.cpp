#include "Scene.h"

Scene::~Scene()
{
}

void Scene::Update(const float dt)
{
    for (std::unique_ptr<GameObject>& go : gameObjects)
        if (go->isActive)
            go->Update(dt);

    for (GameObject*& co : currentObjects)
        if (co->isActive)
            co->Update(dt);
    
}

void Scene::Render()
{
    for (std::unique_ptr<GameObject>& go : gameObjects)
        if (go->isActive)
            go->Render();

    for (GameObject*& co : currentObjects)
        if (co->isActive)
            co->Render();
    
}

void Scene::Unload()
{
    gameObjects.clear();
}

GameObject* Scene::CreateObject(GameObject* obj)
{
    if (std::find(currentObjects.begin(), currentObjects.end(), obj) != currentObjects.end())
        return nullptr;

    currentObjects.push_back(obj);
    return obj;
}

void Scene::RemoveObject(GameObject* obj)
{
    for (int i = 0; i < currentObjects.size(); i++)
    {
        if (currentObjects[i]->GetID() == obj->GetID())
        {
            currentObjects.erase(currentObjects.begin() + i);
            return;
        }
    }

    std::cout << "Couldn't find the object" << std::endl;
}

GameObject* Scene::FindObjectByName(const std::string& name)
{
    for (auto& go : gameObjects)
        if (go->GetName() == name)
            return go.get();
    return nullptr;
}