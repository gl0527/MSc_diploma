#include "Component.h"


namespace Engine
{

Component::Component (const std::string& ident, bool uniq)
	: m_name (ident),
	m_isUnique (uniq),
	m_isEnabled (true),
	m_owner (nullptr)
{
}


//Component::Component (ConstructionData constructionData)
//	: m_name (constructionData.m_name),
//	m_isUnique (constructionData.m_isUnique),
//	m_isEnabled (constructionData.m_isEnabled),
//	m_owner (nullptr)
//{
//}


Component::~Component ()
{
}


void Component::Init (GameObject* object)
{
	m_owner = object;
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

}	// namespace Engine
