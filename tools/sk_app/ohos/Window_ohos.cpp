#include "tools/sk_app/ohos/Window_ohos.h"
#include "tools/window/WindowContext.h"
#include "tools/window/ohos/WindowContextFactory_ohos.h"
#include "tools/sk_app/ohos/logger_common.h"

namespace sk_app {

Window* Windows::CreateNativeWindow(void* platformData) {
    LOGI("Windows::CreateNativeWindow");
    Window_ohos* window = new Window_ohos();
    if (!window->init((OhosSkiaApp*)platformData)) {
        LOGD("Windows::Initialization of HarmonyOS window failed");
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
    LOGI("Window_ohos::setTitle");
    fOhosSkiaApp->setTitle(title);
}

void Window_ohos::setUIState(const char* state) {
    LOGI("Window_ohos::setUIState");
    fOhosSkiaApp->setUIState(state);
}

bool Window_ohos::attach(BackendType attachType) {
    LOGI("Window_ohos::attach");
    fBackendType = attachType;

    // We delay the creation of fWindowContext until Ohos informs us that
    // the native window is ready to use.
    // The creation will be done in initDisplay, which is initiated by kSurfaceCreated event.
    return true;
}

void Window_ohos::setRequestedDisplayParams(std::unique_ptr<const skwindow::DisplayParams> params,
                                            bool allowReattach) {
    LOGI("Window_ohos::setRequestedDisplayParams");
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
}

void Window_ohos::initDisplay(OHNativeWindow* window) {
    LOGI("Window_ohos::initDisplay");
    SkASSERT(window);
    // fBackendType = kNativeGL_BackendType;
    switch (fBackendType) {
#ifdef SK_VULKAN
            case kVulkan_BackendType:
                LOGI("Window_ohos::initDisplay Initializing Ganesh Vulkan");
                fWindowContext = skwindow::MakeVulkanForOhos(
                       window, fRequestedDisplayParams->clone());
                break;
#if defined(SK_GRAPHITE) && !defined(SK_DAWN)
            case kGraphiteVulkan_BackendType:
                LOGI("Window_ohos::initDisplay Initializing Graphite Native Vulkan");
                fWindowContext = skwindow::MakeGraphiteVulkanForOhos(
                       window, fRequestedDisplayParams->clone());
                break;
#endif

#if defined(SK_GRAPHITE) && defined(SK_DAWN)

            case kGraphiteDawn_BackendType:
                LOGI("Window_ohos::initDisplay Initializing Graphite Dawn Vulkan");
                fWindowContext = skwindow::MakeGraphiteDawnVulkanForOhos(
                       window, fRequestedDisplayParams->clone());
                break;
#endif
            default:
                break;

#endif
    }

    this->onBackendCreated();
}

void Window_ohos::onDisplayDestroyed() {
    LOGI("Window_ohos::onDisplayDestroyed");
    detach();
}

void Window_ohos::onInval() {
    LOGI("Window_ohos::onInval");
    // fOhosSkiaApp->postMessage(Message(kContentInvalidated));
}

void Window_ohos::paintIfNeeded() {
    LOGI("Window_ohos::paintIfNeeded");
    if (fWindowContext) { // Check if initDisplay has already been called
        onPaint();
    } else {
        markInvalProcessed();
    }
}

} // namespace sk_app
