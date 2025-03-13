#include "tools/sk_app/ohos/SkiaAppManager.h"
#include "tools/sk_app/ohos/logger_common.h"
#include <arkui/native_interface.h>
#include <arkui/native_node_napi.h>

// NAP Entry to drive the application!
EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports) {
    if ((env == nullptr) || (exports == nullptr)) {
        return nullptr;
    }

    sk_app::SkiaAppManager::GetInstance()->Init(env, exports);
    return exports;
}
EXTERN_C_END

static napi_module viewerModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "viewer",
    .nm_priv = ((void*)0),
    .reserved = { 0 }
};

extern "C" __attribute__((constructor)) void RegisterModule(void) {
    napi_module_register(&viewerModule);
}

// Skia application manager
namespace sk_app {

// Skia application manager methods
bool SkiaAppManager::Init(napi_env env, napi_value exports) {
    napi_value exportInstance = nullptr;
    OH_NativeXComponent* nativeXComponent = nullptr;
    // Fetch the XComponent handler
    napi_get_named_property(env, exports, OH_NATIVE_XCOMPONENT_OBJ, &exportInstance);
    // Get nativeXComponent
    napi_unwrap(env, exportInstance, reinterpret_cast<void**>(&nativeXComponent));

    // Can acquire title handle...
    ArkUI_NodeHandle* handle;
    // napi_value -> Supposed to be the node that I am building...
    OH_ArkUI_GetNodeHandleFromNapiValue(env, exports, handle);

    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    // Get nativeXComponent Id
    OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize);
    std::string id(idStr);
    SetNativeXComponent(id, nativeXComponent);

    ArkUI_NativeNodeAPI_1* nodeAPI = reinterpret_cast<ArkUI_NativeNodeAPI_1*>(
        OH_ArkUI_QueryModuleInterfaceByName(ARKUI_NATIVE_NODE, "ArkUI_NativeNodeAPI_1")
    );

    OhosSkiaApp* skiaInstance = GetRender(id, handle, nodeAPI);
    skiaInstance->SetNativeXComponent(nativeXComponent);

    return true;
}

OhosSkiaApp* SkiaAppManager::GetRender(std::string& id, ArkUI_NodeHandle* handle, ArkUI_NativeNodeAPI_1* nodeApi) {
    if (ohosSkiaMap.find(id) == ohosSkiaMap.end()) {
        OhosSkiaApp* instance = OhosSkiaApp::GetInstance(id, handle, nodeApi);
        ohosSkiaMap[id] = instance;
    }
    return ohosSkiaMap[id];
}

void SkiaAppManager::SetNativeXComponent(std::string& id, OH_NativeXComponent* nativeXComponent) {
    nativeXComponentMap[id] = nativeXComponent;
}

}