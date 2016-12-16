//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>


#include "Projection.h"
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
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
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);


        //user vars
        int orgX;
        int orgY;
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
 