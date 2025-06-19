#pragma once
#include"KL2_API.h"
#include<cstdint>

namespace KL2::Graphics::Primitives {

    namespace Renderer {
        enum class EPrimitives :uint8_t {
            Points, LineStrip, LineLoop, Lines, LineStripAdjacency, LinesAdjacency, TriangleStrip,
            TriangleFan, Triangles, TriangleStripAdjacency, TrianglesAdjacency, Patches
        };
        enum class EIndexBufferDataType :uint8_t {
            UnsignedByte, UnsignedShort, UnsignedInt
        };
        KL2_API void DrawArrays(EPrimitives primitive, unsigned int firstVertexInd, unsigned int amountOfVertexesToRender);
        KL2_API void DrawArraysInstanced(EPrimitives primitive, unsigned int firstVertexInd, unsigned int amountOfVertexesToRender, unsigned int instancesAmount);
        KL2_API void DrawElements(EPrimitives primitive, unsigned int firstIndexInd, unsigned int amountOfIndexesToRender, EIndexBufferDataType indexBufferDataType);
        KL2_API void DrawElementsInstanced(EPrimitives primitive, unsigned int firstIndexInd, unsigned int amountOfIndexesToRender, EIndexBufferDataType indexBufferDataType, unsigned int instancesAmount);

    }
}