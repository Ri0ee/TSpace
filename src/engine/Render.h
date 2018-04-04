#pragma once

#define FILLED 1
#define BORDER 2

#define GLEW_STATIC
#include <GL/glew.h>

#include <string>
#include <vector>

#include "Structures.h"
#include "FreeType.h"

using std::string;
using std::vector;

namespace graphics
{
    void Clear();
    void DrawTexture(vec2 position, float width, float height, GLuint texture, bool use_blend, color color_mask);
    void DrawTexture(vec2 position, float width, float height, GLuint texture, bool use_blend);
    void DrawPolygon(vector<vec2> shape, color shape_color, vec2 position, bool use_blend, int draw_mode);
    void ftDrawText(string text, color text_color, vec2 position, int text_size, freetype::TFreeType &ftlib);
}
