#include "mygfx.h"

bool CMyGfx::Bm2My(char* bmpFileName, char* myFileName)
{
  void* bm = Load(bmpFileName);
  if(!bm) return false;

  _BITMAPFILEHEADER* pbf = (_BITMAPFILEHEADER*)bm;
  BITMAPINFOHEADER* pbi = (BITMAPINFOHEADER*)pbf;
  pbi = (BITMAPINFOHEADER*)(14 + (BYTE*)pbi);




  DWORD nPixls = pbi->biWidth * pbi->biHeight;
  // convert to 565
  WORD* bitmap16 = new WORD[nPixls];


  BYTE* src = (BYTE*)bm;
  src  +=  pbf->bfOffBits;
  DWORD i;

  RGB24_T* rgb;
  RGB32_T* rgb32;
  WORD* p16;
  int x,y;
  enum {UP=0,DOWN}dir;
  if(pbi->biHeight > 0) dir = UP;
  else dir = DOWN;
  switch(pbi->biBitCount)
  {
    case 24:
    /*
    rgb = (RGB24_T*)src;
    p16 = bitmap16;

    for(i = 0; i < nPixls; i++)
    {
      RGB24to16(rgb++, p16++);
    }
    */
    rgb = (RGB24_T*)src;
    p16 = bitmap16;

    for(y = pbi->biHeight-1; y >= 0; y--)
    {
      for (x = 0; x < pbi->biWidth; x++)
      {
        RGB24to16(rgb+pbi->biWidth*y+x, p16++);
      }
    }
    break;

    case 32:
    rgb32 = (RGB32_T*)src;
    p16 = bitmap16;
    /*
    for(i = 0; i < nPixls; i++)
    {
      RGB32to16(rgb32++, p16++);
    }
    */
    for(y = pbi->biHeight-1; y >= 0; y--)
    {
      for (x = 0; x < pbi->biWidth; x++)
      {
        RGB32to16(rgb32+pbi->biWidth*y+x, p16++);
      }
    }
    break;
  }
  WORD clut[256];
  WORD clutUsedSize = BuildCLUT(bitmap16, nPixls, clut);

  BYTE* stream = new BYTE[nPixls];

  DWORD streamsize = GenerateStream(stream,
                 bitmap16,
                 nPixls,
                 clut,
                 clutUsedSize);

  MYHEAD_T myhead;
  strcpy(myhead.id,"MYFM");
  myhead.imgWidth = pbi->biWidth;
  myhead.imgHidth = pbi->biHeight;
  myhead.CLUTOffset = 20;
  myhead.CLUT_size = clutUsedSize;
  myhead.StreamOffset = myhead.CLUTOffset + myhead.CLUT_size * 2;
  myhead.StreamSize = streamsize;
  
  BYTE* mybuf = new BYTE[myhead.StreamOffset + streamsize];
  memcpy(mybuf,(BYTE*)&myhead, sizeof(myhead)); //Write header
  memcpy(mybuf + myhead.CLUTOffset,(BYTE*)clut,myhead.CLUT_size*2);//Write clut
  memcpy(mybuf + myhead.StreamOffset, stream, streamsize);//Write stream

  Save(myFileName,mybuf,myhead.StreamOffset + streamsize);
  delete mybuf;
  delete bitmap16;
  free(bm);
  return true;
}
//------------------------------------------------------------------------------

WORD CMyGfx::CountColors(WORD* bitmap16, DWORD pixelCount)
{
  WORD indx = 0;
  WORD CLUT[256];
  WORD tmp;
  WORD* p16 = bitmap16;
  bool exists;
  for(DWORD i = 0; i < pixelCount; i++)
  {
    tmp = *(p16++);
    exists = false;
    for(DWORD j = 0; j < indx; j++)
    {
      if(CLUT[j] == tmp) exists = true;
    }
    if(!exists)
    {
      CLUT[indx] = tmp;
      indx++;
    }
    if(indx > 255) break;
  }
  return indx;
}
//------------------------------------------------------------------------------
void CMyGfx::RGB24to16(RGB24_T* src, WORD* dest)
{
  *dest = (src->R >> 3);
  *dest <<= 6;
  *dest |= (src->G >> 2);
  *dest <<= 5;
  *dest |= (src->B >> 3);
}
//------------------------------------------------------------------------------
void CMyGfx::RGB32to16(RGB32_T* src, WORD* dest)
{
  *dest = (src->R >> 3);
  *dest <<= 6;
  *dest |= (src->G >> 2);
  *dest <<= 5;
  *dest |= (src->B >> 3);
}

//------------------------------------------------------------------------------
void RGB16to32(WORD* src, DWORD* dest)
{
  WORD c16 = *src;
  DWORD c32;
  c32 = (c16 & 0x1F) << 3;
  c32 <<= 8;
  c16 >>= 5;
  c32 |= (c16 & 0x3F) << 2;
  c32 <<= 8;
  c16 >>= 6;
  c32 |= (c16 & 0x1F) << 3;
  *dest = c32;
}
//------------------------------------------------------------------------------
TColor RGB16toTColor(WORD* src)
{
  WORD c16 = *src;
  DWORD c;

  typedef struct
  {
    BYTE B;
    BYTE G;
    BYTE R;
    BYTE res;
  }rgb32;

   rgb32* pc = (rgb32*)&c;


    pc->R = (c16 & 0x1F) << 3;
    c16 >>= 5;
    pc->G = (c16 & 0x3F) << 2;
    c16 >>= 6;
    pc->B = (c16 & 0x1F) << 3;
    return c;
}


//------------------------------------------------------------------------------
void* Load(char* filepath)
{
  DWORD tmp;
  void* membuf;
  HANDLE hFile = CreateFile(filepath,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

  if(hFile != INVALID_HANDLE_VALUE)
  {
    DWORD fsize = GetFileSize(hFile, &tmp);
    membuf = malloc(fsize);
    ReadFile(hFile, membuf, fsize, &tmp, NULL);
    CloseHandle(hFile);
    return membuf;
  }
  else
  {
    DWORD e = GetLastError();
    return NULL;
  }
}

//------------------------------------------------------------------------------

DWORD Save(char* filepath, BYTE* pMemory, DWORD size)
{
  DWORD tmp;
  HANDLE hFile = CreateFile(filepath,
                            GENERIC_WRITE,
                            0,             // No share
                            NULL,          // SecurityAttributes
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);         // No Template

  if(hFile != INVALID_HANDLE_VALUE)
  {
    WriteFile(hFile, pMemory, size, &tmp, NULL);
    CloseHandle(hFile);
    return tmp;
  }
  else
    return 0;
}

//------------------------------------------------------------------------------
WORD CMyGfx::BuildCLUT(WORD* bitmap16, DWORD pixelCount, WORD* CLUT)
{
  WORD indx = 0;
  WORD tmp;
  WORD* p16 = bitmap16;
  bool exists;

  for(DWORD i = 0; i < pixelCount; i++)
  {
    tmp = *(p16++);
    exists = false;
    // Check color already exist in table
    for(DWORD j = 0; j < indx; j++)
    {
      if(CLUT[j] == tmp) exists = true;
    }
    if(!exists)
    {
      CLUT[indx] = tmp;
      indx++;
    }
    if(indx > 255)
    {
      MessageBox(NULL, "Palette more than 255 colors","Warning",MB_OK);
      break;
    }
  }
  return indx;//CLUT used size;
}

//------------------------------------------------------------------------------
WORD CMyGfx::GetColorIndex(WORD* color, WORD* CLUT, BYTE clutSize)
{
  WORD indx = 0xFFFF;
  for(BYTE i = 0; i < clutSize; i++)
  {
    if(*color == CLUT[i])
    {
      indx = i;
      break;
    }
  }
  return indx;
}
//------------------------------------------------------------------------------
DWORD CMyGfx::GenerateStream(BYTE* stream,
                             WORD* bm16,
                             DWORD bmPixelCount,
                             WORD* clut,
                             BYTE clutSize)
{
  DWORD cnt = 0;
  WORD repeatCnt = 0;
  WORD colindx, colindxprev = 0xFFFF;
  for(DWORD i = 0; i < bmPixelCount; i++)
  {
     colindx = GetColorIndex((bm16 + i), clut, (BYTE)clutSize);
     if(colindx != colindxprev)
     {
       if(repeatCnt != 0)
       {
         if(repeatCnt < 256)
         {
           stream[cnt - 1] = 0xFF;
           stream[cnt++] = (BYTE)repeatCnt;
         }
         else
         {
           stream[cnt - 1] = 0xFE;
           stream[cnt++] = (BYTE)(repeatCnt & 0xFF);
           stream[cnt++] = (BYTE)(repeatCnt >> 8);
         }
         repeatCnt = 0;
       }
       stream[cnt++] = colindx;
       colindxprev = colindx;
     }
     else if(repeatCnt == 0)
     {
       stream[cnt++] = 0xFF;
       repeatCnt++;
     }
     else
     {
       repeatCnt++;
     }

     if(repeatCnt == 65535)//Max repetitions reached
     {
       stream[cnt - 1] = 0xFE;
       stream[cnt++] = 0xFF;
       stream[cnt++] = 0xFF;
       repeatCnt = 0;
       colindxprev = 0xFFFF;
     }
  }
  if(repeatCnt != 0)
  {
    stream[cnt++] = (BYTE)(repeatCnt & 0xFF);
    if(repeatCnt > 255)
    {
      stream[cnt - 2] = 0xFE;
      stream[cnt++] = (BYTE)(repeatCnt >> 8);
    }
  }

  return cnt;
}

//------------------------------------------------------------------------------
void CMyGfx::LoadMYF2Bitmap(char* filename, Graphics::TBitmap* bm)
{
  void* myf = Load(filename);
  if(myf == NULL) return;

  MYHEAD_T* head = (MYHEAD_T*)myf;
  DWORD* fid = (DWORD*)myf;
  if(*fid != 0x4D46594D)
  {
    free(myf);
    return;
  }
  WORD* clut = (WORD*)((BYTE*)myf + head->CLUTOffset);
  DWORD* img = new DWORD[head->imgWidth * head->imgHidth];
  DWORD imglen = head->imgWidth * head->imgHidth;
  DWORD imgindx = 0;
  BYTE* stream = (BYTE*)myf + head->StreamOffset;
  BYTE indexedPixl;
  WORD RGB16;
  WORD repeatCnt = 0;

  TColor c;

  for(DWORD i = 0; i < head->StreamSize; i++)
  {
    if((indexedPixl = stream[i]) < 0xFE)
    {
      RGB16 = clut[indexedPixl];
      RGB16to32(&RGB16, img+imgindx);
      imgindx++;
    }
    else
    {
      repeatCnt = stream[++i];
      if(indexedPixl == 0xFE)
        repeatCnt |= (stream[++i] << 8);
      while(repeatCnt)
      {
        RGB16to32(&RGB16, img+imgindx);
        imgindx++;
        repeatCnt--;
      }
    }

  }

  bm->Width = head->imgWidth;
  bm->Height = head->imgHidth;
  WORD x,y;


  imgindx = 0;
  for(y = 0; y < bm->Height; y++)
  {
    for(x = 0; x < bm->Width; x++)
    {
      c = img[imgindx++];
      bm->Canvas->Pixels[x][y] = c;
    }
  }
  free(myf);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Get CLUT copy and size from file
BYTE CMyGfx::GetCLUT(char* filename, WORD* clut)
{
  void* myf = Load(filename);
  if(myf == NULL) return 0;
  MYHEAD_T* head = (MYHEAD_T*)myf;

  WORD* pclut = (WORD*)((BYTE*)myf + head->CLUTOffset);
  for (BYTE i = 0; i < (BYTE)head->CLUT_size; i++)
  {
    *clut++ = pclut[i];
  }
  return head->CLUT_size;
}
