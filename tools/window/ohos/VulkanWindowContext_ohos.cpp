#include "tools/window/ohos/WindowContextFactory_ohos.h"
#include "tools/window/VulkanWindowContext.h"
#include "tools/gpu/vk/VkTestUtils.h"
#include "tools/sk_app/ohos/logger_common.h"

namespace skwindow {

std::unique_ptr<WindowContext> MakeVulkanForOhos(OHNativeWindow* window,
                                                 std::unique_ptr<const DisplayParams> params) {
    LOGI("VulkanWindowContext_ohos::MakeVulkanForOhos");
    PFN_vkGetInstanceProcAddr instProc;
    if (!sk_gpu_test::LoadVkLibraryAndGetProcAddrFuncs(&instProc)) {
        LOGD("VulkanWindowContext_ohos::MakeVulkanForOHOS Unable to load the vulkan methods");
        return nullptr;
    }

    auto createVkSurface = [window, instProc] (VkInstance instance) -> VkSurfaceKHR {
        LOGI("VulkanWindowContext::createVkSurface");
        PFN_vkCreateSurfaceOHOS createSurfaceOHOS = 
            (PFN_vkCreateSurfaceOHOS) instProc(instance, "vkCreateSurfaceOHOS");
        
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

    auto canPresent = [](VkInstance, VkPhysicalDevice, uint32_t) { return true; };

    std::unique_ptr<WindowContext> ctx(new internal::VulkanWindowContext(std::move(params), createVkSurface, 
                                                                         canPresent, instProc));

    if (!ctx->isValid()) {
        return nullptr;
    }

    return ctx;
}

}

