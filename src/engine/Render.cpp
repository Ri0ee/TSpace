#include "Render.h"

namespace graphics
{
    void Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
    }

    void DrawTexture(vec2 position, float width, float height, GLuint texture, bool use_blend, color color_mask)
    {
        float X0 = position.a;
        float Y0 = position.b;

        if(use_blend)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        glActiveTexture(GL_TEXTURE0);
        glColor4f(float(color_mask.r) / 255, float(color_mask.g) / 255, float(color_mask.b) / 255, float(color_mask.a) / 100);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(X0, Y0 + height, 0.0f);
            glTexCoord2f(1, 0); glVertex3f(X0 + width, Y0 + height, 0.0f);
            glTexCoord2f(1, 1); glVertex3f(X0 + width, Y0, 0.0f);
            glTexCoord2f(0, 1); glVertex3f(X0, Y0, 0.0f);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glColor4f(1, 1, 1, 1);
        if(use_blend) glDisable(GL_BLEND);
    }

    void DrawTexture(vec2 position, float width, float height, GLuint texture, bool use_blend)
    {
        float X0 = position.a;
        float Y0 = position.b;

        if(use_blend)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex3f(X0, Y0 + height, 0.0f);
            glTexCoord2f(1, 0); glVertex3f(X0 + width, Y0 + height, 0.0f);
            glTexCoord2f(1, 1); glVertex3f(X0 + width, Y0, 0.0f);
            glTexCoord2f(0, 1); glVertex3f(X0, Y0, 0.0f);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        if(use_blend) glDisable(GL_BLEND);
    }

    void DrawPolygon(vector<vec2> shape, color shape_color, vec2 position, bool use_blend, int draw_mode)
    {
        float X0 = position.a;
        float Y0 = position.b;

        if(use_blend)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        if(draw_mode == FILLED)
        {
            glColor4f(float(shape_color.r) / 255, float(shape_color.g) / 255, float(shape_color.b) / 255, float(shape_color.a) / 100);
            glBegin(GL_POLYGON);
            for(unsigned int i = 0; i < shape.size(); i++)
                glVertex2f(X0 + shape[i].a, Y0 + shape[i].b);
            glEnd();
        }

        if(draw_mode == BORDER)
        {
            for(unsigned int i = 0; i < shape.size() - 1; i++)
            {
                glColor4f(float(shape_color.r) / 255, float(shape_color.g) / 255, float(shape_color.b) / 255, float(shape_color.a) / 100);
                glBegin(GL_LINES);
                glVertex2f(X0 + shape[i].a, Y0 + shape[i].b);
                glVertex2f(X0 + shape[i+1].a, Y0 + shape[i+1].b);
                glEnd();
            }
            glColor4f(float(shape_color.r) / 255, float(shape_color.g) / 255, float(shape_color.b) / 255, float(shape_color.a) / 100);
            glBegin(GL_LINES);
            glVertex2f(X0 + shape[0].a, Y0 + shape[0].b);
            glVertex2f(X0 + shape[shape.size()-1].a, Y0 + shape[shape.size()-1].b);
            glEnd();
        }

        if(use_blend) glDisable(GL_BLEND);
        glColor4f(1, 1, 1, 1);
    }

    void ftDrawText(string text, color text_color, vec2 position, int text_size, freetype::TFreeType &ftlib)
    {
        float shift = 0;
        for(unsigned int i = 0; i < text.size(); i++)
        {
            freetype::ftchar *tempChar = new freetype::ftchar();
            ftlib.GetSymbol(text[i], tempChar, text_size);

            DrawTexture(vec2(position.a + shift, position.b - tempChar->bearingY), tempChar->width, tempChar->height, tempChar->texture, true, text_color);

            shift+=tempChar->advance;

            delete tempChar;
        }
    }
}
