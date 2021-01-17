inherited BaseFormT1: TBaseFormT1
  Caption = 'BaseFormT1'
  ExplicitWidth = 461
  ExplicitHeight = 319
  PixelsPerInch = 96
  TextHeight = 13
  inherited btnSaveOk: TBitBtn
    ExplicitLeft = 248
    ExplicitTop = 240
  end
  inherited btnCancel: TBitBtn
    ExplicitLeft = 344
    ExplicitTop = 240
  end
  inherited gbMain: TGroupBox
    object Button1: TButton
      Left = 184
      Top = 96
      Width = 75
      Height = 25
      Caption = 'otworz forme'
      TabOrder = 0
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 168
      Top = 144
      Width = 113
      Height = 25
      Caption = 'zamknij connection'
      TabOrder = 1
      OnClick = Button2Click
    end
  end
  inherited STipsPopupMain: TScreenTipsPopup
    ExplicitTop = 247
  end
end
