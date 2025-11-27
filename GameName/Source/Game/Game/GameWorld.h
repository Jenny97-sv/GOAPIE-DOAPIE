#pragma once
#include <vector>
#include <memory>
#include <string>
#include "GameObject.h"
#include "EnemyManager.h"
#include "Scene.h"

class GameWorld : public Scene
{
public:
    ~GameWorld() override;
    bool Init() override;
    void Update(float dt) override;
    void Render() override;

private:
    //std::vector<std::unique_ptr<GameObject>> gameObjects;
    //EnemyManager* enemyManager = &EnemyManager::GetInstance();
};
