#version 450 // Vulkan version 1.0+ ke liye version define karna zaruri hai

// Output: Ye color screen par jayega
layout(location = 0) out vec4 outColor;//​layout(location = 0): Ye sabse important hai. Yaad hai tere C++ code mein colorAttachmentRef.attachment = 0 tha? Ye 0 wahi index hai. Ye bata raha hai ki ye color pehle (0th) frame-buffer attachment mein bhejna hai. 

void main() {
    // ​Values 0.0 se 1.0 ke beech hoti hain.
    // RGBA: Red, Green, Blue, Alpha (Transparency)
    // 1.0, 0.5, 0.0, 1.0 = Bright Orange
    outColor = vec4(1.0, 0.8, 0.2, 0.2); 
}
