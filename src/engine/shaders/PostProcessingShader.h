#pragma once

#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include "ShaderLoader.h"

using std::string;

class TPostProcessingShader
{
    public:
        TPostProcessingShader();
        TPostProcessingShader(string directory, string file_name);
        virtual ~TPostProcessingShader();

        void Create(string directory, string file_name);
        void Activate(float res1, float res2, float time, bool vignette, float doublerating, float coef);
        void Deactivate();

    private:
        GLuint m_shader_program;
        GLuint m_resolution_loc;
        GLuint m_time_loc;
        GLuint m_vignette_loc;
        GLuint m_doublerating_loc;
        GLuint m_coef_loc;
};
