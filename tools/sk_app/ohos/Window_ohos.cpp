#include "tools/sk_app/ohos/Window_ohos.h"
#include "tools/window/WindowContext.h"
#include "tools/window/ohos/WindowContextFactory_ohos.h"
#include "tools/sk_app/ohos/logger_common.h"

namespace sk_app {

Window* Windows::CreateNativeWindow(void* platformData) {
    Window_ohos* window = new Window_ohos();
    LOGD("Windows::Window_ohos CreateNativeWindow creating a Native Window!");
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
    fOhosSkiaApp->fWindow = this;
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

void Window_ohos::setRequestedDisplayParams(std::unique_ptr<const skwindow::DisplayParams> params,
                                            bool allowReattach) {
    LOGD("Window_ohos::setRequestedDisplayParams setting display parameters");
#if defined(SK_VULKAN)
// Vulkan on unix crashes if we try to reinitialize the vulkan context without remaking the
// window.
    if (fBackendType == kVulkan_BackendType && allowReattach) {
        // Need to change these early, so attach() creates the window context correctly
        fRequestedDisplayParams = std::move(params);
        this->detach();
        this->attach(fBackendType);
        return;
    }
#endif

    Window::setRequestedDisplayParams(std::move(params), allowReattach);
    if (fRequestedDisplayParams == nullptr) {
        LOGD("Window_ohos::setRequestedDisplayParams is no working");
    }
}

void Window_ohos::initDisplay(OHNativeWindow* window) {
    LOGD("Window_ohos::initDisplay inside initDisplay");
    attach(kGraphiteDawn_BackendType);
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
                if (fRequestedDisplayParams == nullptr) {
                    LOGD("Window_ohos::initDisplay the display params are not good GRAPHITE NATIVE!!!!!!!!");
                } else {
                    LOGD("Window_ohos::initDisplay the display params are still really good GRAPHITE NATIVE");
                }
                fWindowContext = skwindow::MakeGraphiteVulkanForOhos(window,
                                                                     std::move(fRequestedDisplayParams));
                break;
#endif

#if defined(SK_GRAPHITE) && defined(SK_DAWN)

            case kGraphiteDawn_BackendType:
                if (fRequestedDisplayParams == nullptr) {
                    LOGD("Window_ohos::initDisplay the display params are not good GRAPHITE DAWN!!!!!!!!");
                } else {
                    LOGD("Window_ohos::initDisplay the display params are still really good GRAPHITE DAWN");
                }
                fWindowContext = skwindow::MakeGraphiteDawnVulkanForOhos(window,
                                                                         fRequestedDisplayParams->clone());

#endif

#endif
    }
    if (!fWindowContext) {
        LOGD("Window_ohos::initDisplay Completely failed to create a vulkan window");
    } else {
        LOGD("Window_ohos::initDisplay We did good!");
    }

    if (fRequestedDisplayParams == nullptr) {
        LOGD("Window_ohos::initDisplay the display parameters are screwed!!");
    } else {
        LOGD("Window_ohos::initDisplay the display parameters are the greatest!!");
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
