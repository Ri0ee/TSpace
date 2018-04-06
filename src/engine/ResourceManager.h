#pragma once

#include <string>
#include <map>
#include <TextureLoader.h>

#include "BasicShader.h"
#include "GlitchShader.h"
#include "BlurShader.h"
#include "PostProcessingShader.h"

using std::string;
using std::vector;
using std::map;

class TResourceManager
{
    public:
        TResourceManager();
        virtual ~TResourceManager();

        void Init(string current_dir);
        void LoadTextures();
        void LoadShaders();
        void LoadSettings();

        string m_current_dir;

        TTextureLoader m_texture_loader;

        map<string, GLuint> m_game_textures;
        map<string, string> m_settings;

        TBlurShader m_blur_shader;
        TGlitchShader m_glitch_shader;
        TBasicShader m_basic_shader;
        TPostProcessingShader m_post_processing_shader;
};
