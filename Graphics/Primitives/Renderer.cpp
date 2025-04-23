#include"Renderer.hpp"
#include"Tools/GLDebug.hpp"
#include"glad/glad.h"

using namespace KL2::Graphics::Primitives;

static unsigned int constexpr _GetPrimitive_SwitchCase(Renderer::EPrimitives primitive) noexcept {
    using namespace Renderer;
    switch (primitive) {
    case EPrimitives::Points: return GL_POINTS;
    case EPrimitives::LineStrip: return GL_LINE_STRIP;
    case EPrimitives::LineLoop: return GL_LINE_LOOP;
    case EPrimitives::Lines: return GL_LINES;
    case EPrimitives::LineStripAdjacency: return GL_LINE_STRIP_ADJACENCY;
    case EPrimitives::LinesAdjacency: return GL_LINES_ADJACENCY;
    case EPrimitives::TriangleStrip: return GL_TRIANGLE_STRIP;
    case EPrimitives::TriangleFan: return GL_TRIANGLE_FAN;
    case EPrimitives::Triangles: return GL_TRIANGLES;
    case EPrimitives::TriangleStripAdjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
    case EPrimitives::TrianglesAdjacency: return GL_TRIANGLES_ADJACENCY;
    case EPrimitives::Patches: return GL_PATCHES;
    } return 0;
}
static unsigned int constexpr _GetIndexBufferDataType_SwitchCase(Renderer::EIndexBufferDataType indexBufferDataType) noexcept {
    using namespace Renderer;
    switch (indexBufferDataType)
    {
    case EIndexBufferDataType::UnsignedByte: return GL_UNSIGNED_BYTE;
    case EIndexBufferDataType::UnsignedShort: return GL_UNSIGNED_SHORT;
    case EIndexBufferDataType::UnsignedInt: return GL_UNSIGNED_INT;
    default: return 0;
    }
}
void Renderer::DrawArrays(EPrimitives primitive, unsigned int startingInd, unsigned int amountOfVertexesToRender) {
    glSC(glDrawArrays(_GetPrimitive_SwitchCase(primitive), startingInd, amountOfVertexesToRender));
}
void Renderer::DrawArraysInstanced(EPrimitives primitive, unsigned int firstVertexInd, unsigned int amountOfVertexesToRender, unsigned int instancesAmount) {
    glSC(glDrawArraysInstanced(_GetPrimitive_SwitchCase(primitive), firstVertexInd, amountOfVertexesToRender, instancesAmount));
}
void Renderer::DrawElements(EPrimitives primitive, unsigned int firstIndexInd, unsigned int amountOfIndexesToRender, EIndexBufferDataType indexBufferDataType) {
    glSC(glDrawElements(_GetPrimitive_SwitchCase(primitive), amountOfIndexesToRender, _GetIndexBufferDataType_SwitchCase(indexBufferDataType), (const void*)(long long)firstIndexInd));
}
void Renderer::DrawElementsInstanced(EPrimitives primitive, unsigned int firstIndexInd, unsigned int amountOfIndexesToRender, EIndexBufferDataType indexBufferDataType, unsigned int instancesAmount) {
    glSC(glDrawElementsInstanced(_GetPrimitive_SwitchCase(primitive), amountOfIndexesToRender, _GetIndexBufferDataType_SwitchCase(indexBufferDataType), (const void*)(long long)firstIndexInd, instancesAmount));
}
