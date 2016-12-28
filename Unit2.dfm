object Form2: TForm2
  Left = 261
  Top = 229
  Width = 1088
  Height = 563
  Caption = 'Tile converter and Watermark Remover'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = [fsBold]
  OldCreateOrder = False
  OnDestroy = FormDestroy
  PixelsPerInch = 120
  TextHeight = 16
  object PaintBoxSrc: TPaintBox
    Left = 280
    Top = 24
    Width = 256
    Height = 256
    OnMouseDown = PaintBoxSrcMouseDown
    OnPaint = PaintBoxSrcPaint
  end
  object PaintBoxResult: TPaintBox
    Left = 560
    Top = 24
    Width = 256
    Height = 256
    OnMouseDown = PaintBoxResultMouseDown
    OnPaint = PaintBoxResultPaint
  end
  object PaintBoxPal: TPaintBox
    Left = 944
    Top = 8
    Width = 129
    Height = 513
    OnMouseDown = PaintBoxPalMouseDown
    OnPaint = PaintBoxPalPaint
  end
  object LabelR: TLabel
    Left = 872
    Top = 24
    Width = 16
    Height = 16
    Caption = 'R:'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object LabelG: TLabel
    Left = 872
    Top = 48
    Width = 16
    Height = 16
    Caption = 'G:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object LabelB: TLabel
    Left = 872
    Top = 72
    Width = 15
    Height = 16
    Caption = 'B:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object ListBoxMasks: TListBox
    Left = 8
    Top = 8
    Width = 153
    Height = 185
    ItemHeight = 16
    TabOrder = 0
    OnClick = ListBoxMasksClick
  end
  object ButtonAddMask: TButton
    Left = 8
    Top = 192
    Width = 75
    Height = 25
    Caption = 'Add'
    TabOrder = 1
    OnClick = ButtonAddMaskClick
  end
  object ButtonRemMask: TButton
    Left = 82
    Top = 192
    Width = 75
    Height = 25
    Caption = 'Remove'
    TabOrder = 2
    OnClick = ButtonRemMaskClick
  end
  object ButtonLdTile: TButton
    Left = 368
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Load Tile'
    TabOrder = 3
    OnClick = ButtonLdTileClick
  end
  object Memo1: TMemo
    Left = 520
    Top = 360
    Width = 257
    Height = 145
    Lines.Strings = (
      'Memo1')
    TabOrder = 4
  end
  object UpDownContr: TUpDown
    Left = 144
    Top = 272
    Width = 17
    Height = 25
    Min = -32768
    Max = 32767
    Position = 0
    TabOrder = 5
    Wrap = False
    OnClick = UpDownContrClick
  end
  object EditContrast: TEdit
    Left = 104
    Top = 272
    Width = 41
    Height = 24
    TabOrder = 6
    Text = '1'
  end
  object CheckBox1: TCheckBox
    Left = 104
    Top = 312
    Width = 137
    Height = 17
    Caption = 'Adj Contrast'
    TabOrder = 7
  end
  object OpenDialogMasks: TOpenDialog
    Filter = '.bmp files (*.bmp)|*.bmp|All files (*.*)|*'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofEnableSizing]
    Left = 8
    Top = 192
  end
  object OpenDialogTile: TOpenDialog
    Left = 336
    Top = 288
  end
end
