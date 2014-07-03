// Global variables

texture g_MeshTexture;

float4x4 g_mWorld : WORLD;
float4x4 g_mView : VIEW;
float4x4 g_mProjection : PROJECTION;

// Texture samplers
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
};

VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float2 vTexCoord0 : TEXCOORD0 )
{
    VS_OUTPUT Output;
	
	float4x4 wvp = mul( mul(g_mWorld, g_mView), g_mProjection );
    
    // Transform the position from object space to homogeneous projection space
    Output.Position = mul(vPos, wvp);
	
    // Just copy the texture coordinate through
    Output.TextureUV = vTexCoord0; 
    
    return Output;    
}

struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

PS_OUTPUT RenderScenePS( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture
    Output.RGBColor = tex2D(MeshTextureSampler, In.TextureUV);

    return Output;
}

technique RenderSpriteWithoutTexture
{
    pass P0
    {          
        VertexShader = compile vs_1_1 RenderSceneVS();
        PixelShader  = compile ps_1_1 RenderScenePS(); 
    }
}