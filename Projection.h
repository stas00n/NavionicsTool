#ifndef _PROJECTION_H_
#define _PROJECTION_H_

#include <windows.h>

typedef struct
{
  double X;
  double Y;
}TDoublePoint;

typedef struct
{
  DWORD pixls_total;
  DWORD tiles_total;
  DWORD pixl_x;
  DWORD pixl_y;
  DWORD tile_x;
  DWORD tile_y;
  BYTE  tile_pixl_x;
  BYTE  tile_pixl_y;
}PIXELPOINT_T;

class CProjection
{
  public:

    //FHash: THashValue;
    BYTE _zoom;       //FZoom: Byte;
    //FProjectionType: IProjectionType;

    int iTiles;      //FTiles: Integer;
    int iPixels;     //FPixels: Integer;
    double fTiles;   //FTilesFloat: Double;
    double fPixels;  //FPixelsFloat: Double;


    //FTileSplitCode: Integer;
public:
  void LonLat2TilePosFloat(TDoublePoint LonLat, TDoublePoint* TilePos);
  void Relative2TilePosFloat(TDoublePoint rel, TDoublePoint* TilePos);
  void LonLat2Relative(TDoublePoint LonLat, TDoublePoint* rel);
  void LoadPixels(BYTE zoom);





public:
  PIXELPOINT_T _pixelPoint;
void LatLon2Pixel(double lat, double lon, BYTE zoom, PIXELPOINT_T* pt);
void Pixel2LatLon(DWORD pixlX, DWORD pixlY, double* lat, double* lon);
};





#endif /* _PROJECTION_H_ */
 