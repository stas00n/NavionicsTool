//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "mygfx.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "lcdroutine.h"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
        : TForm(Owner)
{
  bm = new Graphics::TBitmap;
  bm->Width = PaintBoxLCD->Width;
  bm->Height = PaintBoxLCD->Height;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::PaintBoxLCDPaint(TObject *Sender)
{
  PaintBoxLCD->Canvas->Draw(0,0,bm);
}
//---------------------------------------------------------------------------

void __fastcall TForm3::FormDestroy(TObject *Sender)
{
  delete bm;
}
//---------------------------------------------------------------------------

void LCD::SetColumnAddress(WORD sc, WORD ec)
{
  if (sc <= COLUMN_MAX && sc >= COLUMN_MIN)
    _sc = sc;

  if (ec >= _sc && ec <= COLUMN_MAX && ec >= COLUMN_MIN)
    _ec = ec;

  r.Left = _sc;
  r.Right = _ec;
  SetMaxWrite();
}

void LCD::SetPageAddress(WORD sp, WORD ep)
{
  if (sp <= PAGE_MAX && sp >= PAGE_MIN)
    _sp = sp;

  if (ep >= _sp && ep <= PAGE_MAX && ep >= PAGE_MIN)
    _ep = ep;

  r.Top = _sp;
  r.Bottom = _ep;
  SetMaxWrite();
}

void LCD::WriteMemoryStart(WORD* pixels, DWORD n)
{
  writeCnt = 0;
  WriteMemoryContinue(pixels, n);
}

void LCD::WriteMemoryContinue(WORD* pixels, DWORD n)
{

  int x,y;
  TColor c;

  for(DWORD i = 0; i < n; i++)
  {
    if(writeCnt >= maxWrite)
       break;
    x = (writeCnt) % (_ec - _sc + 1) + _sc;
    y = (writeCnt) / (_ec - _sc + 1) + _sp;
    c = RGB16toTColor(pixels + i);
    Form3->PaintBoxLCD->Canvas->Pixels[x][y] = c;
    writeCnt++;
  }

}

void LCD::WriteMemoryRepeat(WORD pixel, DWORD n)
{

  int x,y;
  TColor c = RGB16toTColor(&pixel);

  for(DWORD i = 0; i < n; i++)
  {
    if(writeCnt >= maxWrite)
      break;
    x = (writeCnt) % (_ec - _sc + 1) + _sc;
    y = (writeCnt) / (_ec - _sc + 1) + _sp;
    Form3->PaintBoxLCD->Canvas->Pixels[x][y] = c;
    writeCnt++;
  }

}

void LCD::SetMaxWrite()
{
  maxWrite = (_ec - _sc + 1) *(_ep - _sp + 1);
}

void __fastcall TForm3::Button1Click(TObject *Sender)
{
  lcdroutine();
}
//---------------------------------------------------------------------------

