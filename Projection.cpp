#include "Projection.h"
#include <math.h>

void CProjection::LonLat2TilePosFloat(TDoublePoint LonLat, TDoublePoint* TilePos)
{
//function TProjectionBasic256x256.LonLat2TilePosFloat(
//  const APoint: TDoublePoint
//): TDoublePoint;
//var
  TDoublePoint relative; //VRelative: TDoublePoint;
//begin
  LonLat2Relative(LonLat, &relative);
  Relative2TilePosFloat(relative ,TilePos);
//end;
}

void CProjection::Relative2TilePosFloat(TDoublePoint rel, TDoublePoint* TilePos)
 {
   TilePos->X = rel.X * fTiles;
   TilePos->Y = rel.Y * fTiles;
 }


void CProjection::LonLat2Relative(TDoublePoint LonLat, TDoublePoint* rel)
{
//var
  double z, c;//: Extended;
//begin
  const double pi = 3.1415926536;
  rel->X = 0.5 + LonLat.X / 360;
  z = sin(LonLat.Y * pi / 180);
  c = 1 / (2 * pi);
  rel->Y = 0.5 - 0.5 * log((1 + z) / (1 - z)) * c;

}

void CProjection::LoadPixels(BYTE zoom)
{
  fPixels = (1 << (zoom-1)) * 256.0;
  fTiles = (double)(1 << (zoom-1));
}

void CProjection::LatLon2Pixel(double lat, double lon, BYTE zoom, PIXELPOINT_T* pt)
{
  pt->tiles_total = (1 << (zoom-1));
  pt->pixls_total = (pt->tiles_total) << 8;

  double normX = lon / 360.0 + 0.5;
  double sinLat = sin(lat * 0.01745329252);
  double normY = 0.5 - 0.079577471545948 * log((1 + sinLat) / (1 - sinLat));

  pt->pixl_x = pt->pixls_total * normX;
  pt->pixl_y = pt->pixls_total * normY;

  pt->tile_x = pt->pixl_x >> 8;
  pt->tile_y = pt->pixl_y >> 8;

  pt->tile_pixl_x = pt->pixl_x - (pt->tile_x << 8);
  pt->tile_pixl_y = pt->pixl_y - (pt->tile_y << 8);
}

void CProjection::Pixel2LatLon(DWORD pixlX, DWORD pixlY, double* lat, double* lon)
{
  if(!_pixelPoint.pixls_total) return;
  double normX = ((double)pixlX) / _pixelPoint.pixls_total;
  double normY = ((double)pixlY) / _pixelPoint.pixls_total;

  *lon = 360 * (normX - 0.5);
  double exponent = 12.5663706143592 * (0.5 - normY);// 4 * pi * 0.5 - normY
  double A = exp(exponent); // A = e^(4 * pi * 0.5 - normY)

  *lat = 57.29577951308232 * asin((A - 1) / (A + 1));// lat = asind()
}
