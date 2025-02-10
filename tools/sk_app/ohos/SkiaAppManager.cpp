#include "tools/sk_app/ohos/SkiaAppManager.h"
#include "tools/sk_app/ohos/logger_common.h"

// NAP Entry to drive the application!
EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports) {
    LOGD("We are doing this!");
    if ((env == nullptr) || (exports == nullptr)) {
        return nullptr;
    }

    if (!sk_app::SkiaAppManager::GetInstance()->Init(env, exports)) {
        LOGE("Failed to init NAPI!");
    }

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

    char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
    // Get nativeXComponent Id
    OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize);
    std::string id(idStr);
    SetNativeXComponent(id, nativeXComponent);
    OhosSkiaApp* skiaInstance = GetRender(id);
    skiaInstance->SetNativeXComponent(nativeXComponent);
    return true;
}

OhosSkiaApp* SkiaAppManager::GetRender(std::string& id) {
    if (ohosSkiaMap.find(id) == ohosSkiaMap.end()) {
        OhosSkiaApp* instance = OhosSkiaApp::GetInstance(id);
        ohosSkiaMap[id] = instance;
    }
    return ohosSkiaMap[id];
}

void SkiaAppManager::SetNativeXComponent(std::string& id, OH_NativeXComponent* nativeXComponent) {
    nativeXComponentMap[id] = nativeXComponent;
}
}