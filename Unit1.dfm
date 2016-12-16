object Form1: TForm1
  Left = 243
  Top = 235
  Width = 1134
  Height = 640
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 16
  object Label1: TLabel
    Left = 24
    Top = 16
    Width = 35
    Height = 16
    Caption = 'Zoom'
  end
  object Label2: TLabel
    Left = 24
    Top = 64
    Width = 50
    Height = 16
    Caption = #1064#1080#1088#1086#1090#1072
  end
  object Label3: TLabel
    Left = 24
    Top = 96
    Width = 54
    Height = 16
    Caption = #1044#1086#1083#1075#1086#1090#1072
  end
  object LabelTileX: TLabel
    Left = 288
    Top = 584
    Width = 65
    Height = 16
    Caption = 'LabelTileX'
  end
  object LabelTileY: TLabel
    Left = 368
    Top = 584
    Width = 63
    Height = 16
    Caption = 'LabelTilex'
  end
  object PaintBox1: TPaintBox
    Left = 344
    Top = 56
    Width = 768
    Height = 512
    Color = clCream
    ParentColor = False
    OnMouseMove = PaintBox1MouseMove
  end
  object Label4: TLabel
    Left = 592
    Top = 576
    Width = 22
    Height = 16
    Caption = 'Lat'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clHotLight
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object LabelLat: TLabel
    Left = 624
    Top = 576
    Width = 9
    Height = 16
    Caption = 'y'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clHotLight
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label6: TLabel
    Left = 760
    Top = 576
    Width = 26
    Height = 16
    Caption = 'Lon'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clHotLight
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object LabelLon: TLabel
    Left = 792
    Top = 576
    Width = 8
    Height = 16
    Caption = 'x'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clHotLight
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object PaintBox2: TPaintBox
    Left = 48
    Top = 312
    Width = 256
    Height = 256
  end
  object EditZoom: TEdit
    Left = 72
    Top = 16
    Width = 41
    Height = 24
    TabOrder = 0
    Text = '13'
  end
  object EditLat: TEdit
    Left = 88
    Top = 64
    Width = 121
    Height = 24
    TabOrder = 1
    Text = '54,679'
  end
  object EditLon: TEdit
    Left = 88
    Top = 96
    Width = 121
    Height = 24
    TabOrder = 2
    Text = '20,349'
  end
  object Button1: TButton
    Left = 320
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 440
    Top = 16
    Width = 401
    Height = 24
    TabOrder = 4
    Text = 'C:\Users\User\Desktop\zaliv-maps\'
  end
  object ButtonPath: TButton
    Left = 848
    Top = 16
    Width = 33
    Height = 25
    Caption = '...'
    TabOrder = 5
    OnClick = ButtonPathClick
  end
  object ButtonConvAll: TButton
    Left = 144
    Top = 128
    Width = 75
    Height = 25
    Caption = 'Conv All'
    TabOrder = 6
    OnClick = ButtonConvAllClick
  end
  object ButtonConv: TButton
    Left = 48
    Top = 128
    Width = 75
    Height = 25
    Caption = 'Conv'
    TabOrder = 7
    OnClick = ButtonConvClick
  end
  object ButtonLoadMYF: TButton
    Left = 48
    Top = 576
    Width = 75
    Height = 25
    Caption = 'LoadMYF'
    TabOrder = 8
    OnClick = ButtonLoadMYFClick
  end
  object Memo1: TMemo
    Left = 8
    Top = 160
    Width = 329
    Height = 129
    Lines.Strings = (
      'Memo1')
    ScrollBars = ssVertical
    TabOrder = 9
  end
  object ButtonWM: TButton
    Left = 192
    Top = 0
    Width = 75
    Height = 25
    Caption = 'WM'
    TabOrder = 10
    OnClick = ButtonWMClick
  end
  object ButtonRemWM: TButton
    Left = 240
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Rem WM'
    TabOrder = 11
    OnClick = ButtonRemWMClick
  end
  object OpenDialog1: TOpenDialog
    Left = 888
    Top = 16
  end
end
