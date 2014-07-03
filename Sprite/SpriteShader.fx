// Global variables
float4 colour = {1.0f, 1.0f, 1.0f, 1.0f};      // Material's ambient color
texture texMain;   			// Color texture for mesh
float4x4 matWorld : WORLD; 	// World matrix for object
float4x4 matView : VIEW;    // View matrix/Camera
float4x4 matProj : PROJECTION;
float4x4 matTex;   			// UV transformation matrix

// Texture samplers
sampler TextureSampler = 
sampler_state
{
    Texture = <texMain>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
    float4 Diffuse    : COLOR0;     // vertex diffuse color
};

VS_OUTPUT RenderSpriteVS( float4 vPos : POSITION, float2 vTexCoord0 : TEXCOORD0, uniform bool bTexture )
{
    VS_OUTPUT Output;
	
	//Output.Position = mul( vPos, matWorld * matView );
    Output.Position = vPos;
        
    Output.Diffuse.rgb = colour;
    Output.Diffuse.a = 1.0f;
    
    // Just copy the texture coordinate through
    if( bTexture ) 
        Output.TextureUV = mul( vTexCoord0, matTex );
    else
        Output.TextureUV = 0; 
    
    return Output;    
}

struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

PS_OUTPUT RenderSpritePS( VS_OUTPUT In, uniform bool bTexture ) 
{ 
    PS_OUTPUT Output;

    // Lookup texture and modulate it with diffuse
    if( bTexture )
        Output.RGBColor = tex2D( TextureSampler, In.TextureUV ) * In.Diffuse;
    else
        Output.RGBColor = In.Diffuse;

    return Output;
}

technique RenderSpriteWithTexture
{
    pass P0
    {          
        VertexShader = compile vs_1_1 RenderSpriteVS( true );
        PixelShader  = compile ps_1_1 RenderSpritePS( true );
    }
}

technique RenderSpriteWithoutTexture
{
    pass P0
    {          
        VertexShader = compile vs_1_1 RenderSpriteVS( false );
        PixelShader  = compile ps_1_1 RenderSpritePS( false );
    }
}