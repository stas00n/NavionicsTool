#ifndef _MYGFX_H_
#define _MYGFX_H_

#include <windows.h>
#include <stdio.h>
#include <Graphics.hpp>
// Main gfx header
typedef struct
{
  BYTE id[4];
  WORD imgWidth;
  WORD imgHidth;
  WORD CLUTOffset;
  WORD CLUT_size;
  WORD reserved;
  WORD StreamOffset;
  DWORD StreamSize;
}MYHEAD_T;

#pragma pack(2)
typedef struct {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} _BITMAPFILEHEADER;

typedef struct
{
 BYTE B;
 BYTE G;
 BYTE R;
}RGB24_T;

typedef struct
{
 BYTE B;
 BYTE G;
 BYTE R;
 BYTE A;
}RGB32_T;

class CMyGfx
{
public:
  bool Bm2My(char* bmpFileName, char* myFileName);
  void RGB24to16(RGB24_T* src, WORD* dest);
  //void RGB16to32(WORD* src, DWORD* dest);

  void RGB32to16(RGB32_T* src, WORD* dest);
  void LoadMYF2Bitmap(char* filename, Graphics::TBitmap* bm);

  BYTE GetCLUT(char* filename, WORD* clut);

  WORD CountColors(WORD* bitmap16, DWORD pixelCount);
  WORD BuildCLUT(WORD* bitmap16, DWORD pixelCount, WORD* CLUT);
  DWORD GenerateStream(BYTE* stream, WORD* bm16,DWORD bmPixelCount,WORD* clut,BYTE clutSize);
 // void* Load(char* filepath);
  //DWORD Save(char* filepath, BYTE* pMemory, DWORD size);
// Public vars
public:
  MYHEAD_T* pHead;

  WORD GetColorIndex(WORD* color, WORD* CLUT, BYTE clutSize);

};

  void* Load(char* filepath);
  DWORD Save(char* filepath, BYTE* pMemory, DWORD size);
  void RGB16to32(WORD* src, DWORD* dest);
  TColor RGB16toTColor(WORD* src);
#endif /* _MYGFX_H_ */
 