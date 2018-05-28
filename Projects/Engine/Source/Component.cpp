#include "Component.h"


namespace Engine {

Component::Component (const std::string& ident, bool uniq/* = false*/)
	: m_name (ident),
	m_isUnique (uniq),
	m_isEnabled (true),
	m_isRemovable (false),
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


const std::string& Component::GetName () const
{
	return m_name;
}


const bool Component::IsUnique () const
{
	return m_isUnique;
}


bool Component::IsEnabled () const
{
	return m_isEnabled;
}


void Component::Enable ()
{
	m_isEnabled = true;
}


void Component::Disable ()
{
	m_isEnabled = false;
}


bool Component::IsRemovable () const
{
	return m_isRemovable;
}


GameObject* Component::GetOwner () const
{
	return m_owner;
}


std::string Component::GetTypeName () const
{
	return typeid (*this).name ();
}


void Component::MarkForRemove ()
{
	m_isRemovable = true;
}

}	// namespace Engine
