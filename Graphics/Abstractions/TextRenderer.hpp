#pragma once
#include"KL2_API.h"
#include"Maths/Vector.hpp"
#include"Graphics/Primitives/ShaderProgram.hpp"
#include"Graphics/Primitives/RenderingPreset.hpp"
#include"Graphics/Primitives/VertexArray.hpp"
#include"Graphics/Primitives/VertexBuffer.hpp"
#include"Graphics/Primitives/Texture.hpp"
#include<list>

namespace KL2::Graphics::Abstractions {
    class CTextRenderer {
    public:
        struct SFont {
            struct SCharacter {
                unsigned int UnicodeInd;
                Vector2U Size;       // Size of glyph
                Vector2I Bearing;    // Offset from baseline to left/top of glyph
                unsigned int Advance; // Offset to advance to next glyph
                Vector2F SizeInTexture;
                float XOffsetInTexture;
            };
            Graphics::Primitives::CTexture Texture;
            void* FreeTypeFace;//FT_Face
            std::vector<SCharacter> Characters;
            int MaxCharacterUp = 0; int MaxCharacterDown = 0;

            friend CTextRenderer;
            //made so user wont be able to create font class by himself, only CTextRenderercan do it
            struct GuardFromUser {};

        public:
            SFont(GuardFromUser, unsigned int characterSize, const char* fontDir, const char32_t* chars);
            SFont(const SFont&) = delete;
            SFont(const SFont&&) = delete;
            SFont& operator=(const SFont&) = delete;
            SFont& operator=(SFont&&) = delete;
            ~SFont();
        };
    private:

        std::list<SFont> Fonts;

        Graphics::Primitives::CShaderProgram TEXT_SP;
        Graphics::Primitives::SRenderingPreset TextPreset;
        Graphics::Primitives::CVertexArray TEXT_VA;
        Graphics::Primitives::CVertexBuffer TEXT_VB;

        static void* FreeTypeLib;//FT_Library, nullptr
        static unsigned short AmountOfInstances;// 0

    public:

        struct SErrorsEnumWrapper :KL2::ErrorsSystem::SErrorBase {
            enum EErrors :uint8_t {
                FailedToInitializeFreeType,
                TextTooBigOrNoEndFound,
                FreeTypeError_FailedToLoadChar,
                FreeTypeError_FailedToReadFontFile,
            } Error;
            SErrorsEnumWrapper(EErrors error) :Error(error) {};
        }; using ErrorsEnum = SErrorsEnumWrapper; using AnyError = SErrorsEnumWrapper;

        KL2_API CTextRenderer(const char* vertexShaderDir, const char* fragmentShaderDir);
        KL2_API virtual ~CTextRenderer();
        KL2_API const SFont& AddFont(unsigned int characterSize, const char* fontDir, const char32_t* characters);

        //pixelsInTexture is amount of pixels in the texture you are rendering this text to
        //TODO add functionality for "dividingSymbols" and make so text will be separated in a few lines if possible
        KL2_API void RenderText(const SFont& font, const wchar_t* text, Vector2F pos, Vector2F localOffset, Vector2U pixelsInTexture,
            Vector2F boxSize, float lineSizeInBox, const wchar_t* dividingSymbols = nullptr);


    };
}

