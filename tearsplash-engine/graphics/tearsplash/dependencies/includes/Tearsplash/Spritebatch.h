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

    class Glyph
    {
    public:
        Glyph() {};
        Glyph(const glm::vec4& _destRect, const glm::vec4& _uvRect, GLuint _texture, int _depth, const ColorRGBA8& _color) :
            texture(_texture), depth(_depth){

            //
            //  ----------
            // |          |
            // |  sprite  | l.y
            // |          |
            // 0,0 -------
            //     l.x

            // Bottom left = DestRect(x,y)
            // l.x = destRect.z/uvRect.z (side length)
            // l.y = destRect.w/uvRect.w (side length)

            topLeft.setColor(_color.r, _color.g, _color.b, _color.a);
            topLeft.setPosition(_destRect.x, _destRect.y + _destRect.w);
            topLeft.setUV(_uvRect.x, _uvRect.y + _uvRect.w);

            topRight.setColor(_color.r, _color.g, _color.b, _color.a);
            topRight.setPosition(_destRect.x + _destRect.z, _destRect.y + _destRect.w);
            topRight.setUV(_uvRect.x + _uvRect.z, _uvRect.y + _uvRect.w);

            bottomLeft.setColor(_color.r, _color.g, _color.b, _color.a);
            bottomLeft.setPosition(_destRect.x, _destRect.y);
            bottomLeft.setUV(_uvRect.x, _uvRect.y);

            bottomRight.setColor(_color.r, _color.g, _color.b, _color.a);
            bottomRight.setPosition(_destRect.x + _destRect.z, _destRect.y);
            bottomRight.setUV(_uvRect.x + _uvRect.z, _uvRect.y);
        };

        Glyph(const glm::vec4& _destRect, const glm::vec4& _uvRect, GLuint _texture, int _depth, const ColorRGBA8& _color, float radianAngle) :
            texture(_texture), depth(_depth) {

            //
            //  ----------
            // |          |
            // |  sprite  | l.y
            // |          |
            // 0,0 -------
            //     l.x

            // Bottom left = DestRect(x,y)
            // l.x = destRect.z/uvRect.z (side length)
            // l.y = destRect.w/uvRect.w (side length)

            glm::vec2 halfDims(_destRect.z * 0.5f, _destRect.w * 0.5f);

            // Sprite representation at origin.
            glm::vec2 tl(-halfDims.x, halfDims.y);
            glm::vec2 tr(halfDims.x, halfDims.y);
            glm::vec2 bl(-halfDims.x, -halfDims.y);
            glm::vec2 br(halfDims.x, -halfDims.y);

            // Rotate points around origin.
            tl = rotatePoint(tl, radianAngle) + halfDims;
            tr = rotatePoint(tr, radianAngle) + halfDims;
            bl = rotatePoint(bl, radianAngle) + halfDims;
            br = rotatePoint(br, radianAngle) + halfDims;

            topLeft.setColor(_color.r, _color.g, _color.b, _color.a);
            topLeft.setPosition(_destRect.x + tl.x, _destRect.y + tl.y);
            topLeft.setUV(_uvRect.x, _uvRect.y + _uvRect.w);

            topRight.setColor(_color.r, _color.g, _color.b, _color.a);
            topRight.setPosition(_destRect.x + tr.x, _destRect.y + tr.y);
            topRight.setUV(_uvRect.x + _uvRect.z, _uvRect.y + _uvRect.w);

            bottomLeft.setColor(_color.r, _color.g, _color.b, _color.a);
            bottomLeft.setPosition(_destRect.x + bl.x, _destRect.y + bl.y);
            bottomLeft.setUV(_uvRect.x, _uvRect.y);

            bottomRight.setColor(_color.r, _color.g, _color.b, _color.a);
            bottomRight.setPosition(_destRect.x + br.x, _destRect.y + br.y);
            bottomRight.setUV(_uvRect.x + _uvRect.z, _uvRect.y);
        };

        GLuint texture;
        int depth;

        Vertex topLeft;
        Vertex bottomLeft;
        Vertex topRight;
        Vertex bottomRight;
    private:
        glm::vec2 rotatePoint(const glm::vec2& point, float radianAngle) const;
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
        void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, int depth, const ColorRGBA8& color, const float radianAngle);
        void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, int depth, const ColorRGBA8& color, const glm::vec2& direction);


    private:
        void createVertexArray();
        void createRenderBatches();
        void sortGlyphs();

        std::vector<Glyph*> mGlyphPointers;
        std::vector<Glyph> mGlyphs;
        std::vector<RenderBatch> mRenderBatches;

        GlyphSortType mSortType;
        GLuint mVAO;
        GLuint mVBO;

    };
}

#endif // !SPRITEBATCH_H
