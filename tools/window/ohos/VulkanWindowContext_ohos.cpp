#include "tools/window/ohos/WindowContextFactory_ohos.h"
#include "tools/window/VulkanWindowContext.h"
#include "tools/gpu/vk/VkTestUtils.h"
#include "tools/sk_app/ohos/logger_common.h"

namespace skwindow {

std::unique_ptr<WindowContext> MakeVulkanForOhos(OHNativeWindow* window,
                                                 std::unique_ptr<const DisplayParams> params) {
    LOGD("VulkanWindowContext_ohos::MakeVulkanForOhos Vulkan OHOS making the window!");
    PFN_vkGetInstanceProcAddr instProc;
    if (!sk_gpu_test::LoadVkLibraryAndGetProcAddrFuncs(&instProc)) {
        LOGD("VulkanWindowContext_ohos::MakeVulkanForOHOS Unable to load the proper methods");
        return nullptr;
    }

    LOGD("VulkanWindowContext_ohos::MakeVulkanForOHOS Successfully loaded methods");
    auto createVkSurface = [window, instProc] (VkInstance instance) -> VkSurfaceKHR {
        LOGD("VulkanWindowContext::createVkSurface inside the method");
        PFN_vkCreateSurfaceOHOS createSurfaceOHOS = 
            (PFN_vkCreateSurfaceOHOS) instProc(instance, "vkCreateSurfaceOHOS");
        
        if (!window) {
            LOGD("VulkanWindowContext_ohos::createVkSurface Your window stinks man");
            return VK_NULL_HANDLE;
        } else {
            LOGD("VulkanWindowContext_ohos::createVkSurface Hell yeah your window's good");
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
    };

    auto canPresent = [](VkInstance, VkPhysicalDevice, uint32_t) { return true; };
    std::unique_ptr<WindowContext> ctx(new internal::VulkanWindowContext(std::move(params), createVkSurface, 
                                                                         canPresent, instProc));

    if (!ctx->isValid()) {
        return nullptr;
    }

    LOGD("VulkanWindowContext_ohos::MakeVulkanForOhos Vulkan windowing context is valid!");
    return ctx;
}

}

