#include "Tearsplash/Spritefont.h"
#include "Tearsplash/Errors.h"

#include <GL/glew.h>

using namespace Tearsplash;

Spritefont::Spritefont() :
mPixelWidth(static_cast<FT_UInt>(0)),
mPixelHeight(static_cast<FT_UInt>(48)) {}

Spritefont::~Spritefont() {
    // Do nothing.
}

void Spritefont::init(const char* fontPath, unsigned int pixelWidth/*=0*/, unsigned int pixelHeight/*=48*/) {
    FT_Library ftLibrary;
    FT_Face ftFace;
    if (FT_Init_FreeType(&ftLibrary)) {
        Tearsplash::fatalError("Could not init FreeType Library");
    }

    if (FT_New_Face(ftLibrary, fontPath, 0, &ftFace))
    {
        Tearsplash::fatalError("Failed to load font");
    }

    FT_Set_Pixel_Sizes(ftFace, mPixelWidth, mPixelHeight);

    // Enable blending mode.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable byte-alignment restriction.
    // The reason for this is that the textures
    // requires all textures to be 4-byte aligned
    //, i.e. have a size multiple by 4.  Since
    // we're only going to use the red channel,
    // the texture could have any dimension. We
    // don't want any unnecessary padding added
    // or alignment issues, which is why we disable
    // the aligning.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER))
        {
            //std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            break;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            ftFace->glyph->bitmap.width,
            ftFace->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            ftFace->glyph->bitmap.buffer
        );

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            ftFace->glyph->advance.x,
            glm::ivec2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
            glm::ivec2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
        };
        mCharacters.insert(std::pair<char, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    // Done with the face and library, free from FT.
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLibrary);

    // Initialize the SpriteBatch used for text.
    mSpritebatchText.init();

    // Setup the shaders to be used for this text.
    mTextShader.compileShaders("tearsplash/shaders/2DText.vert", "tearsplash/shaders/2DText.frag");
    mTextShader.addAttribute("vertexPosition");
    mTextShader.addAttribute("vertexColor");
    mTextShader.addAttribute("vertexUV");
    mTextShader.linkShaders();
}

void Spritefont::drawText(const std::string& text, const glm::vec4& position, const glm::mat4& cameraMatrix, const glm::vec3& textColor, const float scale) {
    mTextShader.use();

    float x = position.x;
    float y = position.y;

    glUniform3f(mTextShader.getUniformLocation("textColor"), textColor.x, textColor.y, textColor.z);
    glUniformMatrix4fv(mTextShader.getUniformLocation("P"), 1, GL_FALSE, &(cameraMatrix[0][0]));

    ColorRGBA8 color;
    color.r = static_cast<uint8_t>(textColor.r * 255.0);
    color.g = static_cast<uint8_t>(textColor.g * 255.0);
    color.b = static_cast<uint8_t>(textColor.b * 255.0);
    color.a = 255;

    mSpritebatchText.begin(Tearsplash::GlyphSortType::TEXTURE);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = mCharacters[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        mSpritebatchText.draw(glm::vec4(xpos, ypos, w, h), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), ch.textureID, 0, color);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    mTextShader.dontuse();
}

void Spritefont::render() {
    mTextShader.use();
    mSpritebatchText.end();
    mSpritebatchText.renderBatch();
    mTextShader.dontuse();
}