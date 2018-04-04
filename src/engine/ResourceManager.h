#pragma once

#include <string>
#include <vector>
#include <map>
#include <TextureLoader.h>
#include <iostream>

using std::string;
using std::vector;
using std::map;

class TResourceManager
{
    public:
        TResourceManager();
        virtual ~TResourceManager();

        void Init(string path);
        void LoadTextures();
        void LoadSettings();

        string m_path;
        TTextureLoader m_texture_loader;
        map<string, GLuint> m_game_textures;
};
