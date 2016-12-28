object Form3: TForm3
  Left = 734
  Top = 232
  Width = 295
  Height = 582
  Caption = 'LCD Emulator'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  PixelsPerInch = 120
  TextHeight = 16
  object PaintBoxLCD: TPaintBox
    Left = 8
    Top = 8
    Width = 272
    Height = 482
    OnPaint = PaintBoxLCDPaint
  end
  object Button1: TButton
    Left = 104
    Top = 504
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 0
    OnClick = Button1Click
  end
end
