#ifndef _VULKANALLOCATION_H_
#define _VULKANALLOCATION_H_

#include "Graphics/GraphicsVK.h"
#include <vulkan/vulkan.hpp>

struct vkAllocation
{
	unsigned int Pool = 0;
	unsigned int Block = 0;
	VkDeviceMemory DeviceMemory = VK_NULL_HANDLE;
	VkDeviceSize Offset = 0;
	VkDeviceSize Size = 0;
	byte* data = nullptr;
};

class VulkanMemoryPool
{
	friend class VulkanAllocator;
private:
protected:

	const GraphicsVK& Context;

	struct vkBlock
	{
		unsigned int ID = 0;
		VkDeviceSize Offset = 0;
		VkDeviceSize Size = 0;
		bool Free = true;
	};

	std::list<vkBlock> Blocks;

	unsigned int ID = 0;
	unsigned int NextBlockID = 0;
	unsigned int MemoryTypeIndex = 0;
	bool HostVisible;
	VkDeviceMemory DeviceMemory;
	VkDeviceSize Size = 0;
	VkDeviceSize Allocated = 0;
	byte* Data = nullptr;
public:

	VulkanMemoryPool( const GraphicsVK& Context, const unsigned int ID, const unsigned int MemoryTypeBits, const VkDeviceSize Size, const bool HostVisible );
	~VulkanMemoryPool( void );
	
	bool Init();
	void Shutdown();

	// Return true/false for success.  If true vkAllocation_t reference is filled.
	bool Allocate( const unsigned int Size, const unsigned int Align, vkAllocation& Allocation );
	void Free( vkAllocation & Allocation );
};

class VulkanAllocator
{
private:
protected:

	const GraphicsVK& Context;

	int NextPoolID = 0;
	int GarbageIndex = 0;

	// How big should each pool be when created?
	int DeviceLocalMemoryMB = 0;
	int HostVisibleMemoryMB = 0;

	std::vector<VulkanMemoryPool*> Pools;
	std::vector<vkAllocation> Garbage;

	bool AllocateFromPools( const unsigned int Size, const unsigned int Align, const unsigned int MemoryTypeBits, const bool HostVisible, vkAllocation& Allocation );
public:

	VulkanAllocator( const GraphicsVK& Context );

	void Init( void );

	vkAllocation Allocate( const unsigned int Size, const unsigned int Align, const unsigned int MemoryTypeBits, const bool HostVisible );
	void Free( const vkAllocation Allocation );

	void EmptyGarbage( void );
};

#endif //_VULKANALLOCATION_H_