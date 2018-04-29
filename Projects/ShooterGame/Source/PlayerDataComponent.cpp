#include "PlayerDataComponent.h"


PlayerDataComponent::PlayerDataComponent (const std::string& name):
	Component (name),
	m_healthPoint (100),
	m_kills (0)
{
}


unsigned char PlayerDataComponent::GetHealthPoint () const
{
	return m_healthPoint;
}


unsigned char PlayerDataComponent::GetKills () const
{
	return m_kills;
}


bool PlayerDataComponent::IsDead () const
{
	return m_healthPoint == 0;
}


void PlayerDataComponent::SetHealthPoint (unsigned char healthPoint)
{
	m_healthPoint = healthPoint;
}


void PlayerDataComponent::IncreaseHealtPoint (unsigned char amount)
{
	m_healthPoint += amount;
}


void PlayerDataComponent::DecreaseHealtPoint (unsigned char amount)
{
	if (m_healthPoint - amount < 0)
		m_healthPoint = 0;
	else
		m_healthPoint -= amount;
}


void PlayerDataComponent::IncreaseKills (unsigned char amount)
{
	m_kills += amount;
}