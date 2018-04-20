#include "PostProcessingShader.h"
#include <iostream>

TPostProcessingShader::TPostProcessingShader()
{

}

TPostProcessingShader::TPostProcessingShader(string directory, string file_name)
{
    Create(directory, file_name);
}

TPostProcessingShader::~TPostProcessingShader()
{
    glDeleteProgram(m_shader_program);
}

void TPostProcessingShader::Create(string directory, string file_name)
{
    Shader ShaderLoader;
    m_shader_program = ShaderLoader.CreateProgram(directory + "Shaders\\" + file_name + ".vs",
                                                  directory + "Shaders\\" + file_name + ".fs");

    m_resolution_loc = glGetUniformLocation(m_shader_program, "resolution");
    m_time_loc = glGetUniformLocation(m_shader_program, "time");
    m_vignette_loc = glGetUniformLocation(m_shader_program, "vignette");
    m_doublerating_loc = glGetUniformLocation(m_shader_program, "doublerating");
    m_coef_loc = glGetUniformLocation(m_shader_program, "coef");
}

void TPostProcessingShader::Activate(float res1, float res2, float time, bool vignette, float doublerating, float coef)
{
    glUseProgram(m_shader_program);
    glUniform2f(m_resolution_loc, res1, res2);
    glUniform1f(m_time_loc, time);
    glUniform1f(m_vignette_loc, (vignette == true?1:0));
    glUniform1f(m_doublerating_loc, doublerating);
    glUniform1f(m_coef_loc, coef);
}

void TPostProcessingShader::Deactivate()
{
    glUseProgram(0);
}
