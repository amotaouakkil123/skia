

#define NODE_ADDON_API_DISABLE_DEPRECATED
#include "hello/NapiManager.h"
#include "napi.h"

namespace Viewer {

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  hello::NapiManager::Init(env);
  exports.Set(
      "createNativeNode",
      Napi::Function::New<hello::NapiManager::NapiCreateNativeNode>(env));
  exports.Set(
      "setSkiaBackend",
      Napi::Function::New<hello::NapiManager::NapiSetSkiaBackend>(
          env));

  return exports;
}

}  // namespace helloxcomponent

using Viewer::Init;

NODE_API_MODULE(Viewer, Init)
