#include "tools/sk_app/ohos/surface_glue_ohos.h"
#include "tools/sk_app/ohos/SkiaAppManager.h"
#include "tools/sk_app/ohos/Window_ohos.h"
#include <arkui/native_node.h>
#include <arkui/native_node_napi.h>
#include <arkui/native_interface.h>
#include <arkui/ui_input_event.h>

#include "tools/sk_app/ohos/logger_common.h"

namespace sk_app {

static const std::unordered_map<int, skui::InputState> OHOS_TO_WINDOW_STATEMAP({
    { UI_TOUCH_EVENT_ACTION_CANCEL, skui::InputState::kUp   },
    { UI_TOUCH_EVENT_ACTION_DOWN,   skui::InputState::kDown },
    { UI_TOUCH_EVENT_ACTION_UP,     skui::InputState::kUp   },
    { UI_TOUCH_EVENT_ACTION_MOVE,   skui::InputState::kMove }
});

static const std::unordered_map<int, skui::Key> OHOS_TO_WINDOW_KEYMAP({
    { OH_NATIVEXCOMPONENT_LEFT_BUTTON,  skui::Key::kLeft  },
    { OH_NATIVEXCOMPONENT_RIGHT_BUTTON, skui::Key::kRight }
});

// NAPI variables for running the HarmonyOS application
OH_NativeXComponent* OhosSkiaApp::fXComponent;
OH_NativeXComponent_Callback OhosSkiaApp::fCallback;
std::mutex OhosSkiaApp::fMutex;
std::condition_variable OhosSkiaApp::fCon;
std::unordered_map<std::string, OhosSkiaApp*> OhosSkiaApp::fInstanceMap;

static ArkUI_NodeHandle fHandle;
static ArkUI_NativeNodeAPI_1* fNodeApi;

static std::string GetXComponentId(OH_NativeXComponent* component) {
    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    int32_t ret = OH_NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        return std::string();
    }

    return std::string(idStr);
}

OhosSkiaApp::OhosSkiaApp(std::string& id, ArkUI_NodeHandle handle, ArkUI_NativeNodeAPI_1* nodeApi)
    : fId(id) {
    LOGI("OhosSkiaApp::OhosSkiaApp");
    auto renderCallback = OhosSkiaApp::GetNXComponentCallback();
    renderCallback->OnSurfaceCreated = OnSurfaceCreatedCB;
    renderCallback->OnSurfaceChanged = OnSurfaceChangedCB;
    renderCallback->OnSurfaceDestroyed = OnSurfaceDestroyedCB;
    renderCallback->DispatchTouchEvent = DispatchTouchEventCB;
}

OhosSkiaApp::~OhosSkiaApp() { 
    LOGI("OhosSkiaApp::~OhosSkiaApp");
    delete fInstanceMap[fId];
    fInstanceMap.erase(fId);
}

OH_NativeXComponent_Callback* OhosSkiaApp::GetNXComponentCallback() {
    LOGI("OhosSkiaApp::GetNXComponentCallback");
    return &OhosSkiaApp::fCallback;
}

void OhosSkiaApp::SetNativeXComponent(OH_NativeXComponent* component) {
    LOGI("OhosSkiaApp::SetNativeXComponent");
    fXComponent = component;
    OH_NativeXComponent_RegisterCallback(fXComponent, &OhosSkiaApp::fCallback);
}

void OhosSkiaApp::RenderThread() {
    LOGI("OhosSkiaApp::RenderThread");
    while (fApp != nullptr) {
        std::unique_lock<std::mutex> locker(fMutex);
        if (!isAppQuit) {
            fApp->onIdle();
        } else {
            fCon.wait(locker);
        }
    }
}

OhosSkiaApp* OhosSkiaApp::GetInstance(std::string& id, ArkUI_NodeHandle handle, ArkUI_NativeNodeAPI_1* nodeApi) {
    LOGI("OhosSkiaApp::GetInstance");
    if (fInstanceMap.find(id) == fInstanceMap.end()) {
        fHandle = handle;
        fNodeApi = nodeApi;
        OhosSkiaApp* instance = new OhosSkiaApp(id, handle, nodeApi);
        fInstanceMap[id] = instance;
    }
    return fInstanceMap[id];
}

void OhosSkiaApp::SetInstance(std::string& id) {
    LOGI("OhosSkiaApp::SetInstance");
    if (fInstanceMap.find(id) == fInstanceMap.end()) {
        OhosSkiaApp* instance = new OhosSkiaApp(id, fHandle, fNodeApi);
        fInstanceMap[id] = instance;
    }
}

void OhosSkiaApp::OnSurfaceCreatedCB(OH_NativeXComponent* component, void* window) {
    LOGI("OhosSkiaApp::OnSurfaceCreatedCB");
    std::string id = GetXComponentId(component);
    auto render = OhosSkiaApp::GetInstance(id, fHandle, fNodeApi);
    render->OnSurfaceCreated(component, window);
}

void OhosSkiaApp::OnSurfaceChangedCB(OH_NativeXComponent* component, void* window) {
    LOGI("OhosSkiaApp::OnSurfaceDestroyedCB");
    std::string id = GetXComponentId(component);
    auto render = OhosSkiaApp::GetInstance(id, fHandle, fNodeApi);
    render->OnSurfaceChanged(component, window);
}

void OhosSkiaApp::OnSurfaceDestroyedCB(OH_NativeXComponent* component, void* window) {
    LOGI("OhosSkiaApp::OnSurfaceDestroyedCB");
    std::string id = GetXComponentId(component);
    auto render = OhosSkiaApp::GetInstance(id, fHandle, fNodeApi);
    render->OnSurfaceDestroyed(component, window);
}

void OhosSkiaApp::DispatchTouchEventCB(OH_NativeXComponent* component, void* window) {
    LOGI("OhosSkiaApp::DispatchTouchEventCB");
    std::string id = GetXComponentId(component);
    auto render = OhosSkiaApp::GetInstance(id, fHandle, fNodeApi);
    render->DispatchTouchEvent(component, window);
}

void OhosSkiaApp::OnSurfaceCreated(OH_NativeXComponent* component, void* window) {
    LOGI("OhosSkiaApp::OnSurfaceCreated");
    if (fApp == nullptr) {
        static const char* gCmdLine[] = {
            "viewer",
            "--skps",
            "/data/storage/el1/bundle/entry/resources/resfile/skps",
            "--backend",
            "grdawn"
        };

        fApp = Application::Create(std::size(gCmdLine),
                                   const_cast<char**>(gCmdLine),
                                   this);
        
        fNativeWindow = (OHNativeWindow*)window;
        auto window_ohos = (Window_ohos*)fWindow;
        window_ohos->initDisplay(fNativeWindow);
        window_ohos->paintIfNeeded();
        fRenderThread = std::thread(std::bind(&OhosSkiaApp::RenderThread, this));
    }
}

void OhosSkiaApp::OnSurfaceChanged(OH_NativeXComponent* component, void* window) {
    // E_NOT_IMPLEMENTED
}

void OhosSkiaApp::DispatchTouchEvent(OH_NativeXComponent* component, void* window) {
    LOGI("OhosSkiaApp::DispatchTouchEvent");

    int32_t ret = OH_NativeXComponent_GetTouchEvent(component, window, &fTouchEvent);
    auto touchEvent = OHOS_TO_WINDOW_STATEMAP.find(fTouchEvent.type);
    fWindow->onTouch(fTouchEvent.id, touchEvent->second, fTouchEvent.screenX, fTouchEvent.screenY);
}

void OhosSkiaApp::OnSurfaceDestroyed(OH_NativeXComponent* component, void* window) {
    auto window_ohos = (Window_ohos*)fWindow;
    window_ohos->onDisplayDestroyed();
    fNativeWindow = nullptr;
}

void OhosSkiaApp::setTitle(const char* title) const {
    // Have to find a way to change the title form the XComponent...
    // Fetch the XComponent
    // Create a struct with the new title
    // Call nodeAPI->setAttribute to modify the native application
    ArkUI_AttributeItem item;
    item.string = title;
    if (fHandle == nullptr) {
        LOGI("OhosSkiaApp::setTitle The node handle is invalid");
    }
    fNodeApi->setAttribute(fHandle, NODE_TEXT_CONTENT, &item);
}

void OhosSkiaApp::setUIState(const char* state) const {
    // E_NOT_IMPLEMENTED
}

int OhosSkiaApp::message_callback(int fd, int events, void* data) {
    // E_NOT_IMPLEMENTED
    return 0;
}

} // namespace sk_app
