#include "include/core/SkTypes.h"
#include "include/ohos/SkImageOhos.h"

#include "include/ohos/OhosNativeBufferUtils.h"
#include "include/ohos/GrOhosNativeBufferUtils.h"
#include "include/core/SkAlphaType.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkImageGenerator.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkPixmap.h"
#include "include/core/SkRect.h"
#include "include/core/SkSize.h"
#include "include/core/SkSurface.h"
#include "include/gpu/GpuTypes.h"
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/GrContextThreadSafeProxy.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/GrRecordingContext.h"
#include "include/gpu/ganesh/GrTypes.h"
#include "include/gpu/ganesh/SkImageGanesh.h"
#include "include/private/base/SkAssert.h"
#include "include/private/gpu/ganesh/GrImageContext.h"
#include "include/private/gpu/ganesh/GrTypesPriv.h"
#include "src/core/SkAutoPixmapStorage.h"
#include "src/core/SkImageInfoPriv.h"
#include "src/gpu/RefCntedCallback.h"
#include "src/gpu/SkBackingFit.h"
#include "src/gpu/ganesh/GrAHardwareBufferImageGenerator.h"
#include "src/gpu/ganesh/GrBackendTextureImageGenerator.h"
#include "src/gpu/ganesh/GrBackendUtils.h"
#include "src/gpu/ganesh/GrCaps.h"
#include "src/gpu/ganesh/GrColorInfo.h"
#include "src/gpu/ganesh/GrColorSpaceXform.h"
#include "src/gpu/ganesh/GrContextThreadSafeProxyPriv.h"
#include "src/gpu/ganesh/GrDirectContextPriv.h"
#include "src/gpu/ganesh/GrDrawingManager.h"
#include "src/gpu/ganesh/GrFragmentProcessor.h"
#include "src/gpu/ganesh/GrGpu.h"
#include "src/gpu/ganesh/GrGpuResourcePriv.h"
#include "src/gpu/ganesh/GrImageContextPriv.h"
#include "src/gpu/ganesh/GrImageInfo.h"
#include "src/gpu/ganesh/GrProxyProvider.h"
#include "src/gpu/ganesh/GrRecordingContextPriv.h"
#include "src/gpu/ganesh/GrRenderTask.h"
#include "src/gpu/ganesh/GrSemaphore.h"
#include "src/gpu/ganesh/GrSurfaceProxy.h"
#include "src/gpu/ganesh/GrTexture.h"
#include "src/gpu/ganesh/GrTextureProxy.h"
#include "src/gpu/ganesh/SkGr.h"
#include "src/gpu/ganesh/SurfaceContext.h"
#include "src/gpu/ganesh/SurfaceFillContext.h"
#include "src/gpu/ganesh/effects/GrTextureEffect.h"
#include "src/gpu/ganesh/image/SkImage_Ganesh.h"
#include "src/image/SkImage_Base.h"

#include <algorithm>
#include <cstddef>
#include <utility>

#include <native_buffer/native_buffer.h>

namespace SkImages {

sk_sp<SkImage> DeferredFromOhosNativeBuffer(OH_NativeBuffer* graphicBuffer, SkAlphaType at) {
    auto gen = GrOhosNativeBufferImageGenerator::Make(graphicBuffer, at, nullptr,
                                                      kTopLeft_GrSurfaceOrigin);
    return DeferredFromGenerator(std::move(gen));
}

sk_sp<SkImage> DeferredFromOhosNativeBuffer(OH_NativeBuffer* graphicBuffer,
                                            SkAlphaType at,
                                            sk_sp<SkColorSpace> cs,
                                            GrSurfaceOrigin surfaceOrigin) {
    auto gen = GrOhosNativeImageGenerator::Make(graphicBuffer, at, cs, surfaceOrigin);
    return DeferredFromGenerator(std::move(gen));
}

sk_sp<SkImage> TextureFromAHardwareBufferWithData(GrDirectContext* dContext,
                                                  const SkPixmap& pixmap,
                                                  OH_NativeBuffer* hardwareBuffer,
                                                  GrSurfaceOrigin surfaceOrigin) {
    OH_NativeBuffer_Config bufferDesc;
    OH_NativeBuffer_GetConfig(hardwareBuffer, &bufferDesc);

    if (!SkToBool(bufferDesc.usage & NATIVEBUFFER_USAGE_HW_TEXTURE)) {
        return nullptr;
    }


    GrBackendFormat backendFormat = GrOhosNativeBufferUtils::GetBackendFormat(dContext,
                                                                              hardwareBuffer,
                                                                              bufferDesc.format,
                                                                              true);

    if (!backendFormat.isValid()) {
        return nullptr;
    }

    GrAHardwareBufferUtils::DeleteImageProc deleteImageProc = nullptr;
    GrAHardwareBufferUtils::UpdateImageProc updateImageProc = nullptr;
    GrAHardwareBufferUtils::TexImageCtx deleteImageCtx = nullptr;

    const bool isRenderable = SkToBool(bufferDesc.usage & NATIVEBUFFER_USAGE_HW_RENDER) ||
                              SkToBool(bufferDesc.usage & NATIVEBUFFER_USAGE_HW_TEXTURE);

    GrBackendTexture backendTexture =
            GrAHardwareBufferUtils::MakeBackendTexture(dContext, hardwareBuffer,
                                                       bufferDesc.width, bufferDesc.height,
                                                       &deleteImageProc, &updateImageProc,
                                                       &deleteImageCtx, false, backendFormat,
                                                       isRenderable);
    if (!backendTexture.isValid()) {
        return nullptr;
    }
    SkASSERT(deleteImageProc);

    auto releaseHelper = skgpu::RefCntedCallback::Make(deleteImageProc, deleteImageCtx);

    SkColorType colorType =
            AHardwareBufferUtils::GetSkColorTypeFromBufferFormat(bufferDesc.format);

    GrColorType grColorType = SkColorTypeToGrColorType(colorType);

    GrProxyProvider* proxyProvider = dContext->priv().proxyProvider();
    if (!proxyProvider) {
        return nullptr;
    }

    sk_sp<GrTextureProxy> proxy = proxyProvider->wrapBackendTexture(
            backendTexture, kBorrow_GrWrapOwnership, GrWrapCacheable::kNo, kRW_GrIOType,
            std::move(releaseHelper));
    if (!proxy) {
        return nullptr;
    }

    skgpu::Swizzle swizzle = dContext->priv().caps()->getReadSwizzle(backendFormat, grColorType);
    GrSurfaceProxyView framebufferView(std::move(proxy), surfaceOrigin, swizzle);
    SkColorInfo colorInfo = pixmap.info().colorInfo().makeColorType(colorType);
    sk_sp<SkImage> image = sk_make_sp<SkImage_Ganesh>(
            sk_ref_sp(dContext), kNeedNewImageUniqueID, framebufferView, std::move(colorInfo));
    if (!image) {
        return nullptr;
    }

    GrDrawingManager* drawingManager = dContext->priv().drawingManager();
    if (!drawingManager) {
        return nullptr;
    }

    skgpu::ganesh::SurfaceContext surfaceContext(
            dContext, std::move(framebufferView), image->imageInfo().colorInfo());

    surfaceContext.writePixels(dContext, pixmap, {0, 0});

    GrSurfaceProxy* p[1] = {surfaceContext.asSurfaceProxy()};
    drawingManager->flush(p, SkSurfaces::BackendSurfaceAccess::kNoAccess, {}, nullptr);

    return image;
}

}  // namespace SkImages
