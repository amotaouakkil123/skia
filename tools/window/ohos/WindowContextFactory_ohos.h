#ifndef WindowContextFactory_ohos_DEFINED
#define WindowContextFactory_ohos_DEFINED

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <native_window/external_window.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_ohos.h>

#include <memory>

namespace skwindow {

class WindowContext;
struct DisplayParams;

std::unique_ptr<WindowContext> MakeVulkanForOhos(OHNativeWindow*, const DisplayParams&);

std::unique_ptr<WindowContext> MakeGraphiteVulkanForOhos(OHNativeWindow*, const DisplayParams&);

std::unique_ptr<WindowContext> MakeGraphiteDawnVulkanForOhos(OHNativeWindow*, const DisplayParams);

std::unique_ptr<WindowContext> MakeGLForOhos(OHNativeWindow*, const DisplayParams&);

std::unique_ptr<WindowContext> MakeRasterForOhos(OHNativeWindow*, const DisplayParams&);

} // namespace skwindow

#endif