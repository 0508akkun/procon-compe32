#include "Solver.h"

Solver::Solver(Image image, int32 WH, int32 horDivNum, int32 verDivNum)
    :pieceWH(WH) {
    Array<Color> edgePixel;
    for (int32 y = 0; y < image.height(); y++) {
        if (y % pieceWH == 0 || y % pieceWH == pieceWH - 1) {   //Œ©‚Ä‚¢‚éêŠ‚ª’[‚É‚ ‚éŽž‚Í
            for (int32 x = 0; x < image.width(); x++) {
                edgePixel << image[y][x];
                if (x % pieceWH == pieceWH - 1) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
    }
    for (int32 x = 0; x < image.height(); x++) {
        if (x % pieceWH == 0 || x % pieceWH == pieceWH - 1) {   //Œ©‚Ä‚¢‚éêŠ‚ª’[‚É‚ ‚éŽž‚Í
            for (int32 y = 0; y < image.width(); y++) {
                edgePixel << image[y][x];
                if (y % pieceWH == pieceWH - 1) {
                    edgePixelData << edgePixel;
                    edgePixel.clear();
                }
            }
        }
    }
    Array<int> edgeIndex;
    for (int y = 0; y < verDivNum; y++) {
        for (int x = 0; x < horDivNum; x++) {
            edgeIndex << y * horDivNum + x + horDivNum * y;
            edgeIndex << y * horDivNum + x + horDivNum + horDivNum * y;
            edgeIndex << y * horDivNum + x + horDivNum * verDivNum * 2 + (verDivNum * 2 - 1) * x - y * verDivNum;
            edgeIndex << y * horDivNum + x + horDivNum * verDivNum * 2 + (verDivNum * 2 - 1) * x - y * verDivNum + verDivNum;
        }
        indexList << edgeIndex;
        edgeIndex.clear();
    }
}


std::pair<int32, int32> Solver::calcEdgeDiff(int32 index)
{
    int32 pixelDiff = 0;
    int32 minPixelDiff = 10000000000000000000;
    int32 minIndex = 0;
    Array<Color> rEdgePixelData;
    Array<Array<int32>> remIndexList = indexList.removed_if([index](Array<int32> inList) {
        for (int32 i = 0; i < inList.size(); i++) {
            if (index == inList[i]) {
                return true;
            }
        }
        });
    for (int32 i = 0; i < remIndexList.size(); i++) {
        for (int32 j = 0; j < remIndexList[i].size(); j++) {
            for (int32 k = 0; k < pieceWH; k++) {
                pixelDiff = Abs(edgePixelData[remIndexList[i][j]][k].r - edgePixelData[index][k].r) + Abs(edgePixelData[remIndexList[i][j]][k].g - edgePixelData[index][k].g) + Abs(edgePixelData[remIndexList[i][j]][k].b - edgePixelData[index][k].b);
                if (minPixelDiff > pixelDiff) {
                    minPixelDiff = pixelDiff;
                    minIndex = remIndexList[i][j];
                }
                rEdgePixelData = edgePixelData[remIndexList[i][j]].reversed();
                pixelDiff = Abs(rEdgePixelData[k].r - edgePixelData[index][k].r) + Abs(rEdgePixelData[k].g - edgePixelData[index][k].g) + Abs(rEdgePixelData[k].b - edgePixelData[index][k].b);
                if (minPixelDiff > pixelDiff) {
                    minPixelDiff = pixelDiff;
                    minIndex = remIndexList[i][j];
                }
            }
        }
    }
    return std::make_pair(minPixelDiff, minIndex);
}
