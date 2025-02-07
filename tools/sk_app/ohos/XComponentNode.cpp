#include "tools/sk_app/ohos/XComponentNode.h"
#include "tools/sk_app/ohos/Window_ohos.h"
#include "include/core/SkString.h"
#include "tools/sk_app/ohos/ohos_log.h"
#include <arkui/native_interface.h>

#include <cassert>
#include <functional>
#include <map>
#include <mutex>

namespace sk_app {
namespace {
std::map<OH_NativeXComponent*, XComponentNode*> xcomponentNodes;
}

std::mutex XComponentNode::fMutex;
std::condition_variable XComponentNode::fCon;


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

std::unique_ptr<XComponentNode> XComponentNode::Create(const std::string& id) {
    LOGI("XComponentNode::Create");
    ArkUI_NodeHandle handle = api()->createNode(ARKUI_NODE_XCOMPONENT);
    FATAL_IF(handle == nullptr, "createNode(ARKUI_NODE_XCOMPONENT) failed!");

    std::unique_ptr<XComponentNode> component(new XComponentNode(handle, id));

    return component;
}

XComponentNode::XComponentNode(ArkUI_NodeHandle handle,
                               const std::string& id)
        : fHandle(handle)
        , fId(id)
        , fXComponent(OH_NativeXComponent_GetNativeXComponent(fHandle)) {
    LOGI("XComponentNode::XComponentNode");
    assert(fXComponent);
    xcomponentNodes[fXComponent] = this;

    SetAttribute(NODE_XCOMPONENT_ID, id.c_str());
    SetAttribute(NODE_XCOMPONENT_TYPE, ARKUI_XCOMPONENT_TYPE_SURFACE);

    static OH_NativeXComponent_Callback callbacks = {
        [](OH_NativeXComponent* component, void* window) {
          GetInstance(component)->OnSurfaceCreated(window);
        },
        [](OH_NativeXComponent* component, void* window) {
          GetInstance(component)->OnSurfaceChanged(window);
        },
        [](OH_NativeXComponent* component, void* window) {
          GetInstance(component)->OnSurfaceDestroyed(window);
        },
        [](OH_NativeXComponent* component, void* window) {
          GetInstance(component)->DispatchTouchEvent(window);
        },
    };

    int32_t ret = OH_NativeXComponent_RegisterCallback(fXComponent, &callbacks);
    FATAL_IF(ret != 0,
             "OH_NativeXComponent_RegisterCallback() failed ret=%{public}d",
             ret);
}

XComponentNode::~XComponentNode() {
    LOGI("XComponentNode::~XComponentNode");
    xcomponentNodes.erase(fXComponent);
    api()->disposeNode(fHandle);
}

ArkUI_NativeNodeAPI_1* XComponentNode::api() {
    LOGI("XComponentNode::api");
    static ArkUI_NativeNodeAPI_1* api = nullptr;
    static std::once_flag flag;
    std::call_once(flag, [&] {
        api = reinterpret_cast<ArkUI_NativeNodeAPI_1*>(
            OH_ArkUI_QueryModuleInterfaceByName(ARKUI_NATIVE_NODE,
                                                "ArkUI_NativeNodeAPI_1"));
    });
    
    return api;
}

void XComponentNode::OnSurfaceCreated(void* window) {
    LOGI("XComponentNode::OnSurfaceCreated");

    if (fApp == nullptr) {
        static const char* gCmdLine[] = {
            "viewer",
            "--skps",
            "/data/storage/el1/bundle/entry/resources/resfile/skps",
            "--backend",
            "vk"
        };

        fApp = Application::Create(std::size(gCmdLine),
                                   const_cast<char**>(gCmdLine),
                                   this);
    }

    OH_NativeXComponent_ExpectedRateRange frameRate = {
        .min = 30,
        .max = 60,
        .expected = 30
    };

    OH_NativeXComponent_SetExpectedFrameRateRange(fXComponent, &frameRate);
    OH_NativeXComponent_RegisterOnFrameCallback(fXComponent, OnFrameCB);

    fNativeWindow = (OHNativeWindow*)window;
    auto window_ohos = (Window_ohos*)fWindow;
    window_ohos->initDisplay(fNativeWindow);
    window_ohos->paintIfNeeded();
    fRenderThread = std::thread(std::bind(&XComponentNode::RenderThread, this));
}


void XComponentNode::OnSurfaceChanged(void* window) {
    LOGI("XComponentNode::OnSurfaceChanged");
}

void XComponentNode::OnSurfaceDestroyed(void* window) {
    LOGI("XComponentNode::OnSurfaceDestroyed");
}

void XComponentNode::DispatchTouchEvent(void* window) {
    LOGI("XComponentNode::DispatchTouchEvent");
    
    int32_t ret = OH_NativeXComponent_GetTouchEvent(fXComponent, window, &fTouchEvent);
    if (ret != OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
        LOGD("XComponentNode::DispatchTouchEvent Failed to get Touch Event");
    }
    auto touchEvent = OHOS_TO_WINDOW_STATEMAP.find(fTouchEvent.type);
    if (touchEvent->second == skui::InputState::kDown) {
        fWindow->onFling(skui::InputState::kLeft);
    }
}

void XComponentNode::OnFrameCB(OH_NativeXComponent* component, uint64_t timestamp, uint64_t targetTimestamp) {
    // LOGI("XComponentNode::OnFrameCB");
    GetInstance(component)->OnFrame(timestamp, targetTimestamp);
}

void XComponentNode::OnFrame(uint64_t timestamp, uint64_t targetTimestamp) {
    // LOGI("XComponentNode::OnFrame");
    Window_ohos* window_ohos = (Window_ohos*)fWindow;
    window_ohos->paintIfNeeded();
}

void XComponentNode::RenderThread() {
    LOGI("XComponentNode::RenderThread");
    while (fApp != nullptr) {
        std::unique_lock<std::mutex> locker(fMutex);
        if (!isAppQuit) {
            fApp->onIdle();
        } else {
            fCon.wait(locker);
        }
    }
}

void XComponentNode::SetWindow(Window* window) {
    fWindow = window;
}

void XComponentNode::setTitle(const char* title) {
    LOGI("XComponentNode::setTitle");
}

void XComponentNode::setUIState(const char* state) {
    LOGI("XComponentNode::setUIState");
}

// void XComponentNode::setGaneshGL() {
//     if (fApp == nullptr)
//         return;
//     LOGI("XComponentNode::setGanesh");
//     auto window_ohos = (Window_ohos*)fWindow;
//     window_ohos->onDisplayDestroyed();
//     OH_NativeWindow_DestroyNativeWindow(fNativeWindow);
//     fNativeWindow = nullptr;

//     SkString backend;
//     backend.append("Backend");
//     SkString backendType;
//     backendType.append("OpenGL");
//     fWindow->onUIStateChanged(backend, backendType);

//     window_ohos->initDisplay(fNativeWindow);
// }

void XComponentNode::setGaneshVk() {
    if (fApp == nullptr)
        return;
    LOGI("XComponentNode::setGanesh");
    auto window_ohos = (Window_ohos*)fWindow;
    window_ohos->onDisplayDestroyed();
    // OH_NativeWindow_DestroyNativeWindow(fNativeWindow);
    // fNativeWindow = nullptr;

    SkString backend;
    backend.append("Backend");
    SkString backendType;
    backendType.append("Vulkan");
    fWindow->onUIStateChanged(backend, backendType);

    window_ohos->initDisplay(fNativeWindow);
}

void XComponentNode::setGraphiteVulkan() {
    if (fApp == nullptr)
        return;
    LOGI("XComponentNode::setGraphiteVulkan");
    auto window_ohos = (Window_ohos*)fWindow;
    window_ohos->onDisplayDestroyed();
    // OH_NativeWindow_DestroyNativeWindow(fNativeWindow);
    // fNativeWindow = nullptr;

    SkString backend;
    backend.append("Backend");
    SkString backendType;
    backendType.append("Vulkan (Graphite)");
    fWindow->onUIStateChanged(backend, backendType);

    window_ohos->initDisplay(fNativeWindow);
}

void XComponentNode::setGraphiteDawn() {
    if (fApp == nullptr)
        return;
    LOGI("XComponentNode::setGraphiteDawn");
    auto window_ohos = (Window_ohos*)fWindow;
    window_ohos->onDisplayDestroyed();
    // OH_NativeWindow_DestroyNativeWindow(fNativeWindow);
    // fNativeWindow = nullptr;

    SkString backend;
    backend.append("Backend");
    SkString backendType;
    backendType.append("Dawn (Graphite)");
    fWindow->onUIStateChanged(backend, backendType);

    window_ohos->initDisplay(fNativeWindow);
}

void XComponentNode::AddChild(XComponentNode* child) {
    LOGI("XComponentNode::AddChild");
    api()->addChild(handle(), child->handle());
}

XComponentNode* XComponentNode::GetInstance(OH_NativeXComponent* component) {
    // LOGI("XComponentNode::GetInstance");
    auto it = xcomponentNodes.find(component);
    assert(it != xcomponentNodes.end());
    return it->second;
}

uint64_t XComponentNode::GetHeight() {
    uint64_t width, height;
    int32_t ret = OH_NativeXComponent_GetXComponentSize(fXComponent, fNativeWindow, &height, &width);
    return height;
}

uint64_t XComponentNode::GetWidth() {
    uint64_t width, height;
    int32_t ret = OH_NativeXComponent_GetXComponentSize(fXComponent, fNativeWindow, &height, &width);
    return width;
}

}
