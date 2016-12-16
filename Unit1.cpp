//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include "mygfx.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
  bmOverlay = new Graphics::TBitmap;
  bmOverlay->Width = PaintBox1->Width;
  bmOverlay->Height = PaintBox1->Height;
}
//---------------------------------------------------------------------------
void TForm1::ChangeZoom(BYTE* newZoom)
{
  if(*newZoom > ZOOM_MAX) *newZoom = ZOOM_MAX;
  if(*newZoom < ZOOM_MIN) *newZoom = ZOOM_MIN;
  EditZoom->Text = IntToStr(*newZoom);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
  zoom = ZOOM_DEFAULT;
  ChangeZoom(&zoom);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button1Click(TObject *Sender)
{

  proj._zoom = zoom;//(BYTE)EditZoom->Text.ToInt();

  double lat =  EditLat->Text.ToDouble();
  double lon =  EditLon->Text.ToDouble();
  proj.LatLon2Pixel(lat, lon, proj._zoom, &(proj._pixelPoint));

  orgX = 256;
  orgY = 128;

  tileX = proj._pixelPoint.tile_x;
  tileY = proj._pixelPoint.tile_y;
  LabelTileX->Caption = IntToStr(tileX);
  LabelTileY->Caption = IntToStr(tileY);




  Form1->PaintBox1->Canvas->Brush->Color = (TColor)0xFF00;

  TRect r;
  r.left = 0; r.right =767;
  r.top = 0; r.Bottom = 511;

  //Form1->PaintBox1->Canvas->Rectangle(r) ;
  //Form1->PaintBox1->Canvas->FillRect(r);
  AnsiString filename;

  DWORD x = 0, y = -128;
  DWORD tx = tileX-1;
  DWORD ty = tileY-1;
  for(int i = 0; i<3; i++)
  {
    for(int j = 0; j<3; j++)
    {
      filename = GenerateTilePath(tx+j, ty+i, proj._zoom, &(Edit1->Text));
      if(!FileExists(filename))
        filename = (Edit1->Text) + "missed.bmp";
      LoadBMP(filename.c_str(), x, y);
      x += 256;
    }
    x = 0;
    y += 256;
  }

  r.left = orgX-5+proj._pixelPoint.tile_pixl_x; r.right = r.left+10;
  r.top = orgY-5+proj._pixelPoint.tile_pixl_y; r.Bottom = r.top +10;

  //Form1->PaintBox1->Canvas->Rectangle(r) ;
  Form1->PaintBox1->Canvas->Ellipse(r);
  //bmOverlay->Transparent = true;
  //bmOverlay->TransparentColor = (TColor)(0x00FFFFFF);//RGB(255,255,255);
  //bmOverlay->TransparentMode = tmFixed;
  //bmOverlay->Canvas->Ellipse(r);
  //Form1->PaintBox1->Canvas->Draw(0,0,bmOverlay);
  //Form1->PaintBox1->Canvas->MoveTo(50,50);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonPathClick(TObject *Sender)
{
  if(TForm1::OpenDialog1->Execute())
  Edit1->Text = OpenDialog1->FileName;

}
//---------------------------------------------------------------------------
AnsiString GenerateTilePath(DWORD x, DWORD y, BYTE zoom, AnsiString* tileRoot)
{
  AnsiString s;
  s = *tileRoot;
  s += "z"+IntToStr(zoom)+'\\'+ IntToStr(x/1024) + '\\' + "x";
  s += IntToStr(x) + '\\' + IntToStr(y/1024) + '\\';
  s += "y" + IntToStr(y) + ".bmp";
  return s;
}

void LoadBMP(char* filename, DWORD x, DWORD y)
{
  Graphics::TBitmap *bm = new Graphics::TBitmap;
  bm->LoadFromFile(filename);
  Form1->PaintBox1->Canvas->Draw(x,y,bm);
  delete bm;
}

void LoadMYF(char* filename, DWORD x, DWORD y)
{
  CMyGfx g;
  Graphics::TBitmap *bm = new Graphics::TBitmap;
  g.LoadMYF2Bitmap(filename, bm);
  Form1->PaintBox1->Canvas->Draw(x,y,bm);
  delete bm;
}


void __fastcall TForm1::PaintBox1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  // cursor position relative to base tile origin
  int relX = X - orgX;
  int relY = Y - orgY;

  // tile under cursor
  int dTileX = relX / 256;
  if(relX < 0)
    dTileX -= 1;
  int tileX = proj._pixelPoint.tile_x + dTileX;

  int dTileY = relY / 256;
  if(relY < 0)
    dTileY -= 1;
  int tileY = proj._pixelPoint.tile_y + dTileY;


  // cursor position relative to tile
  int tX = relX - dTileX * 256;
  int tY = relY - dTileY * 256;

  // Absolute pixels
  DWORD pixlX = tileX * 256 + tX;
  DWORD pixlY = tileY * 256 + tY;

  double lat, lon;
  proj.Pixel2LatLon(pixlX,pixlY,&lat,&lon);

  LabelLat->Caption =  Dbl2DegMins(lat);
  LabelLon->Caption =  Dbl2DegMins(lon);
}
//---------------------------------------------------------------------------


AnsiString Dbl2DegMins(double degrees)
{
  double tmp = degrees;
  int d, im, fm;
  d = (int)tmp;
  tmp -= d;
  tmp *= 60;
  im = (int)tmp;
  tmp -= im;
  fm = tmp * 1000;
  char str[100];
  sprintf(str, "%d° %02d,%03d'", d,im,fm);
  return (AnsiString)str;
}
//------------------------------------------------------------------------------
void __fastcall TForm1::ButtonConvAllClick(TObject *Sender)
{
  Memo1->Clear();

  AnsiString path = Edit1->Text+"*";
  HANDLE h;
  bool result;
  WIN32_FIND_DATA fd;
  h = FindFirstFile(path.c_str(), &fd);
  if(h != INVALID_HANDLE_VALUE)
     Memo1->Lines->Add(fd.cFileName);
  result = true;
  while(result)
  {
  result = FindNextFile(h, &fd);
  if(result)
    Memo1->Lines->Add(fd.cFileName);
  }
  FindClose(h);

  TStringList* sl = new TStringList;
  FindFiles(sl,Edit1->Text);
  //Filter bmp
  int indx = 0;
  AnsiString ext;
  int extPtr;
  while(indx < sl->Count)
  {
    extPtr = sl->Strings[indx].Length() - 3;
    ext = sl->Strings[indx].SubString(extPtr,4);
    if (ext != ".bmp")
    {
      sl->Delete(indx);
      continue;
    }
    indx++;
  }
  Memo1->Text = sl->Text;
  //Convert & Save:
  AnsiString saveFilename;
  CMyGfx g;
  for(indx = 0; indx < sl->Count; indx++)
  {
    saveFilename = sl->Strings[indx];
    saveFilename.Delete(saveFilename.Length()-3,4);
    saveFilename += ".myf";
    g.Bm2My(sl->Strings[indx].c_str(), saveFilename.c_str());
  }

  Memo1->Lines->Add("Total "+IntToStr(sl->Count)+ " .bmp Files Found");
  delete sl;
}
//------------------------------------------------------------------------------

void __fastcall TForm1::ButtonConvClick(TObject *Sender)
{
  CMyGfx my;
  if(OpenDialog1->Execute())
  {
    AnsiString saveFilename = OpenDialog1->FileName + ".myf";
    my.Bm2My(OpenDialog1->FileName.c_str(), saveFilename.c_str());
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonLoadMYFClick(TObject *Sender)
{

  CMyGfx g;
  Graphics::TBitmap* bm = new Graphics::TBitmap;
  if(OpenDialog1->Execute())
  {
    g.LoadMYF2Bitmap(OpenDialog1->FileName.c_str(), bm);
    PaintBox2->Canvas->Draw(0,0,bm);
  }
  delete bm;
  return;


}
//---------------------------------------------------------------------------
void FindFiles(TStringList *sp, AnsiString DirName)
{
TSearchRec sr;
if (DirName.Length())
 {
 if (!FindFirst(DirName+"\\*.*",faAnyFile,sr))
 do
  {
  if (!(sr.Name=="." || sr.Name==".."))
   if (((sr.Attr & faDirectory) == faDirectory ) ||
   (sr.Attr == faDirectory))
    {
    FindFiles(sp, DirName+"\\"+sr.Name);
    }
    else
    {
    sp->Add(DirName+"\\"+sr.Name);
    }
  }
 while (!FindNext(sr));
 FindClose(sr);
 }
}
void __fastcall TForm1::ButtonWMClick(TObject *Sender)
{
  if (!OpenDialog1->Execute()) return;
  Graphics::TBitmap* bm = new Graphics::TBitmap;
  bm->LoadFromFile(OpenDialog1->FileName);

  typedef struct
  {
    BYTE R;
    BYTE G;
    BYTE B;
    BYTE A;
  }color32_t;

  TColor color;

  color32_t* pcolor;
  color32_t acolor;  //Target color

  color = bm->Canvas->Pixels[10][10];//Pick clear color
  pcolor = (color32_t*)&color;

  acolor.R = pcolor->R;
  acolor.G = pcolor->G;
  acolor.B = pcolor->B;





  DWORD w = bm->Width;
  DWORD h = bm->Height;
  DWORD x,y;
  for(y=0; y < h; y++)
  {
    for(x=0; x < w; x++)
    {
      color = bm->Canvas->Pixels[x][y];
      pcolor = (color32_t*) &color;
      pcolor->R -= acolor.R;
      pcolor->G -= acolor.G;
      pcolor->B -= acolor.B;
      bm->Canvas->Pixels[x][y] = color;
    }
  }
  PaintBox2->Canvas->Draw(0,0,bm);
  bm->SaveToFile(OpenDialog1->FileName+"1.bmp");
  delete bm;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonRemWMClick(TObject *Sender)
{
// unwater mask
if (!OpenDialog1->Execute()) return;
  Graphics::TBitmap* bmmsk = new Graphics::TBitmap;
  bmmsk->LoadFromFile(OpenDialog1->FileName);

if (!OpenDialog1->Execute()) return;
  Graphics::TBitmap* bm = new Graphics::TBitmap;
  bm->LoadFromFile(OpenDialog1->FileName);
  typedef struct
  {
    BYTE R;
    BYTE G;
    BYTE B;
    BYTE A;
  }color32_t;

  TColor pcolor,acolor;

  color32_t* ppcolor;  //picture color
  color32_t* pacolor;  //unwater color


  DWORD w = bm->Width;
  DWORD h = bm->Height;
  DWORD x,y;
  for(y=0; y < h; y++)
  {
    for(x=0; x < w; x++)
    {
      pcolor = bm->Canvas->Pixels[x][y];
      acolor = bmmsk->Canvas->Pixels[x][y];
      ppcolor = (color32_t*)&pcolor;
      pacolor = (color32_t*)&acolor;
      ppcolor->R -= pacolor->R;
      ppcolor->G -= pacolor->G;
      ppcolor->B -= pacolor->B;
      bm->Canvas->Pixels[x][y] = pcolor;
    }
  }
  PaintBox2->Canvas->Draw(0,0,bm);
  bm->SaveToFile(OpenDialog1->FileName+"unwater.bmp");
  delete bm;

}
//---------------------------------------------------------------------------


void __fastcall TForm1::UpDownZoomClick(TObject *Sender, TUDBtnType Button)
{
  if(Button == btNext) zoom +=1;
  if(Button == btPrev) zoom -=1;
  ChangeZoom(&zoom);
  Button1Click(NULL);
}
//---------------------------------------------------------------------------

