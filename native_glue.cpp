#include "native_glue.h"
#include <android/native_activity.h>
#include <pthread.h>
#include <cstdlib>

// Android standard lifecycle tracker framework loop
static void onDestroy(ANativeActivity* activity) {
    android_app* app = (android_app*)activity->instance;
    app->destroyRequested = 1;
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window) {
    android_app* app = (android_app*)activity->instance;
    app->window = window;
    if (app->onAppCmd) {
        app->onAppCmd(app, APP_CMD_INIT_WINDOW); // Engine ko signal bhejo: Window Ready!
    }
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window) {
    android_app* app = (android_app*)activity->instance;
    if (app->onAppCmd) {
        app->onAppCmd(app, APP_CMD_TERM_WINDOW);
    }
    app->window = nullptr;
}

// OS entry point hook implementation
extern "C" JNIEXPORT void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    android_app* app = (android_app*)malloc(sizeof(android_app));
    app->userData = nullptr;
    app->onAppCmd = nullptr;
    app->window = nullptr;
    app->destroyRequested = 0;

    activity->instance = app;
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;

    // External thread handler trigger mapping function call
    extern void android_main(struct android_app* app);
    
    // Core engine thread register mapping execute logic
    // Kal bina internet ke tum direct is main logic blocks ko change kar paoge
    android_main(app);
}

