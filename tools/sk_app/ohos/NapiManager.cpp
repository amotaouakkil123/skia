#include "hello/NapiManager.h"

#include <arkui/native_node.h>
#include <arkui/native_node_napi.h>

#include <cassert>
#include <cstdint>
#include <string>

#include "hello/DelegatedNodeContent.h"
#include "hello/GLCore.h"
#include "hello/Log.h"
#include "hello/NonDelegatedNodeContent.h"
#include "hello/DisplayManager.h"

namespace sk_app {
namespace {
NapiManager* napiManager = nullptr;
}

// static
void NapiManager::Init(const Napi::Env& env) {
  CHECK(!napiManager);
  napiManager = new NapiManager(env);
}

// static
NapiManager* NapiManager::GetInstance() {
  CHECK(napiManager);
  return napiManager;
}

NapiManager::NapiManager(const Napi::Env& env) : fEnv(env) {
  std::srand(std::time({}));
  fSkiaApp = std::make_unique<OhosSkiaApp>();
  fSkiaApp->Init();
}

NapiManager::~NapiManager() {
  fSkiaApp->Destroy();
}

// static
Napi::Value NapiManager::NapiCreateNativeNode(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 2) {
    Napi::Error::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  ArkUI_NodeContentHandle content_handle = nullptr;
  int32_t retval =
      OH_ArkUI_GetNodeContentFromNapiValue(env, info[0], &content_handle);
  if (retval != ARKUI_ERROR_CODE_NO_ERROR) {
    Napi::Error::New(env, "Arg 0 is not a NodeContent")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[1].IsBoolean()) {
    Napi::Error::New(env, "Arg 1 is not a boolean")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[2].IsBoolean()) {
    Napi::Error::New(env, "Arg 2 is not a boolean")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  bool isGanesh = info[1].As<Napi::Boolean>().Value();
  bool isDawn = info[2].As<Napi::Boolean>().Value();

  GetInstance()->CreateNativeNode(content_handle, isGanesh, isDawn);

  return env.Null();
}

// static
Napi::Value NapiManager::NapiSetSkiaBackend(
    const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::Error::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsBoolean()) {
    Napi::Error::New(env, "Arg 0 is not a boolean")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  bool isGanesh = info[0].As<Napi::Boolean>().Value();
  bool isDawn = info[1].As<Napi::Boolean>().Value();

  GetInstance()->SetSkiaBackend(isGanesh, isGraphite);

  return env.Null();
}

void NapiManager::CreateNativeNode(ArkUI_NodeContentHandle content_handle,
                                   bool isGanesh,
                                   bool isDawn) {
  ohosViewer = std::make_unique<OhosSkiaApp>(content_handle, isGanesh, isDawn);
}

void NapiManager::SetSkiaBackend(bool isGanesh, bool isDawn) {
  fIsGanesh = isGanesh;
  fIsDawn = isDawn;
  Update();
}

void NapiManager::Update() {
  fSkiaApp->SetSkiaBackend(fIsGanesh, fIsDawn);
}

}  // namespace sk_app
