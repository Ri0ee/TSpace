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
    m_game_textures["sprite_ship"] = m_texture_loader.GetTexture(m_path + "\\Res\\Textures\\ship.tga");
    m_game_textures["sprite_bullet"] = m_texture_loader.GetTexture(m_path + "\\Res\\Textures\\bullet.tga");
    m_game_textures["sprite_background"] = m_texture_loader.GetTexture(m_path + "\\Res\\Textures\\background.tga");
}

void TResourceManager::LoadSettings()
{

}
