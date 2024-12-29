#include "AstroidsPrivate.h"
#include "ResourceManager.h"

ResourceId::ResourceId(std::string const & name)
    : mResourceName(name)
    , mHash(static_cast<uint32_t>(std::hash<std::string>{}(name)))
{
}

//------------------------------------------------------------------------------------------------------------------------

size_t ResourceId::GetHash()
{
    return mHash;
}

//------------------------------------------------------------------------------------------------------------------------

std::string & ResourceId::GetName()
{
    return mResourceName;
}

//------------------------------------------------------------------------------------------------------------------------
// ResourceManager
//------------------------------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager(GameManager * pGameManager)
	: BaseManager(pGameManager)
{
}

//------------------------------------------------------------------------------------------------------------------------

std::shared_ptr<sf::Texture> ResourceManager::GetTexture(ResourceId & resourceId)
{
    // Check if the texture already exists
    auto it = mTextureResources.find(resourceId);
    if (it != mTextureResources.end())
    {
        return it->second; // Return cached texture
    }

    // Load texture from disk if not found
    auto texture = std::make_shared<sf::Texture>();
    if (texture->loadFromFile(resourceId.GetName()))
    {
        mTextureResources[resourceId] = texture;
        return texture;
    }

    return nullptr;
}

//------------------------------------------------------------------------------------------------------------------------

void ResourceManager::PreloadResources(std::vector<std::string> const & resourcePaths)
{
    for (auto const & path : resourcePaths)
    {
        ResourceId resourceId(path);
        if (mTextureResources.find(resourceId) == mTextureResources.end())
        {
            auto texture = std::make_shared<sf::Texture>();
            if (texture->loadFromFile(path))
            {
                mTextureResources[resourceId] = texture;
            }
            else
            {
                std::cerr << "Failed to load texture: " << path << std::endl;
            }
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------