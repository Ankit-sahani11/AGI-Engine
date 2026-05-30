#define VK_USE_PLATFORM_ANDROID_KHR
#include <fstream>
#include<string>
#include<iostream> 
#include<vector>
#include <vulkan/vulkan.h> 
#include<android/native_window.h> 
#include <android/native_window_jni.h> 

using namespace std;
struct vulkan{
VkInstance instance;// instance store karane ke liye 
VkSurfaceKHR surface;//surface set kiya VkPhysicalDevice 
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;//physical device ke liye 
VkDevice device;//Logical devcie funcation ko logical devcie ke liye call kiya
VkQueue graphicsQueue; // ye  queue ki index ke liye hai 
int Index_QueueFamily = -1;// ye queue famliy ke index ke liye hai 
VkSwapchainKHR swapChain;// real swap banaya swapChain)
vector<VkImageView> swapChainImageViews; //orignal image ko store karke dikhne ke liye hai 
VkRenderPass renderPass;//render pass ke liye
VkShaderModule shaderModule; // ye sheader module ke liye hai 
VkShaderModule fragModule; // ye real from hai jo vulkan lega fragment shader ka
VkPipelineShaderStageCreateInfo shaderStages[2]; // ye gpu ko frag and vert shader file dets hai 
VkPipelineLayoutCreateInfo pipelineLayoutInfo{};// ye pipe line layout ke liye
VkPipelineLayout pipelineLayout;// ye layout reasult ko store karane ke liye hai 
VkPipeline graphicsPipeline;// te graphics pipe line ke liye hai 
vector<VkFramebuffer> swapChainFramebuffers;// ye frame buffer ke liye hai
VkCommandPool commandPool;// ye command pool save karne ke liye hai 
VkCommandBuffer commandBuffer;//ye khali page commands buffer ke liye hai 
VkCommandBufferBeginInfo beginInfo;// ye Frane buffer record karane ke liye hai
VkSemaphore imageAvailableSemaphore;//ye ek chota signal hai jo gpu me chalata hai ek task katam hone par duahra task
// imageAvailableSemaphore; ye batata hai ki gou ne image swap chain se utha li hai 
// renderFinishedSemaphore; ye batata hai ki drawing katam ho gai hai 
VkSemaphore renderFinishedSemaphore;// ye vahi prani hai
// ye ek bada signal hai to cpu to gpu jata hai 
VkFence inFlightFence;// aur check karata hai ki hawa me drawing processing to nhi ho rahi 
    ~vulkan() {
         cout << "\n--- Cleaning up Engine ---" << endl;
         // Safety Check: Pehle check karo device exist karta hai ya nahi
         if (device == VK_NULL_HANDLE) return;
         // 1. Shaders (Hamesha device se pehle)
         if (shaderModule != VK_NULL_HANDLE) vkDestroyShaderModule(device, shaderModule, nullptr);
         if (fragModule != VK_NULL_HANDLE) vkDestroyShaderModule(device, fragModule, nullptr);
         //abb me pipeline ko delete karuga
         if(graphicsPipeline != VK_NULL_HANDLE) vkDestroyPipeline(device, graphicsPipeline, nullptr);
         // 2. Pipeline aur Layout
         if (pipelineLayout != VK_NULL_HANDLE) vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
         // Agar graphicsPipeline banayi hai toh: 
         // vkDestroyPipeline(device, graphicsPipeline, nullptr);
         // 3. Render Pass
         if (renderPass != VK_NULL_HANDLE) vkDestroyRenderPass(device, renderPass, nullptr);
         // 4. Vie Swap chain ko delete
         auto start = swapChainImageViews.data(); 
         // Aakhri element ke address mein 1 element ki jagah aur add karne par 'end' milta hai
         auto end = &swapChainImageViews.back() + 1; 
         int count = end - start; // Ye aapko total elements de dega
         for(auto  i = 0; i < count;i++){
              if(swapChainImageViews[i] != VK_NULL_HANDLE){
                  vkDestroyImageView(device, swapChainImageViews[i], nullptr);
                }
             }
         if (swapChain != VK_NULL_HANDLE) vkDestroySwapchainKHR(device, swapChain, nullptr);
         // 5. Surface (Ye Instance ke handle se hota hai, Device se nahi!)
         if(surface != VK_NULL_HANDLE) vkDestroySurfaceKHR(instance, surface, nullptr);
         // 6. Logical Device (Ab baaki sab khatam, device ko bye-bye)
         if(device != VK_NULL_HANDLE) vkDestroyDevice(device, nullptr);
         // 7. Instance (Aakhiri step)
         if (instance != VK_NULL_HANDLE) vkDestroyInstance(instance, nullptr);
         }

     vulkan(){
        // Set a Infor of app
        VkApplicationInfo app{}; // Create a  From Name take app
        app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // Selete a Structure 
        app.pApplicationName = "Something app";// Take as name of game /app
        app.applicationVersion = VK_MAKE_VERSION(1,0,0);//take a versiom of app
        app.pEngineName = " AGI ENGINE";// Inilazging a engine name 
        app.engineVersion = VK_MAKE_VERSION(1,0,0);// Inilaizing a version of engine 
        app.apiVersion = VK_API_VERSION_1_1;// set api version
        // Setting a infor from big from
        VkInstanceCreateInfo Information{};// fill a big from
            Information.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;//stelect a stureture 
            Information.pApplicationInfo = &app;//Copy a small from in big from
            const char* extensions[] = {
               "VK_KHR_surface",          // Power 1: Vulkan ko "Surface" (parda) ke baare mein samjhata hai.
               "VK_KHR_android_surface"   // Power 2: Vulkan ko batata hai ki ye parda "Android" ka hai.
            };
            Information.enabledExtensionCount = 2;
            Information.ppEnabledExtensionNames = extensions;
        // Creating a Instances
            VkResult result = vkCreateInstance(&Information,nullptr,&instance);// Create a Instance 
            if(result != VK_SUCCESS){// checking 
                cout<< "Fail"<<endl;
            }else{
                cout<<"Instance created"<<endl;
                physical_devices();
                logical_device();
                render_pass(); 
                Create_garphics_pipeline();
                Framebuffers();
                Command_buffer(); 
                Record_commands(1);
                Syn_GPU_CPU();                            
            }       
   }void physical_devices() {
    uint32_t deviceCount = 0; // GPU ki sankhya store karne ke liye variable
    // Pehle sirf ye pata karo ki system mein kitne GPU hain jo Vulkan support karte hain
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) { // Agar ek bhi GPU nahi mila
        cout << "Koi GPU Vulkan support nahi karta!" << endl;
        return; // Function se bahar nikal jao
    }

    vector<VkPhysicalDevice> devices(deviceCount); // Milne waale GPUs ko store karne ke liye list (vector)
    // Ab saare mile hue GPUs ki handle details is vector mein bhar do
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    VkPhysicalDevice bestDevice = VK_NULL_HANDLE; // Sabse acche GPU ko save karne ke liye khali container
    int maxScore = -1; // Comparison ke liye ek shuruati (minimum) score

    for (const auto& device : devices) { // System ke har ek GPU ko ek-ek karke check karo
        
        VkPhysicalDeviceProperties deviceProperties; // GPU ka naam, type aur limits store karne ke liye struct
        vkGetPhysicalDeviceProperties(device, &deviceProperties); // GPU ki basic info fetch karo

        VkPhysicalDeviceFeatures deviceFeatures; // GPU ke special features (jaise Ray Tracing) store karne ke liye struct
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures); // GPU ke supported features fetch karo

        cout << "Checking GPU: " << deviceProperties.deviceName << endl; // Console pe GPU ka naam print karo


        int currentScore = 0; // Is specific GPU ke liye rating/score shuru karo

        // Agar GPU ek Dedicated Graphics Card (Discrete) hai, toh usse high priority do
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            currentScore += 1000;
        }

        // Agar GPU Processor ke saath built-in (Integrated) hai, toh score kam rakho
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            currentScore += 100;
        }

        // Agar tumhare game/app ko Geometry Shader chahiye aur GPU use support nahi karta
        if (!deviceFeatures.geometryShader) {
            currentScore = 0; // Toh is GPU ko reject kar do (score zero)
        }

        // Agar is GPU ka score pichle waale se behtar hai
        if (currentScore > maxScore) {
            maxScore = currentScore; // Naya high score set karo
            bestDevice = device; // Is GPU ko apna "Best Device" maan lo
            physicalDevice = bestDevice; // Global variable mein save kar liya
        }
    }

    if (bestDevice != VK_NULL_HANDLE) { // Agar loop khatam hone par koi dhang ka GPU mil gaya
        cout << "Sabse badiya GPU select ho gaya!" << endl;
    } else {
        cout << "Koi dhang ka GPU nahi mila." << endl; // Requirement match nahi hui
    }
}

      void logical_device(){
         uint32_t queueFamilyCount = 0;//QueueFamily store karaen ke liye
         vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);// Queue Family ko count kiya
         if(queueFamilyCount == 0){cout<<"Family not found "<<endl;return ; }//check kiya ki Queue count 0 to nhi 
         vector<VkQueueFamilyProperties>QueueFamily (queueFamilyCount);//ek list banaya jishme queeu propertiess daali aur ushki length queue <uont rakha 
         vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,QueueFamily.data());//properties pass ki aur parameter me vector pass kiya 
         for(int i = 0; i < queueFamilyCount; i++){//ek loop chalaya jo quess ki length tak jati hai 
             if(QueueFamily[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){// aur check kiya ques ke kis length pe graphics queue hai
                Index_QueueFamily = i;//aue Index ko pointer kar diya
                break;//loop ko rokne ke liye
              }
         }
         //chota from bhar raha hu
         float queuePriority = 1.0f;// Gpu ke pass task ke liye priorty hoti hai mene 1.0 set kiya uar float zaruri hai double nhi chalega 
         VkDeviceQueueCreateInfo queueCreateInfo{}; //Ek structur bananya like from jishme me queue ka deatail likhuga aur {} laga kar zero set kiya sari thing good  practicea
         queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;//Struture type set kiya
         queueCreateInfo.queueFamilyIndex = Index_QueueFamily; // Jo tumne loop se nikala
         queueCreateInfo.queueCount = 1;// Queue ke 1 Count Something(Math ,graphic) etc ko set kiya 1 ko
         queueCreateInfo.pQueuePriorities = &queuePriority;//ye ek pointer hai jo Priority ke liye use ho raha hai aur set kiya 
         VkDeviceCreateInfo  Device_Info{};// ek from banaya Jishme logical devcice ki Info filll karuga   
         Device_Info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;//Ek structure type set kiya Imfo ka
         Device_Info.pQueueCreateInfos = &queueCreateInfo;// Mene queue family ke liye From bhara tha ushko ish pointer par?point kar va diya  ye pointer puch raha hai konsha queue use karu jawab de diya mene
         Device_Info.queueCreateInfoCount = 1;//Simple Matlab: Kitni queue settings hain? Logic: Humne sirf ek hi rasta (Graphics Queue) manga hai, isliye yahan 1 likha hai.
         VkPhysicalDeviceFeatures deviceFeatures{}; // vulkan ko bataya ne ke liye kitne featrue chahiye abhi iahko chodata hu basic start ke liwy
        const char* deviceExt[] = {
            "VK_KHR_swapchain"
        };//Ek list banai jishme mene swap chain enable kiya 
            Device_Info.enabledExtensionCount = 1;// aur count bataya
            Device_Info.ppEnabledExtensionNames = deviceExt;// aur vulkan ko bataya mene swap chain enable kiya hai 
            Device_Info.pEnabledFeatures = &deviceFeatures;//ye ek pointer hai jo puch raha hainki kitne feature chahiye tumhe hamane ishke liye froms bhara tha ushka address de diya
            if(vkCreateDevice(physicalDevice, &Device_Info, nullptr, &device) != VK_SUCCESS){return;}/* Main Action: Yahan device "paida" hota hai. Arguments: * physicalDevice: Asli GPU (Phone ka chip). &createInfo: Hamara bhara hua form.nullptr: Memory allocation ke liye (Vulkan default use karega).&device: Address jahan naya device "save" hoga. Logic: Agar function VK_SUCCESS return nahi karta, toh matlab koi gadbad hui hai (jaise driver support nahi kar raha).*/ 
                 cout<<"Logical device sucess"<<endl;
                 vkGetDeviceQueue(device, Index_QueueFamily, 0, &graphicsQueue);
                 /* 1. vkGetDeviceQueue (The Action)
​                 Ye Vulkan ka ek "Handle Getter" function hai. Yaad rakh, ye koi naya object create nahi karta. Ye sirf us purani line ka result maangta hai jo tune VkDeviceCreateInfo mein bhari thi.
​                 Logic: Tune Logical Device banate waqt bola tha "Mujhe ek Graphics Queue chahiye". Ab ye function usi queue ka "Address/Handle" dhoond kar lata hai.
​                 2. device (Kahan se?)
​                 Ye tera Logical Device hai jo tune abhi banaya tha. Ye batata hai ki "Bhai, is specific factory ke parking lot mein jao."
​                 3. Index_QueueFamily (Kaunsa rasta?)
​                 Tujhe yaad hoga, tune ek loop chalaya tha VK_QUEUE_GRAPHICS_BIT dhoondne ke liye.
​                 Logic: Ek GPU mein bahut saari families hoti hain (Graphics, Compute, Transfer). Ye parameter GPU ko batata hai ki "Mujhe wahi queue chahiye jo Graphics draw karna jaanti ho."
​                 4. 0 (Queue Index)
​                 Ek hi Queue Family ke paas bahut saari queues ho sakti hain (jaise ek line mein 5 trucks khade hain).
​                 Logic: Tune Device_Info.queueCount = 1 set kiya tha, isliye hum yahan 0 likhte hain (pehle aur iklaute truck ko uthane ke liye).
​                 5. &graphicsQueue (Kahan store karun?)
​                 Ye tera wo variable hai jo tune Struct mein banaya hai.
​                 Logic: Vulkan us "Truck" (Queue) ki chaabi is variable mein daal dega. Ab jab bhi tujhe drawing bhejni hogi, tu graphicsQueue ka use karega.
                 */
     }
     void create_surface(ANativeWindow* window) {
         VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo{};
         surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
         surfaceCreateInfo.window = window; // Ye Java/JNI se aayega
         if (vkCreateAndroidSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) { //Tune Vulkan ko bola, "Bhai, ye CreateInfo lo aur jo surface banega use mere surface variable mein bhar do." aur agar na ho to if block me excut kar do
            cout << "Surface creation fail!" << endl;
        } else {
            cout << "Surface ban gaya!" << endl;
        }
    }
     void swap_chain(){
         VkSurfaceCapabilitiesKHR capabilities;//ek variable banaya ho VkSurfaceCapabilitiesKHR ki help se phone ki length and breath pata karega 
         vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);//aab phone ka physical device check me display size deja 
         VkSwapchainCreateInfoKHR createInfo{};//ek struct banaya aur ushka swap chain k# khr Info ke liye use kiya like a frome 
         createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;//aur vulkan ko bataya kon se type ka struct hai
         createInfo.surface = surface;//vulkan ko bataya ki kaha suface hai ye java ne diya
         createInfo.minImageCount = 2; //minImageCount = 2: Matlab do images. Ek jo screen par dikh rahi hai, aur ek jispar GPU abhi drawing kar raha hai. Isse "Flickering" nahi hoti.
         createInfo.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;//imageFormat: R8G8B8A8 matlab Red, Green, Blue, aur Alpha (Transparency) sab 8-bit ke hain. Ye mobile ka sabse fast format hai.
         createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;//imageColorSpace: SRGB ka matlab hai colors ekdum natural dikhenge, zyada chamkile ya feeke nahi.
         createInfo.imageExtent = capabilities.currentExtent; // Jish size ki display hai ush size ka hi display rakho vulkan ko bataya
         createInfo.imageArrayLayers = 1;//Hum koi 3D VR game nahi bana rahe, isliye sirf 1 layer kaafi hai.
         createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;//vulkan bohut jidi hota hai ish liyr mene bataya ki tuje  kya draw karana hai
         // Optimization Flags jo mene soche the
         createInfo.preTransform = capabilities.currentTransform; //preTransform: Phone agar landscape hai, toh GPU bina extra time waste kiye image ko waisa hi banayega.
         createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;//OPAQUE matlab Mera game transparent nahi hoga. Isse Android system ko mehnat kam karni padti hai.
         createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // Ye V-Sync hai. Ye battery bachata hai aur "Screen Tearing" (image ka beech se katna) rokta hai.
         createInfo.clipped = VK_TRUE;//Agar game ke upar koi notification aa gaya, toh Vulkan un pixels ko render nahi karega jo dikh nahi rahe. Pure Optimization!
         // Swapchain paida karo
         if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {//aur check kiya ki vk sucess mila ki nhi
             cout << "Swapchain setup fail!" << endl;
         } else {
             cout << "Swapchain setup Success! Hardware optimized." << endl;
         }
      }
      //VkImage asli data (pixels) hota hai, aur VkImageView us data ko dekhne ka ek "nazariya" ya "chashma" hai. GPU ko batana padta hai ki us image ko kaise padhna hai.
      void create_image_views() {
        // 1. Pehle pata karo swapchain mein kitni images hain
        // ye gpu ko batata hai image kese dekhna hai 
        uint32_t imageCount;//Ek dabba banayaswap chain ye aaye image count rakhne ke liye.
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr); // abb mene vulkan se pucha Vulkan se pucha, "Bhai, Swapchain mein kitni photos hain?" Usne count imageCount mein bhar diya (mobile pe aksar 2 ya 3 hota hai).
        vector<VkImage> swapChainImages(imageCount);//aur image store karne ke liye ek vector banaya jo image kitna hai utne size ka hai
        vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());// Ab Vulkan ne asli images ke handles is list mein daal diye. 
        // Phase 2: Har Image ke liye "Chashma" (View) banana
        swapChainImageViews.resize(imageCount);//Mene apni main list (jo tumne struct mein declare ki hai) ko utna bada kar diya jitni images hain. Ab loop shuru hota hai: for (size_t i = 0; i < imageCount; i++)
        for (size_t i = 0; i < imageCount; i++) {// ye loop hai jo image count tak jata hai 
            VkImageViewCreateInfo viewInfo{};// ye ek struct Jo image view karne ki power deta hai
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;//ye sturct type hai
            viewInfo.image = swapChainImages[i];// mene vulkan ko bataya ki ye Chashma konshi image ke liye hai i swap chain ka matalab ye swap chain ke i tak ke sare image ko dekhega 
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // ye puch raha hai ki kesa image hai 2d ya 3d 
            viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM; // Wahi rang bharuga jo swap chain me hai ye basilacly puch raha hai kish rang ke image ko dekhu mera swap chain me jo image hai wahi
            // Colors ko kaise map karna hai (Identity matlab normal)
            // 2. Color Shuffling (components) viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;: "Identity" ka matlab hai "No Change". Red ko Red hi rehne do, use Blue se badlo mat. (Identity = Jaisa hai waisa).
            viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            // Image ka kaunsa part dikhana hai (Standard setting)
            // 3. Image ka hissa batana (subresourceRange) Ye sabse important part hai. Ye GPU ko batata hai ki image ke kitne gehraai tak jana hai:
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;// me color render kar rahe hain, Depth (gehraai) nahi.
            // 0 aur levelCount = 1: Me koi texture zoom-in/out (Mipmaps) nahi kar rahe, sirf main image chahiye.
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            //koi VR ya 3D layers nahi use kar rahe, sirf 1 layer hai.
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;
            if (vkCreateImageView(device, &viewInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {//Yahan real action hota hai. Vulkan ko device, bhara hua form, aur nullptr (no extra settings) diye. Usne i number wali image ke liye view bana kar swapChainImageViews[i] mein save kar diya.
                cout << "Image View fail ho gaya bhai!" << endl;
            }
         }
         cout << "Image Views Ready!" << endl;
     }
     void render_pass(){
          //Step 1: Deewar (Attachment Description) Pehle tumne colorAttachment banaya. Ye sirf deewar ki description hai (kaunsa color, kitne samples). Abhi tak iska koi naam ya ID nahi hai, ye bas ek structure hai.
          // Ishme logic yahi hai ki frist frame ki halta batao aur last ki aur khuch nhi 
          VkAttachmentDescription colorAttachment{};//sabse pehele me ek struct bana raha hu jo pixel aur ushki property describe karega like a frome 
          colorAttachment.format =VK_FORMAT_R8G8B8A8_UNORM;//me bata raha hu ke colure kese honge 
          colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;// jab gpu frame draw kar leta hai to ushke pass do raste hote hai ek vo koi care na kare fekh de aur dushra vo ram / vram me sve karle kyoki screen pe dikhana hai ish liye mene set kiya ram /vram me save karana  
          colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;// gpu kab frame draw karega to dushre frame ka kya hoga CLEAR ka matalab ushko chod dena
          colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;//matalab hai kab gpu draw kar lega to frame ko save karana 
          colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;//frane pe gpu mask hota hai mene idhar bola koi care nat usha karana DNOT CARE
          colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;// frame draw hone ke baad stencil buffer jo bhi bache koi care nat karana 
          colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;//UNDIfIDe hi sabse batabkam kar raah hai frame render hote samay ushki halat(layout) kya hoga mene ushem set kar diya mujhe purane frame se koi matalab nnhi ushse override kiya ja sakta hai 
          colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //Final Layout: Render pass khatam hone ke baad image kis halat mein honi chahiye? PRESENT_SRC_KHR ka matlab hai ki ab ye image taiyaar hai screen par "Present" (dikhane) ke liye.
          //Step 2: Ungli Dikhaana (Reference) Ab tumne banaya colorAttachmentRef. Isme tumne likha: colorAttachmentRef.attachment = 0; Ye 0 sabse bada khiladi hai. Ye keh raha hai: "Main us attachment ki baat kar raha hoon jo list mein 0 number par baitha hai."
          VkAttachmentReference colorAttachmentRef{};
          colorAttachmentRef.attachment = 0; // colorAttachment 
          colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Drawing ke waqt layout kaisa ho
          /* Flow Diagram (Asaan Bhasha Mein):
          colorAttachment \rightarrow Ek description hai.
          renderPassInfo.pAttachments \rightarrow Isne colorAttachment ko list mein 0 index par bithaya.
          colorAttachmentRef.attachment = 0 \rightarrow Isne kaha "Mujhe index 0 wala uthana hai."
          subpass.pColorAttachments \rightarrow Isne is Reference ko apne andar rakha taaki GPU ko pata chale ki drawing kahan karni hai.
          Link kisi variable ke naam se nahi, balki Array Index (0, 1, 2...) se banta hai.
          */
          //abb sub pass banayuga aur me render pass ko task me divede karege 
          VkSubpassDescription subpass{};// yaha emne ek sturct banaya jo render pass ko deivede karane ke liye
          subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;//ye line batati hai ki gpu ko only graphics draw karana hai
          subpass.colorAttachmentCount = 1;// screen par kitne bar paint karana hai idhar me 1 rakh raha hu kyoki mere pass ek screen hai br gogol me ye 2 hota hai 2 screen
          subpass.pColorAttachments = &colorAttachmentRef;//ye line gpu ko batati hai kaha paint karana hai
          // Step 3: Final Packing (Render Pass Creation)
          VkRenderPassCreateInfo renderPassInfo{};
          renderPassInfo.attachmentCount = 1;// idhar batate hai ki screen pe kitna bar paint hoag
          renderPassInfo.pAttachments = &colorAttachment; //ye ek pointer hai jo puch raha hai gpu ko kese aur kya draw karama hai
          renderPassInfo.subpassCount = 1;//ye bataya kitne screeen par game banana haib
          renderPassInfo.pSubpasses = &subpass; // aab subpass ko point kiya
          if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {//check kiya sucess ko
              cout << "Render Pass fail!" << endl;
          } else {
              cout << "Render Pass Success! Setup boring tha par kaam khatam." << endl;
          }
      }
      void Create_garphics_pipeline(){
          // Aab me real drawing karne ja raha hu 
          // Basic flow hai ki Data -> Vertex Shader -> Rasterization -> Fragment Shader -> Render Output Me ek ek point ko deatil me exapnlain karuga 
          // Pehele 3d model ka data 
          //strep 1 data dena 
          VkPipelineInputAssemblyStateCreateInfo inputAssembly{};// ye gpu se baatne ke liye hai ushse triangle etc draw karana hai
          inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;//ye struct ka tyep hai
          inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // ye gpu ko batata hai  ki ushse triangle draw karana hai na ki lines
          inputAssembly.primitiveRestartEnable = VK_FALSE;//ye batata hai ki ek triangle ke baad jagaha chod kar draw karana ye flase hota to gpu dono triangle mila kar draw karata 
          VkViewport displayPossion{};//ye batata hai ki display me kese aur kitna bada triangle dikhega
          displayPossion.x = 46.5;
          displayPossion.y = 6.666666f;// ye bata raha hai ki triangle screen ke kitne top - donw hoga
          displayPossion.width = 500.5f;// ye triangel ki wifmdth batayega
          displayPossion.height = 300.6f;//ye truiangle ki heigth
          displayPossion.minDepth = 0.0f; // Sabse paas wala point 0 mana jaye
          displayPossion.maxDepth = 1.0f; // Sabse door wala point 1 mana jaye
          // --- Step 1 ka bacha hua part ---
          VkRect2D scissor{};// ye ek struct(normal langugess me from hai jish me me screen ke bekar part ko chatne  ka frome likhuga 
          scissor.offset = {0, 0}; // offset: Iska hindi matlab hota hai "doori" ya "shuruat".{0, 0}: Ye (x, y) coordinates hain. (0,0) ka matlab hai screen ka bilkul top-left corner. GPU yahan se drawing check karna shuru karega.
          // Jitni Meri width aur height hai, utni hi yahan dalni hai
          scissor.extent.width = 500;  
          scissor.extent.height = 300;
          //dono line ka matalab  extent: Iska matlab hai "phailav" (size).Logic: me GPU ko bol rahe ho ki (0,0) se shuru karke 500 pixels right aur 300 pixels niche tak ki hi "kainchi" (scissor) chalao. Iske bahar agar koi pixel draw hone ki koshish karega, toh GPU usse turant dustbin mein daal dega. 
          // In dono ko ek "State" mein pack karna padega
          VkPipelineViewportStateCreateInfo viewportState{}; //VkPipeline: Kyunki ye Graphics Pipeline ka ek stage hai.CreateInfo: Vulkan mein jis bhi word ke piche ye hota hai, uska matlab hai ki hum ek "Application Form" bhar rahe hain jo baad mein pipeline banane ke kaam aayega.
          viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;//ye ushka type hi
          viewportState.viewportCount = 1;// ye gpu ko batata hai ki ushse ek screen pe hi draw karana hai 
          viewportState.pViewports = &displayPossion; //  ye ek pointer hai j( mujhe se puch raha hai ki gpu me kese aur kitan aur kya draw karuga 
          viewportState.scissorCount = 1;// hum ek screen par hai ish liye ek hi sesior 
          viewportState.pScissors = &scissor; // ye ek pointer hai jo mujhe puch raha hai ki mujhe kitna part katna hai
          // step 1 complete!

          //  step 2 staring 
          /* step 2 vertex sheader
          2. Vertex Shader: The Math Lab
          Yahan par model ke har point (vertex) par matrix multiplication hota hai.
          Model Transformation: Aapka character 3D space mein kahan khada hai (x, y, z).
          View Transformation: Camera kahan dekh raha hai.
          Projection: 3D world ko 2D screen par kaise "project" karna hai (Perspective).
          Output: Iska final result hota hai Clip Space coordinates. GPU check karta hai ki kaunse points screen ke bahar hain aur unhe "clip" (kaat) deta hai.
          */
          // code of step 2
          ifstream file("vert.spv", ios::ate |ios::binary);// vertex shader ko likh kar ushko cpp (cpu ) ko dene ke liye ek new  file likha tha ushko open kiya
          if(!file.is_open()){// aur check kiya ki file hai ki nhi
              cout<<"File reading fail"<<endl;
              return;//agar nhi to program exit
          } 
          size_t fileSize = (size_t) file.tellg();//aur ha to file ko size _t me convert karke kis byte par ho ushka pata do
          std::vector<char> buffer(fileSize);//ek buffer vector liya aur ushka size filesize itna rakh 
          file.seekg(0);//aur bola phir file ke starting me joa
          file.read(buffer.data(), fileSize);//aur j( mil padh kar buffer(vector) me store kar do
          // abb data to mil gaya ishse vulkan ko dene ke liye sturct VkShaderModuleCreateInfo createInfo{}; use karuga
          VkShaderModuleCreateInfo createInfo_vertex_shader{};//ye raha v( struct jo gpu k( dene fe
          createInfo_vertex_shader.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;//ye puch raha hai ki kis type ka struct bamana hai
          createInfo_vertex_shader.codeSize = buffer.size();// vulkan ko khuch ke liye imformation chahiye aur vulkan puch raha hai ki tumre file ka size kya hai mene buffer me file save ki thi to bata diya 
          createInfo_vertex_shader.pCode = reinterpret_cast<const uint32_t*>(buffer.data());// 
          /* Ye thoda technical dikhta hai par simple hai:
          buffer.data(): Hamare data ka shuruati pata (Address).
          uint32_t*: Vulkan ko shader ka data 4-byte ke tukdon (uint32) mein chahiye hota hai.
          reinterpret_cast: Ye C++ ko bolta hai, "Bhai, is char (1-byte) wale data ko uint32 (4-byte) ki nazar se dekho." Ye sirf dekhne ka nazariya badalta hai, data wahi rehta hai.
         */
          //abb me real vulkan ko from dunga ki mere liye ek sheader bana 
          vkCreateShaderModule(device, &createInfo_vertex_shader, nullptr, &shaderModule);//ye vo from hai aur jo bhi reasult nikla ushko sheader module me bhar do jonmene upar banaya tha
          // step 2 complete
          // step 3 restraraijation
         /*
         3. Rasterization: The Bridge
         Ye hardware ka fixed-function part hai (aap isse program nahi karte, ye automatic hota hai). Iska kaam hai 3D points ko 2D grid (pixels) mein convert karna.
         Triangle Setup: Teen points ko jod kar triangle banana.
         Scan Conversion: Triangle ke andar kaun-kaun se pixels aa rahe hain, unki list banana.
         Interpolation: Agar ek vertex "Red" hai aur doosra "Blue", toh rasterizer beech wale har pixel ke liye ek mix color calculate karta hai.
         */
         // starting step 3 in real  code
         
          //Ishme logic khuch nhi hai mere drawing (math points) ko screen ke pixels mein badalti hai. Iske bina GPU ko pata hi nahi chalega ki triangle "solid" hai ya sirf 3 dande (lines).
    
          VkPipelineRasterizationStateCreateInfo rasterizer{};// ye ek frome hai jishme ham me batauga ki traingle soild , srap etc kesa chahiyr
          rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;// ye struct ka type hai 
          rasterizer.rasterizerDiscardEnable = VK_FALSE; // My Perspective agar triangle screm ke baher jata hai hai to ushse chip(caat)  do
          rasterizer.polygonMode = VK_POLYGON_MODE_FILL; // My Perspective ye puch raha hai ki image ko dustbin dali mene false kiya hai kyoki mujhe screen pe dikhana hai 
          rasterizer.lineWidth = 1.0f; // My PerspectiveLogic: Triangle ko kaise rangna hai?Optuions:FILL: Pura solid triangle (Default).LINE: Sirf outline (Wireframe).POINT: Sirf kone waale dots. mujhe pura triangle barana hai sih liye fill
          rasterizer.cullMode = VK_CULL_MODE_BACK_BIT; // My Perspective Logic: Agar tu upar LINE mode select karta, toh line kitni moti hoti? my's Perspective: Standard 1.0 pixel. (Note: Adreno 610 par zyadatar 1.0 hi support hota hai).
          rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; // My Perspective Logic: Kya triangle ki "pichhli side" draw karni hai? my's Perspective: Socho ek paper ka tukda hai. Agar tu use piche se dekh raha hai, toh GPU mehnat kyun kare? BACK_BIT ka matlab hai piche wala hissa hide kar do. Isse FPS badhta hai.
          rasterizer.depthBiasEnable = VK_FALSE;// my  Perspective: Ye advance lighting ke liye hai. Abhi ke liye humein simple triangle chahiye, toh FALSE.
        // step 4 framation shader
         /*
        4. Fragment Shader: The Artist
        Ye sabse zyada "heavy" stage hoti hai. Har wo pixel jo rasterizer ne identify kiya tha, wo ek Fragment ban jata hai.
        Normal Mapping: Ek flat surface ko "bumpy" dikhane ke liye fake shadows add karna.
        Texture Sampling: Image file se exact color uthana aur lighting ke saath mix karna. 
        */
          ifstream frag_file("frag.spv", ios::ate |ios::binary);// vertex shader ko likh kar ushko cpp (cpu ) ko dene ke liye ek new  file likha >
          if(!frag_file.is_open()){// aur check kiya ki file hai ki nhi
              cout<<"File reading fail"<<endl;
              return;//agar nhi to program exit
          }
          size_t frag_fileSize = (size_t) frag_file.tellg();//aur ha to file ko size _t me convert karke kis byte par ho ushka pata do
          std::vector<char> buffer_frag(frag_fileSize);//ek buffer vector liya aur ushka size filesize itna rakh
          frag_file.seekg(0);//aur bola phir file ke starting me joa
          frag_file.read(buffer_frag.data(), frag_fileSize);//aur j( mil padh kar buffer(vector) me store kar do
          VkShaderModuleCreateInfo fragInfo{};// from aab me vulkan ko duga aur gpu paint kar dega
          fragInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;//ye ushka typw hai
          fragInfo.codeSize = buffer_frag.size();// ye vulkan k( batata hai ki kitne byte kdata filll karana hai
          fragInfo.pCode = reinterpret_cast<const uint32_t*>(buffer_frag.data());//ye bus data ko gpu ko de de ta hai 
          vkCreateShaderModule(device, &fragInfo, nullptr, &fragModule);// idhar vo ban jyega aur fragModule me save hoga 
          cout << "Fragment Shader Module Ready!" << endl;
      
          // 1. Vertex Stage ki jankari
          VkPipelineShaderStageCreateInfo vertStageInfo{};// yr 
          vertStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;//ye struct ka type haib
          vertStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; // Word: stage aur VERTEX_BIT Logic: Sabse important! GPU ke paas bohot saari stages hoti hain. Tune yahan GPU ko bola: "Suno, ye wala code sirf Maths aur Coordinates (Vertex) ke liye hai." Agar yahan galti se Fragment likh diya, toh GPU triangle ki jagah pixels calculate karne lagega aur program crash ho jayega.
          vertStageInfo.module = shaderModule; // ye puch raha hai me kon se data ko padhuga Jo mene ifstream se load kiya tha
          vertStageInfo.pName = "main"; // Word: pName (Pointer to Name) Logic: mene apne GLSL code mein likha tha void main(). Ek shader file mein bohot saare functions ho sakte hain, toh Vulkan puchta hai: "Bhai, shuru kahan se karun?" Tune bola: "main se shuru karo."
          // aab mene dono fragment and vertex shader bana to diya but vulkan ko kese pata chalega me vulkan ko batane ke  liye ek struct banauga ho dono ko repersnt karege 
          // 2. Fragment Stage ki jankari
          VkPipelineShaderStageCreateInfo fragStageInfo{};// ye  fragment shader ke liye banaya hai mebe 
          fragStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;//ye ushka type hai 
          fragStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; // ye bolta hai Oye gpu ye fragment shader haib
          fragStageInfo.module = fragModule; // ye gpu ko batata hai ki real binary code kaha hai 
          fragStageInfo.pName = "main"; //Word: pName (Pointer to Name) Logic: mene apne GLSL code mein likha tha void main(). Ek shader file mein bohot saare functions ho sakte hain, toh Vulkan puchta hai: "Bhai, shuru kahan se karun?" Tune bola: "main se shuru karo."
          // 3. In dono ko ek array mein pack karu do
          // gpu ko data dono ek sath chahiye hota hai na ki do alg struct ush liye array
          shaderStages[0] = vertStageInfo;
          shaderStages[1] = fragStageInfo;

       // step 5 render out put
       /*
       5. Render Output (Per-Sample Operations)
       Sab kuch calculate hone ke baad bhi, pixel screen par jayega ya nahi, ye yahan decide hota hai:
       Depth Test (Z-Buffering): Agar ek fragment ke peeche koi doosri cheez pehle se render ho chuki hai, toh naya fragment discard ho jayega.
       Stencil Test: Shadows ya reflections ke liye specific areas ko "mask" karna.
       Blending: Glass ya aag (fire) jaise effects ke liye purane pixel aur naye pixel ko mathematical formula se mix karna (Source \times Alpha + Destination \times (1 - Alpha)).
       */
       // 1. Multisampling (Step 5.1)
       VkPipelineMultisampleStateCreateInfo multisampling{}; // Jab Rasterizer 3D triangle ko pixels mein badalta hai, toh kinaron par "Zid-Zad" (jagged edges) aa sakte hain. Multisampling unhein smooth karta hai.
       multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;//ye ushka sturct type hai 
       multisampling.sampleShadingEnable = VK_FALSE;// ye puch raha hai ki kya tumhe etra power gemorty shader ray tracing etc chahiye false ish liye kyoki me abhi simple triangle bana raha hu
       multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // 1 pixel = 1 sample (Simple)
       //ye part graphics ka "Paint Mixing" stage hai. Jab GPU ek naya pixel calculate kar leta hai, toh wo screen par pehle se maujood rang ke saath use kaise mix karega? Ye code wahi decide karta hai.
       // Color Blending (Step 5.2)
       // 1. Ye batata hai ki ek specific screen (attachment) par blend kaise hoga
       VkPipelineColorBlendAttachmentState colorBlendAttachment{};//ye sturct puch raha hai ki tumhe kitne type ke str&ct chahiye 
       colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;//ye vulkan ko batata hai ki tumhe kon kon se colure chahiye Mene red, blue , green transepancy rakha 
       colorBlendAttachment.blendEnable = VK_FALSE; //  ye abhi ke liye false kyo ki mujhetranpency nhi chahiye 
       // 2. Ye global blending setting hai
       VkPipelineColorBlendStateCreateInfo colorBlending{};//ye sabko vulkan ko golably deta hai 
       colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;//ye sturct type hai 
       colorBlending.logicOpEnable = VK_FALSE;// My Perspective: Ye purane zamane ke effects ke liye hota tha. Aaj kal modern graphics mein ise FALSE hi rakhte hain kyunki humein asli colors chahiye, logic gates nahi.
       colorBlending.attachmentCount = 1;// me abhi ek hi mobile screen pe draw kar raha hu ish liye 1
       colorBlending.pAttachments = &colorBlendAttachment;// ye bata ta hai kaha colrue blanding karani hai
       //ye Pipeline Layout graphics programming ka wo "Naka" (Checkpost) hai jo ye decide karta hai ki tere CPU (C++) se GPU (Shader) ke andar kya-kya data jayega. 
       // Pipeline Layout (Step 5.3)
       // mene jo struct level me variable banaya tha ushse use kiya VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
       pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;//ye ushka tyep hai      pipelineLayoutInfo.setLayoutCount = 0; 
       pipelineLayoutInfo.setLayoutCount = 0;// ye set word Logic: Ye sabse powerful cheez hai. Jab tujhe shader mein Textures (images) ya Uniform Buffers (camera positions) bhejni hoti hain, toh wo Descriptor Sets ke zariye aati hain. // my Perspective: Abhi hamara triangle shader self-contained hai, use bahar se koi texture nahi chahiye, isliye humne ise 0 rakha hai.
       pipelineLayoutInfo.pushConstantRangeCount = 0; // Word: pushConstant ye "Fast Track" delivery jaisa hai. Agar tujhe koi choti si value (jaise ek number) turant GPU ko deni ho bina buffer banaye, toh ise use karte hain. my Perspective: Abhi hamara triangle shader self-contained hai, use bahar se koi texture nahi chahiye, isliye humne ise 0 rakha hai.
       if(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {// ye check kar raha hai
           cout << "Layout creation fail!" << endl;
           return;
       }
       //abb me ye sare from real gpu ko dunga vulkan 
       // Step (5.4)
       VkGraphicsPipelineCreateInfo pipelineInfo{};//ye real GraphicsPipeline ka from (struct) hai jo vulkan leta hai
       pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO; // ye struct type haiushka pipeline ka 
       // 1. Shaders (Jo tune array banaya tha)
       pipelineInfo.stageCount = 2;// ye puch raha hai k*tbe shader hai mere pass 2hai ver and frag ush liye two
       pipelineInfo.pStages = shaderStages;//ye vo array hai jishme frag aur vert shader hai ye vulkan puch raha hai vo variable kaha hai vese ye ek pointer hai 
       // 2. Fixed Functions (Jo tune pehle mehnat se banaye the)
       // 
       VkPipelineVertexInputStateCreateInfo vertexInputInfo{}; // Abhi hum model data nahi bhej rahe
       vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;//ye ushka tyep hai      
       pipelineInfo.pVertexInputState = &vertexInputInfo;// ye hpu ko batata hai ki mere triangle ka vertex point kya hai kyoki me vertex shader me likh raha hu ush liye ye emty hai 
       pipelineInfo.pInputAssemblyState = &inputAssembly;// ye gpu ko batata hai mujhe kesa triangl# chahiye mene ek struct bhada tha input assembly walabahi idhar diya
       pipelineInfo.pViewportState = &viewportState;//ye triangle ko batata hai ki kesa window mujhe chahiye 
       pipelineInfo.pRasterizationState = &rasterizer;// ye puchta hai hai ki rastiration kese karu ayr kaha se karu ushka answer mene struct me bhada tha
       pipelineInfo.pMultisampleState = &multisampling;//Multisample kinaron ko chikna (smooth) karta hai,
       pipelineInfo.pColorBlendState = &colorBlending;//ColorBlend ye decide karta hai ki agar transparent cheez draw karni ho toh purana color aur naya color kaise mix hoga.
      // 3. Layout aur RenderPass
       pipelineInfo.layout = pipelineLayout; // Logic: mer wo "Checkpost" jo mene banaya tha. Iske bina GPU ko pata nahi chalega ki CPU se data (jaise textures) kaise lena hai.
       pipelineInfo.renderPass = renderPass; // Logic: Ye batata hai ki drawing kaunse parde (attachment) par hogi.
       pipelineInfo.subpass = 0; // subpass = 0 matlab pehli baar mein hi drawing karni hai.
       // 4. Optimization (Abhi ke liye off)
       pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
       // --- FINAL ACTION: PIPELINE PAIDA KARO ---
       if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
             cout << "Bhai, Graphics Pipeline fail ho gayi!" << endl;
             /* device: mera logical device (GPU).
             VK_NULL_HANDLE: Pipeline cache ke liye hota hai (Optimize karne ke liye), abhi zaroorat nahi.
             1: Matlab hum ek hi pipeline bana rahe hain.
             &pipelineInfo: Is poore form ka address.
             &graphicsPipeline: Wo handle jisme banne ke baad asli pipeline store hogi.
              */
       } else {
             cout << "MAHAKAL! Graphics Pipeline  are ready!" << endl;
       }
    }
    void Framebuffers(){
        //vector<VkFramebuffer> koi aam variable nahi hai, ye tere GPU ke liye "Pardon ki List" (List of Screens) hai. Ise breakdown karte hain:
        //Vulkan mein VkFramebuffer ek Handle hota hai.
        //Jaise int mein number hota hai,VkFramebuffer mein ek specific "Drawing Container" ka pata (Address) hota hai
        //Ye koi "Class" ya "Objectu" nahi hai, ye sirf ek pointer ki tarah hai jo GPU ko batata hai ki: "Bhai, pixels yahan phainkne hain!"
        // 2. vector<...> (The Container
        //mene vector isliye liya kyunki mere paas Swapchain mein Multiple Images hain (jaise mune minImageCount = 2 rakhatha
        //Pehle Framebuffer mein 1st image ki details hongi
        //Doosre Framebuffer mein 2nd image ki
        //GPU inke beech mein "Flip" karta rehta hai taaki screen jhilmilaye (flicker) nahi
        // Soch ki hamre paas ek Render Pass hai (jo ek "Blueprint" hai ki drawing kaise karni hai). Lekin drawing karne ke liye koi jagah toh chahiye?
        //Framebuffer = Render Pass + Image View
        // Ye wo "Gond" (Glue) hai jo tere Render Pass ko tere Swapchain ki Images se chipkata hai. Iske bina GPU ko ye toh pata hoga ki "Triangle draw karna hai", par use ye nahi pata hoga ki "Kaunsi image par draw karna hai"
        swapChainFramebuffers.resize(swapChainImageViews.size());// ye Frame Buffer itne hi chahiye jitne image hai aur swapChain batata hai ki kitne image hai
            for (size_t i = 0; i < swapChainImageViews.size(); i++) { //swapChainFramebuffers ye ek for loop hai jo  me har ek Image View ke liye ek alag Framebuffer bana rahe hain. Ye loop har image par jaakar use "setup" karega.
            VkImageView attachments[] = { swapChainImageViews[i] };//
            // logic VkImageView attachments[] = { swapChainImageViews[i] }
            // Framebuffer akela nahi hota, use batana padta hai ki wo kaunsi ImageView (image ka chashma) use karega. Humne yahan current image (i) ko ek array mein daal diya.
            VkFramebufferCreateInfo framebufferInfo{};// ye ek struct hai jo Vulkan ko dena hai aur ye frame buffer ke liye hai 
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;// ye ushka type hai 
            // Logic: Ye sabse zaroori hai! Framebuffer ko ye batana padta hai ki wo kis Render Pass ke saath dosti karega. Framebuffer ki settings aur Render Pass ki settings ekdum match honi chahiye (jaise dono mein 1 attachment hona chahiye).
            framebufferInfo.renderPass = renderPass; //  ye render pass hai 
            // logic framebufferInfo.attachmentCount
            // me GPU ko bol raha  hain, "Bhai, is board par sirf ek parda (image) tanga hua hai." pAttachments us parde ka address de deta hai.
            framebufferInfo.attachmentCount = 1;// render pass aur frame buffer ki setting ek jesi honi chahiye ish liye 1 attachment aur ye ushka address vulkan ko de deta hai
            framebufferInfo.pAttachments = attachments;// ye ek pointer hai jo puch raha hai ki attachment kaha hai 
            // logic seting heigth and width
            // Ye Framebuffer ka size hai. Yaad rakhna, ye size tere Swapchain ke size ke barabar hona chahiye. (mene abhi hardcode kiya hai, par baad mein me ise dynamic karenge).
            framebufferInfo.width = 500; // Filhal hardcode, baad mein capabilities se lena
            framebufferInfo.height = 300;// ye haigth hai
            // logic 2d / 3d screen setting
            // Word: layers
            //Logic:Hamari image 2D hai,isliye sirf 1layer kaafi hai. (VR games mein yahan 2 layers hoti hain). 
            framebufferInfo.layers = 1;//ye raha setting layer kiya idhar
            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {// ye real funcation hai jo Frame buffer banata hai aur ushko &swapChainFramebuffers[i]) vector me save kar deta hai 
                 cout << "Framebuffer fail!" << endl;
                 return;
            }
        }
            // Mere paas 3 khali Canvas (Images) hain. Ye code har canvas ko uthata hai, use ek lakdi ke frame (Framebuffer) mein fasta hai, aur uske upar Render Pass ki "Moher" (Stamp) laga deta hai taaki GPU uspar draw kar sake.
             // complete 
    }
    void Command_pool(){
        // Memory Management: GPU memory bar-bar mangna (allocate) aur wapas dena (deallocate) bahut "expensive" (slow) kaam hai.
        // The Bucket (Pool): Command Pool ek bada sa block hota hai memory ka jo me pehle hi GPU se le leta hu
        /* Efficiency: Jab tum allocInfo.commandPool = 
        commandPool likhte ho, toh Vulkan ko 
        nayi memory dhoondni nahi padti. Wo usi pool ke andar se ek chota s
        a tukda kaat kar tumhare buffer ko de deta hai.*/
        // GPU ko command dene ke liye menory chahiye hoti hai
        // yahi commad pool me me karuga 
        // menory allocate
        // ye  command pool ka stuct hai like a from
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;// aur ye ushka type hai 
        // ye flag bata ta haihi me kabhi bhi  ye keh raha hai ki me command buffer se bahaer nilkaml kal kafi bhi command likh sakata hu ye re - use badhata hai 
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;// ye sabse important flag hai 
        poolInfo.queueFamilyIndex = Index_QueueFamily; // Ye dhyan se set karna
        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {// yaha real pool ban raha hai aur Commandpool me save ho jayega 
            // Handle error
        }
    }
    void Command_buffer(){
        // Gpu aur vulkan direct kam nhi karate unko ek buffer (list ) chahiye hota hai hai jishme wrok ka list dekh sake 
        // ye bilkul gpu ko khali panna dene jesa hai
        // jishme me baad me commands likhuga
        // ye ush khali page ka struct(frome) kese hai 
        // Vulkan mein kuch bhi "Allocate" (memory reserve) karne ke liye ye struct chahiye hota hai.
        //memory ki bikh magne ka struct 
        VkCommandBufferAllocateInfo allocInfo{};//ye gpu se memory bikh mange ne ka tarika hai using struct(frome) 
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;//ye struct type hai
        // Yahan me GPU ko bol rahe ho: "Bhai, ye
        // jo main naya Command Buffer (Parcha) maang raha hoon,
        //  ye meri purani Command Pool (Purio ka dabba) se hi nikaalna."
        // Line allocInfo.commandPool = commandPool; ka matlab hai: "Page nikaalne ke liye isi notebook ka use karo."
        allocInfo.commandPool = commandPool;//ye bahi parachi hai 
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // Direct GPU ko jata hai
        // ye line puchati hai ek bar me kitne page ko lena hai
        allocInfo.commandBufferCount = 1;
        if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
             /* vkAllocateCommandBuffers(...): Ye function GPU driver se bolta hai, "Sab setup ready hai, ab memory allot kar do."
​             device: Tumhara Logical Device.
​             &allocInfo: Jo settings humne upar set ki, uska address.
​             &commandBuffer: Jab memory mil jayegi, toh uska handle is variable mein save ho jayega.
​             != VK_SUCCESS: Agar device ki memory full hai ya koi aur problem hai, toh ye error dega.
             */
             // Handle error
        }
    }
    void Record_commands(uint32_t FrameIndex) {//Ye sabse important hai. Swapchain mein 2-3 images (frames) hoti hain. Ye parameter batata hai ki GPU abhi kaunse number wali image (0, 1, ya 2) par paint karega.
        /* Ye poora function ek Waiter ki tarah hai jo ek order slip (commandBuffer) leta hai:
​        Slip par likhna shuru kiya (Begin).
​        Table saaf kiya (ClearColor).
​        Plate rakhi (Framebuffer).
​        Chef ko bola "Triangle" banao (vkCmdDraw).
​        Slip band ki aur Counter par bhej di (End).
        */
        /* pehele mene only page liya tha but page me likhne ke liye pen 
        bhi to chahiye na ye funcation gpu ko batata hai ki aab command ana
        strat hogi */
        VkCommandBufferBeginInfo beginInfo{};
        /* VkCommandBufferBeginInfo beginInfo{};
        ​VkCommandBufferBeginInfo: Jaise allocation ke 
        liye struct tha, waise hi recording shuru 
        karne ke liye ye "Information Packet" hai.
​        {}: Isse saare extra settings (jo humne set nahi 
        kiye) default par set ho jate hain*/
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;// ye ushk struct tyep hai
        // .flags batata hai ki hum is buffer ko kaise use karenge
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //"Bhai, main ye commands likh raha hoon, ek baar chalao aur phir panna faad do (reset)." (Ye heavy games mein optimize karne ke liye hota hai).
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {// ye real create kiya hai aur ushse &beginInfo) pe save kar diya 
        
             // Error: Recording shuru nahi ho payi
             // 3. VkRenderPassBeginInfo (Canvas Ki Setting)
             // ​Yahan hum bata rahe hain ki drawing kahan aur kaise hogi:
             VkRenderPassBeginInfo renderPassInfo{};// ye ushka struct 
             renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;// ye struct type hai 
             // bhai mera banaya hua setting hai ki  image  load hone par clear karana hai aur load karana hai 
             renderPassInfo.renderPass = renderPass;//ye puch raha hai ki render pass kaha hai 
             // framebuffer[imageIndex]:: ye vo parada buffer hai 
             // jo screen ke liye ready hai 
             // ye swap chain ke pass ja kar 
             // bikh manta hai mujhe FrameIndex wala 
             // screen de 
             renderPassInfo.framebuffer = swapChainFramebuffers[FrameIndex];//ye bataya ki abhi konshse image pe hai 
             // renderArea.offset = {0, 0}: Drawing screen ke ekdum kone (top-left) se shuru hogi. 
             renderPassInfo.renderArea.offset = {0, 0};// screen ke top left se start hoga 
             // renderArea.extent = {500, 300}: Sirf itne area mein hi drawing hogi.u
             // ye 500 x 300 hi hona chahiye
             // kyoki mene swapchain me yahi size di thi 
             // agar nhi kiya to screen me achhe se drawing nhi hongi 
             renderPassInfo.renderArea.extent = {500, 300};//  ye set kiya 
             /* ​clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}}: Ye (R, G, B, Alpha) hai.
​             Sab zero hain, iska matlab hai Black Screen.
​             Agar me {{{1.0f, 0.0f, 0.0f, 1.0f}}} likhta, toh screen Red ho jati.
              */
             VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}}; // Black screen
             /* Logic: Ye Vulkan ko batata hai ki tum kitne attachmentus 
             (parde) ko saaf kar rahe ho.
             ​Simple Matlab: Kyunki hum abhi sirf ek hi screen (Color Attachment) 
             par kaam kar rahe hain, isliye humne yahan 1 likha hai. 
             Agar hum "Depth Testing" (3D ke liye) use karte, 
             toh yahan count badh jata kyunki humein color aur depth dono ko saaf karna padta.
             */
             renderPassInfo.clearValueCount = 1;
             /*
             ​Word: pClearValues (Pointer to Clear Values)
             ​Logic: Ye wo address hai jahan tune apna rang (Black: 0,0,0,1) chhipa 
             kar rakha hai.
​             Simple Matlab: Vulkan is pointer ka peecha karta hua us 
             VkClearValue struct tak jayega aur wahan se RGB values utha lega.
             */
             renderPassInfo.pClearValues = &clearColor;
             // VK_SUBPASS_CONTENTS_INLINE: Iska matlab hai ki saari 
             //commands isi main buffer mein likhi hain, koi dusra "secondary" buffer nahi hai.
             vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
             // 3. Pipeline Bind karo (Drawing Style)
             // graphicsPipeline: GPU ko batata hai ki drawing karte waqt kaunse 
             //Shaders (Vert/Frag) aur kaunse rules (Rasterizer settings) use karne hain.
             vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
             // 4. DRAW! (Asli Magic)
             /* Yahi wo command hai jo triangle banati hai:
​             3 (vertexCount): Kitne points draw karne hain? Humne 3 diye, kyunki 3 points mil kar ek Triangle banate hain.
​             1 (instanceCount): Kitni baar? Hum 1 hi triangle draw kar rahe hain.
​             0 (firstVertex): Vertex data mein kahan se shuru karein? 0 se.
​             0 (firstInstance): Pehli instance ka ID
             */
             vkCmdDraw(commandBuffer, 3, 1, 0, 0); // 3 vertices = 1 Triangle
             // 5. Khatam
             // EndRenderPass: Matlab drawing khatam, ab parda (framebuffer) band kar do.
             vkCmdEndRenderPass(commandBuffer);
             // EndCommandBuffer: Ab panna (buffer) "Seal" (lock) ho gaya hai. 
             //Ab ise badla nahi ja sakta, bas GPU ko bheja ja sakta hai.
             vkEndCommandBuffer(commandBuffer);
        }
        // aab page pe likh diya kab gpu ko dena hai 
        
    }
    void Syn_GPU_CPU() {
        VkSemaphoreCreateInfo semaphoreInfo{};// ye semaforce jo gpu par chalta ha* ushka struct hai 
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;// ye vulkan ko bata raha hai ki me ek semaforce ya fence banane ka resipy de raha hu 
        VkFenceCreateInfo fenceInfo{};// ye fence banane ka struct hi jo cpu to gpu jata hai         
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;// ye ushka tyep hai 
        /* ​🛑 Problem: Agar ye flag NA ho toh kya hoga?
​         Maan le tune VkFenceCreateInfo fenceInfo{}; likha bina kisi flag ke. Default mein Vulkan ise Unsignaled (Band) banayega.
​         Ab tera Render Loop shuru hota hai:
​         Loop Start: CPU sabse pehle check karta hai vkWaitForFences. Iska kaam hai check karna ki "Kya GPU ne पिछला (previous) frame finish kar diya hai?"
​         The Crash: Kyunki ye pehla frame hai, GPU ne aaj tak koi kaam kiya hi nahi. Fence "Unsignaled" hai.
​         Deadlock: CPU wahin khade-khade GPU ka intezar karta rahega jo kabhi khatam nahi hoga, kyunki GPU ko kaam tabhi milega jab CPU aage badhega. Tera engine wahi "Freeze" ho jayega.
​         ✅ Solution: VK_FENCE_CREATE_SIGNALED_BIT
​         Is flag ka matlab hai: "Jab Fence paida ho, toh wo pehle se hi 'Signaled' (Khula) state mein ho."
​         Ab Render Loop dekho:
​         Loop Start: CPU check karta hai vkWaitForFences.
​         The Pass: CPU dekhta hai ki "Arre, Fence toh pehle se hi Signaled hai!"
​         Execution: CPU aage badhta hai, GPU ko command submit karta hai, aur vkResetFences chala kar darwaza Band kar deta hai taaki jab tak GPU kaam khatam na kare, CPU agla frame na bhej sake.
​         🛠️ Ek-Ek Word ka Matlab:
​         fenceInfo.flags: Ye wo jagah hai jahan hum Fence ki "Aadat" (Properties) batate hain.
​         VK_FENCE_CREATE: Ye prefix hai jo batata hai ki ye "Creation" ke waqt ki setting hai.
​         SIGNALED_BIT: Ye wo switch hai jo Fence ko "ON" (Khula) position mein rakhta hai.
         */
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Shuruat mein parda khula rakho
        vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore);
        vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore);
        vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence);
        cout<<"Complete"<<endl;
    }
    void Frame_draw(){
       /* Mune command buffer (chitti) likh li, par use GPU (factory) tak 
        bbpahunchane aur wahan se wapas screen tak lane ka kaam ye code karta hai.*/
       // ye funcation kab chagela ek new frame genrate hoga
       /* ​1. Acquire Image from Swapchain
​       Sabse pehle GPU se ek image maangni hogi jispar tu draw kar sake.
​       vkAcquireNextImageKHR use kar.
​       Isme imageAvailableSemaphore pass kar taaki CPU ko pata chale kab image ready hai.
​       2. Submit Command Buffer
       ​Jo command buffer tune record kiya hai, usse GPU queue mein bhej (Submit).
​       VkSubmitInfo struct fill kar.
​       waitSemaphores: Jab tak image available na ho, wait kare.
​       signalSemaphores: Jab rendering khatam ho jaye, toh ye signal kare (renderFinishedSemaphore).
​       vkQueueSubmit call kar aur wahan inFlightFence use kar taaki agle frame se pehle check kar sake ki GPU free hai ya nahi.
​       3. Prentation
       ​Ab final image ko screen par "present" kar.
​       VkPresentInfoKHR struct ka use kar.
​       waitSemaphores: Render finish hone ka wait kare.
​       vkQueuePresentKHR call kar.
        */
       // 1. Intezar karo (Wait for Fence)
       // CPU ruka rahega jab tak GPU pichla kaam khatam na kar d
       // Vulkan me cpu bohut fast h(ta hai but gpu slow
       // me man leta hu ki gpu ek frame draw kar raha hai 
       // to cpu ne ek new frame bejh diya to cod# crash hoga vahi 
       // ye keheta hai kab tak gpu kam kar raha hai rukhe rehe 
       vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
       // yr cpu ko banane ne liye hai ki image drawing katam ho gayi
       // ab new frame bejh 
       // ye line wahi kan karata hai 
       vkResetFences(device, 1, &inFlightFence); // Fence ko wapas band karo
       // 2. Swapchain se Image uthao
       // ye 2 lines gpu se parde ki bikh magati hai 
        // imageIndex; ushse store karane ke liye hai  
        uint32_t imageIndex;// 32 bits lega
        // gpu se parda mangi ish funcation ki madat se
        vkAcquireNextImageKHR(device, swapChain, UINT64_MAX,imageAvailableSemaphore; , VK_NULL_HANDLE, &imageIndex);
        // 3. Command Buffer ko Record karo (Refresh image index)
        // vkResetCommandBuffer: Purane frame ki commands ko dustbin mein daal diya.
        vkResetCommandBuffer(commandBuffer, 0); // Purani commands saaf karo
        // Record_commands(imageIndex): Tune jo function banaya tha, use call 
        // kiya taaki naye imageIndex ke liye "Triangle draw karo" wali command likhi ja sake.
        Record_commands(imageIndex); // Nayi commands likho is image ke liye
        // 4. GPU ko Submit karo
        // VkSubmitInfo submitInfo{}; ye gpu ko bejne wala parecl hai
        VkSubmitInfo submitInfo{};// struct inilazing
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;// ye ushka type hai 
        // Signals ka khel: Drawing tabhi shuru karo jab image mil jaye
        /*GPU ko bola: "Order toh de raha hoon, par tab 
        tak shuru mat karna jab tak imageAvailableSemaphore signal green na ho jaye."*/
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};// ye  order diya 
        /* signalSemaphores: GPU ko bola: 
        "Jab drawing khatam kar lo, toh 
        renderFinishedSemaphore ko green kar dena."*/
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};// order diya 
        submitInfo.waitSemaphoreCount = 1;//uye batata hai ki ek hi image ka wait karna hai 
        // Jisme imageAvailableSemaphore hai
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;// ye ek trafic police jese hai 
        submitInfo.commandBufferCount = 1;// ye command buffer ek bar me kitne denge ushka conut
        submitInfo.pCommandBuffers = &commandBuffer;// ye puvh raba hai kaha commandbuffer hai drawing ke liye 
        // Drawing khatam hone par ye signal bajega
        // signalSemaphores: Ye sabse zaroori hai. Jab GPU triangle draw kar lega, toh wo is signal (renderFinishedSemaphore) ko 
        // Green kar dega. Isse screen ko pata chalega ki "Ab drawing ready hai."
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        //Logic: Tune pura order form GPU ki Queue mein daal diya. inFlightFence yahan bodyguard hai, 
        // jo CPU ko batayega jab GPU pura packet deliver kar dega.
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;// ye signal semaphores ki pata puch raha hai 
        if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {// real form bhara aur check kiya 
            cout << "Queue submit fail!" << endl;
        }
        // 5. Screen par dikhao (Presentation)
        VkPresentInfoKHR presentInfo{};// ye struct screen pe image dikhne ke liye use =(ta =ai 
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;// ye struct type hai
        presentInfo.waitSemaphoreCount = 1;
        // Screen se bola: "Tabhi triangle dikhana jab GPU signal de de ki drawing finish 
        // ho gayi (renderFinishedSemaphore)." Adha-adhura triangle nahi chahiye
        presentInfo.pWaitSemaphores = signalSemaphores; // Wait for rendering to finish
        // ​presentInfo.pSwapchains = swapChains;i
        // ​Logic: Bataya ki kaunse Swapchain par parda hatana hai.
        VkSwapchainKHR swapChains[] = {swapChain};
        //Screen se bola: "Tabhi triangle dikhana jab GPU signal de de ki drawing finish ho gayi 
        //(renderFinishedSemaphore)." Adha-adhura triangle nahi chahiye!
        presentInfo.swapchainCount = 1;
        // swap chain kaha hi bataya 
        presentInfo.pSwapchains = swapChains;
        // ​Logic: Bataya ki Swapchain ki kaunse number wali image (0, 1, ya 2) ab screen par dikhani hai.
        presentInfo.pImageIndices = &imageIndex;
        // Word: Present (Dikhawa).
​        // Logic:Final tep!Parda hat gaya aur tera triangle screen par aa gaya
        vkQueuePresentKHR(graphicsQueue, &presentInfo);
    }
    void While_true(){
       // Jab tak app band nahi hoti, triangle draw karte raho
       while (true) { 
           Frame_draw(); 
       }
    }
};

int main(){
    vulkan v;
    // kal likhugi py
    return 0;
}


