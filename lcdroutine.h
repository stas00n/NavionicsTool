#ifndef _LCDR_
#define _LCDR_

#include "unit3.h"
#include "mygfx.h"
#include <stdint.h>

typedef struct
{
  DWORD first;
  DWORD next;
  DWORD last;
  WORD  draw;
  WORD  skip;
}PICTPART_T;

typedef struct
{
  uint16_t sc;
  uint16_t ec;
  uint16_t sp;
  uint16_t ep;
  uint16_t width;
  uint16_t height;
}DRAWBOUNDS_T;



void lcdroutine();

void LoadMYF(int x, int y, char* filename);
void LoadMYF2(int x, int y, char* filename);

void SetDrawBounds(DRAWBOUNDS_T* db, int x, int y, uint16_t w, uint16_t h);
WORD Seek(uint8_t** pStream, uint32_t* pixIndx, uint32_t pos, BYTE* found);
WORD Draw(uint8_t** pStream,
          uint8_t* first,
          int firstRepeats,
          uint16_t* clut,
          uint32_t* pixIndx,
          uint32_t drawCnt,
          uint32_t skipCnt);


#endif /* _LCDR_ */
 