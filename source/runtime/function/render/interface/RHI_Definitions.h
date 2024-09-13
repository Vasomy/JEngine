#ifndef RHI_DEFINITIONS
#define RHI_DEFINITIONS

enum class RHIInterfaceType
{
    Unknow,
    OpenGL,
    DirectX12
};

enum class RHIClearDepthStencil
{
    None,
    Depth,
    Stencil,
    DepthStencil
};

enum class ColorWriteMask
{
    CWM_Red = 1<<0,
    CWM_Green = 1<<1,
    CWM_Blue= 1<<2,
    CWM_Alpha = 1<<3,

    CWM_NONE = 0,
    CWM_RGB = CWM_Red|CWM_Green|CWM_Blue,
    CWM_RGBA = CWM_Red|CWM_Green|CWM_Blue|CWM_Alpha,

};

enum StencilOP
{
    SO_Keep = 1,
    SO_Zero = 2,
    SO_Replace = 3,
    SO_Invert = 4,
    SO_SaturatedIncrement = 5,
    SO_SaturatedDecrement = 6,
    SO_Increment = 7,
    SO_Decrement = 8,
};

enum class BlendOP
{
    BO_Add,
    BO_Subtract,
    BO_Min,
    BO_Max,
};

enum RHIResourceType
{
    RT_SamplerState,
    RT_GraphicsPipeLineState,
    RT_RootSinature,

    RT_Buffer,
    RT_ShaderUniformBuffer,
    RT_RenderTarget,

    RT_Texture2D,

    
    RT_Shader,
    RT_VertexShader,
    RT_PixelShader,
};

enum class ShaderUniformType
{
    SUT_ConstantsBuffer,
    SUT_Texture,
    SUT_PassData,
};

enum ShaderUsage: uint64
{
    SU_Unknow =0,
    SU_Vertex = 1<<0,
    SU_Pixel = 1<<1,// fragment
};

enum ColorFormat
{
    CF_UnKnow,
    CF_R32G32B32A32,
    CF_R8G8B8A8,
    CF_R32G32B32,
    CF_R8G8B8,
};
enum Format
{
    F_Unknow,
    F_R32G32B32A32_Typeless,
    F_R32G32B32A32_Float,
    F_R32G32B32A32_Uint,
    F_R32G32B32A32_Sint,
    F_R32G32B32_Typeless,
    F_R32G32B32_Float,
    F_R32G32B32_Uint,
    F_R32G32B32_Sint,
    F_R16G16B16A16_Typeless,
    F_R16G16B16A16_Float,
    F_R16G16B16A16_UNorm,
    F_R16G16B16A16_Uint,
    F_R16G16B16A16_SNorm,
    F_R16G16B16A16_Sint,
    F_R32G32_Typeless = 15,
    F_R32G32_Float = 16,
    F_R32G32_Uint = 17,
    F_R32G32_Sint = 18,


    F_R8G8B8A8_Typeless = 27,
    F_R8G8B8A8_Unorm = 28,
    F_R8G8B8A8_Unorm_SRGB = 29,
    F_R8G8B8A8_Uint = 30,
    F_R8G8B8A8_Snorm = 31,
    F_R8G8B8A8_Sint = 32,

    F_D24_Unorm_S8_Uint = 45,

};

enum RHISamplerFilter : uint8
{
    SF_Point,
    SF_Linear,
    SF_Anisotropic
};
enum class RHISamplerAddress : uint8
{
    SA_Clamp,
    SA_Wrap
};
enum class RHIBufferUsage : uint32
{
    None = 0,

    Static = 1<<0,//write once

    Dynamic = 1<<1,// GPU read only,CPU write only,



    VertexBuffer = 1<<14,
    IndexBuffer = 1<<15,
    UniformBuffer = 1<<16,

};
enum class TextureCreateFlags : uint64
{
    None = 0,

    RenderTargetable = 1ull<<0,
    ResolveTargetable = 1ull<<1,
    DepthStencilTargetable = 1ull<<2,

    //texture can be used as a shader resource
    ShaderResource = 1ull<<16,
};

enum class ClearBinding
{
    CB_Color,
    CB_DepthStencil
};
enum FillMode : uint8
{
    FM_Wire = 2,
    FM_Soild = 3,
    FM_Default = FM_Soild,
};
enum class CullMode : uint8
{
    CM_None = 1,
    CM_FrontCut =2,
    CM_BackCut = 3,
    CM_Default = CM_None,
};
enum Blend // blend вђзг
{
    B_Unknow = 0,
    B_Zero,
    B_One,
    B_SrcColor,
    B_SrcInvColor,
    B_SrcAlpha,
    B_SrcInvAlpha,
    B_DestAlpha,
    B_DestInvAlpha,
    B_DestColor,
    B_DestInvColor,
    ///...........
};
enum class BlendOp
{
    BO_Add=1,
    BO_Subtract=2,
    BO_Rev_Subtract=3,
    BO_Min=4,
    BO_Max=5
};

enum ComparisonFunc
{
    COMP_Never =1,
    COMP_Less = 2,
    COMP_Equal = 3,
    COMP_LessEqual = 4,
    COMP_Greater = 5,
    COMP_NotEqual = 6,
    COMP_GreaterEqual = 7,
    COMP_Always = 8
};
enum class DepthMask
{
    DM_Zero =0 ,
    DM_All = 1
};
enum WriteColorEnable
{
    WCE_Red = 1,
    WCE_Green = 2,
    WCE_Blue = 4,
    WCE_Alpha = 8,
    WCE_All = ((WCE_Red) | (WCE_Green) | (WCE_Blue) | (WCE_Alpha))
};
enum PrimitiveTopologyType
{
    PTT_Unknow,
    PTT_Point,
    PTT_Line,
    PTT_Triangle,
    PTT_Patch
};

enum TextureLoadType
{
    TEXTURE_DDS,
    TEXTURE_JPG,
    TEXTURE_PNG,

};
typedef
enum ShaderFlags
{
    Unknow = 0,
    VertexShader = 1 << 0,
    PixelShader = 1 << 1,
} ShaderFlags;
#endif // !RHI_DEFINITIONS
