#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <optional>
#include <set>

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
  };

  void mainLoop() {
    while( !glfwWindowShouldClose( _window ) ) {
      glfwPollEvents();
    }
  };

  // Cleans up various things after application is finished running
  void cleanup() {
    // Cleanup Vulkan-related things
    vkDestroySurfaceKHR( _instance, _surface, nullptr );
    vkDestroyInstance( _instance, nullptr );
    vkDestroyDevice( _logicalDevice, nullptr );

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

  // Member variables
  GLFWwindow* _window;
  VkInstance _instance;
  VkSurfaceKHR _surface;
  VkPhysicalDevice _physicalDevice;
  VkDevice _logicalDevice;
  VkQueue _graphicsQueue;
  VkQueue _presentationQueue;

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