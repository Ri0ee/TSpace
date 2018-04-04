#include "PostProcessingShader.h"
#include <iostream>

PostProcessingShader::PostProcessingShader()
{

}

PostProcessingShader::PostProcessingShader(string directory, string file_name)
{
    Create(directory, file_name);
}

PostProcessingShader::~PostProcessingShader()
{
    glDeleteProgram(m_shader_program);
}

void PostProcessingShader::Create(string directory, string file_name)
{
    Shader ShaderLoader;
    m_shader_program = ShaderLoader.CreateProgram(directory + "Shaders\\" + file_name + ".vs",
                                                  directory + "Shaders\\" + file_name + ".fs");
}

void PostProcessingShader::Activate()
{
    glUseProgram(m_shader_program);
}

void PostProcessingShader::Deactivate()
{
    glUseProgram(0);
}
