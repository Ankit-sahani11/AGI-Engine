#include<iostream>
#include<vulkan/vulkan.h>
#include<vector>
using namespace std;

struct Vulkan_Instance{
    Vulkan_Instance(){
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "SASTA Application";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Unreal Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_1;

    }

};

int main(){
  return 0;
}
