#include "tools/sk_app/ohos/Window_ohos.h"
#include "tools/window/WindowContext.h"
#include "tools/window/ohos/WindowContextFactory_ohos.h"
#include "tools/sk_app/ohos/logger_common.h"

namespace sk_app {

Window* Windows::CreateNativeWindow(void* platformData) {
    Window_ohos* window = new Window_ohos();
    if (!window->init((OhosSkiaApp*)platformData)) {
        LOGD("Window_ohos::Initialization of OHOS window failed");
        delete window;
        return nullptr;
    }

    return window;
}

bool Window_ohos::init(OhosSkiaApp* ohosSkiaApp) {
    SkASSERT(ohosSkiaApp);
    fOhosSkiaApp = ohosSkiaApp;
    fOhosSkiaApp ->fWindow = this;
    return true;
}

void Window_ohos::setTitle(const char* title) {
    fOhosSkiaApp->setTitle(title);
}

void Window_ohos::setUIState(const char* state) {
    fOhosSkiaApp->setUIState(state);
}

bool Window_ohos::attach(BackendType attachType) {
    fBackendType = attachType;

    // We delay the creation of fWindowContext until Ohos informs us that
    // the native window is ready to use.
    // The creation will be done in initDisplay, which is initiated by kSurfaceCreated event.
    return true;
}

void Window_ohos::initDisplay(OHNativeWindow* window) {
    LOGD("Window_ohos::initDisplay inside initDisplay");
    attach(kVulkan_BackendType);
    SkASSERT(window);
    // fBackendType = kNativeGL_BackendType;
    switch (fBackendType) {
// #ifdef SK_GL
//             case kNativeGL_BackendType:
//             default:
//                 LOGD("Window_ohos::initDisplay creating a OpenGL windowing context");
//                 fWindowContext = skwindow::MakeGLForOhos(window, 
//                                                          std::move(fRequestedDisplayParams));
//                 break;
// #else
//             case kRaster_BackendType:
//                 fWindowContext = skwindow::MakeRasterForAndroid(window, 
//                                                                 std::move(fRequestedDisplayParams));
//                 break;
// #endif
#ifdef SK_VULKAN
            case kVulkan_BackendType:
                LOGD("Window_ohos::initDisplay Creating a Vulkan windowing context!");
                fWindowContext = skwindow::MakeVulkanForOhos(window, 
                                                             std::move(fRequestedDisplayParams));
                break;
#ifdef SK_GRAPHITE
            case kGraphiteVulkan_BackendType:
                fWindowContext = skwindow::MakeGraphiteVulkanForOhos(window,
                                                                     std::move(fRequestedDisplayParams));
                break;
#endif

#if defined(SK_GRAPHITE) && defined(SK_DAWN)

            case kGraphiteDawn_BackendType:
                fWindowContext = skwindow::MakeGraphiteDawnVulkanForOhos(window,
                                                                         std::move(fRequestedDisplayParams));

#endif

#endif
    }
    if (!fWindowContext) {
        LOGD("Window_ohos::initDisplay Completely failed to create a vulkan window");
    } else {
        LOGD("Window_ohos::initDisplay We did good!");
    }
    this->onBackendCreated();
}

void Window_ohos::onDisplayDestroyed() {
    detach();
}

void Window_ohos::onInval() {
    // fOhosSkiaApp->postMessage(Message(kContentInvalidated));
}

void Window_ohos::paintIfNeeded() {
    if (fWindowContext) { // Check if initDisplay has already been called
        onPaint();
    } else {
        markInvalProcessed();
    }
}

} // namespace sk_app
