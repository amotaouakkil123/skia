#include "tools/sk_app/ohos/surface_glue_ohos.h"
#include "tools/sk_app/ohos/SkiaAppManager.h"
#include "tools/sk_app/ohos/Window_ohos.h"
#include <arkui/native_node_napi.h>
#include <arkui/native_interface.h>
#include "tools/sk_app/ohos/logger_common.h"

namespace sk_app {

OH_NativeXComponent* OhosSkiaApp::fXComponent;
OH_NativeXComponent_Callback OhosSkiaApp::fCallback;
std::mutex OhosSkiaApp::fMutex;
std::condition_variable OhosSkiaApp::fCon;
std::unordered_map<std::string, OhosSkiaApp*> OhosSkiaApp::fInstanceMap;

static std::string GetXComponentId(OH_NativeXComponent* component) {
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    int32_t ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return std::string();
    }
    return std::string(idStr);
}

OhosSkiaApp::OhosSkiaApp(std::string& id)
    : fId(id) {
        LOGD("OhosSkiaApp constructor!");
        auto renderCallback = OhosSkiaApp::GetNXComponentCallback();
        renderCallback->OnSurfaceCreated = OnSurfaceCreatedCB;
        renderCallback->OnSurfaceChanged = OnSurfaceChangedCB;
        renderCallback->OnSurfaceDestroyed = OnSurfaceDestroyedCB;
        renderCallback->DispatchTouchEvent = DispatchTouchEventCB;
}

OhosSkiaApp::~OhosSkiaApp() { 
    LOGD("OhosSkiaApp destructor!");
    delete fInstanceMap[fId];
    fInstanceMap.erase(fId);
}

OH_NativeXComponent_Callback* OhosSkiaApp::GetNXComponentCallback() {
    LOGD("OhosSkiaApp::GetNXComponentCallback");
    return &OhosSkiaApp::fCallback;
}

void OhosSkiaApp::SetNativeXComponent(OH_NativeXComponent* component) {
    LOGD("OhosSkiaApp::SetNativeXComponent");
    fXComponent = component;
    OH_NativeXComponent_RegisterCallback(fXComponent, &OhosSkiaApp::fCallback);
}

void OhosSkiaApp::RenderThread() {
    LOGD("OhosSkiaApp::RenderThread We are threading the render!");
    while (fApp != nullptr) {
        std::unique_lock<std::mutex> locker(fMutex);
        if (!isAppQuit) {
            fApp->onIdle();
        } else {
            fCon.wait(locker);
        }
    }
}

OhosSkiaApp* OhosSkiaApp::GetInstance(std::string& id) {
    LOGD("OhosSkiaApp::GetInstance We are getting an instance");
    if (fInstanceMap.find(id) == fInstanceMap.end()) {
        OhosSkiaApp* instance = new OhosSkiaApp(id);
        fInstanceMap[id] = instance;
    }
    return fInstanceMap[id];
}

void OhosSkiaApp::SetInstance(std::string& id) {
    LOGD("OhosSkiaApp::SetInstance We are setting instances");
    if (fInstanceMap.find(id) == fInstanceMap.end()) {
        OhosSkiaApp* instance = new OhosSkiaApp(id);
        fInstanceMap[id] = instance;
    }
}

void OhosSkiaApp::OnSurfaceCreatedCB(OH_NativeXComponent* component, void* window) {
    LOGD("OhosSkiaApp::OnSurfaceCreatedCB");
    std::string id = GetXComponentId(component);
    auto render = OhosSkiaApp::GetInstance(id);
    render->OnSurfaceCreated(component, window);
}

void OhosSkiaApp::OnSurfaceChangedCB(OH_NativeXComponent* component, void* window) {
    LOGD("OhosSkiaApp::OnSurfaceDestroyedCB");
    std::string id = GetXComponentId(component);
    auto render = OhosSkiaApp::GetInstance(id);
    render->OnSurfaceChanged(component, window);
}

void OhosSkiaApp::OnSurfaceDestroyedCB(OH_NativeXComponent* component, void* window) {
    LOGD("OhosSkiaApp::OnSurfaceDestroyedCB");
    std::string id = GetXComponentId(component);
    auto render = OhosSkiaApp::GetInstance(id);
    render->OnSurfaceDestroyedCB(component, window);
}

void OhosSkiaApp::DispatchTouchEventCB(OH_NativeXComponent* component, void* window) {
    LOGD("OhosSkiaApp::DispatchTouchEventCB");
    std::string id = GetXComponentId(component);
    auto render = OhosSkiaApp::GetInstance(id);
    render->DispatchTouchEvent(component, window);
}

void OhosSkiaApp::OnSurfaceCreated(OH_NativeXComponent* component, void* window) {
    if (fApp == nullptr) {
        LOGD("OhosSkiaApp::OnSurfaceCreated We're about to create an application instance");
        static const char* gCmdLine[] = {
            "viewer",
            "--skps",
            "/data/storage/el1/bundle/entry/resources/resfile/skps",
            "--config",
            "grdawn_vk"
        };

        fApp = Application::Create(std::size(gCmdLine),
                                   const_cast<char**>(gCmdLine),
                                   this);
        
        fNativeWindow = (OHNativeWindow*)window;
        auto window_ohos = (Window_ohos*)fWindow;
        window_ohos->initDisplay(fNativeWindow);
        ((Window_ohos*)fWindow)->paintIfNeeded();
        fRenderThread = std::thread(std::bind(&OhosSkiaApp::RenderThread, this));
    }
}

void OhosSkiaApp::OnSurfaceChanged(OH_NativeXComponent* component, void* window) {
    // E_NOT_IMPLEMENTED
}

void OhosSkiaApp::DispatchTouchEvent(OH_NativeXComponent* component, void* window) {
    // E_NOT_IMPLEMENTED
}

void OhosSkiaApp::OnSurfaceDestroyed(OH_NativeXComponent* component, void* window) {
    auto window_ohos = (Window_ohos*)fWindow;
    window_ohos->onDisplayDestroyed();
    fNativeWindow = nullptr;
}

void OhosSkiaApp::setTitle(const char* title) const {
    // E_NOT_IMPLEMENTED
}

void OhosSkiaApp::setUIState(const char* state) const {
    // E_NOT_IMPLEMENTED
}

int OhosSkiaApp::message_callback(int fd, int events, void* data) {
    // E_NOT_IMPLEMENTED
    return 0;
}

} // namespace sk_app
