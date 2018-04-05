#include "GameObject.h"
#include "ObjectManager.h"
#include "TransformComponent.h"


namespace Engine {

GameObject::GameObject (const std::string& id)
	: m_Name (id),
	m_isDestroyed (false),
	m_pParent (std::shared_ptr<GameObject> (nullptr))
{
	std::shared_ptr<TransformComponent> transform (new TransformComponent (id));
	AddComponent (transform);
}


void GameObject::AddComponent (const Component::SPtr& comp, bool replaceOld/* = true*/)
{
	auto it = m_components.begin (), itEnd = m_components.end ();
	
	for (; it != itEnd; ++it) {
		if ((*it)->GetTypeName () == comp->GetTypeName ())
			break;
	}

	if (it != itEnd) {
		if (comp->IsUnique () || (*it)->GetName () == comp->GetName ()) {
			if (replaceOld) {
				RemoveComponent ((*it)->GetName ());
				m_components.push_back (comp);
				comp->Init (this);
			}

			return;
		}
	}
	m_components.push_back (comp);
	comp->Init (this);
}


void GameObject::InsertComponent (size_t index, const std::shared_ptr<Component>& comp)
{
	if (comp) {
		m_components.insert (m_components.begin () + index, comp);
		comp->Init (this);
	}
}


void GameObject::RemoveComponent (const std::string& compName)
{
	for (auto it = m_components.begin (); it != m_components.end (); ++it) {
		if ((*it)->GetName () == compName) {
			(*it)->Destroy ();
			m_components.erase (it);
			break;
		}
	}
}


void GameObject::RemoveComponent (size_t index)
{
	if (index > m_components.size () - 1)
		return;

	m_components.erase (m_components.begin () + index);
}


void GameObject::RemoveComponent (const Component::SPtr& comp)
{
	m_components.erase (std::remove (m_components.begin (), m_components.end (), comp), m_components.end ());
}


void GameObject::RemoveComponent ()
{
	m_components.clear ();
}


void GameObject::AddTag (const std::string& tag)
{
	m_tags.insert (tag);
}


void GameObject::RemoveTag (const std::string& tag)
{
	m_tags.erase (tag);
}


void GameObject::RemoveTag ()
{
	m_tags.clear ();
}


void GameObject::AddChild (const std::string& childName)
{
	if (const auto& child = ObjectManager::GetInstance ().GetGameObjectByName (childName).lock ()) {
		m_children.push_back (child);
	}
}


void GameObject::RemoveChild (const std::string& childName)
{
	auto predicate = [&childName] (WPtr elem) -> bool {
		if (auto child = elem.lock ()) {
			if (child->GetName () == childName) {
				child->Destroy ();
				return true;
			}
		}
		return false;
	};
	m_children.erase (std::remove_if (m_children.begin (), m_children.end (), predicate), m_children.end ());
}


void GameObject::RemoveChildren ()
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
	ClearParent ();

	for (const auto& component : m_components)
		component->Destroy ();

	m_isDestroyed = true;
}


const std::string& GameObject::GetName () const
{
	return m_Name;
}


TransformComponent* GameObject::Transform () const
{
	return reinterpret_cast<TransformComponent*> (m_components[0].get ());
}


std::weak_ptr<Component> GameObject::GetComponent (const std::string& cID) const
{
	for (auto it = m_components.cbegin (), end = m_components.cend (); it != end; ++it)
		if ((*it)->GetName () == cID)
			return *it;
	return Component::SPtr (nullptr);
}


GameObject::WPtr GameObject::GetParent () const
{
	return m_pParent;
}


std::vector<std::string> GameObject::GetChildrenNames () const
{
	std::vector<std::string> childrenNames;
	for (auto child = m_children.begin (), end = m_children.end (); child != end; ++child) {
		childrenNames.push_back (child->lock ()->GetName ());
	}
	return childrenNames;
}


const std::vector<GameObject::WPtr> GameObject::GetChildren () const
{
	return m_children;
}


void GameObject::ClearParent ()
{
	m_pParent.reset ();
}


bool GameObject::HasParent () const
{
	return m_pParent != nullptr;
}


void GameObject::SetParent (const std::string& parentName)
{
	if (auto ancestor = ObjectManager::GetInstance ().GetGameObjectByName (parentName).lock ()) {
		m_pParent = ancestor;
		ancestor->AddChild (m_Name);
	}
}


bool GameObject::HasTag (const std::string& t) const
{
	for (auto it = m_tags.cbegin (), end = m_tags.cend (); it != end; ++it) {
		if (*it == t)
			return true;
	}
	return false;
}


bool GameObject::IsDestroyed () const
{
	return m_isDestroyed;
}


GameObject::~GameObject ()
{
	RemoveComponent ();
	RemoveTag ();
	RemoveChildren ();
}

}	// namespace Engine
