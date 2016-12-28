//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
#include "mygfx.h"
#include "utils.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TPaintBox *PaintBoxSrc;
        TPaintBox *PaintBoxResult;
        TListBox *ListBoxMasks;
        TButton *ButtonAddMask;
        TButton *ButtonRemMask;
        TOpenDialog *OpenDialogMasks;
        TButton *ButtonLdTile;
        TOpenDialog *OpenDialogTile;
        TMemo *Memo1;
        TPaintBox *PaintBoxPal;
        TLabel *LabelR;
        TLabel *LabelG;
        TLabel *LabelB;
        TUpDown *UpDownContr;
        TEdit *EditContrast;
        TCheckBox *CheckBox1;
        void __fastcall ButtonAddMaskClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall ListBoxMasksClick(TObject *Sender);
        void __fastcall ButtonRemMaskClick(TObject *Sender);
        void __fastcall ButtonLdTileClick(TObject *Sender);
        void __fastcall PaintBoxResultPaint(TObject *Sender);
        void __fastcall PaintBoxSrcPaint(TObject *Sender);
        void __fastcall PaintBoxPalPaint(TObject *Sender);
        void __fastcall PaintBoxPalMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall UpDownContrClick(TObject *Sender,
          TUDBtnType Button);
        void __fastcall PaintBoxResultMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall PaintBoxSrcMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
  Graphics::TBitmap* bmsrc;
  Graphics::TBitmap* bmdst;
  Graphics::TBitmap* bmmsk;
  Graphics::TBitmap* bmpal;

  TStringList* maskPaths;
  AnsiString workdir;
  float wmContrast;

public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
void MergeWithMask(Graphics::TBitmap* src, Graphics::TBitmap* dest, Graphics::TBitmap* mask);
void DrawPalette(WORD* clut, WORD usedCnt, Graphics::TBitmap* bm);
void AdjWMContrast(Graphics::TBitmap* bm, float contrast);
#endif
