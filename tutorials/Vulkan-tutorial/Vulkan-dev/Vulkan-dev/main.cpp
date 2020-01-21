#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <set>
#include <algorithm>
#include <cstdint>

const int WIDTH = 800;
const int HEIGHT = 600;

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

// Setup Vulkan validation layers if compiling for debug, else not.
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


class HelloTriangleApplication {
public:
  HelloTriangleApplication() :
    _window( nullptr ),
    _physicalDevice( VK_NULL_HANDLE ) {};

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
  // Member functions

  // Initializes and creates a window to display the graphics in.
  void initWindow() {
    glfwInit();

    // Tell glfw not to create an OpenGL context, since that's what glfw usually does otherwise.
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    // Don't resize window now
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
    _window = glfwCreateWindow( WIDTH, HEIGHT, "Vulkan", nullptr, nullptr );
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
    createGraphicsPipeline();
  };

  void mainLoop() {
    while( !glfwWindowShouldClose( _window ) ) {
      glfwPollEvents();
    }
  };

  // Cleans up various things after application is finished running
  void cleanup() {
    // Cleanup Vulkan-related resources create explicitly by the user.
    vkDestroySwapchainKHR( _logicalDevice, _swapChain, nullptr );
    vkDestroySurfaceKHR( _instance, _surface, nullptr );
    vkDestroyInstance( _instance, nullptr );
    vkDestroyDevice( _logicalDevice, nullptr );
    for( auto imageView : _swapChainImageViews ) {
      vkDestroyImageView( _logicalDevice, imageView, nullptr );
    }

    // Cleanup glfw things
    glfwDestroyWindow( _window );
    glfwTerminate();
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
    appInfo.pApplicationName = "Hello triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.pEngineName = "No engine";
    appInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.apiVersion = VK_API_VERSION_1_0;


    // Check which instance extensions that are supported and print them out.
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, nullptr );
    std::vector<VkExtensionProperties> extensions( extensionCount );
    vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, extensions.data() );

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

    VkResult result = vkCreateInstance( &instanceCreateInfo, /* allocationCallbacks */ nullptr, &_instance );

    if( result != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create instance!" );
    }
  }

  // Creates a surface in Vulkan using glfw. This is where Vulkan (which isn't platform agnostic)
  // connects with the glfw window used to present graphics.
  void createSurface() {
    if( glfwCreateWindowSurface( _instance, _window, nullptr, &_surface ) != VK_SUCCESS ) {
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
      // Physical device suits out needs.
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

    if( vkCreateDevice( _physicalDevice, &createInfo, nullptr, &_logicalDevice ) != VK_SUCCESS ) {
      throw std::runtime_error( "failed to create logical device!" );
    }

    vkGetDeviceQueue( _logicalDevice, indices.presentationFamily.value(), 0, &_graphicsQueue );
  }

  // Makes sure that all the required device extensions specified by deviceExtensions are supported.
  bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, nullptr );
    std::vector<VkExtensionProperties> availableExtensions( extensionCount );
    vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, availableExtensions.data() );

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
    vkGetPhysicalDeviceSurfaceFormatsKHR( device, _surface, &formatCount, nullptr );
    if( formatCount != 0 ) {
      details.formats.resize( formatCount );
      // Query for surface format.
      vkGetPhysicalDeviceSurfaceFormatsKHR( device, _surface, &formatCount, details.formats.data() );
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR( device, _surface, &presentModeCount, nullptr );
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
      if( availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR ) {
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
      VkExtent2D actualExtent = { WIDTH, HEIGHT };

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

    QueueFamilyIndices indices = findQueueFamilies( _physicalDevice );
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentationFamily.value() };

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; // Could be more than 1 if we want stereo graphics.
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // VK_IMAGE_USAGE_TRANSFER_DST_BIT could be used when we want post-processing.
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
    
    if( vkCreateSwapchainKHR( _logicalDevice, &createInfo, nullptr, &_swapChain ) != VK_SUCCESS ) {
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
      if( vkCreateImageView( _logicalDevice, &createInfo, nullptr, &_swapChainImageViews[i] ) != VK_SUCCESS ) {
        throw std::runtime_error( "failed to create image views!" );
      }
    }
  }

  void createGraphicsPipeline() {

  }

  // Member variables
  GLFWwindow* _window;
  VkInstance _instance;
  VkSurfaceKHR _surface;
  VkPhysicalDevice _physicalDevice;
  VkDevice _logicalDevice;
  VkQueue _graphicsQueue;
  VkQueue _presentationQueue;
  VkSwapchainKHR _swapChain;
  std::vector<VkImage> _swapChainImages;
  VkFormat _swapChainImageFormat;
  VkExtent2D _swapChainExtent;
  std::vector<VkImageView> _swapChainImageViews;

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