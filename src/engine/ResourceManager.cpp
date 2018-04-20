#include "ResourceManager.h"
#include <iostream>

TResourceManager::TResourceManager()
{

}

TResourceManager::~TResourceManager()
{

}

void TResourceManager::Init(string current_dir)
{
    m_current_dir = current_dir;
    std::cout << "Loading Textures" << std::endl;
    LoadTextures();
    std::cout << "Loading Shaders" << std::endl;
    LoadShaders();
}

void TResourceManager::LoadTextures()
{
    m_game_textures["sprite_ship"] = m_texture_loader.GetTexture(m_current_dir + "Res\\Textures\\ship.tga");
    m_game_textures["sprite_bullet"] = m_texture_loader.GetTexture(m_current_dir + "Res\\Textures\\bullet.tga");
    m_game_textures["sprite_background"] = m_texture_loader.GetTexture(m_current_dir + "Res\\Textures\\background.tga");
    m_game_textures["sprite_enemy_1"] = m_texture_loader.GetTexture(m_current_dir + "Res\\Textures\\enemy_1.tga");
    m_game_textures["sprite_enemy_2"] = m_texture_loader.GetTexture(m_current_dir + "Res\\Textures\\enemy_2.tga");
    m_game_textures["sprite_enemy_3"] = m_texture_loader.GetTexture(m_current_dir + "Res\\Textures\\enemy_3.tga");
    m_game_textures["sprite_logo"] = m_texture_loader.GetTexture(m_current_dir + "Res\\Textures\\logo.tga");
}

void TResourceManager::LoadShaders()
{
    m_glitch_shader.Create(m_current_dir + "Res\\", "Glitch_Shader");
    m_blur_shader.Create(m_current_dir + "Res\\", "Blur_Shader");
    m_basic_shader.Create(m_current_dir + "Res\\", "Basic_Shader");
    m_post_processing_shader.Create(m_current_dir + "Res\\", "Post_Processing_Shader");
}

void TResourceManager::LoadSettings()
{

}
