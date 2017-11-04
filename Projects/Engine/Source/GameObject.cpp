#include "GameObject.h"
#include "ObjectManager.h"
#include "TransformComponent.h"


namespace Engine
{

GameObject::GameObject (const std::string& id)
	: m_Name (id),
	m_isDestroyed (false),
	m_pParent (std::shared_ptr<GameObject> (nullptr))
{
	std::shared_ptr<TransformComponent> transform (new TransformComponent (id));
	AddComponent (transform);
}


void GameObject::AddComponent (const Component::SPtr& comp)
{
	// TODO: unicitást ellenorizni kellene!
	if (comp)
	{
		m_components.push_back (comp);
		comp->Init (this);
	}
}


void GameObject::removeComponent (const std::string& compName)
{
	auto predicate = [&compName] (const Component::SPtr& elem) { return elem->GetName () == compName; };
	m_components.erase (std::remove_if (m_components.begin (), m_components.end (), predicate), m_components.end ());
}


void GameObject::removeComponent (const Component::SPtr& comp)
{
	m_components.erase (std::remove (m_components.begin (), m_components.end (), comp), m_components.end ());
}


void GameObject::removeComponent ()
{
	m_components.clear ();
}


void GameObject::addTag (const std::string& tag)
{
	m_tags.insert (tag);
}


void GameObject::removeTag (const std::string& tag)
{
	m_tags.erase (tag);
}


void GameObject::removeTag ()
{
	m_tags.clear ();
}


void GameObject::addChild (const std::string& childName)
{
	if (const auto& child = ObjectManager::GetSingletonInstance ().GetGameObjectByName (childName).lock ())
	{
		m_children.push_back (child);
	}

	//child->transform()->setPosition(this->transform()->position() + child->transform()->position());
	/*child->transform()->setRotation(this->transform()->rotation() * child->transform()->rotation());
	child->transform()->setScale(this->transform()->position() + child->transform()->position());*/
}


void GameObject::removeChild (const std::string& childName)
{
	auto predicate = [&childName] (WPtr elem) -> bool
	{
		if (auto& child = elem.lock ())
		{
			if (child->GetName () == childName)
			{
				child->Destroy ();
				return true;
			}
		}
		return false;
	};
	m_children.erase (std::remove_if (m_children.begin (), m_children.end (), predicate), m_children.end ());
}


void GameObject::removeChildren ()
{
	m_children.clear ();
}


void GameObject::Start ()
{
	for (const auto& component : m_components)
		component->Start ();
}


void GameObject::PreUpdate (float t, float dt)
{
	for (const auto& component : m_components)
		component->PreUpdate (t, dt);
}


void GameObject::Update (float t, float dt)
{
	for (const auto& component : m_components)
		component->Update (t, dt);
}


void GameObject::PostUpdate (float t, float dt)
{
	for (const auto& component : m_components)
		component->PostUpdate (t, dt);
}


void GameObject::Destroy ()
{
	clearParent ();

	for (const auto& component : m_components)
		component->Destroy ();

	m_isDestroyed = true;
}


inline const std::string& GameObject::GetName () const
{
	return m_Name;
}


inline TransformComponent* GameObject::Transform () const
{
	return (TransformComponent*)m_components[0].get ();
}


std::weak_ptr<Component> GameObject::GetComponent (const std::string& cID) const
{
	for (auto it = m_components.cbegin (), end = m_components.cend (); it != end; ++it)
		if ((*it)->GetName () == cID)
			return *it;
	return Component::SPtr (nullptr);
}


inline GameObject::WPtr GameObject::GetParent () const
{
	return m_pParent; 
}


std::vector<std::string> GameObject::getChildrenNames () const
{
	std::vector<std::string> childrenNames;
	for (auto child = m_children.begin (), end = m_children.end (); child != end; ++child)
	{
		childrenNames.push_back (child->lock ()->GetName ());
	}
	return childrenNames;
}


inline void GameObject::clearParent ()
{
	m_pParent.reset ();
}


inline bool GameObject::hasParent () const
{
	return m_pParent != nullptr;
}


void GameObject::setParent (const std::string& parentName)
{
	if (auto& ancestor = ObjectManager::GetSingletonInstance ().GetGameObjectByName (parentName).lock ())
	{
		m_pParent = ancestor;
		ancestor->addChild (m_Name);
	}
}


bool GameObject::hasTag (const std::string& t)
{
	for (auto it = m_tags.cbegin (), end = m_tags.cend (); it != end; ++it)
	{
		if (*it == t)
			return true;
	}
	return false;
}


inline bool GameObject::isDestroyed () const
{
	return m_isDestroyed; 
}


GameObject::~GameObject ()
{
	removeComponent ();
	removeTag ();
	removeChildren ();
}

}	// namespace Engine
