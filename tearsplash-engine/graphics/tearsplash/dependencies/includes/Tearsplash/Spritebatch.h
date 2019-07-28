// Spritebatch.h

#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include "Tearsplash/Vertex.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace Tearsplash
{

    enum class GlyphSortType
    {
        NONE,
        FRONT_TO_BACK,
        BACK_TO_FRONT,
        TEXTURE
    };

    struct Glyph
    {
        GLuint texture;
        int depth;

        Vertex topLeft;
        Vertex bottomLeft;
        Vertex topRight;
        Vertex bottomRight;
    };

    class RenderBatch
    {
    public:
        RenderBatch(GLuint offset, GLuint numVertices, GLuint texture) : mOffset(offset), mNumVertices(numVertices), mTexture(texture) {};

        GLuint mOffset;
        GLuint mNumVertices;
        GLuint mTexture;
    };

    class Spritebatch 
    {

    public:
        Spritebatch();
        ~Spritebatch();

        void init();
        void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
        void end();
        void renderBatch();
        void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, int depth, const ColorRGBA8& color);

    private:
        void createVertexArray();
        void createRenderBatches();
        void sortGlyphs();

        std::vector<Glyph*> mGlyphs;
        std::vector<RenderBatch> mRenderBatches;

        GlyphSortType mSortType;
        GLuint mVAO;
        GLuint mVBO;

    };
}

#endif // !SPRITEBATCH_H
