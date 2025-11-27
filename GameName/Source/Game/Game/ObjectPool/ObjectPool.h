#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "GameObject.h"

template <typename T>
class ObjectPool
{
public:
    ObjectPool(size_t initialSize = 10)
    {
        static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");
        for (size_t i = 0; i < initialSize; ++i)
        {
            auto obj = std::make_shared<T>();
            obj->Init();
            obj->isActive = false;
            pool.push_back(std::move(obj));
        }
    }

    T* Get()
    {
        for (auto& obj : pool)
        {
            if (!obj->isActive)
            {
                obj->isActive = true;
                obj->Start();
                return obj.get();
            }
        }

        return nullptr;

        // Optional expansion if no free object
        //auto obj = std::make_unique<T>();
        //obj->isActive = true;
        //obj->Init();
        //T* raw = obj.get();
        //pool.push_back(std::move(obj));
        //return raw;
    }

    void Return(T* obj)
    {
        obj->isActive = false;
    }

private:
    std::vector<std::shared_ptr<T>> pool;
};
