#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Scene.h"

class SceneManager
{
public:
    static SceneManager& GetInstance()
    {
        static SceneManager instance;
        return instance;
    }

    void RegisterScene(const std::string& name, std::unique_ptr<Scene> scene)
    {
        scenes[name] = std::move(scene);
    }

    bool LoadScene(const std::string& name)
    {
        auto it = scenes.find(name);
        if (it != scenes.end())
        {
            // Unload previous scene before switching
            UnloadCurrentScene();

            activeScene = it->second.get();
            return activeScene->Init();
        }

        return false;
    }

    void UnloadCurrentScene()
    {
        if (activeScene)
        {
            activeScene->Unload();     // optional if you have an unload
            activeScene = nullptr;     // important
        }
    }

    void Update(float dt)
    {
        if (activeScene)
            activeScene->Update(dt);
    }

    void Render()
    {
        if (activeScene)
            activeScene->Render();
    }

    Scene* GetActiveScene() const { return activeScene; }
    bool shouldRun = true;

private:
    SceneManager() = default;

    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
    Scene* activeScene = nullptr;
};
