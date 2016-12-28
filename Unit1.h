//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>


#include "Projection.h"
#include "user.h"
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TEdit *EditZoom;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *EditLat;
        TEdit *EditLon;
        TLabel *LabelTileX;
        TLabel *LabelTileY;
        TButton *Button1;
        TEdit *Edit1;
        TButton *ButtonPath;
        TOpenDialog *OpenDialog1;
        TPaintBox *PaintBox1;
        TLabel *Label4;
        TLabel *LabelLat;
        TLabel *Label6;
        TLabel *LabelLon;
        TButton *ButtonConvAll;
        TButton *ButtonConv;
        TPaintBox *PaintBox2;
        TButton *ButtonLoadMYF;
        TMemo *Memo1;
        TButton *ButtonWM;
        TButton *ButtonRemWM;
        TUpDown *UpDownZoom;
        TButton *Button2;
        TButton *Button3;
        TButton *ButtonPNG;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ButtonPathClick(TObject *Sender);
        void __fastcall PaintBox1MouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall ButtonConvAllClick(TObject *Sender);
        void __fastcall ButtonConvClick(TObject *Sender);
        void __fastcall ButtonLoadMYFClick(TObject *Sender);
        void __fastcall ButtonWMClick(TObject *Sender);
        void __fastcall ButtonRemWMClick(TObject *Sender);
        void __fastcall UpDownZoomClick(TObject *Sender,
          TUDBtnType Button);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall ButtonPNGClick(TObject *Sender);
        void __fastcall PaintBox2Paint(TObject *Sender);
        void __fastcall PaintBox1Paint(TObject *Sender);
private:	// User declarations
  void ChangeZoom(BYTE* newZoom);
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);


        //user vars
  int orgX;
  int orgY;
  BYTE zoom;
  Graphics::TBitmap* bmOverlay;
  Graphics::TBitmap* pngbm;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------


CProjection proj;

DWORD tileX;
DWORD tileY;

AnsiString Tiledir;

AnsiString GenerateTilePath(DWORD x, DWORD y, BYTE zoom, AnsiString* tileRoot);
void LoadBMP(char* filename, DWORD x, DWORD y);
void LoadMYF(char* filename, DWORD x, DWORD y);
AnsiString Dbl2DegMins(double degrees);

void FindFiles(TStringList *sp, AnsiString DirName);
//bool FileExists(const char *fname);
#endif
 