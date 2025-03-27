#ifndef XCOMPONENTNODE_H
#define XCOMPONENTNODE_H

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <arkui/native_node.h>
#include <native_window/external_window.h>

#include "tools/sk_app/ohos/surface_glue_ohos.h"
#include "tools/sk_app/ohos/logger_common.h"

#include <deque>
#include <memory>
#include <string>

namespace sk_app {

class XComponentNode {
public:

  struct OhosSkiaApp {
        Application* fApp;
        Window* fWindow;
    
        void setTitle(const char* title) const;
        void setUIState(const char* state) const;
  };

  static std::unique_ptr<XComponentNode> Create(ohosSkiaApp* skiaApp,
                                                const std::string& id,
                                                Type type);

  ~XComponentNode();

  void AddChild(XComponentNode* child);

  void SetPosition(float x, float y) { SetAttribute(NODE_POSITION, x, y); }
  void SetWidth(float width) { SetAttribute(NODE_WIDTH, width); }
  void SetHeight(float height) { SetAttribute(NODE_HEIGHT, height); }
  void SetWidthPercent(float width) { SetAttribute(NODE_WIDTH_PERCENT, width); }

  void SetHeightPercent(float height) {
    SetAttribute(NODE_HEIGHT_PERCENT, height);
  }

  void SetTranslate(float x, float y, float z) {
    SetAttribute(NODE_TRANSLATE, x, y, z);
  }

  void SetScale(float x, float y) { SetAttribute(NODE_SCALE, x, y); }
  
  void SetRotate(float x, float y, float z, float angle, float depth) {
    SetAttribute(NODE_ROTATE, x, y, z, angle, depth);
  }

  void SetSurfaceSize(uint32_t width, uint32_t height) {
    SetAttribute(NODE_XCOMPONENT_SURFACE_SIZE, width, height);
  }

  void RenderThread();

  ArkUI_NodeHandle handle() const { return handle_; }
  static ArkUI_NativeNodeAPI_1* api();
  
protected:
  virtual void OnSurfaceCreated(void* window);
  virtual void OnSurfaceChanged(void* window);
  virtual void OnSurfaceDestroyed(void* window);
  virtual void DispatchTouchEvent(void* window);
  virtual void OnFrame(uint64_t timestamp, uint64_t target_timestamp);

private:
  XComponentNode(OhosSkiaApp* delegate,
                 ArkUI_NodeHandle handle,
                 const std::string& id,
                 Type type);

  static XComponentNode* GetInstance(OH_NativeXComponent* component);

  void SetAttribute(ArkUI_NodeAttributeType attribute, uint32_t u32) {
    ArkUI_NumberValue value = {.u32 = u32};
    ArkUI_AttributeItem item = {&value, 1};
    api()->setAttribute(handle_, attribute, &item);
  }

  void SetAttribute(ArkUI_NodeAttributeType attribute,
                    uint32_t u1,
                    uint32_t u2) {
    ArkUI_NumberValue values[2] = {{.u32 = u1}, {.u32 = u2}};
    ArkUI_AttributeItem item = {values, 2};
    api()->setAttribute(handle_, attribute, &item);
  }

  void SetAttribute(ArkUI_NodeAttributeType attribute, int32_t u32) {
    ArkUI_NumberValue value = {.i32 = u32};
    ArkUI_AttributeItem item = {&value, 1};
    api()->setAttribute(handle_, attribute, &item);
  }

  void SetAttribute(ArkUI_NodeAttributeType attribute, float f32) {
    ArkUI_NumberValue value = {.f32 = f32};
    ArkUI_AttributeItem item = {&value, 1};
    api()->setAttribute(handle_, attribute, &item);
  }

  void SetAttribute(ArkUI_NodeAttributeType attribute, float f1, float f2) {
    ArkUI_NumberValue values[2] = {{.f32 = f1}, {.f32 = f2}};
    ArkUI_AttributeItem item = {values, 2};
    api()->setAttribute(handle_, attribute, &item);
  }

  void SetAttribute(ArkUI_NodeAttributeType attribute, float f1, float f2, float f3) {
    ArkUI_NumberValue values[3] = {{.f32 = f1}, {.f32 = f2}, {.f32 = f3}};
    ArkUI_AttributeItem item = {values, 3};
    api()->setAttribute(handle_, attribute, &item);
  }

  void SetAttribute(ArkUI_NodeAttributeType attribute,
                    float f1,
                    float f2,
                    float f3,
                    float f4,
                    float f5) {
    ArkUI_NumberValue values[5] = {
        {.f32 = f1}, {.f32 = f2}, {.f32 = f3}, {.f32 = f4}, {.f32 = f5}};
    ArkUI_AttributeItem item = {values, 5};
    api()->setAttribute(handle_, attribute, &item);
  }

  void SetAttribute(ArkUI_NodeAttributeType attribute, const char* string) {
    ArkUI_AttributeItem item = {.string = string};
    api()->setAttribute(handle_, attribute, &item);
  }

  OhosSkiaApp* fSkiaApp;
  const ArkUI_NodeHandle fHandle;
  const std::string fId;
  const Type fType;
  OH_NativeXComponent* const fComponent;

  std::thread fRenderThread;

  std::unique_ptr<OHNativeWindow> fNativeWindow;
  uint64_t fWidth = 0;
  uint64_t fHeight = 0;
  
};

}  // namespace sk_app

#endif  // XCOMPONENTNODE_H
