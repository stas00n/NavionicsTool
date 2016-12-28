#include "lcdroutine.h"

LCD lcd;

void lcdroutine()
{

  LoadMYF(4,4,"C:\\Users\\User\\Desktop\\zaliv-maps\\64.bmp.myf");

}

void LoadMYF(int x, int y, char* filename)
{
  void* tp = Load(filename);
  if(tp == NULL) return;
  uint8_t* myf = (BYTE*)tp;
  MYHEAD_T* head = (MYHEAD_T*)myf;
  uint16_t* clut = (WORD*)(myf + head->CLUTOffset);
  uint8_t* stream = myf + head->StreamOffset;

  uint16_t w = head->imgWidth;
  uint16_t h = head->imgHidth;

  DRAWBOUNDS_T db;
  SetDrawBounds(&db, x,y,w,h);
  lcd.SetColumnAddress(db.sc, db.ec);
  lcd.SetPageAddress(db.sp, db.ep);

  PICTPART_T pp;
  if(y < 0) pp.first = -y * w;
  else pp.first = 0;
  if(x < 0) pp.first += -x;

  pp.next = pp.first;
  pp.draw = db.width;
  pp.skip = w - db.width;
  pp.last = pp.first + ((db.height - 1) * w + db.width - 1);
       // ¬ыше работает правильно вроде.
  //--------------------------------------------------------------
  uint8_t indexedPixl;
  uint32_t pixIndx = 0;
  uint16_t rgb16;
  //uint16_t repeatCnt = 0;

  uint8_t* streampos = stream;
  volatile WORD rep = 0;

  lcd.WriteMemoryStart(&rgb16, 0);//затычка

  while(1)
  {
    if(!rep) rep = Seek(&streampos, &pixIndx, pp.next, &indexedPixl);
    Draw(&streampos, &indexedPixl, rep, clut,&pixIndx, pp.draw, pp.skip);
    pp.next = pixIndx--;
    if(pixIndx >= pp.last) break;
  }


 /*
  for(DWORD i = 1; i < head->StreamSize; i++)
  {
    if((indexedPixl = stream[i]) < 0xFE)
    {
      rgb16 = clut[indexedPixl];
      if(pixIndx >= pp.first)
        lcd.WriteMemoryContinue(&rgb16, 1);
      pixIndx++;
    }
    else
    {
      repeatCnt = stream[++i];
      if(indexedPixl == 0xFE)
        repeatCnt |= (stream[++i] << 8);
      if(pixIndx >= pp.first)
        lcd.WriteMemoryRepeat(rgb16, repeatCnt);
      pixIndx += repeatCnt;
    }
  }
  */
  free(tp);
}

//------------------------------------------------------------------------------

void LoadMYF2(int x, int y, char* filename)
{
  LCD lcd;
  void* tp = Load(filename);
  if(tp == NULL) return;
  uint8_t* myf = (BYTE*)tp;
  MYHEAD_T* head = (MYHEAD_T*)myf;
  uint16_t* clut = (WORD*)(myf + head->CLUTOffset);
  uint8_t* stream = myf + head->StreamOffset;

  lcd.SetColumnAddress(x, x + head->imgWidth - 1);
  lcd.SetPageAddress(y, y+head->imgHidth - 1);


  uint8_t indexedPixl;
  uint32_t pixIndx = 0;
  uint16_t rgb16;
  uint16_t repeatCnt = 0;
  
  // Write first pixel
  indexedPixl = stream[0];
  rgb16 = clut[indexedPixl];
  lcd.WriteMemoryStart(&rgb16, 1);
  pixIndx++;


  for(DWORD i = 1; i < head->StreamSize; i++)
  {
    if((indexedPixl = stream[i]) < 0xFE)
    {
      rgb16 = clut[indexedPixl];
      lcd.WriteMemoryContinue(&rgb16, 1);
      pixIndx++;
    }
    else
    {
      repeatCnt = stream[++i];
      if(indexedPixl == 0xFE)
        repeatCnt |= (stream[++i] << 8);
      lcd.WriteMemoryRepeat(rgb16, repeatCnt);
      pixIndx += repeatCnt;
    }
  }
  free(tp);
}

void SetDrawBounds(DRAWBOUNDS_T* db, int x, int y, uint16_t w, uint16_t h)
{
  if(x < 0) db->sc = 0;
  else db->sc = x;
  if(x > 271) db->sc = 271;
  db->ec = x + w - 1;
  if(db->ec < db->sc) db->ec = db->sc;
  if(db->ec > 271) db->ec = 271;

  if(y < 0) db->sp = 0;
  else db->sp = y;
  if(y > 481) db->sp = 481;
  db->ep = y + h - 1;
  if(db->ep < db->sp) db->ep = db->sp;
  if(db->ep > 481) db->ep = 481;

  db->width = db->ec - db->sc + 1;
  db->height = db->ep - db->sp + 1;
}

WORD Seek(uint8_t** pStream, uint32_t* pixIndx, uint32_t pos, BYTE* found)
{
  uint8_t tmp;
  uint16_t repeatCnt;

  while (*pixIndx < pos)
  {
    tmp = *(*pStream);
    *pStream += 1;
    if(tmp < 0xFE)
    {
      *found = tmp;
      (*pixIndx) += 1;
    }
    else
    {
      repeatCnt = *(*pStream);
      *pStream += 1;
      if(tmp == 0xFE)
      {
        repeatCnt |= (*(*pStream) << 8);
        *pStream += 1;
      }
      (*pixIndx) += repeatCnt;
    }
  }

  if(*pixIndx > pos)
  {
    repeatCnt = (*pixIndx) - pos;
    *pixIndx = pos;
    return repeatCnt;
  }
  else return 0;
}

WORD Draw(uint8_t** pStream,
          uint8_t* first,
          int firstRepeats,
          uint16_t* clut,
          uint32_t* pixIndx,
          uint32_t drawCnt,
          uint32_t skipCnt)
{
  uint8_t tmp;
  uint16_t repeatCnt;
  uint32_t draw;
  while(firstRepeats > 0)
  {
    if (firstRepeats  >= drawCnt)
    {
      draw = drawCnt;
      lcd.WriteMemoryRepeat(clut[*first], draw);
      *pixIndx += draw;
      *pixIndx += skipCnt;
      firstRepeats -= draw;
      firstRepeats -= skipCnt;
    }
    else
    {
      draw = firstRepeats;
      lcd.WriteMemoryRepeat(clut[*first], draw);
      firstRepeats -= draw;
      *pixIndx += draw;
      drawCnt -= draw;
    }
  }

  while (drawCnt)
  {
    tmp = *(*pStream);
    *pStream += 1;
    if(tmp < 0xFE)
    {
      lcd.WriteMemoryContinue((clut + tmp), 1);
      (*pixIndx) += 1;
      drawCnt--;
    }
    else
    {
      repeatCnt = *(*pStream);
      *pStream += 1;
      if(tmp == 0xFE)
      {
        repeatCnt |= (*(*pStream) << 8);
        *pStream += 1;
      }
      if(repeatCnt <= drawCnt)
      {
        lcd.WriteMemoryRepeat((clut[tmp]), repeatCnt);
        drawCnt -= repeatCnt;
        (*pixIndx) += repeatCnt;
      }
      else
      {
        lcd.WriteMemoryRepeat((clut[tmp]), drawCnt);
        (*pixIndx) += drawCnt;
        drawCnt = 0;
      }
    }
  }
  *pixIndx += skipCnt;
  return 0;
}


