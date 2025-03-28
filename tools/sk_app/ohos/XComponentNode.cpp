#include "tools/sk_app/ohos/XComponentNode.h"

#include <arkui/native_interface.h>
#include <native_buffer/native_buffer.h>
#include <native_window/external_window.h>
#include <unistd.h>
#include <uv.h>

#include <cassert>
#include <functional>
#include <map>
#include <mutex>

namespace sk_app {
namespace {
std::map<OH_NativeXComponent*, XComponentNode*> xcomponentNodes;
}  // namespace

// static
std::unique_ptr<XComponentNode> XComponentNode::Create(ohosSkiaApp* skiaApp,
                                                       const std::string& id,
                                                       Type type) {
    ArkUI_NodeHandle handle = api()->createNode(ARKUI_NODE_XCOMPONENT);
    FATAL_IF(handle == nullptr, "createNode(ARKUI_NODE_XCOMPONENT) failed!");

    std::unique_ptr<XComponentNode> component(
        new XComponentNode(skiaApp, handle, id, type));

    return component;
}

XComponentNode::XComponentNode(ohosSkiaApp* skiaApp,
                               ArkUI_NodeHandle handle,
                               const std::string& id,
                               Type type)
    : fSkiaApp(skiaApp),
      fHandle(handle),
      fId(id),
      fType(type),
      fComponent(OH_NativeXComponent_GetNativeXComponent(fHandle)) {
    assert(fComponent);
    xcomponent_nodes_[fComponent] = this;
  
    SetAttribute(NODE_XCOMPONENT_ID, id_.c_str());
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

    int32_t retval = OH_NativeXComponent_RegisterCallback(fComponent, &callbacks);
    FATAL_IF(retval != 0,
             "OH_NativeXComponent_RegisterCallback() failed retval=%{public}d",
             retval);

}

XComponentNode::~XComponentNode() {
    xcomponentNodes.erase(fComponent);
    api()->disposeNode(fHandle);
}

// static
ArkUI_NativeNodeAPI_1* XComponentNode::api() {
    static ArkUI_NativeNodeAPI_1* api = nullptr;
    static std::once_flag flag;
    std::call_once(flag, [&] {
        api = reinterpret_cast<ArkUI_NativeNodeAPI_1*>(
            OH_ArkUI_QueryModuleInterfaceByName(ARKUI_NATIVE_NODE,
                                              "ArkUI_NativeNodeAPI_1"));
    });

  return api;
}

void XComponentNode::RenderThread() {
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

void XComponentNode::AddChild(XComponentNode* child) {
  api()->addChild(handle(), child->handle());
}

void XComponentNode::OnSurfaceCreated(void* window) {
    LOGI("OhosSkiaApp::OnSurfaceCreated");
    if (fSkiaApp == nullptr) {
        static const char* gCmdLine[] = {
            "viewer",
            "--skps",
            "/data/storage/el1/bundle/entry/resources/resfile/skps",
            "--backend",
            "grvk"
        };

      fSkiaApp = Application::Create(std::size(gCmdLine),
                                 const_cast<char**>(gCmdLine),
                                 this);
      
      OH_NativeXComponent_ExpectedRateRange frameRate = {
          .min = 30,
          .max = 60,
          .expected = 30
      };

      OH_NativeXComponent_SetExpectedFrameRateRange(fComponent, &frameRate);
      OH_NativeXComponent_RegisterOnFrameCallback(fComponent, 
        [](OH_NativeXComponent* component, 
           uint64_t timestamp,
           uint64_t target_timestamp) {
              GetInstance(component)->OnPaintIfNeeded(timestamp, target_timestamp);
      });

      fWindow = (OHNativeWindow*)window;
      auto window_ohos = (Window_ohos*)fWindow;
      window_ohos->initDisplay(fNativeWindow);
      window_ohos->paintIfNeeded();
      fRenderThread = std::thread(std::bind(&XComponentNode::RenderThread, this));
    }
}

void XComponentNode::OnSurfaceChanged(void* window) {
  // NOT_IMPLEMENTED
}


void XComponentNode::OnSurfaceDestroyed(void* window) {}

void XComponentNode::DispatchTouchEvent(void* window) {}

void XComponentNode::OnPaintIfNeeded(uint64_t timestamp, uint64_t targetTimestamp) {
    auto window_ohos = (Window_ohos*)fWindow;
    window_ohos->paintIfNeeded();
}

// static
XComponentNode* XComponentNode::GetInstance(OH_NativeXComponent* component) {
    auto it = xcomponentNodes.find(component);
    assert(it != xcomponentNodes.end());
    return it->second;
}

}  // namespace sk_app
