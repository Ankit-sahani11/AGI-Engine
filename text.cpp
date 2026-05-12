#include <iostream>
#include <vulkan/vulkan.h>
#include <vector>

using namespace std;

struct Vulkan_App {
    VkInstance instance;

    Vulkan_App() {
        // 1. Application Info setup
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Artificial General Intelligence  App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Unreal Engine 100";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_1; // bhai mene 1.3 kiya tabhi bhi code sucess ful run ho rh hai  

        // 2. Instance Create Info setup
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

        if (result != VK_SUCCESS) {
            cout << "Error throws  " << result << endl;
        } else {
            cout << "Vulkan Instance Created!" << endl;
        }
    }void pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        cout << "Bhai, tere phone mein Vulkan GPU hi nahi hai!" << endl;
        return;
    }

    vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    cout << "Total GPUs found: " << deviceCount << endl;
   VkPhysicalDeviceProperties deviceProperties;
vkGetPhysicalDeviceProperties(devices[0], &deviceProperties);

uint32_t apiVer = deviceProperties.apiVersion;

cout << "Naam: " << deviceProperties.deviceName << endl;
cout << "Vulkan API: " 
     << VK_VERSION_MAJOR(apiVer) << "." 
     << VK_VERSION_MINOR(apiVer) << "." 
     << VK_VERSION_PATCH(apiVer) << endl;
    uint32_t queueFamilyCount = 0;
vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queueFamilyCount, nullptr);

vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queueFamilyCount, queueFamilies.data());

cout << "Total Queue Families found: " << queueFamilyCount << endl;

for (int i = 0; i < queueFamilyCount; i++) {
    cout << "Family " << i << ": ";
    if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) cout << "Graphics ";
    if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) cout << "Compute ";
    cout << "| Count: " << queueFamilies[i].queueCount << endl;
}

}


};
int main(){
   Vulkan_App app;
   app.pickPhysicalDevice();
   return 0;
}
