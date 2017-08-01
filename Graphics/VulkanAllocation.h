#ifndef _VULKANALLOCATION_H_
#define _VULKANALLOCATION_H_

#include "Graphics/GraphicsVK.h"
#include <vulkan/vulkan.hpp>

struct vkAllocation
{
	//If host visible, map device memory to 'Data' using vkMapMemory
	byte* Data = nullptr;

	//Vulkan Memory Binding information
	VkDeviceMemory DeviceMemory = VK_NULL_HANDLE;
	VkDeviceSize Offset = 0;
	VkDeviceSize Size = 0;

	unsigned int PoolID = 0;
	unsigned int BlockID = 0;
};

class VulkanMemoryPool
{
	friend class VulkanAllocator;
private:
protected:

	const GraphicsVK& Context;

	VkDeviceMemory DeviceMemory;
	VkDeviceSize PoolSize = 0;
	VkDeviceSize Allocated = 0;

	//If host visible, map device memory to 'Data' using vkMapMemory
	byte* Data = nullptr;

	unsigned int PoolID = 0;
	unsigned int NextBlockID = 0;
	unsigned int MemoryTypeIndex = 0;

	bool HostVisible;

	struct vkBlock
	{
		VkDeviceSize Size = 0;
		VkDeviceSize Offset = 0; //Offset from the start of allocated memory

		unsigned int ID = 0;
		bool Free = true;
	};

	std::list<vkBlock> Blocks;
public:

	VulkanMemoryPool( const GraphicsVK& Context, const unsigned int ID, const unsigned int MemoryTypeBits, const VkDeviceSize Size, const bool HostVisible );
	~VulkanMemoryPool( void );
	
	bool Init();
	void Shutdown();

	// Return true/false for success.  If true Allocation reference is filled.
	bool Allocate( const unsigned int Size, const unsigned int Align, vkAllocation& Allocation );
	void Free( vkAllocation & Allocation );
};

class VulkanAllocator
{
private:
protected:

	const GraphicsVK& Context;

	unsigned int NextPoolID = 0;
	unsigned int GarbageIndex = 0;

	//How big should each pool be when created?
	unsigned int DeviceLocalMemoryMB = 0;
	unsigned int HostVisibleMemoryMB = 0;

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