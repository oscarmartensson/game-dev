/**********************************************************************/
// File:	Spritebatch.cpp
// Summary:	This is a batch class for sprites, so that it's more convenient
//          to use multiple sprites.
//
// Version: Version 1.0 - 2019-03-17
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2019-03-17 File created
/**********************************************************************/

#include "Tearsplash/Spritebatch.h"
#include "Tearsplash/Errors.h"

#include <vector>
#include <algorithm>

using namespace Tearsplash;

Spritebatch::Spritebatch() : mVBO(0), mVAO(0){};

Spritebatch::~Spritebatch() {};

void Spritebatch::init() 
{
    createVertexArray();
}

void Spritebatch::begin(GlyphSortType sortType)
{
    mSortType = sortType;
    mRenderBatches.clear();
    mGlyphs.clear();
}

void Spritebatch::end() 
{
    mGlyphPointers.resize(mGlyphs.size());
    for (int i = 0; i < mGlyphPointers.size(); i++) {
        mGlyphPointers[i] = &mGlyphs[i];
    }
    sortGlyphs();
    createRenderBatches();
}

void Spritebatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, int depth, const ColorRGBA8& color)
{
    mGlyphs.emplace_back(destRect, uvRect, texture, depth, color);
}

void Tearsplash::Spritebatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, int depth, const ColorRGBA8& color, float radianAngle)
{
    mGlyphs.emplace_back(destRect, uvRect, texture, depth, color, radianAngle);
}

void Tearsplash::Spritebatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, int depth, const ColorRGBA8& color, const glm::vec2& direction)
{
    // Standard convention to always point to the right as initial.
    const glm::vec2 right(1.0f, 0.0);

    // Calculate rotational angle.
    float radianAngle = glm::acos(glm::dot(right, direction));
    if (direction.y < 0.0f) {
        radianAngle = -radianAngle;
    }

    mGlyphs.emplace_back(destRect, uvRect, texture, depth, color, radianAngle);
}

void Spritebatch::createVertexArray()
{
    if (mVAO == 0)
    {
        glGenVertexArrays(1, &mVAO);
    }
    glBindVertexArray(mVAO);
   
    if (mVBO == 0)
    {
        glGenBuffers(1, &mVBO);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    // Zeroth index, only vertex data
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    // Color attribute pointer
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color)); // GL_TRUE == wants to normalize colors to [0,1]
    // UV attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
}

void Spritebatch::sortGlyphs()
{
    switch (mSortType)
    {
        case GlyphSortType::BACK_TO_FRONT:
            std::sort(mGlyphPointers.begin(), mGlyphPointers.end(), [](const Glyph* a, const Glyph* b) { return a->depth > b->depth; });
            break;

        case GlyphSortType::FRONT_TO_BACK:
            std::sort(mGlyphPointers.begin(), mGlyphPointers.end(), [](const Glyph* a, const Glyph* b) { return a->depth < b->depth; });
            break;

        case GlyphSortType::TEXTURE:
            std::sort(mGlyphPointers.begin(), mGlyphPointers.end(), [](const Glyph* a, const Glyph* b) { return a->texture < b->texture; });
            break;

        case GlyphSortType::NONE:
        default:
            softError("Trying to sort batches with GlyphSortType::NONE");
            break;
    }
}

void Spritebatch::renderBatch()
{
    glBindVertexArray(mVAO);

    // Render all batches
    for (size_t i = 0; i < mRenderBatches.size(); i++)
    {
        glBindTexture(GL_TEXTURE_2D, mRenderBatches[i].mTexture);
        glDrawArrays(GL_TRIANGLES, mRenderBatches[i].mOffset, mRenderBatches[i].mNumVertices);
    }

    // Unbind VAO
    glBindVertexArray(0);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Spritebatch::createRenderBatches()
{
    std::vector<Vertex> vertices;
    vertices.resize(mGlyphs.size() * 6);

    if (mGlyphs.empty())
    {
        return;
    }

    int offset = 0;
    int currentVertex = 0;

    // Constructs a new render batch with arguments and pushes to back in vector mRenderBatches
    mRenderBatches.emplace_back(0, 6, mGlyphPointers[0]->texture);
    vertices[currentVertex++] = mGlyphPointers[0]->topLeft; // Increments currentVertex after vector assignment
    vertices[currentVertex++] = mGlyphPointers[0]->bottomLeft;
    vertices[currentVertex++] = mGlyphPointers[0]->bottomRight;
    vertices[currentVertex++] = mGlyphPointers[0]->bottomRight;
    vertices[currentVertex++] = mGlyphPointers[0]->topRight;
    vertices[currentVertex++] = mGlyphPointers[0]->topLeft;
    offset += 6;

    for (size_t currentGlyph = 1; currentGlyph < mGlyphPointers.size(); currentGlyph++)
    {
        if (mGlyphPointers[currentGlyph]->texture != mGlyphPointers[currentGlyph - 1]->texture)
        {
            // Only push if new texture is present
            mRenderBatches.emplace_back(offset, 6, mGlyphPointers[currentGlyph]->texture);
        }
        else
        {
            mRenderBatches.back().mNumVertices += 6;
        }

        vertices[currentVertex++] = mGlyphPointers[currentGlyph]->topLeft;
        vertices[currentVertex++] = mGlyphPointers[currentGlyph]->bottomLeft;
        vertices[currentVertex++] = mGlyphPointers[currentGlyph]->bottomRight;
        vertices[currentVertex++] = mGlyphPointers[currentGlyph]->bottomRight;
        vertices[currentVertex++] = mGlyphPointers[currentGlyph]->topRight;
        vertices[currentVertex++] = mGlyphPointers[currentGlyph]->topLeft;
        offset += 6;
    }

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    // Orphan the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    // Upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

glm::vec2 Glyph::rotatePoint(const glm::vec2& point, float radianAngle) const
{
    glm::vec2 newPos;
    newPos.x = point.x * glm::cos(radianAngle) - point.y * glm::sin(radianAngle);
    newPos.y = point.x * glm::sin(radianAngle) + point.y * glm::cos(radianAngle);
    return newPos;
}
