#include <iostream>
#include <string>
#include <cmath>
#include <windows.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "BlurShader.h"
#include "GlitchShader.h"
#include "BasicShader.h"
#include "PostProcessingShader.h"

#include "ETime.h"
#include "FrameBuffer.h"
#include "Game.h"
#include "Utility.h"
#include "Render.h"
#include "FreeType.h"
#include "Gui.h"
#include "ResourceManager.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;

float FPS_LIMIT         = 150.0;
int frameCount          = 0;
float FPS               = 0;

float unprocessedTime   = 0.0;
float frameCounter      = 0.0;
float frameTime         = 1.0/FPS_LIMIT;
float deltaTime         = 1.0/150.0;

int window_width        = 1280;
int window_height       = 720;
int max_msaa_samples    = 0;
const char* window_name = "OpenGLEngine";

vec2 mouse_pos;

GLFWwindow* window;

string current_dir;

game::TGame Game;
freetype::TFreeType ftlib;
gui::TGui guilib;

TResourceManager resmngr;

bool keyDown[1000];

TBlurShader BlurShader;
TGlitchShader GlitchShader;
TBasicShader BasicShader;
TPostProcessingShader PPShader;

TFrameBuffer fbo;
TFrameBuffer fbo_msaa;
TFrameBuffer one_more_buffer;

float background_shift;

///BEGIN

void HideGUI()
{
    std::cout << "GUI hidden" << std::endl;
    guilib.m_visible = false;
}

void ShowGUI()
{
    std::cout << "GUI shown" << std::endl;
    guilib.m_visible = true;
}

void GameplayRender()
{
    graphics::DrawTexture(vec2(0 - background_shift, 0), window_width, window_height, resmngr.m_game_textures["sprite_background"], false);
    graphics::DrawTexture(vec2(window_width - background_shift, 0), window_width, window_height, resmngr.m_game_textures["sprite_background"], false);

    Game.m_vecBorderMeshes[0].Draw(0 - background_shift, 0, BORDER);
    Game.m_vecBorderMeshes[1].Draw(window_width - background_shift, 0, BORDER);
    Game.m_vecBorderMeshes[2].Draw(0 - background_shift, window_height - BORDER_MESH_BOUND_Y * 2 - 20, BORDER);
    Game.m_vecBorderMeshes[3].Draw(window_width - background_shift, window_height - BORDER_MESH_BOUND_Y * 2 - 20, BORDER);

    background_shift++;
    if(background_shift == window_width)
    {
        background_shift = 0;
        Game.m_vecBorderMeshes[0] = Game.m_vecBorderMeshes[1];
        Game.m_vecBorderMeshes[1] = Game.GenerateBorderMesh(true);
        Game.m_vecBorderMeshes[2] = Game.m_vecBorderMeshes[3];
        Game.m_vecBorderMeshes[3] = Game.GenerateBorderMesh(false);
    }

    Game.m_border_shift = background_shift;

    if(Game.m_game_over)
    {
        freetype::textinf string_info, string_info_2;
        string_info.font_size = 30;
        string_info_2.font_size = 25;
        ftlib.GetTextInfo("GAME OVER", 30, &string_info);
        ftlib.GetTextInfo("Press enter to continue", 25, &string_info_2);

        TPolygon nice_border(100, 100, 100, 100);
        nice_border.addVertex(0, 0);
        nice_border.addVertex(string_info_2.width, 0);
        nice_border.addVertex(string_info_2.width, 100);
        nice_border.addVertex(0, 100);
        nice_border.Draw((window_width - string_info_2.width) / 2, window_height / 2 - 50, FILLED);

        graphics::ftDrawText("GAME OVER", color{0, 255, 255, 100}, vec2((window_width - string_info.width) / 2, (window_height - string_info.height) / 2), 30, ftlib);
        graphics::ftDrawText("Press enter to continue", color{0, 255, 255, 100}, vec2((window_width - string_info_2.width) / 2, (window_height + string_info_2.height + 30) / 2), 25, ftlib);
    }

    if(Game.m_is_running)
    {
        for(auto it = Game.m_vecEntities.begin(); it != Game.m_vecEntities.end(); it++)
        {
            graphics::DrawTexture(vec2(it->m_x, it->m_y),
                                  it->m_width,
                                  it->m_height,
                                  resmngr.m_game_textures[it->m_sprite_name],
                                  true,
                                  it->m_vecPolygon[0].m_color);

            if(it->m_type == PLAYER || it->m_type == ENEMY)
            {
                float width = it->m_width / it->m_maximum_life * it->m_current_life;

                TPolygon health_bar(255, 255, 255, 100);
                health_bar.addVertex(0, 0);
                health_bar.addVertex(width, 0);
                health_bar.addVertex(width, 5);
                health_bar.addVertex(0, 5);
                health_bar.Draw(it->m_x, it->m_y - 7, FILLED);
            }
        }
    }
    else
    {
        if(!guilib.m_visible && !Game.m_game_over)
        {
            ShowGUI();
        }
    }
}

void Render()
{
    if(fbo_msaa.m_status && Game.m_msaa)
    {
        fbo_msaa.Bind();
            graphics::Clear();
            GameplayRender();
        fbo_msaa.Unbind();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_msaa.m_framebuffer_id);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.m_framebuffer_id);
        glBlitFramebuffer(0, 0, fbo_msaa.m_width, fbo_msaa.m_height, 0, 0, fbo.m_width, fbo.m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
    else
    {
        fbo.Bind();
            graphics::Clear();
            GameplayRender();
        fbo.Unbind();
    }

    one_more_buffer.Bind();
    graphics::Clear();
    resmngr.m_post_processing_shader.Activate(window_width, window_height, GetTime() / 1000, true, 0.003);
    graphics::DrawTexture(vec2(0, 0), window_width, window_height, fbo.m_texture_id, true);
    resmngr.m_post_processing_shader.Deactivate();
    one_more_buffer.Unbind();

    graphics::Clear();
    resmngr.m_glitch_shader.Activate(Game.m_shaking_rating, window_width, window_height);
    graphics::DrawTexture(vec2(0, 0), window_width, window_height, one_more_buffer.m_texture_id, false);
    resmngr.m_glitch_shader.Deactivate();

    if(Game.m_shaking_rating > 1)
        Game.m_shaking_rating = Game.m_shaking_rating - 0.2;

    guilib.Draw();

    color text_color{255, 255, 0, 100};
    graphics::ftDrawText("Fps: " + to_string(FPS), text_color, vec2(0, 10), 10, ftlib);
    //graphics::ftDrawText((Game.m_collision_flag?"collision: yes":"collision: no"), text_color, vec2(0, 21), 10, ftlib);
    //graphics::ftDrawText("Depth: " + to_string(Game.m_collsion_depth), text_color, vec2(0, 32), 10, ftlib);
    //graphics::ftDrawText("Mouse pos: (" + to_string(mouse_pos.a) + ";" + to_string(mouse_pos.b) + ")", text_color, vec2(0, 44), 10, ftlib);
    //graphics::ftDrawText("Player pos: (" + to_string(Game.m_vecEntities[0].m_x) + ";" + to_string(Game.m_vecEntities[0].m_y) + ")", text_color, vec2(0, 55), 10, ftlib);
    //graphics::ftDrawText("Enemy pos: (" + to_string(Game.m_vecEntities[1].m_x) + ";" + to_string(Game.m_vecEntities[1].m_y) + ")", text_color, vec2(0, 66), 10, ftlib);
    //graphics::ftDrawText("Entity count: " + to_string((int)Game.m_vecEntities.size()), text_color, vec2(0, 78), 10, ftlib);
    //graphics::ftDrawText("Entity[1] velocity_x: " + to_string((int)Game.m_vecEntities[1].m_velocity_x), text_color, vec2(0, 89), 10, ftlib);

    glfwSwapBuffers(window);
}

void Start_button_callback()
{
    cout << "Callback function called\n";
    HideGUI();
    Game.Start();
}

void Exit_button_callback()
{
    std::cout << "Exit button callback called" << std::endl;
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void InitGUI()
{
    color btncolor{77, 0, 57, 50};
    color hl_btncolor{133, 51, 85, 50};
    color fg_btncolor{255, 51, 204, 100};

    vector<vec2> vertex;
    vertex.push_back(vec2(0, 0));    vertex.push_back(vec2(300, 0));
    vertex.push_back(vec2(300, 40)); vertex.push_back(vec2(50, 40));
    TButton button_1(vertex, vec2(window_width - 300, 10), 2, btncolor, fg_btncolor, hl_btncolor, "Start", &ftlib);
    TButton button_2(vertex, vec2(window_width - 300, 80), 2, btncolor, fg_btncolor, hl_btncolor, "Exit", &ftlib);
    button_1.SetCallback(Start_button_callback);
    button_2.SetCallback(Exit_button_callback);
    guilib.AddButton(button_1);
    guilib.AddButton(button_2);
    guilib.m_visible = true;
}

void DebugOutput()
{
    cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    cout << "Shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    cout << "Maximum MSAA Samples: " << max_msaa_samples << "\n";
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS)
    {
        if(key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GL_TRUE);
        keyDown[key] = true;
        return;
    }

    if(action == GLFW_RELEASE)
        keyDown[key] = false;
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse_pos.a = xpos;
    mouse_pos.b = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        guilib.Press();
}

void InitEverything()
{
    ///GLFW INIT
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

    window = glfwCreateWindow(window_width, window_height, window_name, 0, 0);
    glfwSetWindowPos(window, 5, 50);

    if(window == 0)
    {
        cout << "Failed to create window\n";
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    ///GL INIT
    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, window_width, window_height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    fbo.Create(window_width, window_height);
    fbo_msaa.CreateMultisampled(window_width, window_height, 4);
    one_more_buffer.Create(window_width, window_height);

    glGetIntegerv(GL_MAX_SAMPLES, &max_msaa_samples);
    DebugOutput();

    ///OTHER INITs
    resmngr.Init(current_dir);
    ftlib.Init(current_dir, "Res\\Fontin-Regular.ttf");
    Game.Init(window_width, window_height);
    InitGUI();
}

int main(int argc, char **argv)
{
    SetConsoleTitle("OpenGL debug console");
    string argv_str = argv[0];
	current_dir = argv_str.substr(0, argv_str.find_last_of("\\") + 1);
	cout << "current_dir: " << current_dir << endl;

	InitEverything();

    glfwSetTime(0);
    double lastTime = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        bool render_state = false;
        float startTime = GetTime();
        float passedTime = startTime - lastTime;
        lastTime = GetTime();

        unprocessedTime+=passedTime/1000;
        frameCounter+=passedTime/1000;

        while(unprocessedTime > deltaTime)
        {
            unprocessedTime-=deltaTime;

            Game.Input(keyDown);
            Game.Update(deltaTime);
            guilib.CheckCollsions(mouse_pos);

            if(frameCounter >= 0.5)
            {
                FPS = frameCount / frameCounter;
                frameCount = 0;
                frameCounter = 0;
            }
            render_state = true;
        }
        if(render_state)
        {
            Render();
            frameCount++;
        }
    }

    glfwTerminate();
    return 0;
}
