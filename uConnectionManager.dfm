object ConnectionManager: TConnectionManager
  OldCreateOrder = False
  Height = 88
  Width = 133
  object fdcMain: TFDConnection
    Params.Strings = (
      'Database=FairWare'
      'User_Name=postgres'
      'Server=localhost'
      'DriverID=PG')
    Connected = True
    LoginPrompt = False
    Left = 40
    Top = 24
  end
end
