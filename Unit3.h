//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
        TPaintBox *PaintBoxLCD;
        TButton *Button1;
        void __fastcall PaintBoxLCDPaint(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations


public:		// User declarations
  Graphics::TBitmap* bm;
        __fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------


#define COLUMN_MIN 0
#define COLUMN_MAX 271
#define PAGE_MIN 0
#define PAGE_MAX 481
class LCD
{
public:
  void SetColumnAddress(WORD sc, WORD ec);
  void SetPageAddress(WORD sp, WORD ep);
  void WriteMemoryStart(WORD* pixels, DWORD n);
  void WriteMemoryContinue(WORD* pixels, DWORD n);
  void WriteMemoryRepeat(WORD pixel, DWORD n);
private:
  WORD _sc, _ec;
  WORD _sp, _ep;
  DWORD maxWrite;
  DWORD writeCnt;
  TRect r;

  void SetMaxWrite();

};
#endif
