#pragma once
#include"KL2_API.h"
#include"Tools/ArrayView.hpp"
#include"Maths/Vector.hpp"

namespace KL2::Graphics::Primitives {
    //this thing can save and quickly apply settings for rendering, such as cull faces, depth test, blending, etc...
    struct SRenderingPreset {

        struct SColors {
            //if all colors disabled, then fragment shader wont be called
            Vector4B EnabledRGBA = Vector4B(true);
            Vector4F ClearRGBA = Vector4F(0.f);
            /*
            if you are new to OpenGL you may be asking yourself, how do i render to a window
            in default FrameBuffer(aka window FB), and you just use BackLeft/FrontLeft, they are
            like back and front buffers for your window, and BackRight/FrontRight can be used for vr
            */
            enum class EColorOutput :uint8_t {
                None, BackLeft, BackRight, FrontLeft, FrontRight,
                //if you want to use for example ColorAttachment1, just add 1 to ColorAttachment0, for 2 add 2 and etc
                ColorAttachment0,
            };
            std::vector<EColorOutput> ColorsOutputs{ 0u };

            KL2_API static void UpdEnabledRGBA(Vector4B colorEnabledRGBA);
            KL2_API static void UpdClearRGBA(Vector4F clearRGBA);
            KL2_API static void UpdColorsOutputs(const CArrayView<EColorOutput>& colorsOutputs);

            struct SFlags {
                static constexpr unsigned
                    EnabledRGBA = 1 << 0, ClearRGBA = 1 << 1, ColorsOutputs = 1 << 2,
                    All = EnabledRGBA | ClearRGBA | ColorsOutputs;
            } Flags;
        } Colors;

        struct SBlending {
            bool Enabled = false;
            Vector4F ConstRGBA = Vector4F(0.f);
            //for full explanation visit "https://docs.gl/gl4/glBlendFunc"
            enum class BlendingFuncValE :uint8_t {
                Zero, One, Src0Color, OneMinusSrc0Color, DstColor, OneMinusDstColor, Src0Alpha, OneMinusSrc0Alpha,
                DstAlpha, OneMinusDstAlpha, ConstColor, OneMinusConstColor, ConstAlpha, OneMinusConstAlpha,
                SrcAlphaSaturate, Src1Color, OneMinusSrc1Color, Src1Alpha, OneMinusSrc1Alpha
            } SrcVal = BlendingFuncValE::One, DstVal = BlendingFuncValE::Zero;

            KL2_API static void UpdEnabled(bool enabled);
            KL2_API static void UpdConstRGBA(Vector4F constRGBA);
            KL2_API static void UpdBlendingFunc(BlendingFuncValE srcVal, BlendingFuncValE dstVal);

            struct SFlags {
                static constexpr unsigned
                    Enabled = 1 << 3, ConstRGBA = 1 << 4, BlendingFunc = 1 << 5,
                    All = Enabled | ConstRGBA | BlendingFunc;
            } Flags;
        } Blending;

        struct SFaceCulling {
            bool Enabled = false;
            //describes which face to cull
            enum class FaceToCullE :uint8_t { Back, Front } FaceToCull = FaceToCullE::Back;
            /*describe how to determine fs face front or back,
            to determine that, gpu takes position of 3 vertexes of triangle from camera perspective
            and then IF they positioned clockwise and setting is set to clockwise then its front face,
            and IF they positioned anticlockwise then its back face, if settings is set to anticlockwise then everything is same but reversed*/
            enum class FaceDeterminationE :uint8_t { Clockwise, AntiClockwise, } FaceDetermination = FaceDeterminationE::AntiClockwise;

            KL2_API static void UpdEnabled(bool enabled);
            KL2_API static void UpdFaceToCull(FaceToCullE faceToCull);
            KL2_API static void UpdFaceDetermination(FaceDeterminationE faceDetermination);

            struct SFlags {
                static constexpr unsigned Enabled = 1 << 6, FaceToCull = 1 << 7, FaceDetermination = 1 << 8,
                    All = Enabled | FaceToCull | FaceDetermination;
            } Flags;
        } FaceCulling;

        struct SDepthTest {
            bool Enabled = false, WriteInBufferEnabled = true;
            //gpu will compare two values, current value and stored value, if test passes then pixel will be drawn, otherwise it will be discarded
            enum class TypeOfComparisonE :uint8_t {
                Never, Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual, AlwaysPass
            } TypeOfComparison = TypeOfComparisonE::Less;

            KL2_API static void UpdEnabled(bool enabled);
            KL2_API static void UpdWriteInBufferEnabled(bool writeInBufferEnabled);
            KL2_API static void UpdTypeOfComparison(TypeOfComparisonE typeOfComparison);

            struct SFlags {
                static constexpr unsigned Enabled = 1 << 9, WriteInBufferEnabled = 1 << 10, TypeOfComparison = 1 << 11,
                    All = Enabled | WriteInBufferEnabled | TypeOfComparison;
            } Flags;
        } DepthTest;

        struct SStencilTest {
            bool Enabled = false;
            unsigned BaseMask = ~0u;
            //gpu will compare two values, if comparison fails then pixel is discarded, otherwise it wont
            enum class ComparisonTypeE :uint8_t {
                Never, Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual, AlwaysPass
            } ComparisonType = ComparisonTypeE::AlwaysPass;
            unsigned ReferenceValue = 0u, Mask = ~0u;
            //actions to take with current stencil buffer value on different events that may occur, for move info visit "https://docs.gl/gl3/glStencilOp"
            enum class ActionsE :uint8_t {
                Keep, Zero, Replace, Increase, IncreaseWrap, Decrease, DecreaseWrap, Invert
            } ActionOnSF = ActionsE::Keep, ActionOnSPDF = ActionsE::Keep, ActionOnSPDP = ActionsE::Keep;

            KL2_API static void UpdEnabled(bool enabled);
            KL2_API static void UpdBaseMask(unsigned int baseMask);
            KL2_API static void UpdComparisonFunc(ComparisonTypeE comparisonType, unsigned int referenceValue, unsigned int mask);
            KL2_API static void UpdActionFunc(ActionsE actionOnSF, ActionsE actionOnSPDF, ActionsE actionOnSPDP);

            struct SFlags {
                static constexpr unsigned
                    Enabled = 1 << 12, BaseMask = 1 << 13, ComparisonFunc = 1 << 14, ActionFunc = 1 << 15,
                    All = Enabled | BaseMask | ComparisonFunc | ActionFunc;
            } Flags;
        } StencilTest;
        //TODO add ability to set stencil test for Front and Back faces, for example glStencilMaskSeparate

        //determines bitwise operations between fragment color and texture color before writing
        struct SLogicalOperation {
            bool Enabled = false;
            //S-source(fragment color),D-destination(texture color),I-inverse(~),A-and(&),O-or(|),X-xor(^)
            //works only if texture have some kind of integer format, for more info google it
            enum class OperE :uint8_t {
                Zero, One, S, IS, D, ID, SAD, I_SAD, SOD, I_SOD, SXD, I_SXD, SAID, ISAD, SOID, ISOD
            } Oper = OperE::S;

            KL2_API static void UpdEnabled(bool enabled);
            KL2_API static void UpdOper(OperE oper);

            struct SFlags { static constexpr unsigned Enabled = 1 << 16, Oper = 1 << 17, All = Enabled | Oper; } Flags;
        } LogicalOperation;

        //all pixels that are not inside region, will be discarded
        //it also works for clearing buffers and etc, so you can clear only portion of texture
        struct SScissorTest {
            bool Enabled = false;
            //default value of size should be size of window, but making this true is not all that easy, so its just 100 by 100 so you can notice this instantly
            Vector2U Offset = Vector2U(0u), Size = Vector2U{ 100u, 100u };

            KL2_API static void UpdEnabled(bool enabled);
            KL2_API static void UpdOffsetAndBase(Vector2U offset, Vector2U size);

            struct SFlags { static constexpr unsigned Enabled = 1 << 18, OffsetAndSize = 1 << 19, All = Enabled | OffsetAndSize; } Flags;
        } ScissorTest;

        void Update(unsigned flags = ~0u) {
            if (flags & SColors::SFlags::EnabledRGBA) SColors::UpdEnabledRGBA(Colors.EnabledRGBA);
            if (flags & SColors::SFlags::ClearRGBA) SColors::UpdClearRGBA(Colors.ClearRGBA);
            if (flags & SColors::SFlags::ColorsOutputs) SColors::UpdColorsOutputs(Colors.ColorsOutputs);

            if (flags & SBlending::SFlags::Enabled) SBlending::UpdEnabled(Blending.Enabled);
            if (flags & SBlending::SFlags::ConstRGBA) SBlending::UpdConstRGBA(Blending.ConstRGBA);
            if (flags & SBlending::SFlags::BlendingFunc) SBlending::UpdBlendingFunc(Blending.SrcVal, Blending.DstVal);

            if (flags & SFaceCulling::SFlags::Enabled) SFaceCulling::UpdEnabled(FaceCulling.Enabled);
            if (flags & SFaceCulling::SFlags::FaceToCull) SFaceCulling::UpdFaceToCull(FaceCulling.FaceToCull);
            if (flags & SFaceCulling::SFlags::FaceDetermination) SFaceCulling::UpdFaceDetermination(FaceCulling.FaceDetermination);

            if (flags & SDepthTest::SFlags::Enabled) SDepthTest::UpdEnabled(DepthTest.Enabled);
            if (flags & SDepthTest::SFlags::WriteInBufferEnabled) SDepthTest::UpdWriteInBufferEnabled(DepthTest.WriteInBufferEnabled);
            if (flags & SDepthTest::SFlags::TypeOfComparison) SDepthTest::UpdTypeOfComparison(DepthTest.TypeOfComparison);

            if (flags & SStencilTest::SFlags::Enabled) SStencilTest::UpdEnabled(StencilTest.Enabled);
            if (flags & SStencilTest::SFlags::BaseMask) SStencilTest::UpdBaseMask(StencilTest.BaseMask);
            if (flags & SStencilTest::SFlags::ComparisonFunc) SStencilTest::UpdComparisonFunc(StencilTest.ComparisonType, StencilTest.ReferenceValue, StencilTest.Mask);
            if (flags & SStencilTest::SFlags::ActionFunc) SStencilTest::UpdActionFunc(StencilTest.ActionOnSF, StencilTest.ActionOnSPDF, StencilTest.ActionOnSPDP);

            if (flags & SLogicalOperation::SFlags::Enabled) SLogicalOperation::UpdEnabled(LogicalOperation.Enabled);
            if (flags & SLogicalOperation::SFlags::Oper) SLogicalOperation::UpdOper(LogicalOperation.Oper);

            if (flags & SScissorTest::SFlags::Enabled) SScissorTest::UpdEnabled(ScissorTest.Enabled);
            if (flags & SScissorTest::SFlags::OffsetAndSize) SScissorTest::UpdOffsetAndBase(ScissorTest.Offset, ScissorTest.Size);
        }
    };
}