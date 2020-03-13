#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

// STL
#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <set>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <array>
#include <chrono>

// GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int WIDTH = 800;
const int HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;

// Validation layers
const std::vector<const char*> validationLayers = {
  // Validation layer
    "VK_LAYER_KHRONOS_validation"
};

// Device extensions
const std::vector<const char*> deviceExtensions = {
  // Swap chain extension required
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#define DEBUG
// Setup Vulkan validation layers if compiling for debug, else not.
#ifdef DEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;
  
  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof( Vertex );
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    // Common binding description formats for different data types:
    // float: VK_FORMAT_R32_SFLOAT
    // vec2 : VK_FORMAT_R32G32_SFLOAT
    // vec3 : VK_FORMAT_R32G32B32_SFLOAT
    // vec4 : VK_FORMAT_R32G32B32A32_SFLOAT
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof( Vertex, pos );
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof( Vertex, color );

    return attributeDescriptions;
  }
};

const std::vector<Vertex> vertices = {
  {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
  {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
  {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
  {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
   0, 1, 2, 2, 3, 0
};

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};


class HelloTriangleApplication {
public:
  HelloTriangleApplication() :
    _window( nullptr ),
    _physicalDevice( VK_NULL_HANDLE ),
    _currentFrame(0),
    _framebufferResized(false) {};

  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentationFamily;

    bool isComplete() {
      return graphicsFamily.has_value() && presentationFamily.has_value();
    }
  };

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

private:

  //--- Member functions ---

  static void framebufferResizeCallback( GLFWwindow* window, int width, int height ) {
    auto app = reinterpret_cast< HelloTriangleApplication* >( glfwGetWindowUserPointer( window ) );
    app->_framebufferResized = true;
  }

  // Initializes and creates a window to display the graphics in.
  void initWindow() {
    glfwInit();

    // Tell glfw not to create an OpenGL context, since that's what glfw usually does otherwise.
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

    _window = glfwCreateWindow( WIDTH, HEIGHT, "Vulkan", nullptr, nullptr );
    glfwSetWindowUserPointer( _window, this );
    glfwSetFramebufferSizeCallback( _window, framebufferResizeCallback );
  };

  // Initializes Vulkan
  void initVulkan() {
    createVKInstance();
    //setupDebugMessenger();
    createSurface();
    selectPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPools();
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();
  };

  void mainLoop() {
    while( /*!glfwWindowShouldClose( _window )*/true ) {
      glfwPollEvents();
      drawFrame();
    }

    vkDeviceWaitIdle( _logicalDevice );
  };

  // Cleans up various things after application is finished running
  void cleanup() {
    // Cleanup Vulkan-related resources create explicitly by the user.
    const VkAllocationCallbacks* allocator = VK_NULL_HANDLE;

    cleanupSwapChain();

    vkDestroyDescriptorSetLayout( _logicalDevice, _descriptorSetLayout, allocator );

    vkDestroyBuffer( _logicalDevice, _vertexBuffer, allocator );
    vkFreeMemory( _logicalDevice, _vertexBufferMemory, allocator );

    vkDestroyBuffer( _logicalDevice, _indexBuffer, allocator );
    vkFreeMemory( _logicalDevice, _indexBufferMemory, allocator );

    for( int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++ ) {
      vkDestroySemaphore( _logicalDevice, _renderFinishedSemaphore[i], allocator );
      vkDestroySemaphore( _logicalDevice, _imageAvailableSemaphore[i], allocator );
      vkDestroyFence( _logicalDevice, _inFlightFences[i], allocator );
    }

    vkDestroyCommandPool( _logicalDevice, _commandPool, allocator );

    vkDestroyDevice( _logicalDevice, allocator );
    vkDestroySurfaceKHR( _instance, _surface, allocator );
    vkDestroyInstance( _instance, allocator );

    // Cleanup glfw things
    glfwTerminate();
    glfwDestroyWindow( _window );
  };

  // Check if validation layers are supported for debugging for Vulkan commands.
  bool checkValidationLayerSupport() {
    uint32_t layerCount;

    vkEnumerateInstanceLayerProperties( &layerCount, nullptr );
    std::vector<VkLayerProperties> availableLayers( layerCount );
    vkEnumerateInstanceLayerProperties( &layerCount, availableLayers.data() );

    for( const auto& layerName : validationLayers ) {
      bool layerFound = false;
      for( const auto& layerProperty : availableLayers ) {
        if( strcmp( layerName, layerProperty.layerName ) ) {
          return true;
        }
      }
    }

    return false;
  }

  // Creates a Vulkan instance.
  void createVKInstance() {

    // Check if want to and can use validation layers for debug purposes.
    if( enableValidationLayers && !checkValidationLayerSupport() ) {
      throw std::runtime_error( "validation layers requested, but not available!" );
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.apiVersion = VK_API_VERSION_1_0;


    // Check which instance extensions that are supported and print them out.
    uint32_t extensionCount = 0;
    const char* layerName = nullptr;
    vkEnumerateInstanceExtensionProperties( layerName, &extensionCount, nullptr );
    std::vector<VkExtensionProperties> extensions( extensionCount );
    vkEnumerateInstanceExtensionProperties( layerName, &extensionCount, extensions.data() );

    std::cout << "available extensions:" << std::endl;
    for( const auto& extension : extensions ) {
      std::cout << "\t" << extension.extensionName << std::endl;
    }

    // Check required extensions by glfw
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

    std::cout << "glfw extensions: " << std::endl << "\t" << *glfwExtensions << std::endl;

    // Define createInfo
    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledExtensionCount = glfwExtensionCount;
    instanceCreateInfo.ppEnabledExtensionNames = glfwExtensions;
    instanceCreateInfo.enabledLayerCount = 0;
    if( enableValidationLayers ) {
      instanceCreateInfo.enabledLayerCount = static_cast< uint32_t >( validationLayers.size() );
      instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
      instanceCreateInfo.enabledLayerCount = 0;
    }

    const VkAllocationCallbacks* allocator = VK_NULL_HANDLE; // Not used
    if( vkCreateInstance( &instanceCreateInfo, allocator, &_instance ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create instance!" );
    }
  }

  // Creates a surface in Vulkan using glfw. This is where Vulkan (which isn't platform agnostic)
  // connects with the glfw window used to present graphics.
  void createSurface() {
    const VkAllocationCallbacks* allocator = VK_NULL_HANDLE; // Not used
    if( glfwCreateWindowSurface( _instance, _window, allocator, &_surface ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create window surface!" );
    }

  }

  // This lets the user select a physical device, i.e. a graphics card
  // available to use with the Vulkan instance.
  void selectPhysicalDevice() {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices( _instance, &deviceCount, nullptr );

    if( deviceCount == 0 ) {
      throw std::runtime_error( "failed to find GPUs with Vulkan support!" );
    }

    std::vector< VkPhysicalDevice > devices( deviceCount );
    vkEnumeratePhysicalDevices( _instance, &deviceCount, devices.data() );

    for( const auto& device : devices ) {
      // Loop over alla physical device to see if there is a suitable one to use.
      if( isDeviceSuitable( device ) ) {
        physicalDevice = device;
        break;
      }
    }

    if( physicalDevice == VK_NULL_HANDLE ) {
      throw std::runtime_error( "failed to find a suitable GPU!" );
    }

    _physicalDevice = physicalDevice;
  }

  // Checks whether a physical device is suitable to use together with the instance.
  bool isDeviceSuitable( VkPhysicalDevice device ) {
    // Here you can querie the device for properties and features,
    // use a scoring system to select the most advanced one based on
    // the features it has etc. to select the optimal gpu to use.
    // For instance you could use this to destinguish from an integrated
    // or dedicated gpu and select the one you want to use.
    // This tutorial won't do anything that advanced though.

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties( device, &deviceProperties );
    vkGetPhysicalDeviceFeatures( device, &deviceFeatures );
    
    // Make sure that required extensions are supported.
    const bool extensionsSupported = checkDeviceExtensionSupport( device );

    // Make find queue families.
    QueueFamilyIndices indices = findQueueFamilies( device );

    // Make sure swap chain has adequate capabilities (in addition to actually being supported, which is check at checkDeviceExtensionSupport).
    bool swapChainAdequate = false;
    if( extensionsSupported ) {
      SwapChainSupportDetails swapChainSupport = querySwapChainSupport( device );
      swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    if( indices.isComplete() && extensionsSupported && swapChainAdequate ) {
      // Physical device suits our needs.
      std::cout << "Physical device '" << deviceProperties.deviceName << "' selected." << std::endl;
      return true;
    }

    return false;
  }

  QueueFamilyIndices findQueueFamilies( VkPhysicalDevice device ) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, nullptr );
    std::vector<VkQueueFamilyProperties> queueFamilies( queueFamilyCount );
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, queueFamilies.data() );

    int i = 0;
    for( const auto& queueFamily : queueFamilies ) {
      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR( device, i, _surface, &presentSupport );
      if( presentSupport ) {
        // Check if the device and surface are compatible.
        indices.presentationFamily = i;
      }
      if( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
        // Check if queue family needs are met.
        indices.graphicsFamily = i;
      }
      if( indices.isComplete() ) {
        break;
      }
      i++;
    }

    return indices;
  }

  // Creates a logical device. This is the encapsulation of the physical device
  // and all the extensions used.
  void createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies( _physicalDevice );

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentationFamily.value() };

    const float queuePriority = 1.0;
    for( const auto& queueFamily : uniqueQueueFamilies ) {
      VkDeviceQueueCreateInfo  queueCreateInfo = {};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back( queueCreateInfo );
    }

    // Leave empty for now - no special device features needed.
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast< uint32_t >( queueCreateInfos.size() );
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>( deviceExtensions.size() );
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    if( enableValidationLayers ) {
      // These are ignored for later Vulkan implementations since device validation layers don't exist anymore,
      // only the instance validation layers do. Assigning here anyways for compability with older versions.
      createInfo.enabledLayerCount = static_cast< uint32_t >( validationLayers.size() );
      createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
      createInfo.enabledLayerCount = 0;
    }

    const VkAllocationCallbacks* allocator = VK_NULL_HANDLE; // Not used
    if( vkCreateDevice( _physicalDevice, &createInfo, allocator, &_logicalDevice ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create logical device!" );
    }

    // Get queues.
    vkGetDeviceQueue( _logicalDevice, indices.presentationFamily.value(), 0, &_presentationQueue );
    vkGetDeviceQueue( _logicalDevice, indices.graphicsFamily.value(), 0, &_graphicsQueue );
  }

  // Makes sure that all the required device extensions specified by deviceExtensions are supported.
  bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    const char* layerName = nullptr;
    vkEnumerateDeviceExtensionProperties( device, layerName, &extensionCount, nullptr );
    std::vector<VkExtensionProperties> availableExtensions( extensionCount );
    vkEnumerateDeviceExtensionProperties( device, layerName, &extensionCount, availableExtensions.data() );

    std::set<std::string> requiredExtensions( deviceExtensions.begin(), deviceExtensions.end() );

    for( const auto& extension : availableExtensions ) {
      requiredExtensions.erase( extension.extensionName );
    }

    return requiredExtensions.empty();
  }

  // Query the physical device to see if it supports the swap chain we need.
  SwapChainSupportDetails querySwapChainSupport( VkPhysicalDevice device ) {
    SwapChainSupportDetails details;
    
    // Query for suface capabilities.
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device, _surface, &details.capabilities );

    uint32_t formatCount;
    VkSurfaceFormatKHR* surfaceFormat = nullptr;
    vkGetPhysicalDeviceSurfaceFormatsKHR( device, _surface, &formatCount, surfaceFormat );
    if( formatCount != 0 ) {
      details.formats.resize( formatCount );
      // Query for surface format.
      vkGetPhysicalDeviceSurfaceFormatsKHR( device, _surface, &formatCount, details.formats.data() );
    }

    uint32_t presentModeCount;
    VkPresentModeKHR* presentModes = nullptr;
    vkGetPhysicalDeviceSurfacePresentModesKHR( device, _surface, &presentModeCount, presentModes );
    if( presentModeCount != 0 ) {
      details.presentModes.resize( presentModeCount );
      // Query for presentation mode.
      vkGetPhysicalDeviceSurfacePresentModesKHR( device, _surface, &presentModeCount, details.presentModes.data() );
    }

    return details;
  }

  // Choose the surface format of the swap chain.
  VkSurfaceFormatKHR chooseSwapSurfaceFormat( const std::vector<VkSurfaceFormatKHR>& availableFormats ) {

    for( const auto& availableFormat : availableFormats ) {
      if( availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM /*VK_FORMAT_B8G8R8A8_SRGB*/ && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR ) {
        return availableFormat;
      }
    }
    // If nothing good is found, just use anything.
    return availableFormats[0];
  }

  // Choose the present mode of the swap chain (e.g. double buffering, tripple buffering etc. )
  VkPresentModeKHR chooseSwapPresentMode( const std::vector<VkPresentModeKHR>& availablePresentModes ) {
    for( const auto& availablePresentMode : availablePresentModes ) {
      if( availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR ) {
        return availablePresentMode;
      }
    }

    // Guaranteed to be present.
    return VK_PRESENT_MODE_FIFO_KHR;
  }

  // Choose the width, height of the images in the swap chain.
  VkExtent2D chooseSwapExtent( const VkSurfaceCapabilitiesKHR& capabilities ) {
    if( capabilities.currentExtent.width != UINT32_MAX ) {
      // Use same size as the window.
      return capabilities.currentExtent;
    } else {
      int width, height;
      glfwGetFramebufferSize( _window, &width, &height );
      VkExtent2D actualExtent = { static_cast< uint32_t >( width ),
                                  static_cast< uint32_t >( height ) };

      // Get the min and max values previously specified.
      actualExtent.width = std::max( capabilities.minImageExtent.width, std::min( capabilities.maxImageExtent.width, actualExtent.width ) );
      actualExtent.height = std::max( capabilities.minImageExtent.height, std::min( capabilities.maxImageExtent.height, actualExtent.height ) );

      return actualExtent;
    }
  }

  // Creates a swap chain (queue of images to be rendered basically). It is
  // important that this is done straight after creating the logical device.
  void createSwapChain() {
    // Query the swapchain for support based on the physical device chosen.
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport( _physicalDevice );

    // Gather additional preferences and details on the swap chain.
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat( swapChainSupport.formats );
    VkPresentModeKHR presentMode = chooseSwapPresentMode( swapChainSupport.presentModes );
    VkExtent2D extent = chooseSwapExtent( swapChainSupport.capabilities );

    // Request one more image than bare minimum due to driver may becoming a limitation.
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if( swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount ) {
      imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; // Could be more than 1 if we want stereo graphics.
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // VK_IMAGE_USAGE_TRANSFER_DST_BIT could be used when we want post-processing.

    QueueFamilyIndices indices = findQueueFamilies( _physicalDevice );
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentationFamily.value() };

    if( indices.graphicsFamily != indices.presentationFamily ) {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
      createInfo.queueFamilyIndexCount = 0; // Optional
      createInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // No special transform on all images in swap chain.
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    // If window is resized, the swapchain could be invalidated, and would thus need to be to recreated, in which case it could be passed here.
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    
    const VkAllocationCallbacks* allocator = VK_NULL_HANDLE; // Not used
    if( vkCreateSwapchainKHR( _logicalDevice, &createInfo, allocator, &_swapChain ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create swap chain!" );
    }

    // Load swap chain images into member variable.
    vkGetSwapchainImagesKHR( _logicalDevice, _swapChain, &imageCount, nullptr );
    _swapChainImages.resize( imageCount );
    vkGetSwapchainImagesKHR( _logicalDevice, _swapChain, &imageCount, _swapChainImages.data() );

    // Save for future use.
    _swapChainImageFormat = surfaceFormat.format;
    _swapChainExtent = extent;
  }

  void createImageViews() {
    _swapChainImageViews.resize( _swapChainImages.size() );
    for( size_t i = 0; i < _swapChainImages.size(); i++ ) {
      const VkAllocationCallbacks* allocator = VK_NULL_HANDLE; // Not used
      VkImageViewCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      createInfo.image = _swapChainImages[i];
      createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // Could be 3D texture, cube map etc.
      createInfo.format = _swapChainImageFormat;
      // Use default rgba mapping
      createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      createInfo.subresourceRange.baseMipLevel = 0; // No mip-map levels
      // These data could differ if working with stereo applications, with layers for each eye etc.
      createInfo.subresourceRange.levelCount = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount = 1;
      if( vkCreateImageView( _logicalDevice, &createInfo, allocator, &_swapChainImageViews[i] ) != VK_SUCCESS ) {
        throw std::runtime_error( "failed to create image views!" );
      }
    }
  }

  // Creates a vulkan graphics pipeline
  void createGraphicsPipeline() {

    // Load and read shaders from file
    auto vertShaderCode = readFile( "../shaders/vert.spv" );
    auto fragShaderCode = readFile( "../shaders/frag.spv" );

    VkShaderModule vertShaderModule = createShaderModule( vertShaderCode );
    VkShaderModule fragShaderModule = createShaderModule( fragShaderCode );

    // Describe the programmable parts of the pipeline.
    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo , fragShaderStageInfo };

    // Get vertex related descriptions
    VkVertexInputBindingDescription bindingDescription = Vertex::getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 2> attributeDescription = Vertex::getAttributeDescriptions();

    // Describe the fixed function parts of the pipeline.
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>( attributeDescription.size() );
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();

    // Specify how we should draw the points, similar to specifying
    // if we should use an ebo and use indices in OpenGL, and also if
    // we should draw points, lines, triangles etc.
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Specify viewport
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast< float >( _swapChainExtent.width ); //Note, not the width of the window, but the frameBuffer instead.
    viewport.height = static_cast< float >( _swapChainExtent.height ); //Note, not the height of the window, but the frameBuffer instead.
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Specify a scissor extent that cover the entire frameBuffer.
    VkRect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = _swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // Specify the rasterization states such as depthClamp, frontFace, cullMode etc.
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    // Since we're using descriptor sets and flipped the y-coordinate of the projection matrix,
    // we need to draw counter clockwise rather than clockwise, otherwise the faces will be culled.
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    // Can be used for AA, disable for now.
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = nullptr; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    // We won't use any depth and/or stencil buffer for now, so just pass a nullptr later on.
    // VkPipelineDepthStencilStateCreateInfo

    // Determine color blend between the fragment output and the framebuffer
    // See pseudo-code below to get an idea of how the different flags works.
    // if( blendEnable ) {
    //   finalColor.rgb = ( srcColorBlendFactor * newColor.rgb ) < colorBlendOp > ( dstColorBlendFactor * oldColor.rgb );
    //   finalColor.a = ( srcAlphaBlendFactor * newColor.a ) < alphaBlendOp > ( dstAlphaBlendFactor * oldColor.a );
    // } else {
    //   finalColor = newColor;
    // }
    // finalColor = finalColor & colorWriteMask;
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    // For alpha blending with following pseudo code:
    //finalColor.rgb = newAlpha * newColor + ( 1 - newAlpha ) * oldColor;
    //finalColor.a = newAlpha.a;
    //colorBlendAttachment.blendEnable = VK_TRUE;
    //colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    //colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    //colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    //colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    //colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    //colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

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

    // Dynamic state info can be used to specify some parts of the
    // pipeline that will change dynamically, so that you don't
    // have th rebuild the entire pipeline when making changes to them.
    // Such states can be blend constants, viewport size, line width etc.
    //VkDynamicState dynamicStates[] = {
    //VK_DYNAMIC_STATE_VIEWPORT,
    //VK_DYNAMIC_STATE_LINE_WIDTH
    //};

    //VkPipelineDynamicStateCreateInfo dynamicState = {};
    //dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    //dynamicState.dynamicStateCount = 2;
    //dynamicState.pDynamicStates = dynamicStates;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &_descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if( vkCreatePipelineLayout( _logicalDevice, &pipelineLayoutInfo, VK_NULL_HANDLE, &_pipelineLayout ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create pipeline layout!" );
    }

    // With all previous info, create the pipeline info
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr; // Optional
    pipelineInfo.layout = _pipelineLayout;
    pipelineInfo.renderPass = _renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // If a similar pipeline exists that wants to be reused, this could be used as pointer to that.
    pipelineInfo.basePipelineIndex = -1; // Optional

    if( vkCreateGraphicsPipelines( _logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, VK_NULL_HANDLE, &_graphicsPipeline ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create graphics pipeline!" );
    }

    const VkAllocationCallbacks* allocator = VK_NULL_HANDLE; // Not used

    vkDestroyShaderModule( _logicalDevice, vertShaderModule, allocator );
    vkDestroyShaderModule( _logicalDevice, fragShaderModule, allocator );
  }

  // Reads a file and returns a vector containing the content.
  // Throws runtime_errror if file can't be opened.
  static std::vector<char> readFile( const std::string& filename ) {
    // ate = start reading at the end of file.
    // binary = read file as binary.
    std::ifstream file( filename, std::ios::ate | std::ios::binary );

    if( !file.is_open() ) {
      throw std::runtime_error( "Failed to open file: '" + filename + "'." );
    }

    // Get size of file, return read pointer to beginning of file, and collect the file content.
    size_t fileSize = static_cast< size_t >( file.tellg() );
    std::vector<char> buffer( fileSize );
    file.seekg( 0 );
    file.read( buffer.data(), fileSize );

    file.close();
    return buffer;
  }

  VkShaderModule createShaderModule( const std::vector<char>& code ) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>( code.data() );

    VkShaderModule shaderModule;
    const VkAllocationCallbacks* allocator = VK_NULL_HANDLE; // Not used
    if( vkCreateShaderModule( _logicalDevice, &createInfo, allocator, &shaderModule ) != VK_SUCCESS ) {
      throw std::runtime_error( "Failed to create shader module!" );
    }

    return shaderModule;
  }

  void createRenderPass() {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = _swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Clear before loading the data
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0; // Specifies the index in the renderPassInfo.pAttachments array created later.
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Graphics subpass. Other values could be a compute subpass, for instance.
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if( vkCreateRenderPass( _logicalDevice, &renderPassInfo, VK_NULL_HANDLE, &_renderPass ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create render pass!" );
    }
  }

  // Creates framebuffer(s) for the pipeline to use.
  void createFramebuffers() {
    _swapChainFramebuffers.resize( _swapChainImageViews.size() );

    for( size_t i = 0; i < _swapChainImageViews.size(); i++ ) {
      // Create a framebuffer for each imageView.
      VkImageView attachments[] = {
        _swapChainImageViews[i]
      };

      VkFramebufferCreateInfo framebufferInfo = {};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = _renderPass;
      framebufferInfo.attachmentCount = 1;
      framebufferInfo.pAttachments = attachments;
      framebufferInfo.width = _swapChainExtent.width;
      framebufferInfo.height = _swapChainExtent.height;
      framebufferInfo.layers = 1;

      const VkAllocationCallbacks* allocator = VK_NULL_HANDLE; // Not used

      if( vkCreateFramebuffer( _logicalDevice, &framebufferInfo, allocator, &_swapChainFramebuffers[i] ) != VK_SUCCESS ) {
        throw std::runtime_error( "failed to create framebuffer!" );
      }
    }
  }

  void createCommandPools() {
    QueueFamilyIndices familyIndices = findQueueFamilies( _physicalDevice );

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = familyIndices.graphicsFamily.value();
    poolInfo.flags = 0;

    // Create command pools, which will manage the memory used to store the command buffers.
    if( vkCreateCommandPool( _logicalDevice, &poolInfo, VK_NULL_HANDLE, &_commandPool ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create command pool!" );
    }
  }

  // Create command buffers which will hold our rendering commands.
  void createCommandBuffers() {
    _commandBuffers.resize( _swapChainFramebuffers.size() );

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast< uint32_t >( _commandBuffers.size() );

    // Allocate space for the command buffers.
    if( vkAllocateCommandBuffers( _logicalDevice, &allocInfo, _commandBuffers.data() ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to allocate command buffers!" );
    }

    for( size_t i = 0; i < _commandBuffers.size(); i++ ) {
      VkCommandBufferBeginInfo beginInfo = {};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      //beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT; //TODO
      beginInfo.pInheritanceInfo = nullptr; // Only relevant for secondary commandBuffers.

      if( vkBeginCommandBuffer( _commandBuffers[i], &beginInfo ) != VK_SUCCESS ) {
        throw std::runtime_error( "failed to begin recording command buffer!" );
      }

      VkRenderPassBeginInfo renderPassInfo = {};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = _renderPass;
      renderPassInfo.framebuffer = _swapChainFramebuffers[i];
      renderPassInfo.renderArea.offset = { 0, 0 };
      renderPassInfo.renderArea.extent = _swapChainExtent;

      VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f }; // Clear to black.
      renderPassInfo.clearValueCount = 1;
      renderPassInfo.pClearValues = &clearColor;

      // Begin recording rendering operations using the commandBuffer and renderPass we've specified.
      vkCmdBeginRenderPass( _commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );

      // Bind the pipeline we want to render to.
      vkCmdBindPipeline( _commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline );

      VkBuffer vertexBuffers[] = { _vertexBuffer };
      VkDeviceSize offsets[] = { 0 };

      // Bind the vertex buffers.
      vkCmdBindVertexBuffers( _commandBuffers[i], 0, 1, vertexBuffers, offsets );

      // Bind the index buffer.
      vkCmdBindIndexBuffer( _commandBuffers[i], _indexBuffer, 0, VK_INDEX_TYPE_UINT16 );

      // Bind the descriptor sets used for the uniform buffers.
      vkCmdBindDescriptorSets( _commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &_descriptorSets[i], 0, VK_NULL_HANDLE );

      // Draw.
      vkCmdDrawIndexed( _commandBuffers[i], static_cast< uint32_t >( indices.size() ), 1, 0, 0, 0 );

      // Used when not using index buffers.
      //vkCmdDraw( _commandBuffers[i],
      //            static_cast<uint32_t>( vertices.size() ), // Vertex count
      //            1, // Instance count
      //            0, // First vertex
      //            0  // First instance 
      //         );

      // End therender pass
      vkCmdEndRenderPass( _commandBuffers[i] );

      if( vkEndCommandBuffer( _commandBuffers[i] ) != VK_SUCCESS ) {
        throw std::runtime_error( "failed to record command buffer!" );
      }
    }
  }

  // Creates objects to be used as locking mechanism during rendering.
  void createSyncObjects() {

    // One semaphore for each frame in flight
    _imageAvailableSemaphore.resize( MAX_FRAMES_IN_FLIGHT );
    _renderFinishedSemaphore.resize( MAX_FRAMES_IN_FLIGHT );
    _inFlightFences.resize( MAX_FRAMES_IN_FLIGHT );
    _imagesInFlight.resize( _swapChainImages.size(), VK_NULL_HANDLE );

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Initialize as signaled, otherwise we'd wait forever if we don't signal explicitly before checking vkWaitForFences.

    const VkAllocationCallbacks* allocator = VK_NULL_HANDLE; // Not used
    for( size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++ ) {
      if( vkCreateSemaphore( _logicalDevice, &semaphoreInfo, allocator, &_imageAvailableSemaphore[i] ) != VK_SUCCESS ||
          vkCreateSemaphore( _logicalDevice, &semaphoreInfo, allocator, &_renderFinishedSemaphore[i] ) != VK_SUCCESS ||
          vkCreateFence( _logicalDevice, &fenceInfo, allocator, &_inFlightFences[i] ) != VK_SUCCESS ) {
        throw std::runtime_error( "failed to create semaphores!" );
      }
    }
    
  }

  // Draws the frame and also utilize the synchronization variables in place to make sure more
  // work isn't sent to the GPU before it is ready to process it.
  void drawFrame() {
    // Wait for previous fences to be signaled
    vkWaitForFences( _logicalDevice, 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX );

    uint32_t imageIndex;
    const uint64_t timeout = UINT64_MAX; // in nanoseconds

    // Acquires an image from the _logicalDevice and the _swapChain, and assigns the index of it to imageIndex. Also 
    VkResult result = vkAcquireNextImageKHR( _logicalDevice,
                                             _swapChain,
                                             timeout,
                                             _imageAvailableSemaphore[_currentFrame],
                                             VK_NULL_HANDLE /* Initialize to no fence since initially no images are used by the frame */,
                                             &imageIndex );

    if( result == VK_ERROR_OUT_OF_DATE_KHR ) {
      // Swap chain is outdated for various reasons (could be window resize). Recreate it.
      recreateSwapChain();
      return;
    } else if( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR ) {
      throw std::runtime_error( "Failed to acquire swap chain image" );
    }

    // Check if a previous frame is using this image (i.e. there is a fence to wait on)
    if( _imagesInFlight[imageIndex] != VK_NULL_HANDLE ) {
      vkWaitForFences( _logicalDevice, 1, &_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX );
    }
    // Mark the image as now being in use by this frame
    _imagesInFlight[imageIndex] = _inFlightFences[_currentFrame];

    VkSemaphore waitSemaphores[] = { _imageAvailableSemaphore[_currentFrame] };
    VkSemaphore signalSemaphores[] = { _renderFinishedSemaphore[_currentFrame] };
    // Wait for color attachment output before executing commands.
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    // Update the uniform buffers.
    updateUniformBuffers( imageIndex );

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_commandBuffers[imageIndex];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Un-signal fences (reset). Will otherwise continue to be signaled.
    vkResetFences( _logicalDevice, 1, &_inFlightFences[_currentFrame] );

    const uint32_t submitCount = 1;
    // Submit commandBuffer to the graphicsQueue.
    if( vkQueueSubmit( _graphicsQueue, submitCount, &submitInfo, _inFlightFences[_currentFrame] ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to submit draw command buffer!" );
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { _swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    // Present image.
    result = vkQueuePresentKHR( _presentationQueue, &presentInfo );
    if( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||_framebufferResized ) {
      _framebufferResized = false;
      // Swap chain not good enough, recreate.
      recreateSwapChain();
    } else if( result != VK_SUCCESS ) {
      throw std::runtime_error( "failed to present queue!" );
    }

    // Not needed when new synchronization scheme is in place
    //vkQueueWaitIdle( _presentationQueue );

    // Update frame and adjust to allowed number of frames in flight
    _currentFrame = ( _currentFrame + 1 ) % MAX_FRAMES_IN_FLIGHT;
  }

  // There are events that may invalidate our current swap-chain
  // (such as resizing the window), in which case we must recreate it
  // in order to continue drawing.
  void recreateSwapChain() {

    int width = 0, height = 0;
    glfwGetFramebufferSize( _window, &width, &height );
    while( width == 0 || height == 0 ) {
      // If minimized, keep polling until it isn't anymore.
      glfwGetFramebufferSize( _window, &width, &height );
      glfwWaitEvents();
    }

    std::cout << "Recreating swap chain with window width " << width << " and height " << height << std::endl;

    vkDeviceWaitIdle( _logicalDevice );

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
  }

  // Cleans up a swap chain and related resources.
  void cleanupSwapChain() {
    const VkAllocationCallbacks* allocator = VK_NULL_HANDLE;

    for( size_t i = 0; i < _swapChainFramebuffers.size(); i++ ) {
      vkDestroyFramebuffer( _logicalDevice, _swapChainFramebuffers[i], allocator );
    }

    vkFreeCommandBuffers( _logicalDevice, _commandPool, static_cast< uint32_t >( _commandBuffers.size() ), _commandBuffers.data() );
    vkDestroyPipeline( _logicalDevice, _graphicsPipeline, allocator );
    vkDestroyPipelineLayout( _logicalDevice, _pipelineLayout, allocator );
    vkDestroyRenderPass( _logicalDevice, _renderPass, allocator );

    for( size_t i = 0; i < _swapChainImageViews.size(); i++ ) {
      vkDestroyImageView( _logicalDevice, _swapChainImageViews[i], allocator );
    }

    vkDestroySwapchainKHR( _logicalDevice, _swapChain, allocator );

    for( size_t i = 0; i < _swapChainImages.size(); i++ ) {
      vkDestroyBuffer( _logicalDevice, _uniformBuffers[i], allocator );
      vkFreeMemory( _logicalDevice, _uniformBuffersMemory[i], allocator );
    }

    vkDestroyDescriptorPool( _logicalDevice, _descriptorPool, allocator );

  }

  // Creates a vertex buffer.
  void createVertexBuffer() {

    VkDeviceSize bufferSize = sizeof( vertices[0] ) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // Creates a temporary staging buffer. Since we can't map memory directly to the most efficient memory type, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, we use a staging buffer first
    // to map memory, then copy the content of that to the real vertexBuffer using the copyBuffer function.
    createBuffer( bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

    // Maps the data pointer to the stagingBufferMemory adress, then copies memory from vertices to the data pointer (i.e. to the stagingBufferMemory adress),
    // and finally unmaps the data pointer.
    void* data;
    vkMapMemory(_logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data );
    memcpy( data, vertices.data(), static_cast< size_t >( bufferSize ) );
    vkUnmapMemory( _logicalDevice, stagingBufferMemory );

    // Creates a vertex buffer on device memory, the most optimal memory area most efficient for device access.
    createBuffer( bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory );

    // Copy the data from the staging buffer to the actual vertex buffer in device local memory.
    copyBuffer( stagingBuffer, _vertexBuffer, bufferSize );

    // Destroy and free the temporary staging buffer.
    const VkAllocationCallbacks* allocatorCallbacks = VK_NULL_HANDLE;
    vkDestroyBuffer( _logicalDevice, stagingBuffer, allocatorCallbacks );
    vkFreeMemory( _logicalDevice, stagingBufferMemory, allocatorCallbacks );

  }

  // Queries the physical device for different types of gpu memory and selects one.
  uint32_t findMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties ) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties( _physicalDevice, &memProperties );

    for( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ ) {
      if( (typeFilter & ( 1 << i )) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties ) {
        return i;
      }
    }

    throw std::runtime_error( "Failed to find suitable memory type!" );
  }

  void createBuffer( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory ) {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if( vkCreateBuffer( _logicalDevice, &bufferInfo, VK_NULL_HANDLE, &buffer ) != VK_SUCCESS ) {
      throw std::runtime_error( "Failed to create vertex buffer!" );
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements( _logicalDevice, buffer, &memRequirements );
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType( memRequirements.memoryTypeBits, properties );

    // Ideally you shouldn't allocate memory many times, since this is limited by maxMemoryAllocationCount physical device limit,
    // and can be as low as 4096 even on good GPUs. A costum allocator or the VulkanMemoryAllocator should be used instead to allocate many objects.
    if( vkAllocateMemory( _logicalDevice, &allocInfo, VK_NULL_HANDLE, &bufferMemory ) != VK_SUCCESS ) {
      throw std::runtime_error( "Failed to allocate vertex buffer memory!" );
    }

    vkBindBufferMemory( _logicalDevice, buffer, bufferMemory, 0 );
  }

  // Copy a buffer from srcBuffer to dstBuffer of size size.
  void copyBuffer( VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size ) {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = _commandPool;
    allocInfo.commandBufferCount = 1;

    // Create a command buffer for copying the other buffers. Copying buffers, just as drawing,
    // is done with command buffers.
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers( _logicalDevice, &allocInfo, &commandBuffer );

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    // Begin recording commands.
    if( vkBeginCommandBuffer( commandBuffer, &beginInfo ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to begin recording command buffer!" );
    }

    VkBufferCopy copyRegion = {};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;

    // Copy command.
    vkCmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion );

    // End command recording.
    vkEndCommandBuffer( commandBuffer );

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    // submit the commands to the queue and wait for it to finish.
    vkQueueSubmit( _graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE );
    vkQueueWaitIdle( _graphicsQueue );

    // Free the command buffer used for copying.
    vkFreeCommandBuffers( _logicalDevice, _commandPool, 1, &commandBuffer );
  }

  // Creates index buffers to use for rendering.
  void createIndexBuffer() {
    VkDeviceSize bufferSize = sizeof( indices[0] ) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // Create the staging buffer
    createBuffer( bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory );

    // Copy the data from the indices to the stagingBuffer.
    void* data;
    vkMapMemory( _logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data );
    memcpy( data, indices.data(), ( size_t )bufferSize );
    vkUnmapMemory( _logicalDevice, stagingBufferMemory );

    // Create the index buffer.
    createBuffer( bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexBufferMemory );

    // Cop the data from the staging buffer to the index buffer.
    copyBuffer( stagingBuffer, _indexBuffer, bufferSize );

    const VkAllocationCallbacks* allocatorCallbacks = VK_NULL_HANDLE;
    vkDestroyBuffer( _logicalDevice, stagingBuffer, allocatorCallbacks );
    vkFreeMemory( _logicalDevice, stagingBufferMemory, allocatorCallbacks );
  }

  void createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Notifies that this ubo will act on the vertex shader stage.
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if( vkCreateDescriptorSetLayout( _logicalDevice, &layoutInfo, VK_NULL_HANDLE, &_descriptorSetLayout ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create descriptor set layout!" );
    }
  }

  // Creates uniform buffers.
  void createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof( UniformBufferObject );
    _uniformBuffers.resize( _swapChainImages.size() );
    _uniformBuffersMemory.resize( _swapChainImages.size() );

    for( size_t i = 0; i < _swapChainImages.size(); i++ ) {
      createBuffer( bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _uniformBuffers[i], _uniformBuffersMemory[i] );
    }
  }

  // Updates the uniform buffers.
  void updateUniformBuffers( const uint32_t imageIndex) {
    static std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>( currentTime - startTime ).count();

    UniformBufferObject ubo = {};
    // Rotate the model around the z-axis 90 degress as time progresses.
    ubo.model = glm::rotate( glm::mat4( 1.0f ), time * glm::radians( 90.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );

    // Look at the geometry from "above" at 45 deg angle.
    ubo.view = glm::lookAt( glm::vec3( 2.0f, 2.0f, 2.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );

    // 45 deg vertical field of view. Aspect ratio is swapchain extent width / height.
    ubo.proj = glm::perspective( glm::radians( 45.0f ), _swapChainExtent.width / static_cast< float >( _swapChainExtent.height ), 0.1f, 10.0f );
    // glm, designed for OpenGL, has y-coordinate flipped upside down. Compensate for this change.
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory( _logicalDevice, _uniformBuffersMemory[imageIndex], 0, sizeof( ubo ), 0, &data );
    memcpy( data, &ubo, sizeof( ubo ) );
    vkUnmapMemory( _logicalDevice, _uniformBuffersMemory[imageIndex] );
  }

  // Creates descriptor pool necessary to create the descriptor sets, in turn used for the uniform buffers.
  void createDescriptorPool() {
    VkDescriptorPoolSize poolSize = {};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast< uint32_t >( _swapChainImages.size() );

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast< uint32_t >( _swapChainImages.size() );

    if( vkCreateDescriptorPool( _logicalDevice, &poolInfo, VK_NULL_HANDLE, &_descriptorPool ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create descriptor pool!" );
    }
  }

  // Create descriptor sets
  void createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts( _swapChainImages.size(), _descriptorSetLayout );
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _descriptorPool;
    allocInfo.descriptorSetCount = static_cast< uint32_t >( _swapChainImages.size() );
    allocInfo.pSetLayouts = layouts.data();

    // Resize the descriptor sets to correct size, and allocate it.
    _descriptorSets.resize( _swapChainImages.size() );
    if( vkAllocateDescriptorSets( _logicalDevice, &allocInfo, _descriptorSets.data() ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to allocate descriptor sets!" );
    }

    // configure the descriptor sets.
    for( size_t i = 0; i < _swapChainImages.size(); i++ ) {
      VkDescriptorBufferInfo bufferInfo = {};
      bufferInfo.buffer = _uniformBuffers[i];
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof( UniformBufferObject );

      VkWriteDescriptorSet descriptorWrite = {};
      descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrite.dstSet = _descriptorSets[i];
      descriptorWrite.dstBinding = 0; // The binding location we gave it in the shader.
      descriptorWrite.dstArrayElement = 0; // Descriptor is not an array, but a struct, so just put 0.
      descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrite.descriptorCount = 1;
      descriptorWrite.pBufferInfo = &bufferInfo;
      descriptorWrite.pImageInfo = nullptr;
      descriptorWrite.pTexelBufferView = nullptr;

      vkUpdateDescriptorSets( _logicalDevice, 1, &descriptorWrite, 0, nullptr );
    }
  }

  // --- Member variables ---
  GLFWwindow* _window;
  VkInstance _instance;
  VkSurfaceKHR _surface;
  VkPhysicalDevice _physicalDevice;
  VkDevice _logicalDevice;
  VkQueue _graphicsQueue;
  VkQueue _presentationQueue;

  // Graphics pipeline and swapchain variables
  VkSwapchainKHR _swapChain;
  std::vector<VkImage> _swapChainImages;
  VkFormat _swapChainImageFormat;
  VkExtent2D _swapChainExtent;
  std::vector<VkImageView> _swapChainImageViews;
  VkRenderPass _renderPass;
  VkDescriptorSetLayout _descriptorSetLayout;
  VkPipelineLayout _pipelineLayout;
  VkPipeline _graphicsPipeline;
  std::vector<VkFramebuffer> _swapChainFramebuffers;
  VkDescriptorPool _descriptorPool;
  std::vector<VkDescriptorSet> _descriptorSets;
  VkCommandPool _commandPool;
  std::vector<VkCommandBuffer> _commandBuffers;

  // Buffer variables
  VkBuffer _vertexBuffer;
  VkDeviceMemory _vertexBufferMemory;
  VkBuffer _indexBuffer;
  VkDeviceMemory _indexBufferMemory;
  std::vector<VkBuffer> _uniformBuffers;
  std::vector<VkDeviceMemory> _uniformBuffersMemory;

  // Misc
  size_t _currentFrame;
  bool _framebufferResized;

  // Synchronization variables
  std::vector<VkSemaphore> _imageAvailableSemaphore;
  std::vector<VkSemaphore> _renderFinishedSemaphore;
  // Fences that synchronize the CPU with the GPU. Prevents frames to be sent from CPU -> GPU if the GPU is not done processing previous frame(s).
  std::vector<VkFence> _inFlightFences;
  // Prevent swap-chains from being rendered while in-flight.
  std::vector<VkFence> _imagesInFlight;

};

int main() {
  HelloTriangleApplication app;

  try {
    app.run();
  } catch ( const std::exception& e ) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}