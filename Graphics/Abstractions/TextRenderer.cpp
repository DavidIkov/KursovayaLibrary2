
#include"TextRenderer.hpp"
#include"Graphics/Primitives/Renderer.hpp"
#include"Tools/GLDebug.hpp"
#include<algorithm>
#ifndef KL2_DONT_USE_FREE_TYPE
#include"FreeType/include/ft2build.h"
#include"FreeType/include/freetype/freetype.h"
#endif

using namespace KL2;
namespace GP = Graphics::Primitives;
namespace GA = Graphics::Abstractions;

void* GA::CTextRenderer::FreeTypeLib = nullptr;
unsigned short GA::CTextRenderer::AmountOfInstances = 0;

#ifndef KL2_DONT_USE_FREE_TYPE
GA::CTextRenderer::CTextRenderer(const char* vertexShaderDir, const char* fragmentShaderDir) :TextPreset{
            GP::SRenderingPreset::SColors{ Vector4B(true),Vector4F{0.f,0.f,0.f,1.f},{ GP::SRenderingPreset::SColors::EColorOutput::ColorAttachment0 } },
            GP::SRenderingPreset::SBlending{true,Vector4F(0.f),GP::SRenderingPreset::SBlending::BlendingFuncValE::Src0Alpha,
            GP::SRenderingPreset::SBlending::BlendingFuncValE::OneMinusSrc0Alpha} } {

    AmountOfInstances++;

    if (FreeTypeLib == nullptr) {
        FreeTypeLib = (void*)(new FT_Library);

        int FT_InitErrorCode = FT_Init_FreeType((FT_Library*)FreeTypeLib);
        if (FT_InitErrorCode != 0)
            ErrorsSystem::SendError << "Failed to initialize FreeType library, FreeType returned error code :[" << std::to_string(FT_InitErrorCode) << "]" >> SErrorsEnumWrapper(ErrorsEnum::FailedToInitializeFreeType);
    }

    {
        GP::CShader VS(vertexShaderDir, GP::CShader::ETypes::Vertex);
        VS.Compile();
        TEXT_SP.AttachShader(VS.gID());

        GP::CShader FS(fragmentShaderDir, GP::CShader::ETypes::Fragment);
        FS.Compile();
        TEXT_SP.AttachShader(FS.gID());

        TEXT_SP.LinkShaders();

        TEXT_SP.Bind();
        TEXT_SP.SetUniform1i(TEXT_SP.GetUniformIDByName("u_TextTexture"), 0);
    }

    TEXT_VA.Bind();
    TEXT_VB.Bind();
    TEXT_VA.SetAttributes(CArrayView<GP::CVertexArray::SAttributeData>({
        GP::CVertexArray::SAttributeData{TEXT_VB,false,0,2,0,sizeof(float) * (2 + 2),GP::CVertexArray::SAttributeData::EDataTypeInMemory::Float,GP::CVertexArray::SAttributeData::EDataTypeForReadingOnGPU::Float},
        GP::CVertexArray::SAttributeData{TEXT_VB,false,0,2,sizeof(float) * 2,sizeof(float) * (2 + 2),GP::CVertexArray::SAttributeData::EDataTypeInMemory::Float,GP::CVertexArray::SAttributeData::EDataTypeForReadingOnGPU::Float},
        }));
}

GA::CTextRenderer::~CTextRenderer() {

    AmountOfInstances--;

    Fonts.clear();

    if (AmountOfInstances == 0) {
        FT_Done_FreeType(*(FT_Library*)FreeTypeLib);
        delete (FT_Library*)FreeTypeLib;
        FreeTypeLib = nullptr;
    }
}

const GA::CTextRenderer::SFont& GA::CTextRenderer::AddFont(unsigned int characterSize, const char* fontDir, const char32_t* characters) {
    const SFont& insertedFont = Fonts.emplace_back(SFont::GuardFromUser(), characterSize, fontDir, characters);
    return insertedFont;
}

GA::CTextRenderer::SFont::~SFont() {
    if (FreeTypeFace != nullptr) {
        FT_Done_Face(*(FT_Face*)FreeTypeFace);
        FreeTypeFace = nullptr;
    }
}

static constexpr GP::CTexture::SSettings TextureSettings{
                    Vector<3,GP::CTexture::SSettings::EWrapType>{
                        GP::CTexture::SSettings::EWrapType::ClampToEdge,GP::CTexture::SSettings::EWrapType::ClampToEdge,
                        GP::CTexture::SSettings::EWrapType::ClampToEdge},
                    GP::CTexture::SSettings::EDownscalingFilterFunc::Linear,GP::CTexture::SSettings::EUpscalingFilterFunc::Linear,
                    GP::CTexture::SSettings::EDepthStencilReadMode::Depth };
static constexpr GP::CTexture::SDataSettings TextureDataSettings{ GP::CTexture::SDataSettings::EDataFormatOnGPU::R8UN,
                    GP::CTexture::SDataSettings::EDataFormatOnCPU::R,GP::CTexture::SDataSettings::EDataTypeOnCPU::UnsignedByte };
GA::CTextRenderer::SFont::SFont(GuardFromUser, unsigned int characterSize, const char* fontDir, const char32_t* chars) :
    Texture(GP::CTexture::EDimensions::Two, Vector3U{0u, 0u, 0u}, nullptr, 0, TextureSettings, TextureDataSettings)
{ 
    FreeTypeFace = new FT_Face;

    {
        int FT_FontReadingError = FT_New_Face(*(FT_Library*)CTextRenderer::FreeTypeLib, fontDir, 0, (FT_Face*)FreeTypeFace);
        if (FT_FontReadingError != 0)
            ErrorsSystem::SendError << "FreeType failed to read font file and returned error code: ["
            << std::to_string(FT_FontReadingError) << "]" >> SErrorsEnumWrapper(ErrorsEnum::FreeTypeError_FailedToReadFontFile);
    }

    FT_Set_Pixel_Sizes(*(FT_Face*)FreeTypeFace, 0, characterSize);


    unsigned int charsAmount = 0; while (chars[charsAmount] != L'\0') charsAmount++;

    Characters.reserve(charsAmount);
    std::vector<std::vector<unsigned char>> buffers; buffers.reserve(charsAmount);
    std::vector<unsigned int> nums; nums.reserve(charsAmount);


    unsigned int maxWidth = 0; unsigned int maxHeight = 0;
    unsigned int totalXSize = 0;

    for (unsigned int chi = 0; chi < charsAmount; chi++) {

        unsigned int unicodeInd = (unsigned int)chars[chi];

        auto insertIter = (Characters.size() == 0) ? Characters.begin() :
            std::upper_bound(Characters.begin(), Characters.end(), unicodeInd,
                [](unsigned unicode, decltype(Characters)::value_type const& val)->bool {return unicode >= val.UnicodeInd;});

        if (insertIter != Characters.end() && insertIter->UnicodeInd == unicodeInd) continue;

        {
            int FT_CharLoadingError = FT_Load_Char(*(FT_Face*)FreeTypeFace, unicodeInd, FT_LOAD_RENDER);
            if (FT_CharLoadingError != 0) {
                ErrorsSystem::SendError << "FreeType failed to load charater with unicode index of: [" << std::to_string(unicodeInd) <<
                    "] and returned error code: [" << std::to_string(FT_CharLoadingError) << "]" >> SErrorsEnumWrapper(ErrorsEnum::FreeTypeError_FailedToLoadChar);
                continue;
            }
        }

        FT_Face& face = *(FT_Face*)FreeTypeFace;

        buffers.emplace(buffers.begin() + std::distance(Characters.begin(), insertIter), std::vector<unsigned char>(face->glyph->bitmap.buffer, 
            face->glyph->bitmap.buffer + face->glyph->bitmap.width * face->glyph->bitmap.rows));

        if (face->glyph->bitmap.width > maxWidth) maxWidth = face->glyph->bitmap.width;
        if (face->glyph->bitmap.rows > maxHeight) maxHeight = face->glyph->bitmap.rows;

        if (face->glyph->bitmap_top > MaxCharacterUp) MaxCharacterUp = face->glyph->bitmap_top;
        if ((int)face->glyph->bitmap.rows - face->glyph->bitmap_top > MaxCharacterDown) MaxCharacterDown = (int)face->glyph->bitmap.rows - face->glyph->bitmap_top;

        totalXSize += face->glyph->bitmap.width;

        Characters.emplace(Characters.begin() + std::distance(Characters.begin(), insertIter), SCharacter{
            unicodeInd,
            Vector2U{face->glyph->bitmap.width, face->glyph->bitmap.rows},
            Vector2I{face->glyph->bitmap_left, face->glyph->bitmap_top},
            (unsigned int)face->glyph->advance.x / 64,
            Vector2F{0.f,0.f},0
            });
    }

    totalXSize += charsAmount + 1;

    std::vector<unsigned char> textureBuffer(totalXSize * maxHeight, (unsigned char)0);
    
    {
        unsigned int xOffset = 1;
        for (unsigned int ci = 0; ci < Characters.size(); ci++) {
            SCharacter& char_ = Characters[ci];
            char_.XOffsetInTexture = xOffset / (float)totalXSize;
            char_.SizeInTexture = Vector2F{char_.Size[0] / (float)totalXSize, char_.Size[1] / (float)maxHeight};
            xOffset += char_.Size[0] + 1;
        }
    }

    {
        unsigned int xOffset = 0;
        for (unsigned int ci = 0; ci < Characters.size(); ci++) {
            SCharacter& char_ = Characters[ci];
            for (unsigned int x = 0; x < char_.Size[0]; x++) for (unsigned int y = 0; y < char_.Size[1]; y++) {
                textureBuffer[ci + 1 + y * totalXSize + xOffset + x] = buffers[ci][(char_.Size[1] - y - 1) * char_.Size[0] + x];
            }

            xOffset += char_.Size[0];
        }
    }

    glSC(glPixelStorei, GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    Texture = GP::CTexture(GP::CTexture::EDimensions::Two, Vector3U{totalXSize, maxHeight, 0u}, &*textureBuffer.begin(), 0, TextureSettings, TextureDataSettings);

    glSC(glPixelStorei, GL_UNPACK_ALIGNMENT, 4); // enable byte-alignment back

}

static constexpr unsigned int MAX_TEXT_LEN = 100000;

void GA::CTextRenderer::RenderText(const SFont& font, const wchar_t* text, Vector2F pos, Vector2F localOffset, Vector2U pixelsInTexture,
    Vector2F boxSize, float lineSizeInBox, const wchar_t* dividingSymbols) {
    
    if (boxSize[0] == 0 && boxSize[1] == 0) return;//theres nothing we can do

    TextPreset.Update();
    TEXT_SP.Bind();
    TEXT_VA.Bind();
    TEXT_VB.Bind();

    font.Texture.Bind(0);

    unsigned int textLen = 0;
    while (textLen != MAX_TEXT_LEN && text[textLen] != L'\0') textLen++;
    if (textLen == MAX_TEXT_LEN) ErrorsSystem::SendError << "Provided text is too big or end symbol was not found, current limit on symbols is: [" <<
        std::to_string(MAX_TEXT_LEN) << "]" >> SErrorsEnumWrapper(ErrorsEnum::TextTooBigOrNoEndFound);

    std::vector<size_t> charsInds; charsInds.resize(textLen, 0);//adding +1 to mark unexisting characters, ind=0 will mean that character not found
    for (unsigned int ci = 0; ci < textLen; ci++) {
        charsInds[ci] = std::distance(font.Characters.begin(), std::upper_bound(font.Characters.begin(), font.Characters.end(), (unsigned int)text[ci],
            [](unsigned unicode, decltype(font.Characters)::value_type const& val)->bool {return unicode >= val.UnicodeInd; }));
        if (charsInds[ci] >= font.Characters.size() || font.Characters[charsInds[ci]].UnicodeInd != (unsigned int)text[ci]) charsInds[ci] = 0;
        else charsInds[ci]++;
    }


    unsigned int fullLocalXSize = 0;
    int maxUp = 0; int maxDown = 0;
    unsigned int realTextLen = 0;
    for (unsigned int ci = 0; ci < textLen; ci++) {
        if (charsInds[ci] == 0) continue; realTextLen++;
        const SFont::SCharacter& char_ = font.Characters[charsInds[ci] - 1];
        fullLocalXSize += char_.Advance;
        int up = char_.Bearing[1]; int down = char_.Size[1] - up;
        if (maxUp < up) maxUp = up; if (maxDown < down) maxDown = down;
    }

    unsigned int fullLocalYSize = font.MaxCharacterUp + font.MaxCharacterDown;



    float localPixelsToTexPixels = 0;
    {
        float byX = (pixelsInTexture[0] * boxSize[0]) / fullLocalXSize;
        float byY = (pixelsInTexture[1] * boxSize[1] * lineSizeInBox) / fullLocalYSize;
        if (boxSize[0] != 0 && boxSize[1] == 0) localPixelsToTexPixels = byX;
        else if (boxSize[0] == 0 && boxSize[1] != 0) localPixelsToTexPixels = byY;
        else if (boxSize[0] != 0 && boxSize[1] != 0) localPixelsToTexPixels = (byX > byY) ? byY : byX;
    }

    float localPixelsToTexScaleByX = localPixelsToTexPixels / pixelsInTexture[0] * 2;
    float localPixelsToTexScaleByY = localPixelsToTexPixels / pixelsInTexture[1] * 2;

    Vector2F realTextBoxSize{fullLocalXSize * localPixelsToTexScaleByX, fullLocalYSize * localPixelsToTexScaleByY};

    Vector2F scaledLocalOffset = ((localOffset + 1) / 2) * -realTextBoxSize;

    float xOffset = 0;
    std::vector<float> VertexBufferData; VertexBufferData.resize(6 * (2 + 2) * realTextLen, 0.f); 
    for (unsigned int ci = 0, real_ci = 0; ci < textLen; ci++) {
        if (charsInds[ci] == 0) continue;

        const SFont::SCharacter& char_ = font.Characters[charsInds[ci] - 1];
        
        Vector2F lbp{ pos[0] + xOffset + char_.Bearing[0] * localPixelsToTexScaleByX,
            pos[1] + (font.MaxCharacterDown - ((int)char_.Size[1] - char_.Bearing[1])) * localPixelsToTexScaleByY };
        Vector2F rbp{lbp[0] + char_.Size[0] * localPixelsToTexScaleByX, lbp[1]};
        Vector2F ltp{lbp[0], lbp[1] + char_.Size[1] * localPixelsToTexScaleByY};
        Vector2F rtp{rbp[0], ltp[1]};

        Vector2F lbc{char_.XOffsetInTexture, 0.f};
        Vector2F rbc{lbc[0] + char_.SizeInTexture[0], 0.f};
        Vector2F ltc{lbc[0], char_.SizeInTexture[1]};
        Vector2F rtc{rbc[0], ltc[1]};


        lbp[0] += scaledLocalOffset[0];
        lbp[1] += scaledLocalOffset[1];
        rbp[0] += scaledLocalOffset[0];
        rbp[1] += scaledLocalOffset[1];
        ltp[0] += scaledLocalOffset[0];
        ltp[1] += scaledLocalOffset[1];
        rtp[0] += scaledLocalOffset[0];
        rtp[1] += scaledLocalOffset[1];


        float data[4 * 6] = {
            lbp[0],lbp[1],lbc[0],lbc[1],
            rbp[0],rbp[1],rbc[0],rbc[1],
            rtp[0],rtp[1],rtc[0],rtc[1],
            
            rtp[0],rtp[1],rtc[0],rtc[1],
            ltp[0],ltp[1],ltc[0],ltc[1],
            lbp[0],lbp[1],lbc[0],lbc[1],
        };

        for (unsigned int i = 0; i < 6 * 4; i++) VertexBufferData[real_ci * 4 * 6 + i] = data[i];

        real_ci++;

        xOffset += char_.Advance * localPixelsToTexScaleByX;

    }

    TEXT_VB.SetData(CArrayView<float>(VertexBufferData), GP::CVertexBuffer::EBufferReadWriteMode::StreamDraw);
    GP::Renderer::DrawArrays(GP::Renderer::EPrimitives::Triangles, 0, 6 * realTextLen);


    TEXT_VA.Unbind();
}
#endif