#pragma once

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include "ShaderLoader.h"

using std::string;

class PostProcessingShader
{
    public:
        PostProcessingShader();
        PostProcessingShader(string directory, string file_name);
        virtual ~PostProcessingShader();

        void Create(string directory, string file_name);
        void Activate();
        void Deactivate();

    private:
        GLuint m_shader_program;
};
