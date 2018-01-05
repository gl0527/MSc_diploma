#include "Component.h"


namespace Engine {

Component::Component (const std::string& ident, bool uniq/* = false*/)
	: m_name (ident),
	m_isUnique (uniq),
	m_isEnabled (true),
	m_owner (nullptr)
{
}


Component::~Component ()
{
}


void Component::Init (GameObject* object)
{
	m_owner = object;
	PostInit (object);
}


void Component::PostInit (GameObject* /*object*/)
{
}


void Component::Start ()
{
}


void Component::PreUpdate (float /*t*/, float /*dt*/)
{
}


void Component::Update (float /*t*/, float /*dt*/)
{
}


void Component::PostUpdate (float /*t*/, float /*dt*/)
{
}


void Component::Destroy ()
{
}


inline const std::string& Component::GetName () const
{
	return m_name;
}


inline const bool Component::IsUnique () const
{
	return m_isUnique;
}


inline bool Component::IsEnabled () const
{
	return m_isEnabled;
}


inline void Component::Enable ()
{
	m_isEnabled = true;
}


inline void Component::Disable ()
{
	m_isEnabled = false;
}


inline GameObject* Component::GetOwner () const
{
	return m_owner;
}


inline std::string Component::GetTypeName () const
{
	return typeid (*this).name ();
}

}	// namespace Engine
