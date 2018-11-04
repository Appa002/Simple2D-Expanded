#include "../header/Character.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include<freetype/ftbitmap.h>
#include<freetype/freetype.h>
#include <iostream>

#include <stb_image.h>

Simple2D::Character::Character(char c)
{
    const unsigned int FONT_RESOLUTION = 64;

    FT_Library  library;   /* handle to library     */
    FT_Face     face;      /* handle to face object */

    int error = FT_Init_FreeType(&library);
    if (error) {
        std::cout << "Couldn't init libray" << std::endl;
    }

    error = FT_New_Face(library, "/Windows/Fonts/Arial.ttf", 0, &face);
    
    if (error == FT_Err_Unknown_File_Format)
    {
        std::cout << "... the font file could be opened and read, but it appears that its font format is unsupported" << std::endl;
    }
    else if (error)
    {
        std::cout << "... another error code means that the font file could not ... be opened or read, or that it is broken..." << std::endl;
    }

    FT_UInt  glyph_index;

    glyph_index = FT_Get_Char_Index(face, c);
    error = FT_Set_Pixel_Sizes(face, 0, FONT_RESOLUTION);
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    
    // Filp Images upside down
    int widthInBytes = 1 * face->glyph->bitmap.width;
    unsigned char* top = nullptr;
    unsigned char* bottom = nullptr;
    unsigned char tmp = 0;
    int halfHeight = face->glyph->bitmap.rows / 2;

    for (int row = 0; row < halfHeight; row++) {
        top = face->glyph->bitmap.buffer + row * widthInBytes;
        bottom = face->glyph->bitmap.buffer + (face->glyph->bitmap.rows - row - 1) * widthInBytes;
        for (int col = 0; col < widthInBytes; col++) {
            tmp = *top;
            *top = *bottom;
            *bottom = tmp;
            top++;
            bottom++;
        }
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load into opengl texture....
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   
    // Set remaining parameter
    advance = vec2(face->glyph->advance.x, face->glyph->advance.y);
    bearing = vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
    size = vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
    fontSize = FONT_RESOLUTION;

    FT_Done_Face(face);
    FT_Done_FreeType(library);
}
