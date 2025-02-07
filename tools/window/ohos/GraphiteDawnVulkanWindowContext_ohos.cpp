#include "dawn/native/DawnNative.h"
#include "webgpu/webgpu_cpp.h"

#include "tools/window/GraphiteDawnWindowContext.h"
#include "tools/window/ohos/WindowContextFactory_ohos.h"

#include "tools/sk_app/ohos/ohos_log.h"

using skwindow::DisplayParams;
using skwindow::internal::GraphiteDawnWindowContext;

namespace {

class GraphiteDawnVulkanWindowContext_ohos : public GraphiteDawnWindowContext {
public:
    GraphiteDawnVulkanWindowContext_ohos(OHNativeWindow* window, 
                                         std::unique_ptr<const DisplayParams> params, 
                                         uint32_t width,
                                         uint32_t height);

    bool onInitializeContext() override;
    void onDestroyContext() override;
    void resize(int w, int h) override;

private:
    OHNativeWindow* fWindow;
};

GraphiteDawnVulkanWindowContext_ohos::GraphiteDawnVulkanWindowContext_ohos(
    OHNativeWindow* window,
    std::unique_ptr<const DisplayParams> params,
    uint32_t width,
    uint32_t height)
    : GraphiteDawnWindowContext(std::move(params), wgpu::TextureFormat::RGBA8Unorm)
    , fWindow(window) {
    
    // Switch width and height for window to fit on the phone
    this->initializeContext(height, width);
}

bool GraphiteDawnVulkanWindowContext_ohos::onInitializeContext() {
    LOGI("GraphiteDawnVulkanWindowContext_ohos::onInitializeContext");
    SkASSERT(!!fWindow);

    auto device = this->createDevice(wgpu::BackendType::Vulkan);
    if (!device) {
        SkASSERT(device);
        return false;
    }

    wgpu::SurfaceSourceOHNativeWindow surfaceChainedDesc;
    surfaceChainedDesc.window = fWindow;

    wgpu::SurfaceDescriptor surfaceDesc;
    surfaceDesc.nextInChain = &surfaceChainedDesc;

    auto surface = wgpu::Instance(fInstance->Get()).CreateSurface(&surfaceDesc);
    if (!surface) {
        SkASSERT(false);
        return false;
    }

    fDevice = std::move(device);
    fSurface = std::move(surface);
    configureSurface();

    return true;
}

void GraphiteDawnVulkanWindowContext_ohos::onDestroyContext() {
    if (!fWindow) {
        return;
    }
    
    OH_NativeWindow_DestroyNativeWindow(fWindow);
}

void GraphiteDawnVulkanWindowContext_ohos::resize(int w, int h) {
    configureSurface();
}

} // anonymous namespace

namespace skwindow {

std::unique_ptr<WindowContext> MakeGraphiteDawnVulkanForOhos(OHNativeWindow* window,
                                                             std::unique_ptr<const DisplayParams> params,
                                                             uint32_t width,
                                                             uint32_t height) {
    LOGI("Window::MakeGraphiteDawnVulkanForOhos");
    std::unique_ptr<WindowContext> ctx(
            new GraphiteDawnVulkanWindowContext_ohos(window, std::move(params), height, width));
    if (!ctx->isValid()) {
        return nullptr;
    }

    return ctx;
}

}