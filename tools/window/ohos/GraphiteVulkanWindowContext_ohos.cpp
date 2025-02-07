#include "tools/window/ohos/WindowContextFactory_ohos.h"
#include "tools/window/GraphiteNativeVulkanWindowContext.h"
#include "tools/gpu/vk/VkTestUtils.h"
#include "tools/sk_app/ohos/ohos_log.h"

using skwindow::DisplayParams;
using skwindow::internal::GraphiteVulkanWindowContext;

namespace {

class GraphiteVulkanWindowContext_ohos : public GraphiteVulkanWindowContext {
public:
    GraphiteVulkanWindowContext_ohos(std::unique_ptr<const DisplayParams> params,
                                     CreateVkSurfaceFn createVkSurface,
                                     CanPresentFn canPresent,
                                     PFN_vkGetInstanceProcAddr instProc);

    bool onInitializeContext();
    void onDestroyContext();

    /** Platform specific function that creates a VkSurfaceKHR for a window */
    using CreateVkSurfaceFn = std::function<VkSurfaceKHR(VkInstance)>;
    /** Platform specific function that determines whether presentation will succeed. */
    using CanPresentFn = sk_gpu_test::CanPresentFn;

private:
    OHNativeWindow* fWindow;
};

GraphiteVulkanWindowContext_ohos::GraphiteVulkanWindowContext_ohos(
    std::unique_ptr<const DisplayParams> params,
    CreateVkSurfaceFn createVkSurface,
    CanPresentFn canPresent,
    PFN_vkGetInstanceProcAddr instProc)
    : GraphiteVulkanWindowContext(std::move(params), createVkSurface, canPresent, instProc) {}

void GraphiteVulkanWindowContext_ohos::onDestroyContext() {
    if (!fWindow) {
        return;
    }

    OH_NativeWindow_DestroyNativeWindow(fWindow);
}

}

namespace skwindow {

std::unique_ptr<WindowContext> MakeGraphiteVulkanForOhos(OHNativeWindow* window,
                                                         std::unique_ptr<const DisplayParams> params) {
    LOGI("VulkanWindowContext_ohos::MakeGraphiteVulkanForOhos");
    PFN_vkGetInstanceProcAddr instProc;
    if (!sk_gpu_test::LoadVkLibraryAndGetProcAddrFuncs(&instProc)) {
        LOGD("VulkanWindowContext_ohos::MakeGraphiteVulkanForOhos Unable to load the vulkan methods");
        return nullptr;
    }

    auto createVkSurface = [window, instProc] (VkInstance instance) -> VkSurfaceKHR {
        LOGI("VulkanWindowContext_ohos::createVkSurface");
        PFN_vkCreateSurfaceOHOS createSurfaceOHOS =
            (PFN_vkCreateSurfaceOHOS) instProc(instance, "vkCreateSurfaceOHOS");

        if (!window) {
            LOGD("VulkanWindowContext_ohos::createVkSurface Window creation failed");
            return VK_NULL_HANDLE;
        }

        VkSurfaceKHR surface;

        VkSurfaceCreateInfoOHOS surfaceCreateInfo;
        memset(&surfaceCreateInfo, 0, sizeof(VkSurfaceCreateInfoOHOS));
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_SURFACE_CREATE_INFO_OHOS;
        surfaceCreateInfo.pNext = nullptr;
        surfaceCreateInfo.flags = 0;
        surfaceCreateInfo.window = window;

        VkResult res = createSurfaceOHOS(instance, &surfaceCreateInfo,
                                         nullptr, &surface);

        return (VK_SUCCESS == res) ? surface : VK_NULL_HANDLE;
    };

    auto canPresent = [](VkInstance, VkPhysicalDevice, uint32_t) {return true;};

    std::unique_ptr<WindowContext> ctx(new GraphiteVulkanWindowContext_ohos(std::move(params), createVkSurface, canPresent, instProc));

    if (!ctx->isValid()) {
        return nullptr;
    }

    return ctx;
}

}