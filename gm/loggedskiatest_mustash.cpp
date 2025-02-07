//['SaveLayer', 'DrawRRect', 'DrawColor', 'Save', 'ClipRect', 'Scale', 'Restore', 'Concat', 'ClipPath', 'DrawRect', 'ClipRRect', 'Translate']

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "gm.h"

#include "include/core/SkCanvas.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkBlendMode.h"
#include "include/core/SkColor.h"
#include "include/core/SkRRect.h"
#include "include/core/SkPath.h"
#include "include/private/chromium/Slug.h"
#include "src/core/SkPaintPriv.h"
#include "src/core/SkReadBuffer.h"

#include "tools/ToolUtils.h"

#define inf 9999999

// Deserialize hex string to raw data
void hexToRawData(const std::string& hexString, void* output, size_t size) {
    unsigned char* byteOutput = static_cast<unsigned char*>(output);
    for (size_t i = 0; i < size; ++i) {
        std::istringstream iss(hexString.substr(i * 2, 2));
        int byte;
        iss >> std::hex >> byte;
        byteOutput[i] = static_cast<unsigned char>(byte);
    }
}



// Begin loading ops and loading data when needed
// Failed to find matching Op: Scale
// Failed to find matching Op: ClipPath

                DEF_SIMPLE_GM(loggedskiatest1, canvas, 1280, 640) {
                
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//ClipRectOp
std::cout << "init op type: RestoreOp\n";
//ClipRectOp
std::cout << "init op type: RestoreOp\n";
//DrawColorOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//ClipRectOp
std::cout << "init op type: RestoreOp\n";
//Scale
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//DrawRectOp
        void *paintdata_rect0 = malloc(28);
        hexToRawData("00000000000080400000803f0000803f0000803f0000803f01010000", paintdata_rect0, 28);
        SkReadBuffer paintBuffer_rect0(paintdata_rect0, 28);
        SkPaint paintRect0 = SkPaintPriv::Unflatten(paintBuffer_rect0);
    
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//TranslateOp
std::cout << "init op type: RestoreOp\n";
//SaveLayerOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//ClipRRectOp
std::cout << "init op type: RestoreOp\n";
//DrawRRectOp
        void *paintdata_rrect0 = malloc(28);
        hexToRawData("00000000000080400000000000000000000000000000803f01030000", paintdata_rrect0, 28);
        SkReadBuffer paintBuffer_rrect0(paintdata_rrect0, 28);
        SkPaint paintRRect0 = SkPaintPriv::Unflatten(paintBuffer_rrect0);
    
std::cout << "init op type: RestoreOp\n";
//DrawRRectOp
        void *paintdata_rrect1 = malloc(28);
        hexToRawData("00000000000080400000000000000000000000000000803f01030000", paintdata_rrect1, 28);
        SkReadBuffer paintBuffer_rrect1(paintdata_rrect1, 28);
        SkPaint paintRRect1 = SkPaintPriv::Unflatten(paintBuffer_rrect1);
    
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//ConcatOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//ClipRRectOp
std::cout << "init op type: RestoreOp\n";
//ClipRRectOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//ClipPath
std::cout << "init op type: RestoreOp\n";
//DrawRectOp
        void *paintdata_rect1 = malloc(28);
        hexToRawData("00000000000080400000000000000000000000000000803f01030000", paintdata_rect1, 28);
        SkReadBuffer paintBuffer_rect1(paintdata_rect1, 28);
        SkPaint paintRect1 = SkPaintPriv::Unflatten(paintBuffer_rect1);
    
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
// Begin rendering ops to skia
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: ClipRectOp\n";
{
            SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 1262.000000, 640.000000);
            canvas->clipRect(rect, SkClipOp::kIntersect, false);
        }
std::cout << "render op type: ClipRectOp\n";
{
            SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 1260.000000, 640.000000);
            canvas->clipRect(rect, SkClipOp::kDifference, false);
        }
std::cout << "render op type: DrawColorOp\n";
{
            SkColor4f color = SkColor4f::FromBytes_RGBA(4294967295);
            canvas->drawColor(color, SkBlendMode::kSrc);
        }
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: ClipRectOp\n";
{
            SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 1280.000000, 640.000000);
            canvas->clipRect(rect, SkClipOp::kIntersect, false);
        }
std::cout << "render op type: Scale\n";
// Render not implemented for: Scale
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: DrawRectOp\n";
{
        SkRect rect= SkRect::MakeLTRB(0.000000, 0.000000, 3185.000000, 6338.000000);
        //SkPaint p;
        //p.setColor(SkColors::kYellow);
        canvas->drawRect(rect, paintRect0);
        }
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: TranslateOp\n";
canvas->translate(26.000000, 26.000000);
std::cout << "render op type: SaveLayerOp\n";
{
        SkRect bounds = SkRect::MakeLTRB(-202.032181, 0.000000, 1734.000000, 1928.252441);
        SkPaint paint;
        bool unset = bounds.left() == SK_ScalarInfinity;
        canvas->saveLayer(unset ? nullptr : &bounds, &paint);
        }
        
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: ClipRRectOp\n";
{
        SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 650.000000, 650.000000);
        SkVector radii[4] = { { 325.000000,325.000000 },
            { 325.000000, 325.000000 },
            { 325.000000, 325.000000 },
            { 325.000000, 325.000000 },
            };
        int type = 0;

        // private constructor, can't use: SkRRect rrect(rect, radii, type);
        SkRRect rrect;
        rrect.setRectRadii(rect, radii);
        canvas->clipRRect(rrect, SkClipOp::kDifference, true);
        }
std::cout << "render op type: DrawRRectOp\n";
{
        SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 650.000000, 650.000000);
        SkVector radii[4] = { { 325.000000,325.000000 },
            { 325.000000, 325.000000 },
            { 325.000000, 325.000000 },
            { 325.000000, 325.000000 },
            };
        int type = 0;

        // private constructor, can't use: SkRRect rrect(rect, radii, type);
        SkRRect rrect;
        rrect.setRectRadii(rect, radii);
        //SkPaint p;
        //p.setColor(SkColors::kGreen);
        canvas->drawRRect(rrect, paintRRect0);
        }
std::cout << "render op type: DrawRRectOp\n";
{
        SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 650.000000, 650.000000);
        SkVector radii[4] = { { 325.000000,325.000000 },
            { 325.000000, 325.000000 },
            { 325.000000, 325.000000 },
            { 325.000000, 325.000000 },
            };
        int type = 0;

        // private constructor, can't use: SkRRect rrect(rect, radii, type);
        SkRRect rrect;
        rrect.setRectRadii(rect, radii);
        //SkPaint p;
        //p.setColor(SkColors::kGreen);
        canvas->drawRRect(rrect, paintRRect1);
        }
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: ConcatOp\n";
{
            SkM44 mat = SkM44(0.766044,0.642788,0.000000,-202.032181,-0.642788,0.766044,0.000000,1097.860229,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000);
            canvas->concat(mat);
            }
        
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: ClipRRectOp\n";
{
        SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 758.000000, 1083.000000);
        SkVector radii[4] = { { 0.000000,0.000000 },
            { 0.000000, 0.000000 },
            { 0.000000, 0.000000 },
            { 758.000000, 1082.859375 },
            };
        int type = 0;

        // private constructor, can't use: SkRRect rrect(rect, radii, type);
        SkRRect rrect;
        rrect.setRectRadii(rect, radii);
        canvas->clipRRect(rrect, SkClipOp::kIntersect, true);
        }
std::cout << "render op type: ClipRRectOp\n";
{
        SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 758.000000, 433.000000);
        SkVector radii[4] = { { 0.000000,0.000000 },
            { 0.000000, 0.000000 },
            { 0.000000, 0.000000 },
            { 758.000000, 432.859375 },
            };
        int type = 0;

        // private constructor, can't use: SkRRect rrect(rect, radii, type);
        SkRRect rrect;
        rrect.setRectRadii(rect, radii);
        canvas->clipRRect(rrect, SkClipOp::kDifference, true);
        }
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: ClipPath\n";
// Render not implemented for: ClipPath
std::cout << "render op type: DrawRectOp\n";
{
        SkRect rect= SkRect::MakeLTRB(0.000000, 0.000000, 758.000000, 1083.000000);
        //SkPaint p;
        //p.setColor(SkColors::kYellow);
        canvas->drawRect(rect, paintRect1);
        }
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
}// End Paint Buffer: 41277EDFA217EF5C7DA400E4B403CF1B_1744813181993205.paintbuf
// Failed to find matching Op: Scale
// Failed to find matching Op: ClipPath

                DEF_SIMPLE_GM(loggedskiatest2, canvas, 1280, 640) {
                
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//ClipRectOp
std::cout << "init op type: RestoreOp\n";
//ClipRectOp
std::cout << "init op type: RestoreOp\n";
//DrawColorOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//TranslateOp
std::cout << "init op type: RestoreOp\n";
//ClipRectOp
std::cout << "init op type: RestoreOp\n";
//Scale
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//DrawRectOp
        void *paintdata_rect2 = malloc(28);
        hexToRawData("00000000000080400000803f0000803f0000803f0000803f01010000", paintdata_rect2, 28);
        SkReadBuffer paintBuffer_rect2(paintdata_rect2, 28);
        SkPaint paintRect2 = SkPaintPriv::Unflatten(paintBuffer_rect2);
    
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//TranslateOp
std::cout << "init op type: RestoreOp\n";
//SaveLayerOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//ConcatOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//ClipRRectOp
std::cout << "init op type: RestoreOp\n";
//ClipRRectOp
std::cout << "init op type: RestoreOp\n";
//SaveOp
std::cout << "init op type: RestoreOp\n";
//ClipPath
std::cout << "init op type: RestoreOp\n";
//DrawRectOp
        void *paintdata_rect3 = malloc(28);
        hexToRawData("00000000000080400000000000000000000000000000803f01030000", paintdata_rect3, 28);
        SkReadBuffer paintBuffer_rect3(paintdata_rect3, 28);
        SkPaint paintRect3 = SkPaintPriv::Unflatten(paintBuffer_rect3);
    
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
std::cout << "init op type: RestoreOp\n";
//RestoreOp
// Begin rendering ops to skia
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: ClipRectOp\n";
{
            SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 1262.000000, 640.000000);
            canvas->clipRect(rect, SkClipOp::kIntersect, false);
        }
std::cout << "render op type: ClipRectOp\n";
{
            SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 1260.000000, 640.000000);
            canvas->clipRect(rect, SkClipOp::kDifference, false);
        }
std::cout << "render op type: DrawColorOp\n";
{
            SkColor4f color = SkColor4f::FromBytes_RGBA(4294967295);
            canvas->drawColor(color, SkBlendMode::kSrc);
        }
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: TranslateOp\n";
canvas->translate(0.000000, -638.000000);
std::cout << "render op type: ClipRectOp\n";
{
            SkRect rect = SkRect::MakeLTRB(0.000000, 638.000000, 1280.000000, 1278.000000);
            canvas->clipRect(rect, SkClipOp::kIntersect, false);
        }
std::cout << "render op type: Scale\n";
// Render not implemented for: Scale
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: DrawRectOp\n";
{
        SkRect rect= SkRect::MakeLTRB(0.000000, 0.000000, 3185.000000, 6338.000000);
        //SkPaint p;
        //p.setColor(SkColors::kYellow);
        canvas->drawRect(rect, paintRect2);
        }
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: TranslateOp\n";
canvas->translate(26.000000, 26.000000);
std::cout << "render op type: SaveLayerOp\n";
{
        SkRect bounds = SkRect::MakeLTRB(-202.032181, 0.000000, 1734.000000, 1928.252441);
        SkPaint paint;
        bool unset = bounds.left() == SK_ScalarInfinity;
        canvas->saveLayer(unset ? nullptr : &bounds, &paint);
        }
        
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: ConcatOp\n";
{
            SkM44 mat = SkM44(0.766044,0.642788,0.000000,-202.032181,-0.642788,0.766044,0.000000,1097.860229,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000);
            canvas->concat(mat);
            }
        
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: ClipRRectOp\n";
{
        SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 758.000000, 1083.000000);
        SkVector radii[4] = { { 0.000000,0.000000 },
            { 0.000000, 0.000000 },
            { 0.000000, 0.000000 },
            { 758.000000, 1082.859375 },
            };
        int type = 0;

        // private constructor, can't use: SkRRect rrect(rect, radii, type);
        SkRRect rrect;
        rrect.setRectRadii(rect, radii);
        canvas->clipRRect(rrect, SkClipOp::kIntersect, true);
        }
std::cout << "render op type: ClipRRectOp\n";
{
        SkRect rect = SkRect::MakeLTRB(0.000000, 0.000000, 758.000000, 433.000000);
        SkVector radii[4] = { { 0.000000,0.000000 },
            { 0.000000, 0.000000 },
            { 0.000000, 0.000000 },
            { 758.000000, 432.859375 },
            };
        int type = 0;

        // private constructor, can't use: SkRRect rrect(rect, radii, type);
        SkRRect rrect;
        rrect.setRectRadii(rect, radii);
        canvas->clipRRect(rrect, SkClipOp::kDifference, true);
        }
std::cout << "render op type: SaveOp\n";
canvas->save();
std::cout << "render op type: ClipPath\n";
// Render not implemented for: ClipPath
std::cout << "render op type: DrawRectOp\n";
{
        SkRect rect= SkRect::MakeLTRB(0.000000, 0.000000, 758.000000, 1083.000000);
        //SkPaint p;
        //p.setColor(SkColors::kYellow);
        canvas->drawRect(rect, paintRect3);
        }
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
std::cout << "render op type: RestoreOp\n";
canvas->restore();
}// End Paint Buffer: F231E850337A7AC4DF0B827848EA84E1_1744813182051615.paintbuf
