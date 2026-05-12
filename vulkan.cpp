#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

int main() {
    // 1. Application ki Information (Optional par achhi practice hai)
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Ankit_Vulkan_Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Ankit_Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1; // Tere phone ke liye 1.1 best hai

    // 2. Vulkan Instance Create karna (The Foundation)
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    VkInstance instance;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cout << "Bhai, Vulkan Instance fail ho gaya!" << std::endl;
        return -1;
    }
    std::cout << "Vulkan Instance: SUCCESS! 🚀" << std::endl;

    // 3. Apne Phone ke GPU ki details check karna
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::cout << "Total GPU found: " << deviceCount << std::endl;

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        std::cout << "GPU Name: " << deviceProperties.deviceName << std::endl;
        std::cout << "Driver Version: " << deviceProperties.driverVersion << std::endl;
    }

    // Cleanup
    vkDestroyInstance(instance, nullptr);
    return 0;
}
