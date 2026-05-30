#ifndef NATIVE_GLUE_H
#define NATIVE_GLUE_H

#include <android/native_window.h>
#include <android/configuration.h>
#include <android/looper.h> // 🔥 Yeh 'ALooper_pollAll' ko zinda karega

// Android system structure definition
struct android_poll_source {
    int32_t id;
    struct android_app* app;
    void (*process)(struct android_app* app, struct android_poll_source* source);
};

struct android_app {
    void* userData;                  
    void (*onAppCmd)(struct android_app* app, int32_t cmd); 
    ANativeWindow* window;           
    int destroyRequested;            
};

enum {
    APP_CMD_INIT_WINDOW = 1,
    APP_CMD_TERM_WINDOW = 2
};

#endif
