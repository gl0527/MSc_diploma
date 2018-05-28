#include "GameObject.h"
#include "ObjectManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"


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


void GameObject::AddTransformComponent (const std::shared_ptr<TransformComponent>& transform)
{
	RemoveComponent (0);
	InsertComponent (0, transform);
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
			(*it)->MarkForRemove ();
			break;
		}
	}
}


void GameObject::RemoveComponent (size_t index)
{
	if (index > m_components.size () - 1)
		return;

	m_components[index]->MarkForRemove ();
}


void GameObject::RemoveMarkedComponents ()
{
	for (auto comp : m_components) {
		if (comp->IsRemovable ())
			comp->Destroy ();
	}

	m_components.erase (std::remove_if (m_components.begin (), m_components.end (), [] (const std::shared_ptr<Component>& comp) {
			return comp->IsRemovable ();}), m_components.end ());
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
	if (m_children.find (childName) == m_children.end ()) {
		if (const auto& child = ObjectManager::GetInstance ().GetGameObjectByName (childName).lock ()) {
			m_children[childName] = child;
		}
	}
}


void GameObject::RemoveChild (const std::string& childName)
{
	if (m_children.find (childName) != m_children.end ()) {
		m_children[childName]->Destroy ();
		m_children.erase (childName);
	}
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
	return dynamic_cast<TransformComponent*> (m_components[0].get ());
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
	childrenNames.reserve (m_children.size ());

	for (auto it = m_children.begin (), itEnd = m_children.end (); it != itEnd; ++it)
		childrenNames.push_back (it->first);
	
	return childrenNames;
}


std::vector<GameObject::SPtr> GameObject::GetChildren () const
{
	std::vector<GameObject::SPtr> children;
	children.reserve (m_children.size ());

	for (auto it = m_children.begin (), itEnd = m_children.end (); it != itEnd; ++it)
		children.push_back (it->second);

	return children;
}


GameObject::SPtr GameObject::GetChild (const std::string& name)
{
	if (m_children.find (name) != m_children.end ())
		return m_children[name];

	return nullptr;
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
		Transform ()->SetParentTransform ();
		if (auto mesh = GetFirstComponentByType<MeshComponent> ().lock ()) {
			mesh->MoveNode ();
		}
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
	m_components.clear ();
	RemoveTag ();
	RemoveChildren ();
}

}	// namespace Engine
