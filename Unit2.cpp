//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
  bmsrc = new Graphics::TBitmap;
  bmsrc->Width = PaintBoxSrc->Width;
  bmsrc->Height = PaintBoxSrc->Height;

  bmdst = new Graphics::TBitmap;
  bmdst->Width = PaintBoxResult->Width;
  bmdst->Height = PaintBoxResult->Height;

  bmmsk = new Graphics::TBitmap;

  bmpal = new Graphics::TBitmap;
  bmpal->Width = PaintBoxPal->Width;
  bmpal->Height = PaintBoxPal->Height;
  DrawPalette(NULL,0,bmpal);
  maskPaths = new TStringList;
  wmContrast = 1;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormDestroy(TObject *Sender)
{
  delete bmsrc;
  delete bmdst;
  delete bmmsk;
  delete bmpal;
  delete maskPaths;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ButtonAddMaskClick(TObject *Sender)
{
  if(!OpenDialogMasks->Execute()) return;
  AnsiString s;
  // Clear Box
  ListBoxMasks->Clear();

  if(maskPaths->Count == 0)
    maskPaths->Add("None");
  // Save file list
  for (WORD i = 0; i < OpenDialogMasks->Files->Count; i++)
  {
    s = (OpenDialogMasks->Files->Strings[i]);
    maskPaths->Add(s);
  }
  // Read filenames to box
  for (WORD i = 0; i < maskPaths->Count; i++)
  {
    s = ExtractFileName(maskPaths->Strings[i]);
    ListBoxMasks->Items->Add(s);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ButtonRemMaskClick(TObject *Sender)
{
  AnsiString s;
  int selindx = ListBoxMasks->ItemIndex;
  if(selindx <= maskPaths->Count && selindx > 0)
  {
    maskPaths->Delete(selindx);
  }
  // Update Box
  ListBoxMasks->Clear();
  for (WORD i = 0; i < maskPaths->Count; i++)
  {
    s = ExtractFileName(maskPaths->Strings[i]);
    ListBoxMasks->Items->Add(s);
  }
  // Select item
  while(selindx >= ListBoxMasks->Items->Count) {selindx--;}
  ListBoxMasks->ItemIndex = selindx;
  // Reload with new selected item
  ListBoxMasksClick(NULL);
}

//---------------------------------------------------------------------------

void __fastcall TForm2::ListBoxMasksClick(TObject *Sender)
{
  int selind = ListBoxMasks->ItemIndex;
  AnsiString s, s2;
  s = maskPaths->Strings[selind];
  if(s != "None")
  {
    bmmsk->LoadFromFile(s);
    if(CheckBox1->Checked)AdjWMContrast(bmmsk, wmContrast);
  }
  else
  {
    TRect r;
    r.Left = 0; r.Right = 256;
    r.Top = 0; r.Bottom = 256;
    bmmsk->Width = 256;
    bmmsk->Height = 256;
    bmmsk->Canvas->Brush->Color = (TColor)0;
    bmmsk->Canvas->FillRect(r);
  }

  bmdst->Width = bmsrc->Width;
  bmdst->Height = bmsrc->Height;
  MergeWithMask(bmsrc, bmdst, bmmsk);
  bmdst->SaveToFile(workdir+"\\tmp.bmp");

  CMyGfx g;
  s = workdir+"\\tmp.bmp";
  s2= workdir+"\\tmp.myf";
  g.Bm2My(s.c_str(),s2.c_str());
  g.LoadMYF2Bitmap(s2.c_str(),bmdst);
  DWORD size = GetFileSize(s2.c_str());
  WORD clut[256];
  BYTE CLUTsize = g.GetCLUT(s2.c_str(), clut);
  DrawPalette(clut, CLUTsize,bmpal);
  Memo1->Clear();
  Memo1->Lines->Add(IntToStr(CLUTsize));
  Memo1->Lines->Add(IntToStr(size));
  PaintBoxResult->Canvas->Draw(0,0,bmdst);
  PaintBoxPal->Canvas->Draw(0,0,bmpal);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ButtonLdTileClick(TObject *Sender)
{
  if(!OpenDialogTile->Execute()) return;
  AnsiString s = OpenDialogTile->FileName;
  workdir = ExtractFileDir(s);
  bmsrc->LoadFromFile(s);
  PaintBoxSrc->Canvas->Draw(0,0,bmsrc);
}
//---------------------------------------------------------------------------
void MergeWithMask(Graphics::TBitmap* src, Graphics::TBitmap* dest, Graphics::TBitmap* mask)
{
  typedef struct
  {
    BYTE B;
    BYTE G;
    BYTE R;
    BYTE res;
  }rgb32;

  rgb32* rgb_s;
  rgb32* rgb_d;
  rgb32* rgb_m;
  TColor cs,cd,cm;
  rgb_s = (rgb32*)&cs;
  rgb_d = (rgb32*)&cd;
  rgb_m = (rgb32*)&cm;

  int x, y;
  for(y = 0; y < mask->Height; y++)
  {
    for(x = 0; x < mask->Width; x++)
    {
      cs = src->Canvas->Pixels[x][y];
      cm = mask->Canvas->Pixels[x][y];
      rgb_d->R = rgb_s->R  - rgb_m->R;
      rgb_d->G = rgb_s->G  - rgb_m->G;
      rgb_d->B = rgb_s->B  - rgb_m->B;
      dest->Canvas->Pixels[x][y] = cd;
    }
  }
}

void __fastcall TForm2::PaintBoxResultPaint(TObject *Sender)
{
  PaintBoxResult->Canvas->Draw(0,0,bmdst);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::PaintBoxSrcPaint(TObject *Sender)
{
  PaintBoxSrc->Canvas->Draw(0,0,bmsrc);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::PaintBoxPalPaint(TObject *Sender)
{
    PaintBoxPal->Canvas->Draw(0,0,bmpal);
}
//---------------------------------------------------------------------------
void DrawPalette(WORD* clut, WORD usedCnt, Graphics::TBitmap* bm)
{

  BYTE nx = 8, ny = 32;
  int x,y;
  TRect r;
  r.Left = 0;
  r.Right = bm->Width - 1;
  r.Top = 0;
  r.Bottom = bm->Height - 1;
  bm->Canvas->Brush->Color = (TColor)(RGB(200,200,200));
  bm->Canvas->FillRect(r);
  bm->Canvas->Brush->Color = (TColor)(RGB(150,150,150));

  // Draw vlines
  for (BYTE i = 0; i <= nx; i++)
  {
    x = i * (bm->Width-1) / nx;
    y = 0;
    bm->Canvas->MoveTo(x,y);
    y = bm->Height;
    bm->Canvas->LineTo(x,y);
  }
  // Draw Hlines
  for (BYTE i = 0; i <= ny; i++)
  {
    y = i * (bm->Height-1) / ny;
    x = 0;
    bm->Canvas->MoveTo(x,y);
    x = bm->Width;
    bm->Canvas->LineTo(x,y);
  }
  // Exit on NULL clut pointer
  if(clut == NULL) return;
  // Trim color count
  if(usedCnt > 254) usedCnt = 254;


  int w = (bm->Width - 1) / nx;
  int h = (bm->Height - 1) / ny;
  TColor c;
  WORD c16;
    typedef struct
  {

    BYTE R;
    BYTE G;
    BYTE B;
    BYTE res;
  }rgb32;

  rgb32* pc = (rgb32*)&c;
  BYTE posx, posy;
  for(int i = 0; i < usedCnt; i++)
  {
    posx = i % nx;
    posy = i / nx;
    r.Left = 1 + posx * w;
    r.Right = r.Left + w - 1;
    r.Top = 1 + posy * h;
    r.Bottom = r.top + h - 1;

    c16 = clut[i];
    pc->B = (c16 & 0x1F) << 3;
    c16 >>= 5;
    pc->G = (c16 & 0x3F) << 2;
    c16 >>= 6;
    pc->R = (c16 & 0x1F) << 3;
    bm->Canvas->Brush->Color = c;
    bm->Canvas->FillRect(r);
  }
}


void __fastcall TForm2::PaintBoxPalMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
BYTE r,g,b;
  DWORD c = (DWORD)bmpal->Canvas->Pixels[X][Y];
  r = c & 0xFF;
  c >>= 8;
  g = c & 0xFF;
  c >>= 8;
  b = c & 0xFF;
  LabelR->Caption = "R: " + IntToStr(r);
  LabelG->Caption = "G: " + IntToStr(g);
  LabelB->Caption = "B: " + IntToStr(b);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::UpDownContrClick(TObject *Sender,
      TUDBtnType Button)
{
  if(Button == btNext) wmContrast *= 1.05;
  if(Button == btPrev) wmContrast /= 1.05;
  EditContrast->Text = FloatToStr(wmContrast);
  //ListBoxMasksClick(NULL);
  //AdjWMContrast(bmmsk, wmContrast);
  //PaintBoxResult->Canvas->Draw(0,0,bmmsk);

}
//---------------------------------------------------------------------------
void AdjWMContrast(Graphics::TBitmap* bm, float contrast)
{
  int x, y;
  float fr,fg,fb;

  TColor c;
  typedef struct
  {
    BYTE R;
    BYTE G;
    BYTE B;
    BYTE res;
  }rgb32;

  rgb32* pc = (rgb32*)&c;

  for(y = 0; y < bm->Height; y++)
  {
    for(x = 0; x < bm->Width; x++)
    {
      c = bm->Canvas->Pixels[x][y];
      fr =  ((256 - pc->R) & 0xFF) * contrast;
      fg =  ((256 - pc->G) & 0xFF) * contrast;
      fb =  ((256 - pc->B) & 0xFF) * contrast;
      pc->R = 256 - (BYTE)(fr+0.5);
      pc->G = 256 - (BYTE)(fg+0.5);
      pc->B = 256 - (BYTE)(fb+0.5);
      bm->Canvas->Pixels[x][y] = c;
    }
  }
}
void __fastcall TForm2::PaintBoxResultMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  BYTE r,g,b;
  DWORD c = (DWORD)bmdst->Canvas->Pixels[X][Y];
  r = c & 0xFF;
  c >>= 8;
  g = c & 0xFF;
  c >>= 8;
  b = c & 0xFF;
  LabelR->Caption = "R: " + IntToStr(r);
  LabelG->Caption = "G: " + IntToStr(g);
  LabelB->Caption = "B: " + IntToStr(b);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::PaintBoxSrcMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
BYTE r,g,b;
  DWORD c = (DWORD)bmsrc->Canvas->Pixels[X][Y];
  r = c & 0xFF;
  c >>= 8;
  g = c & 0xFF;
  c >>= 8;
  b = c & 0xFF;
  LabelR->Caption = "R: " + IntToStr(r);
  LabelG->Caption = "G: " + IntToStr(g);
  LabelB->Caption = "B: " + IntToStr(b);
}
//---------------------------------------------------------------------------

