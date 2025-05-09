#include"RenderingPreset.hpp"
#include"glad/glad.h"
#include"Tools/GLDebug.hpp"

using namespace KL2;
using namespace Graphics::Primitives;


void SRenderingPreset::SColors::UpdEnabledRGBA(Vector4B colorEnabledRGBA) {
    glSC(glColorMask, colorEnabledRGBA[0], colorEnabledRGBA[1], colorEnabledRGBA[2], colorEnabledRGBA[3]);
}
void SRenderingPreset::SColors::UpdClearRGBA(Vector4F clearRGBA) {
    glSC(glClearColor, clearRGBA[0], clearRGBA[1], clearRGBA[2], clearRGBA[3]);
}
void SRenderingPreset::SColors::UpdColorsOutputs(const CArrayView<EColorOutput>& colorsOutputs) {
    std::vector<unsigned> buffs; buffs.reserve(colorsOutputs.GetLen());
    for (size_t i = 0;i < colorsOutputs.GetLen();i++) {
        switch (colorsOutputs[i]) {
        case EColorOutput::None: buffs.emplace_back(GL_NONE); break;
        case EColorOutput::BackLeft: buffs.emplace_back(GL_BACK_LEFT); break;
        case EColorOutput::BackRight: buffs.emplace_back(GL_BACK_RIGHT); break;
        case EColorOutput::FrontLeft: buffs.emplace_back(GL_FRONT_LEFT); break;
        case EColorOutput::FrontRight: buffs.emplace_back(GL_FRONT_RIGHT); break;
        default: {
            int ind = (int)colorsOutputs[i] - (int)EColorOutput::ColorAttachment0;
            if (ind >= 0 && ind <= GL_COLOR_ATTACHMENT31 - GL_COLOR_ATTACHMENT0) buffs.emplace_back(GL_COLOR_ATTACHMENT0 + ind);
            else buffs.emplace_back(GL_NONE);
        }
        }
    }
    glSC(glDrawBuffers, buffs.size(), buffs.data());
}
void SRenderingPreset::SBlending::UpdEnabled(bool enabled) {
    if (enabled) { glSC(glEnable, GL_BLEND); }
    else { glSC(glDisable, GL_BLEND); }
}
void SRenderingPreset::SBlending::UpdConstRGBA(Vector4F constRGBA) {
    glSC(glBlendColor, constRGBA[0], constRGBA[1], constRGBA[2], constRGBA[3]);
}
void SRenderingPreset::SBlending::UpdBlendingFunc(BlendingFuncValE srcVal, BlendingFuncValE dstVal) {
    unsigned args[2] = { 0u,0u };
    BlendingFuncValE checkVal[2] = { srcVal,dstVal };
    for (unsigned int i = 0; i < 2; i++) {
        switch (checkVal[i]) {
        case BlendingFuncValE::Zero: args[i] = GL_ZERO; break;
        case BlendingFuncValE::One: args[i] = GL_ONE; break;
        case BlendingFuncValE::Src0Color: args[i] = GL_SRC_COLOR; break;
        case BlendingFuncValE::OneMinusSrc0Color: args[i] = GL_ONE_MINUS_SRC_COLOR; break;
        case BlendingFuncValE::DstColor: args[i] = GL_DST_COLOR; break;
        case BlendingFuncValE::OneMinusDstColor: args[i] = GL_ONE_MINUS_DST_COLOR; break;
        case BlendingFuncValE::Src0Alpha: args[i] = GL_SRC_ALPHA; break;
        case BlendingFuncValE::OneMinusSrc0Alpha: args[i] = GL_ONE_MINUS_SRC_ALPHA; break;
        case BlendingFuncValE::DstAlpha: args[i] = GL_DST_ALPHA; break;
        case BlendingFuncValE::OneMinusDstAlpha: args[i] = GL_ONE_MINUS_DST_ALPHA; break;
        case BlendingFuncValE::ConstColor: args[i] = GL_CONSTANT_COLOR; break;
        case BlendingFuncValE::OneMinusConstColor: args[i] = GL_ONE_MINUS_CONSTANT_COLOR; break;
        case BlendingFuncValE::ConstAlpha: args[i] = GL_CONSTANT_ALPHA; break;
        case BlendingFuncValE::OneMinusConstAlpha: args[i] = GL_ONE_MINUS_CONSTANT_ALPHA; break;
        case BlendingFuncValE::SrcAlphaSaturate: args[i] = GL_SRC_ALPHA_SATURATE; break;
        case BlendingFuncValE::Src1Color: args[i] = GL_SRC1_COLOR; break;
        case BlendingFuncValE::OneMinusSrc1Color: args[i] = GL_ONE_MINUS_SRC1_COLOR; break;
        case BlendingFuncValE::Src1Alpha: args[i] = GL_SRC1_ALPHA; break;
        case BlendingFuncValE::OneMinusSrc1Alpha: args[i] = GL_ONE_MINUS_SRC1_ALPHA; break;
        }
    }
    glSC(glBlendFunc, args[0], args[1]);
}

void SRenderingPreset::SFaceCulling::UpdEnabled(bool enabled) {
    if (enabled) { glSC(glEnable, GL_CULL_FACE); }
    else { glSC(glDisable, GL_CULL_FACE); }
}
void SRenderingPreset::SFaceCulling::UpdFaceToCull(FaceToCullE faceToCull) {
    switch (faceToCull) {
    case FaceToCullE::Back: glSC(glCullFace, GL_BACK); break;
    case FaceToCullE::Front: glSC(glCullFace, GL_FRONT); break;
    }
}
void SRenderingPreset::SFaceCulling::UpdFaceDetermination(FaceDeterminationE faceDetermination) {
    switch (faceDetermination) {
    case FaceDeterminationE::Clockwise: glSC(glFrontFace, GL_CW); break;
    case FaceDeterminationE::AntiClockwise:  glSC(glFrontFace, GL_CCW); break;
    }
}

void SRenderingPreset::SDepthTest::UpdEnabled(bool enabled) {
    if (enabled) { glSC(glEnable, GL_DEPTH_TEST); }
    else { glSC(glDisable, GL_DEPTH_TEST); }
}
void SRenderingPreset::SDepthTest::UpdWriteInBufferEnabled(bool writeInBufferEnabled) {
    if (writeInBufferEnabled) { glSC(glDepthMask, GL_TRUE); }
    else { glSC(glDepthMask, GL_FALSE); }
}
void SRenderingPreset::SDepthTest::UpdTypeOfComparison(TypeOfComparisonE typeOfComparison) {
    switch (typeOfComparison) {
    case TypeOfComparisonE::Never: glSC(glDepthFunc, GL_NEVER); break;
    case TypeOfComparisonE::Less: glSC(glDepthFunc, GL_LESS); break;
    case TypeOfComparisonE::LessOrEqual: glSC(glDepthFunc, GL_LEQUAL); break;
    case TypeOfComparisonE::Greater: glSC(glDepthFunc, GL_GREATER); break;
    case TypeOfComparisonE::GreaterOrEqual: glSC(glDepthFunc, GL_GEQUAL); break;
    case TypeOfComparisonE::Equal: glSC(glDepthFunc, GL_EQUAL); break;
    case TypeOfComparisonE::NotEqual: glSC(glDepthFunc, GL_NOTEQUAL); break;
    case TypeOfComparisonE::AlwaysPass: glSC(glDepthFunc, GL_ALWAYS); break;
    }
}

void SRenderingPreset::SStencilTest::UpdEnabled(bool enabled) {
    if (enabled) { glSC(glEnable, GL_STENCIL_TEST); }
    else { glSC(glDisable, GL_STENCIL_TEST); }
}
void SRenderingPreset::SStencilTest::UpdBaseMask(unsigned int baseMask) {
    glSC(glStencilMask, baseMask);
}
void SRenderingPreset::SStencilTest::UpdComparisonFunc(ComparisonTypeE comparisonType, unsigned int referenceValue, unsigned int mask) {
    unsigned func = 0u;
    switch (comparisonType) {
    case ComparisonTypeE::Never: func = GL_NEVER; break;
    case ComparisonTypeE::Less: func = GL_LESS; break;
    case ComparisonTypeE::LessOrEqual: func = GL_LEQUAL; break;
    case ComparisonTypeE::Greater: func = GL_GREATER; break;
    case ComparisonTypeE::GreaterOrEqual: func = GL_GEQUAL; break;
    case ComparisonTypeE::Equal: func = GL_EQUAL; break;
    case ComparisonTypeE::NotEqual: func = GL_NOTEQUAL; break;
    case ComparisonTypeE::AlwaysPass: func = GL_ALWAYS; break;
    }
    glSC(glStencilFunc, func, referenceValue, mask);
}
void SRenderingPreset::SStencilTest::UpdActionFunc(ActionsE actionOnSF, ActionsE actionOnSPDF, ActionsE actionOnSPDP) {
    int args[3] = { 0,0,0 };
    ActionsE checkVal[3] = { actionOnSF,actionOnSPDF,actionOnSPDP };
    for (unsigned i = 0; i < 3; i++) {
        switch (checkVal[i]) {
        case ActionsE::Keep: args[i] = GL_KEEP; break;
        case ActionsE::Zero: args[i] = GL_ZERO; break;
        case ActionsE::Replace: args[i] = GL_REPLACE; break;
        case ActionsE::Increase: args[i] = GL_INCR; break;
        case ActionsE::IncreaseWrap: args[i] = GL_INCR_WRAP; break;
        case ActionsE::Decrease: args[i] = GL_DECR; break;
        case ActionsE::DecreaseWrap: args[i] = GL_DECR_WRAP; break;
        case ActionsE::Invert: args[i] = GL_INVERT; break;
        }
    } glSC(glStencilOp, args[0], args[1], args[2]);
}
void SRenderingPreset::SLogicalOperation::UpdEnabled(bool enabled) {
    if (enabled) { glSC(glEnable, GL_COLOR_LOGIC_OP); }
    else { glSC(glDisable, GL_COLOR_LOGIC_OP); }
}
void SRenderingPreset::SLogicalOperation::UpdOper(OperE oper) {
    switch (oper) {
    case OperE::Zero: glSC(glLogicOp, GL_CLEAR); break;
    case OperE::One: glSC(glLogicOp, GL_SET); break;
    case OperE::S: glSC(glLogicOp, GL_COPY); break;
    case OperE::IS: glSC(glLogicOp, GL_COPY_INVERTED); break;
    case OperE::D: glSC(glLogicOp, GL_NOOP); break;
    case OperE::ID: glSC(glLogicOp, GL_INVERT); break;
    case OperE::SAD: glSC(glLogicOp, GL_AND); break;
    case OperE::I_SAD: glSC(glLogicOp, GL_NAND); break;
    case OperE::SOD: glSC(glLogicOp, GL_OR); break;
    case OperE::I_SOD: glSC(glLogicOp, GL_NOR); break;
    case OperE::SXD: glSC(glLogicOp, GL_XOR); break;
    case OperE::I_SXD: glSC(glLogicOp, GL_EQUIV); break;
    case OperE::SAID: glSC(glLogicOp, GL_AND_REVERSE); break;
    case OperE::ISAD: glSC(glLogicOp, GL_AND_INVERTED); break;
    case OperE::SOID: glSC(glLogicOp, GL_OR_REVERSE); break;
    case OperE::ISOD: glSC(glLogicOp, GL_OR_INVERTED); break;
    }
}
void SRenderingPreset::SScissorTest::UpdEnabled(bool enabled) {
    if (enabled) { glSC(glEnable, GL_SCISSOR_TEST); }
    else { glSC(glDisable, GL_SCISSOR_TEST); }
}
void SRenderingPreset::SScissorTest::UpdOffsetAndBase(Vector2U offset, Vector2U size) {
    glSC(glScissor, offset[0], offset[1], size[0], size[1]);
}

