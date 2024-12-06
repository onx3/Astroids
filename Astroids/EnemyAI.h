#pragma once

#include <SFML/Graphics.hpp>
#include <string>

enum class EEnemy
{
    Ship,
    Ufo,
};

class EnemyAI : public sf::Drawable
{
public:
    EnemyAI(const EEnemy & enemyType);
    ~EnemyAI();

    void InitEnemy();
    void SetTexture(const std::string & file);
    void SetTexture();
    void SetPosition(const sf::Vector2f & vec);
    void SetPosition(const float x, const float y);

protected:
    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    sf::Texture mTexture;
    sf::Sprite mSprite;
    EEnemy mEnemyType;
};

