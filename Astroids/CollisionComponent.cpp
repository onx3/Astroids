#include "CollisionComponent.h"

CollisionComponent::CollisionComponent(GameObject * pOwner, sf::Vector2f size)
	: GameComponent(pOwner)
	, mSize(size)
{

}

//------------------------------------------------------------------------------------------------------------------------

CollisionComponent::~CollisionComponent()
{
}

//------------------------------------------------------------------------------------------------------------------------

sf::FloatRect CollisionComponent::GetBounds() const
{
	return sf::FloatRect(mpOwner->GetPosition(), mSize);
}

//------------------------------------------------------------------------------------------------------------------------

bool CollisionComponent::CheckCollision(const CollisionComponent & other) const
{
	if (mpOwner->IsActive() && other.mpOwner->IsActive())
	{
		return GetBounds().intersects(other.GetBounds());
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------------

void CollisionComponent::Update()
{
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------