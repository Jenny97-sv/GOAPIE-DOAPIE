#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <typeindex>
#include <iostream>
#include "ObjectPool.h"

class ObjectPoolManager
{
public:
    static ObjectPoolManager& GetInstance()
    {
        static ObjectPoolManager instance;
        return instance;
    }

    template<typename T>
    void RegisterPool(const std::string& id, size_t size = 10)
    {
        if (pools.find(id) != pools.end())
            return; // already registered

        pools[id] = std::make_unique<ObjectPool<T>>(size);
    }

    template<typename T>
    T* Get(const std::string& id)
    {
        auto it = pools.find(id);
        if (it == pools.end())
        {
            std::cerr << "Pool not registered: " << id << std::endl;
            return nullptr;
        }

        auto pool = std::static_pointer_cast<ObjectPool<T>>(it->second);
        return pool->Get();
    }

    template<typename T>
    void Return(const std::string& id, T* obj)
    {
        auto it = pools.find(id);
        if (it == pools.end()) return;

        auto pool = std::static_pointer_cast<ObjectPool<T>>(it->second);
        pool->Return(obj);
    }

private:
    ObjectPoolManager() = default;
    std::unordered_map<std::string, std::shared_ptr<void>> pools;
};
