#ifndef XCOMPONENT_NODE_DEFINED
#define XCOMPONENT_NODE_DEFINED

#define NODE_ADDON_API_DISABLE_DEPRECATED

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <arkui/native_node.h>
#include <native_window/external_window.h>

#include "tools/sk_app/Window.h"
#include "tools/sk_app/ohos/ohos_log.h"

#include "tools/sk_app/Application.h"

#include <deque>
#include <memory>
#include <string>
#include <thread>
#include <condition_variable>

namespace sk_app {

class XComponentNode {
public:
    static std::unique_ptr<XComponentNode> Create(const std::string& id);
    static void OnFrameCB(OH_NativeXComponent* component, uint64_t timestamp, uint64_t targetTimestamp);
    void OnFrame(uint64_t timestamp, uint64_t targetTimestamp);
    void OnSurfaceCreated(void* window);
    void OnSurfaceChanged(void* window);
    void OnSurfaceDestroyed(void* window);
    void DispatchTouchEvent(void* window);

    void setTitle(const char* title);
    void setUIState(const char* state);

    uint64_t GetWidth();
    uint64_t GetHeight();

    ~XComponentNode();

    // void setGaneshGL();
    void setGaneshVk();
    void setGraphiteVulkan();
    void setGraphiteDawn();

    void SetPosition(float x, float y) { SetAttribute(NODE_POSITION, x, y); }
    void SetWidth(float width) { SetAttribute(NODE_WIDTH, width); }
    void SetHeight(float height) { SetAttribute(NODE_HEIGHT, height); }
    void SetWidthPercent(float width) { LOGI("XComponentNode::SetWidthPercent"); SetAttribute(NODE_WIDTH_PERCENT, width); }
    void SetHeightPercent(float height) { LOGI("XComponentNode::SetHeightPercent"); SetAttribute(NODE_HEIGHT_PERCENT, height); }
    void SetTranslate(float x, float y, float z) { SetAttribute(NODE_TRANSLATE, x, y, z); }
    void SetScale(float x, float y) { SetAttribute(NODE_SCALE, x, y); }
    void SetRotate(float x, float y, float z, float angle, float depth) { 
        SetAttribute(NODE_ROTATE, x, y, z, angle, depth);
    }
    void SetBackgroundColor(uint32_t rgba) { SetAttribute(NODE_BACKGROUND_COLOR, rgba); }
    void SetFocusable(bool focusable) { SetAttribute(NODE_FOCUSABLE, focusable ? 1 : 0); }
    void SetSurfaceSize(uint32_t width, uint32_t height) {
        SetAttribute(NODE_XCOMPONENT_SURFACE_SIZE, width, height);
    }

    ArkUI_NodeHandle handle() const { return fHandle; }
    static ArkUI_NativeNodeAPI_1* api();

    OH_NativeXComponent* GetXComponent() { return fXComponent; }
    void SetWindow(Window* window);

    void RenderThread();

    void AddChild(XComponentNode* child);

protected:

private:
    XComponentNode(ArkUI_NodeHandle handle,
                   const std::string& id);

    static XComponentNode* GetInstance(OH_NativeXComponent* component);

    void SetAttribute(ArkUI_NodeAttributeType attribute, uint32_t u32) {
        ArkUI_NumberValue value = {.u32 = u32};
        ArkUI_AttributeItem item = {&value, 1};
        api()->setAttribute(fHandle, attribute, &item);
    }

    void SetAttribute(ArkUI_NodeAttributeType attribute,
                      uint32_t u1,
                      uint32_t u2) {
        ArkUI_NumberValue values[2] = {{.u32 = u1}, {.u32 = u2}};
        ArkUI_AttributeItem item = {values, 2};
        api()->setAttribute(fHandle, attribute, &item);
    }

    void SetAttribute(ArkUI_NodeAttributeType attribute, int32_t i32) {
        ArkUI_NumberValue value = {.i32 = i32};
        ArkUI_AttributeItem item = {&value, 1};
    }

    void SetAttribute(ArkUI_NodeAttributeType attribute, float f32) {
        ArkUI_NumberValue value = {.f32 = f32};
        ArkUI_AttributeItem item = {&value, 1};
        api()->setAttribute(fHandle, attribute, &item);
    }

    void SetAttribute(ArkUI_NodeAttributeType attribute, float f1, float f2) {
        ArkUI_NumberValue values[2] = {{.f32 = f1}, {.f32 = f2}};
        ArkUI_AttributeItem item = {values, 2};
        api()->setAttribute(fHandle, attribute, &item);
    }

    void SetAttribute(ArkUI_NodeAttributeType attribute, float f1, float f2, float f3) {
        ArkUI_NumberValue values[3] = {{.f32 = f1}, {.f32 = f2}, {.f32 = f3}};
        ArkUI_AttributeItem item = {values, 3};
        api()->setAttribute(fHandle, attribute, &item);
    }

    void SetAttribute(ArkUI_NodeAttributeType attribute,
                      float f1,
                      float f2,
                      float f3,
                      float f4,
                      float f5) {
        ArkUI_NumberValue values[5] = {
            {.f32 = f1},
            {.f32 = f2},
            {.f32 = f3},
            {.f32 = f4},
            {.f32 = f5}
        };
        ArkUI_AttributeItem item = {values, 5};
        api()->setAttribute(fHandle, attribute, &item);
    }

    void SetAttribute(ArkUI_NodeAttributeType attribute, const char* string) {
        LOGI("XComponentNode::SetAttribute");
        ArkUI_AttributeItem item = {.string = string};
        api()->setAttribute(fHandle, attribute, &item);
    }

        
    Application* fApp;
    Window* fWindow;
    OHNativeWindow* fNativeWindow;
    OH_NativeXComponent_TouchEvent fTouchEvent;

    uint32_t fWidth;
    uint32_t fHeight;

    std::thread fRenderThread;

    const ArkUI_NodeHandle fHandle;
    const std::string fId;
    OH_NativeXComponent* const fXComponent;

    bool isAppQuit = false;
    static std::mutex fMutex;
    static std::condition_variable fCon;
};

}

#endif