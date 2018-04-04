#include "ResourceManager.h"

TResourceManager::TResourceManager()
{}

TResourceManager::~TResourceManager()
{}

void TResourceManager::Init(string path)
{
    m_path = path;
    LoadTextures();
    LoadSettings();
}

void TResourceManager::LoadTextures()
{
    m_game_textures["ship"] = m_texture_loader.GetTexture(m_path + "\Textures\\ship.tga");
    //m_game_textures.push_back(m_texture_loader.GetTexture(m_path + "\Textures\\ship.tga"));
}

void TResourceManager::LoadSettings()
{

}
