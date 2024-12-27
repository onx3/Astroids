#include "CollisionComponent.h"
#include "box2d/box2d.h"

CollisionComponent::CollisionComponent(GameObject * pOwner, b2World * pWorld, b2Body * pBody, sf::Vector2f size, bool isDynamic)
    : GameComponent(pOwner)
    , mpWorld(pWorld)
    , mpBody(pBody)
    , mSize(size)
{
}

//------------------------------------------------------------------------------------------------------------------------

CollisionComponent::~CollisionComponent()
{
    mpWorld->DestroyBody(mpBody);
}

//------------------------------------------------------------------------------------------------------------------------

void CollisionComponent::Update()
{
    float scale = mpOwner->PIXELS_PER_METER;
    auto spritePos = mpOwner->GetPosition();
    b2Vec2 box2dPosition(spritePos.x / scale, spritePos.y / scale);
    auto rotation = mpOwner->GetRotation() * (b2_pi / 180.0f);

    // Only update if there's a difference
    if (box2dPosition != mpBody->GetPosition() || rotation != mpBody->GetAngle())
    {
        mpBody->SetTransform(box2dPosition, rotation);
    }
}

void CollisionComponent::Draw(sf::RenderWindow & window)
{
    // Create a rectangle shape for the bounding box
    sf::RectangleShape boundingBox(mSize);

    // Set the origin to the center for proper rotation
    boundingBox.setOrigin(mSize.x / 2, mSize.y / 2);

    // Set position and rotation
    b2Vec2 box2dPosition = mpBody->GetPosition();
    float scale = mpOwner->PIXELS_PER_METER;
    boundingBox.setPosition(box2dPosition.x * scale, box2dPosition.y * scale);
    boundingBox.setRotation(mpBody->GetAngle() * (180.0f / b2_pi)); // Convert radians to degrees

    // Set a color (e.g., semi-transparent red for debugging)
    boundingBox.setFillColor(sf::Color(0, 0, 0, 0)); // Transparent fill
    boundingBox.setOutlineColor(sf::Color::Red);     // Red outline
    boundingBox.setOutlineThickness(1.0f);

    // Draw the bounding box
    window.draw(boundingBox);
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------