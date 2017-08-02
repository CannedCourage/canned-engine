#include "Graphics/VulkanStaging.h"
#include "Graphics/GraphicsVK.h"

VulkanStagingManager::VulkanStagingManager( GraphicsVK& Context ) : Context( Context ), Buffers{ Context.bufferCount }
{
}