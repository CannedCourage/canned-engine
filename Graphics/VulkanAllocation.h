#ifndef _VULKANALLOCATION_H_
#define _VULKANALLOCATION_H_

#include <vulkan/vulkan.hpp>
#include <list>

class GraphicsVK;

struct vkAllocation
{
	//If host visible, map device memory to 'Data' using vkMapMemory
	unsigned char* Data = nullptr;

	//Vulkan Memory Binding information
	VkDeviceMemory DeviceMemory = VK_NULL_HANDLE;
	VkDeviceSize Offset = 0;
	VkDeviceSize Size = 0;

	int PoolID = -1;
	int BlockID = -1;
};

class VulkanMemoryPool
{
	friend class VulkanAllocator;
private:
protected:

	GraphicsVK& Context;

	VkDeviceMemory DeviceMemory = VK_NULL_HANDLE;
	VkDeviceSize PoolSize = 0;
	VkDeviceSize Allocated = 0;

	//If host visible, map device memory to 'Data' using vkMapMemory
	unsigned char* Data = nullptr;

	int PoolID = -1;
	int NextBlockID = 0;
	unsigned int MemoryTypeIndex = UINT32_MAX;

	bool HostVisible = true;

	struct vkBlock
	{
		VkDeviceSize Size = 0;
		VkDeviceSize Offset = 0; //Offset from the start of allocated memory

		int ID = -1;
		bool Free = true;
	};

	std::list<vkBlock> Blocks;

	VkResult VulkanMemoryPool::FindMemoryTypeIndex( const unsigned int MemoryTypeBits, const bool NeedHostVisible, unsigned int& SelectedMemoryTypeIndex );
public:

	VulkanMemoryPool( GraphicsVK& Context, const unsigned int ID, const unsigned int MemoryTypeBits, const VkDeviceSize Size, const bool HostVisible );
	~VulkanMemoryPool( void );
	
	bool Init( void );
	void CleanUp( void );

	// Return true/false for success.  If true Allocation reference is filled.
	bool Allocate( const unsigned int Size, const unsigned int Align, vkAllocation& Allocation );
	void Free( vkAllocation & Allocation );
};

class VulkanAllocator
{
private:
protected:

	GraphicsVK& Context;

	unsigned int NextPoolID = 0;
	unsigned int GarbageIndex = 0;

	//How big should each pool be when created?
	unsigned int DeviceLocalMemoryMB = 128;
	unsigned int HostVisibleMemoryMB = 64;

	std::vector<VulkanMemoryPool*> Pools;
	std::vector<std::vector<vkAllocation>> Garbage;

	bool AllocateFromPools( const unsigned int Size, const unsigned int Align, const unsigned int MemoryTypeBits, const bool HostVisible, vkAllocation& Allocation );
public:

	VulkanAllocator( GraphicsVK& Context );

	void Init( void );
	void CleanUp( void );

	vkAllocation Allocate( const unsigned int Size, const unsigned int Align, const unsigned int MemoryTypeBits, const bool HostVisible );
	void Free( vkAllocation& Allocation );

	void EmptyGarbage( void );
};

#endif //_VULKANALLOCATION_H_