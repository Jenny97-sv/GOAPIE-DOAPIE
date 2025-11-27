#include "GameObject.h"
#include "Engine/Components/TransformComponent.h"

static int idCounter = 0; // Is is problematic?

GameObject::GameObject() : 
	components{},
	id(idCounter++)
{
}

GameObject::~GameObject()
{
}

bool GameObject::Init()
{
	for (std::unique_ptr<Component>& comp : components)
	{
		comp->Init();
	}
	return true;
}

void GameObject::Start()
{
}

void GameObject::Update(const float dt)
{
	for (std::unique_ptr<Component>& comp : components)
	{
		comp->Update(dt);
	}
}

void GameObject::Render()
{
	for (std::unique_ptr<Component>& comp : components)
	{
		comp->Render();
	}
}
