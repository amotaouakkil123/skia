#ifndef surface_glue_ohos_DEFINED
#define surface_glue_ohos_DEFINED

#include <napi/native_api.h>
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <arkui/native_node.h>
#include <native_window/external_window.h>

#include <string>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <memory>

#include "include/core/SkString.h"
#include "tools/sk_app/Application.h"
#include "tools/sk_app/Window.h"
#include "tools/sk_app/ohos/SkiaAppManager.h"

namespace sk_app {

enum MessageType {
    kUndefined,
    kSurfaceCreated,
    kSurfaceChanged,
    kSurfaceDestroyed,
    kDestroyApp,
    kContentInvalidated,
    kKeyPressed,
    kTouched,
    kUIStateChanged
};

struct Message {
    MessageType fType = kUndefined;
    OHNativeWindow* fNativeWindow = nullptr;
    int fKeycode = 0;
    int fTouchOwner, fTouchState;
    float fTouchX, fTouchY;

    SkString* stateName;
    SkString* stateValue;

    Message() {}
    Message(MessageType t) : fType(t) {}
};

class OhosSkiaApp {

public:

    Application* fApp;
    Window* fWindow;

    OhosSkiaApp(std::string& id);

    void postMessage(const Message& message) const;
    void readMessage(Message* message) const;

    void setTitle(const char* title) const;
    void setUIState(const char* state) const;

    static OH_NativeXComponent_Callback* GetNXComponentCallback();

    void OnSurfaceCreated(OH_NativeXComponent* component, void* window);
    void OnSurfaceChanged(OH_NativeXComponent* component, void* window);
    void OnSurfaceDestroyed(OH_NativeXComponent* component, void* window);
    void DispatchTouchEvent(OH_NativeXComponent* component, void* window);

    void SetNativeXComponent(OH_NativeXComponent* component);
    void RenderThread();

    static void OnSurfaceCreatedCB(OH_NativeXComponent* component, void* window);
    static void OnSurfaceChangedCB(OH_NativeXComponent* component, void* window);
    static void OnSurfaceDestroyedCB(OH_NativeXComponent* component, void* window);
    static void DispatchTouchEventCB(OH_NativeXComponent* component, void* window);

    static OhosSkiaApp* GetInstance(std::string& id);
    void SetInstance(std::string& id);

private:

    std::thread fRenderThread;
    OHNativeWindow* fNativeWindow;

    // Contain all the instance that make the OHOS interactions work
    static OH_NativeXComponent* fXComponent;
    static OH_NativeXComponent_Callback fCallback;

    bool isAppQuit = false;
    std::string fId;
    static std::mutex fMutex;
    static std::condition_variable fCon;

    static std::unordered_map<std::string, OhosSkiaApp*> fInstanceMap;
    ~OhosSkiaApp();

    static int message_callback(int fd, int events, void* data);
    static void* pthread_main(void*);
};

} // namespace sk_app

#endif
