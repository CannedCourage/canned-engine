#include "Sprite/SpriteProcessor.h"
#include "Transform/TransformProcessor.h"

#include <string>

const std::vector<Vertex> vertices =
{
	{ { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
    { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
    { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
    { { -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f } }
};

const std::vector<uint16_t> drawIndices = { 0, 1, 2, 2, 3, 0 };

/*
Vertex vertices[] =
{
	{ -0.5f, 0.5f, 0.0f, 0.0f, 0.0f },
	{ 0.5f, 0.5f, 0.0f, 1.0f, 0.0f },
	{ -0.5f, -0.5f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, -0.5f, 0.0f, 1.0f, 1.0f }
};
//*/

SpriteProcessor::SpriteProcessor( GraphicsVK* g, TransformProcessor& t ) : Graphics( g ), transforms( t )
{
	//Create vertex buffer
	VkDeviceSize vertBufferSize = sizeof(vertices[0]) * vertices.size();

	VertexBuffer = Graphics->CreateBuffer(
		vertBufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VertexBufferMemory
		);

	Graphics->StagingManager.StageData( vertices.data(), vertBufferSize, VertexBuffer );

	//Create Index Buffer
	VkDeviceSize indexBufferSize = sizeof(drawIndices[0]) * drawIndices.size();

    IndexBuffer = Graphics->CreateBuffer(
		indexBufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		IndexBufferMemory
		);

    Graphics->StagingManager.StageData( drawIndices.data(), indexBufferSize, IndexBuffer );

	//Load shaders
	VertShaderModule = Graphics->LoadShaderModule( "vert.spv" );
	FragShaderModule = Graphics->LoadShaderModule( "frag.spv" );
	
	//Set world, view, projection, and translation matrices

	//Set texture in shader

	//Set technique

	//Calculate matrices

	//Set matrices
}

SpriteProcessor::~SpriteProcessor( void )
{
	//Release vertex and index buffer
	vkDestroyBuffer( Graphics->LogicalDevice, VertexBuffer, nullptr );
    vkDestroyBuffer( Graphics->LogicalDevice, IndexBuffer, nullptr );
	
	//Release shader
	vkDestroyShaderModule( Graphics->LogicalDevice, FragShaderModule, nullptr );
	vkDestroyShaderModule( Graphics->LogicalDevice, VertShaderModule, nullptr );

	//Release pipeline
	vkDestroyPipeline( Graphics->LogicalDevice, SpritePipeline, nullptr );
	vkDestroyPipelineLayout( Graphics->LogicalDevice, PipelineLayout, nullptr );
}

//TODO: Removed TextureAssetName parameter
void SpriteProcessor::AddSpriteComponent( const unsigned int entityID, const std::string& TextureAssetName )
{
	//TODO: Check if component already exists

	SpriteComponent s;

	//s.Texture = graphics.GetTexture( TextureAssetName );

	spriteComponents[entityID] = s;
}

SpriteComponent& SpriteProcessor::GetSpriteComponent( const unsigned int entityID )
{
	//TODO: Check if component exists
	return spriteComponents[entityID];
}

void SpriteProcessor::Start( void )
{
}

void SpriteProcessor::DrawSprite( const unsigned int entityID, const SpriteComponent& sprite )
{
	VkCommandBuffer buffer = Graphics->GetCurrentCommandBuffer();
	//Declare matrices

	//Grab the Transform Component
	const TransformComponent& tForm = transforms.GetTransformComponent( entityID );

	//CALCULATE WORLD TRANSFORMATION//

	//Multplication order: scaling * localRotation * translation * rotation
	//TODO: Implement non-local rotations?

	//Set world matrix in shader
	
	//Set texture in shader

	//CALCULATE TEXTURE TRANSFORMATION//

	//Hardcoded texture coordinates have height/length of 1
	//Scaling amount equals new height/width divided by the old (1.0f)
	float heightScaling = ( sprite.TextureDimensions.bottom - sprite.TextureDimensions.top );
	float widthScaling = ( sprite.TextureDimensions.right - sprite.TextureDimensions.left );

	//This transforms the texture coordinates that are hard coded in the vertex array
	
	//Set texture matrices
	
	//Draw primitive
	vkCmdDrawIndexed( buffer, static_cast<uint16_t>(drawIndices.size()), 1, 0, 0, 0 );
}

void SpriteProcessor::Update( const EngineDuration& deltaT )
{
	//TODO: Should the processor own the command buffer?
	//TODO: Should the command buffer be secondary?
	VkCommandBuffer commandBuffer = Graphics->GetCurrentCommandBuffer();

	vkCmdBindPipeline( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, SpritePipeline );

	//Set vertex data source
	VkBuffer vertexBuffers[] = { VertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers( commandBuffer, 0, 1, vertexBuffers, offsets );

	//Set index data source
	vkCmdBindIndexBuffer( commandBuffer, IndexBuffer, 0, VK_INDEX_TYPE_UINT16 );

	for( auto& sprite : spriteComponents )
	{
		DrawSprite( sprite.first, sprite.second );
	}
}

void SpriteProcessor::End( void )
{
}

VkPipelineVertexInputStateCreateInfo CreateInputDesc( void )
{
	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>( attributeDescriptions.size() );
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyDesc( void )
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	return inputAssembly;
}

VkPipelineViewportStateCreateInfo CreateViewportDesc( VkExtent2D extent )
{
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) extent.width;
	viewport.height = (float) extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = extent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	return viewportState;
}

VkPipelineRasterizationStateCreateInfo CreateRasterizerDesc( void )
{
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	return rasterizer;
}

VkPipelineMultisampleStateCreateInfo CreateMultisampleDesc( void )
{
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	return multisampling;
}

VkPipelineColorBlendStateCreateInfo CreateColorBlendDesc( void )
{
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	return colorBlending;
}

VkPipelineDepthStencilStateCreateInfo CreateDepthStencilDesc( void )
{
	VkStencilOpState front = {}, back = {};

	front.failOp = VK_STENCIL_OP_KEEP;
    front.passOp = VK_STENCIL_OP_KEEP;
    front.depthFailOp = VK_STENCIL_OP_KEEP;
    front.compareOp = VK_COMPARE_OP_ALWAYS;
    front.compareMask = 0;
    front.writeMask = 0;
    front.reference = 0;

	VkPipelineDepthStencilStateCreateInfo depth = {};

	depth.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth.depthTestEnable = VK_FALSE;
	depth.depthWriteEnable = VK_FALSE;
	depth.depthCompareOp = VK_COMPARE_OP_ALWAYS ;
	depth.depthBoundsTestEnable = VK_FALSE;
	depth.stencilTestEnable = VK_FALSE;
	depth.front = front;
	depth.back = front;
	depth.minDepthBounds = 0.0f;
	depth.maxDepthBounds = 1.0f;

	return depth;
}

void SpriteProcessor::CreatePipeline( void )
{
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};

	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = VertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};

	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = FragShaderModule;
	fragShaderStageInfo.pName = "main";

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = { vertShaderStageInfo, fragShaderStageInfo };

	auto vertexInputInfo = CreateInputDesc();
	auto inputAssembly = CreateInputAssemblyDesc();
	auto viewportState = CreateViewportDesc( Graphics->GetFrameBufferExtent() );
	auto rasterizer = CreateRasterizerDesc();
	auto multisampling = CreateMultisampleDesc();
	auto colorBlending = CreateColorBlendDesc();
	auto depth = CreateDepthStencilDesc();

	VkPipelineLayoutCreateInfo PipelineLayoutInfo = {};
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.setLayoutCount = 0; // Optional
	PipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	PipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	PipelineLayoutInfo.pPushConstantRanges = 0; // Optional

	VERIFY( VK_SUCCESS == vkCreatePipelineLayout( Graphics->LogicalDevice, &PipelineLayoutInfo, nullptr, &PipelineLayout ) );

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depth; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional

	pipelineInfo.layout = PipelineLayout;

	pipelineInfo.renderPass = Graphics->GetRenderPass();
	pipelineInfo.subpass = 0;

	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	VERIFY( VK_SUCCESS == vkCreateGraphicsPipelines( Graphics->LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &SpritePipeline ) );
}