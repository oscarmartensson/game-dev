#ifndef SPRITEFONT_H
#define SPRITEFONT_H

#include <map>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <Tearsplash/Spritebatch.h>
#include <Tearsplash/ShaderProgram.h>

namespace Tearsplash {
    struct Character {
        unsigned int textureID; // ID handle of the glyph texture
        unsigned int advance;   // Offset to advance to next glyph
        glm::ivec2   size;      // Size of glyph
        glm::ivec2   bearing;   // Offset from baseline to left/top of glyph
    };

    class Spritefont {

    public:
        Spritefont();
        ~Spritefont();

        // Creates a font library with a TrueType font (.ttf).
        // The font library supports ascii characters 0-128.
        // @param fontPath: Path to the .ttf font.
        // @param pixelWidth: Width of the font. Default 0 means that the width is
        //                    dynamically calculated from the height.
        // @param pixelWidth: Height of the font.
        void init(const char* fontPath, unsigned int pixelWidth = 0, unsigned int pixelHeight = 48);

        // Draw a text string with the help of the Spritebatch sb.
        // @param sb: Spritebatch to render to.
        // @param text: The text string that is to be displayed.
        void drawText(const std::string& text,
                      const glm::vec4& position,
                      const glm::mat4& cameraMatrix,
                      const glm::vec3& textColor,
                      const float scale);

        // Renders all the text.
        void render();

    private:
        FT_UInt mPixelWidth, mPixelHeight;
        std::map<char, Character> mCharacters;
        Tearsplash::ShaderProgram mTextShader;
        Tearsplash::Spritebatch   mSpritebatchText;
    };
}

#endif // SPRITEFONT_H