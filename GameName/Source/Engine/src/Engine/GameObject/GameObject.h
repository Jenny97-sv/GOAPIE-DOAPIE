#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Engine/Components/Component.h"
#include <iostream>

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	unsigned int GetID() const { return id; }
	const std::string& GetName() const { return name; }
	void SetName(const std::string& n) { name = n; }

	virtual bool Init();

	virtual void Start();
	virtual void Update(const float dt);
	virtual void Render();

	template<typename T, typename... Args>
	T* AddComponent(Args&&... args);

	template<typename T>
	T* GetComponent();

	template<typename T>
	bool TryGetComponent(T*& outComponent);

	bool isActive = true;

private:
	unsigned int id;
	std::string name;

	std::vector<std::unique_ptr<Component>> components;
	std::unordered_map<size_t, Component*> componentLookup;
	template <typename T>
	size_t ComponentTypeID();
};

//template<typename T, typename... Args>
//T* GameObject::AddComponent(Args&&... args)
//{
//	auto comp = std::make_unique<T>(std::forward<Args>(args)...);
//	T* ptr = comp.get();
//
//	components.emplace_back(std::move(comp));
//	componentLookup[ComponentTypeID<T>()] = ptr;
//
//	return ptr;
//}

template<typename T, typename... Args>
T* GameObject::AddComponent(Args&&... args)
{
	try
	{
		auto comp = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = comp.get();

		components.emplace_back(std::move(comp));
		componentLookup[ComponentTypeID<T>()] = ptr;

		return ptr;
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception thrown while adding component "
			<< typeid(T).name() << ": " << e.what() << "\n";
		return nullptr;
	}
}


template<typename T>
T* GameObject::GetComponent()
{
	auto it = componentLookup.find(ComponentTypeID<T>());
	if (it != componentLookup.end())
		return static_cast<T*>(it->second);

	return nullptr;
}


template<typename T>
bool GameObject::TryGetComponent(T*& outComponent)
{
	static_assert(std::is_base_of<Component, T>::value,
		"T must inherit from Component");

	for (std::unique_ptr<Component>& comp : components)
	{
		if (auto casted = dynamic_cast<T*>(comp.get()))
		{
			outComponent = casted;
			return true;
		}
	}
	outComponent = nullptr;
	return false;
}


template <typename T>
size_t GameObject::ComponentTypeID()
{
	static const size_t id = typeid(T).hash_code();
	return id;
}
