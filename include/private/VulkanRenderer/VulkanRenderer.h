#pragma once
#ifdef VULKANRENDERER

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#include <pch.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <VulkanRenderer/VulkanEXT.h>
#include <stb_image.h>
#include <Mesmerize/Renderer.h>


#define MAX_COMMANDS 200

namespace MZ {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct VkDrawIndexedIndirectCommand {
        uint32_t    indexCount;
        uint32_t    instanceCount;
        uint32_t    firstIndex;
        int32_t     vertexOffset;
        uint32_t    firstInstance;
    };


    enum BufferMappingType {
        NoMapping,
        Mapping,
        PersitantMapping,
    };

    GLFWwindow* window;
    bool framebufferResized = false;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VmaAllocator allocator;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;

    VkCommandPool commandPool;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkBuffer> drawCommandBuffer;
    std::vector<VmaAllocation> drawCommandBufferMemory;
    std::vector<VmaAllocationInfo> drawCommandBufferMapped;

    VkImage depthImage;
    VmaAllocation depthImageMemory;
    VkImageView depthImageView;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    VkImage colorImage;
    VmaAllocation colorImageMemory;
    VkImageView colorImageView;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

    std::vector<VkBuffer> viewPerspectiveBuffer;
    std::vector<VmaAllocation> viewPerspectiveBufferMemory;
    std::vector<VmaAllocationInfo> viewPerspectiveBufferMapped;

    const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    // should be index by ObjectID
    std::vector<MeshID> objectMeshIDs;
    std::vector<ShaderID> objectShaderIDs;
    std::vector<VkDescriptorPool> objectDescriptorPools;
    std::vector<std::vector<VkDescriptorSet>> objectDescriptorSets;
    std::vector<std::vector<VkBuffer>> objectUniformBuffers;
    std::vector<std::vector<VmaAllocation>> objectUniformBuffersMemorys;
    std::vector<std::vector<VmaAllocationInfo>> objectUniformBuffersMappeds;
    std::vector<void*> objectUBO;

    // should be index by ShaderID
    std::vector<VkPipelineLayout> shaderPipelineLayouts;
    std::vector<VkPipeline> shaderGraphicsPipelines;
    std::vector<VkDescriptorSetLayout> shaderDescriptorSetLayouts;
    std::vector<int> shaderUboSize;

    // should be index by MeshID
    std::vector<VkBuffer> meshVertexBuffers;
    std::vector<VmaAllocation> meshVertexBufferMemorys;
    std::vector<VkBuffer> meshIndexBuffers;
    std::vector<VmaAllocation> meshIndexBufferMemorys;
    std::vector<uint32_t> meshIndicesSizes;


    // should be index by TextureID
    std::vector<VkImage> textureImages;
    std::vector<VmaAllocation> textureImageMemorys;
    std::vector<VkImageView> textureImageViews;
    std::vector<VkSampler> textureSamplers;

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
    };

    void drawObjects(VkCommandBuffer& commandBuffer, uint32_t renderFrame);

    void recreateSwapChain();

    void cleanupSwapChain();

    void createTextureSampler(VkSampler& textureSampler);
        
    void createTextureImage(std::string filepath, VmaAllocation& textureImageMemory, VkImage& textureImage, VkImageView& textureImageView);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels = 1);

    bool hasStencilComponent(VkFormat format);

    void updateCamera(uint32_t renderingFrame);

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, uint32_t renderFrame);

    void createUniformBuffers(std::vector<VkBuffer>& uniformBuffers, std::vector<VmaAllocation>& uniformBuffersMemory, std::vector<VmaAllocationInfo>& uniformBuffersMapped, VkDeviceSize bufferSize);

    void createViewAndPerspectiveBuffer();

    void createDrawCommandBuffer();

    void createDescriptorPool(VkDescriptorPool& descriptorPool, int numTextures);

    void createDescriptorSets(std::vector<VkDescriptorSet>& descriptorSets, VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& descriptorSetLayout, std::vector<VkBuffer>& uniformBuffers, std::vector<TextureID>& textureIDs, VkDeviceSize uboSize);

    void createGraphicsPipline(std::string vertShaderPath, std::string fragShaderPath, VkPipelineLayout& pipelineLayout, VkPipeline& graphicsPipeline, VkDescriptorSetLayout& descriptorSetLayout, std::vector<VertexValueType>& vertexValues);

    void createDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout, int numTextures);

    void createIndexBuffer(std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VmaAllocation& indexBufferMemory);

    void createVertexBuffer(void* vertices, VkBuffer& vertexBuffer, VmaAllocation& vertexBufferMemory, VkDeviceSize bufferSize);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VmaAllocation& bufferMemory, BufferMappingType mapping, VmaAllocationInfo* allocationInfo = nullptr);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    uint32_t getOffsetVertexValue(VertexValueType vertexValue);

    VkVertexInputBindingDescription getBindingDescription(std::vector<VertexValueType>& VertexValues);

    std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions(std::vector<VertexValueType>& VertexValues);

    static std::vector<char> readFile(const std::string& filename);

    VkShaderModule createShaderModule(const std::vector<char>& code);

    void createSyncObjects();

    void createVmaAllocator();

    void createCommandBuffers();

    void createFramebuffers();

    void createCommandPool();

    void createColorResources();

    void createDepthResources();

    VkFormat findDepthFormat();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VmaAllocation& imageMemory);

    void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createRenderPass();

    void createSwapChain();

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    void createImageViews();

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

    void createLogicalDevice();

    void pickPhysicalDevice();

    VkSampleCountFlagBits getMaxUsableSampleCount();

    bool isDeviceSuitable(VkPhysicalDevice device);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    void setupDebugMessenger();

    void createSurface();

    void createInstance();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    int64_t ratePhysicalDevice(VkPhysicalDevice phDevice);

    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();

    void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

}



#endif 