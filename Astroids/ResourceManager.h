#pragma once
#include "AstroidsPrivate.h"
#include "BaseManager.h"
#include <string>
#include <functional>

class ResourceId
{
public:
    explicit ResourceId(std::string const & name);        

    size_t GetHash();

    std::string & GetName();

    bool operator==(ResourceId const & other) const
    {
        return mHash == other.mHash;
    }
    bool operator<(ResourceId const & other) const
    {
        return mHash < other.mHash;
    }

private:
    std::string mResourceName;
    size_t mHash;
    // Cache the hash value
};

//------------------------------------------------------------------------------------------------------------------------
// ResourceManager
//------------------------------------------------------------------------------------------------------------------------

class ResourceManager : public BaseManager
{
public:
	ResourceManager(GameManager * pGameManager);
	
    std::shared_ptr<sf::Texture> GetTexture(ResourceId & resourceId);

    void PreloadResources(std::vector<std::string> const & resourcePaths);

    void PreloadResources(std::vector<ResourceId> & resourceIds);
    
private:
    std::map<ResourceId, std::shared_ptr<sf::Texture>> mTextureResources;
};
