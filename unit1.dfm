object Form1: TForm1
  Left = 171
  Top = 54
  AutoScroll = False
  Caption = 'AOP'
  ClientHeight = 598
  ClientWidth = 1055
  Color = clBtnFace
  Constraints.MaxHeight = 1080
  Constraints.MaxWidth = 1936
  Constraints.MinHeight = 631
  Constraints.MinWidth = 1063
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  DesignSize = (
    1055
    598)
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox2: TGroupBox
    Left = 837
    Top = -4
    Width = 218
    Height = 600
    Anchors = [akTop, akRight]
    TabOrder = 0
    object GroupBox8: TGroupBox
      Left = 4
      Top = 7
      Width = 210
      Height = 103
      Caption = ' calibrate '
      TabOrder = 0
      object Button16: TButton
        Left = 4
        Top = 15
        Width = 200
        Height = 22
        Hint = '     '#1082#1072#1083#1080#1073#1088#1086#1074#1082#1072' '#1090#1077#1084#1085#1086#1074#1086#1075#1086' '#1096#1091#1084#1072'     '
        Caption = 'by dark signal'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnClick = Button16Click
      end
      object Button3: TButton
        Left = 10
        Top = 77
        Width = 92
        Height = 20
        Hint = '   '#1074#1099#1095#1080#1089#1083#1080#1090#1100' '#1079#1085#1072#1095#1077#1085#1080#1077' shift   '
        Caption = '1100 MHz'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        OnClick = Button3Click
      end
      object LabeledEdit2: TLabeledEdit
        Left = 115
        Top = 53
        Width = 75
        Height = 21
        Hint = '   '#1082#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1076#1080#1086#1076#1086#1074' '#1074' '#1087#1086#1083#1086#1089#1077' 500 '#1052#1043#1094'   '
        EditLabel.Width = 48
        EditLabel.Height = 13
        EditLabel.Caption = '       range'
        LabelPosition = lpAbove
        LabelSpacing = 3
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnChange = LabeledEdit2Change
        OnKeyPress = LabeledEdit2KeyPress
      end
      object LabeledEdit1: TLabeledEdit
        Left = 19
        Top = 53
        Width = 75
        Height = 21
        Hint = 
          '   "'#1085#1086#1084#1077#1088'" '#1076#1080#1086#1076#1072' '#1085#1072' '#1087#1077#1088#1074#1086#1081' '#1086#1087#1086#1088#1085#1086#1081' '#1095#1072#1089#1090#1086#1090#1077' ('#1084#1086#1078#1077#1090' '#1073#1099#1090#1100' '#1085#1077' '#1094#1077#1083#1099#1084')' +
          '   '
        EditLabel.Width = 43
        EditLabel.Height = 13
        EditLabel.Caption = '        shift'
        LabelPosition = lpAbove
        LabelSpacing = 3
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        OnChange = LabeledEdit1Change
        OnKeyPress = LabeledEdit1KeyPress
      end
      object Button14: TButton
        Left = 109
        Top = 77
        Width = 92
        Height = 20
        Hint = '   '#1074#1099#1095#1080#1089#1083#1080#1090#1100' '#1079#1085#1072#1095#1077#1085#1080#1077' range   '
        Caption = '1600 MHz'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        OnClick = Button14Click
      end
    end
    object GroupBox5: TGroupBox
      Left = 4
      Top = 398
      Width = 210
      Height = 30
      Caption = ' scale '
      TabOrder = 1
      object RadioButton1: TRadioButton
        Left = 87
        Top = 11
        Width = 54
        Height = 16
        Hint = '   '#1083#1080#1085#1077#1081#1085#1099#1081' '#1084#1072#1089#1096#1090#1072#1073' '#1096#1082#1072#1083#1099' '#1072#1084#1087#1083#1080#1090#1091#1076'   '
        Caption = 'linear'
        Checked = True
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        TabStop = True
        OnClick = RadioButton1Click
      end
      object RadioButton2: TRadioButton
        Left = 156
        Top = 11
        Width = 44
        Height = 17
        Hint = '   '#1083#1086#1075#1072#1088#1080#1092#1084#1080#1095#1077#1089#1082#1080#1081' '#1084#1072#1089#1096#1090#1072#1073' '#1096#1082#1072#1083#1099' '#1072#1084#1087#1083#1080#1090#1091#1076'   '
        Caption = 'log'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        OnClick = RadioButton2Click
      end
      object CheckBox2: TCheckBox
        Left = 17
        Top = 13
        Width = 52
        Height = 15
        Hint = '   '#1072#1074#1090#1086#1084#1072#1089#1096#1090#1072#1073' '#1096#1082#1072#1083#1099' '#1072#1084#1087#1083#1080#1090#1091#1076'   '
        Caption = 'auto'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnClick = CheckBox2Click
      end
    end
    object GroupBox15: TGroupBox
      Left = 4
      Top = 472
      Width = 210
      Height = 62
      Caption = ' AOP'
      TabOrder = 2
      object Button8: TButton
        Left = 5
        Top = 15
        Width = 63
        Height = 20
        Hint = '   '#1079#1072#1076#1077#1081#1089#1090#1074#1086#1074#1072#1090#1100' USB '#1072#1085#1072#1083#1080#1079#1072#1090#1086#1088#1072'   '
        Caption = 'open'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnClick = Button8Click
      end
      object Button10: TButton
        Left = 141
        Top = 15
        Width = 63
        Height = 20
        Hint = '   '#1089#1073#1088#1086#1089#1080#1090#1100' USB '#1072#1085#1072#1083#1080#1079#1072#1090#1086#1088#1072'    '
        Caption = 'reset'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnClick = Button10Click
      end
      object Button9: TButton
        Left = 73
        Top = 15
        Width = 63
        Height = 20
        Hint = '   '#1079#1072#1082#1088#1099#1090#1100' USB '#1072#1085#1072#1083#1080#1079#1072#1090#1086#1088#1072'   '
        Caption = 'close'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        OnClick = Button9Click
      end
      object Panel4: TPanel
        Left = 4
        Top = 38
        Width = 202
        Height = 20
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
    end
    object GroupBox1: TGroupBox
      Left = 4
      Top = 110
      Width = 210
      Height = 113
      Caption = ' work '
      TabOrder = 3
      object CheckBox5: TCheckBox
        Left = 9
        Top = 11
        Width = 52
        Height = 17
        Hint = '   '#1094#1080#1082#1083#1080#1095#1077#1089#1082#1080#1081' / '#1086#1076#1085#1086#1082#1088#1072#1090#1085#1099#1081' '#1072#1085#1072#1083#1080#1079'   '
        Caption = 'auto'
        Checked = True
        ParentShowHint = False
        ShowHint = True
        State = cbChecked
        TabOrder = 0
      end
      object Button6: TButton
        Left = 8
        Top = 31
        Width = 100
        Height = 23
        Hint = '   '#1085#1072#1095#1072#1090#1100' '#1072#1085#1072#1083#1080#1079'   '
        Caption = 'start'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        OnClick = Button6Click
      end
      object Button7: TButton
        Left = 8
        Top = 58
        Width = 100
        Height = 23
        Hint = '   '#1086#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1072#1085#1072#1083#1080#1079'   '
        Caption = 'stop'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnClick = Button7Click
      end
      object Button2: TButton
        Left = 8
        Top = 85
        Width = 100
        Height = 23
        Hint = '   '#1086#1095#1080#1089#1090#1080#1090#1100'   '
        Caption = 'clear'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        OnClick = Button2Click
      end
      object CheckBox1: TCheckBox
        Left = 127
        Top = 11
        Width = 68
        Height = 17
        Hint = '   '#1074#1099#1095#1080#1089#1083#1080#1090#1100' '#1087#1072#1088#1072#1084#1077#1090#1088#1099' '#1074' '#1084#1072#1082#1089#1080#1084#1091#1084#1077' '#1088#1072#1089#1087#1088#1077#1076#1077#1083#1077#1085#1080#1103'   '
        Caption = 'calculate'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
      end
      object LabeledEdit3: TLabeledEdit
        Left = 120
        Top = 43
        Width = 82
        Height = 28
        Hint = '   '#1079#1085#1072#1095#1077#1085#1080#1077' '#1084#1072#1082#1089#1080#1084#1091#1084#1072' '#1088#1072#1089#1087#1088#1077#1076#1077#1083#1077#1085#1080#1103'   '
        EditLabel.Width = 25
        EditLabel.Height = 13
        EditLabel.Caption = 'Fmax'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        LabelPosition = lpAbove
        LabelSpacing = 3
        ParentFont = False
        ParentShowHint = False
        ReadOnly = True
        ShowHint = True
        TabOrder = 5
      end
      object LabeledEdit4: TLabeledEdit
        Left = 120
        Top = 87
        Width = 82
        Height = 21
        Hint = '   '#1079#1085#1072#1095#1077#1085#1080#1077' '#1095#1072#1089#1090#1086#1090#1099' '#1084#1072#1082#1089#1080#1084#1072#1083#1100#1085#1086#1075#1086' '#1089#1080#1075#1085#1072#1083#1072'   '
        EditLabel.Width = 26
        EditLabel.Height = 13
        EditLabel.Caption = 'Amax'
        LabelPosition = lpAbove
        LabelSpacing = 3
        ParentShowHint = False
        ReadOnly = True
        ShowHint = True
        TabOrder = 6
      end
      object CheckBox3: TCheckBox
        Left = 69
        Top = 11
        Width = 41
        Height = 17
        Caption = 'file'
        TabOrder = 7
      end
    end
    object GroupBox3: TGroupBox
      Left = 4
      Top = 333
      Width = 210
      Height = 64
      Caption = ' threshold '
      TabOrder = 4
      object Edit2: TEdit
        Left = 136
        Top = 12
        Width = 64
        Height = 21
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnKeyPress = Edit2KeyPress
      end
      object TrackBar1: TTrackBar
        Left = 2
        Top = 33
        Width = 205
        Height = 25
        Hint = '     '#1091#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1079#1085#1072#1095#1077#1085#1080#1077' '#1087#1086#1088#1086#1075#1072'     '
        Max = 3000
        Orientation = trHorizontal
        ParentShowHint = False
        PageSize = 10
        Frequency = 200
        Position = 0
        SelEnd = 0
        SelStart = 0
        ShowHint = True
        TabOrder = 1
        ThumbLength = 17
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = TrackBar1Change
      end
      object CheckBox4: TCheckBox
        Left = 34
        Top = 16
        Width = 72
        Height = 17
        Hint = '     '#1086#1090#1086#1073#1088#1072#1079#1080#1090#1100' '#1087#1086#1088#1086#1075#1086#1074#1091#1102' '#1083#1080#1085#1080#1102'     '
        Caption = 'show'
        Checked = True
        ParentShowHint = False
        ShowHint = True
        State = cbChecked
        TabOrder = 2
        OnClick = CheckBox4Click
      end
    end
    object GroupBox4: TGroupBox
      Left = 4
      Top = 223
      Width = 210
      Height = 110
      Caption = ' files '
      TabOrder = 5
      object Button5: TButton
        Left = 22
        Top = 16
        Width = 63
        Height = 20
        Hint = '   '#1089#1086#1093#1088#1072#1085#1080#1090#1100' '#1088#1072#1089#1087#1088#1077#1076#1077#1083#1077#1085#1080#1077'   '
        Caption = 'save'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnClick = Button5Click
      end
      object Button1: TButton
        Left = 22
        Top = 38
        Width = 63
        Height = 20
        Hint = '   '#1079#1072#1075#1088#1091#1079#1080#1090#1100' '#1083#1102#1073#1086#1077' '#1088#1072#1089#1087#1088#1077#1076#1077#1083#1077#1085#1080#1077' ( '#1080#1089#1093#1086#1076#1085#1099#1081' '#1087#1091#1090#1100' '#1089':\ )     '
        Caption = 'load'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        OnClick = Button1Click
      end
      object Button11: TButton
        Left = 136
        Top = 13
        Width = 63
        Height = 20
        Hint = '   '#1079#1072#1075#1088#1091#1079#1080#1090#1100' '#1090#1077#1089#1090#1086#1074#1086#1077' '#1088#1072#1089#1087#1088#1077#1076#1077#1083#1077#1085#1080#1077'   '
        Caption = 'create'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnClick = Button11Click
      end
      object LabeledEdit5: TLabeledEdit
        Left = 115
        Top = 39
        Width = 85
        Height = 24
        EditLabel.Width = 13
        EditLabel.Height = 13
        EditLabel.Caption = 'Dir'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        LabelPosition = lpAbove
        LabelSpacing = 3
        ParentFont = False
        TabOrder = 3
        Text = '1'
        OnKeyPress = LabeledEdit5KeyPress
      end
      object ScrollBar1: TScrollBar
        Left = 4
        Top = 64
        Width = 202
        Height = 15
        LargeChange = 10
        PageSize = 0
        TabOrder = 4
        OnChange = ScrollBar1Change
      end
      object Button4: TButton
        Left = 115
        Top = 83
        Width = 74
        Height = 22
        Caption = 'show file'
        TabOrder = 5
        Visible = False
        OnClick = Button4Click
      end
      object Edit1: TEdit
        Left = 24
        Top = 84
        Width = 73
        Height = 21
        TabOrder = 6
        Text = '1'
        OnKeyPress = Edit1KeyPress
      end
    end
    object GroupBox6: TGroupBox
      Left = 4
      Top = 425
      Width = 210
      Height = 48
      TabOrder = 6
      object Panel13: TPanel
        Left = 4
        Top = 8
        Width = 202
        Height = 18
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object Button25: TButton
        Left = 4
        Top = 27
        Width = 202
        Height = 18
        Hint = '   '#1089#1072#1084#1086#1083#1080#1082#1074#1080#1076#1072#1094#1080#1103'   '
        Caption = 'Emergence'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        OnClick = Button25Click
      end
    end
    object GroupBox7: TGroupBox
      Left = 4
      Top = 530
      Width = 210
      Height = 58
      TabOrder = 7
      object Panel7: TPanel
        Left = 12
        Top = 11
        Width = 185
        Height = 41
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -21
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
    end
    object Panel9: TPanel
      Left = 4
      Top = 598
      Width = 210
      Height = 1
      BevelOuter = bvNone
      Color = clNavy
      TabOrder = 8
    end
  end
  object Chart1: TChart
    Left = 0
    Top = 1
    Width = 835
    Height = 595
    AnimatedZoom = True
    BackWall.Brush.Color = clWhite
    BackWall.Color = 11075583
    BackWall.Dark3D = False
    LeftWall.Brush.Color = clWhite
    LeftWall.Color = 10223615
    LeftWall.Dark3D = False
    MarginBottom = 1
    MarginLeft = 1
    MarginRight = 1
    MarginTop = 0
    Title.Text.Strings = (
      '')
    OnScroll = Chart1Scroll
    OnZoom = Chart1Zoom
    BackColor = 11075583
    BottomAxis.Automatic = False
    BottomAxis.AutomaticMaximum = False
    BottomAxis.AutomaticMinimum = False
    BottomAxis.Maximum = 2050
    BottomAxis.Minimum = 1450
    BottomAxis.MinorTickCount = 4
    LeftAxis.Automatic = False
    LeftAxis.AutomaticMaximum = False
    LeftAxis.AutomaticMinimum = False
    LeftAxis.ExactDateTime = False
    LeftAxis.Maximum = 4000
    Legend.Visible = False
    View3D = False
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 1
    OnClick = Chart1Click
    OnMouseDown = Chart1MouseDown
    OnMouseMove = Chart1MouseMove
    object Panel5: TPanel
      Left = 739
      Top = 20
      Width = 88
      Height = 56
      BorderStyle = bsSingle
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Panel10: TPanel
        Left = 0
        Top = 2
        Width = 82
        Height = 19
        Alignment = taLeftJustify
        BevelOuter = bvNone
        Caption = ' F1: 1500.0'
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object Panel3: TPanel
        Left = 0
        Top = 30
        Width = 82
        Height = 19
        Alignment = taLeftJustify
        BevelOuter = bvNone
        Color = clWhite
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object Panel1: TPanel
        Left = 1
        Top = 23
        Width = 81
        Height = 3
        TabOrder = 2
      end
    end
    object Panel2: TPanel
      Left = 752
      Top = 2
      Width = 65
      Height = 16
      BevelOuter = bvNone
      TabOrder = 1
    end
    object Panel6: TPanel
      Left = 592
      Top = 50
      Width = 82
      Height = 19
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      Visible = False
    end
    object Panel11: TPanel
      Left = 592
      Top = 28
      Width = 82
      Height = 19
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Visible = False
    end
    object Series1: TLineSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = 10485760
      ShowInLegend = False
      LinePen.Width = 2
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series2: TLineSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clGreen
      Dark3D = False
      LinePen.Width = 2
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series5: TLineSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = clRed
      ShowInLegend = False
      LinePen.Width = 2
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series3: TLineSeries
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = 64
      LinePen.Width = 2
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
    object Series4: TLineSeries
      Active = False
      Marks.ArrowLength = 8
      Marks.Visible = False
      SeriesColor = 8388863
      Pointer.InflateMargins = True
      Pointer.Style = psRectangle
      Pointer.Visible = False
      XValues.DateTime = False
      XValues.Name = 'X'
      XValues.Multiplier = 1
      XValues.Order = loAscending
      YValues.DateTime = False
      YValues.Name = 'Y'
      YValues.Multiplier = 1
      YValues.Order = loNone
    end
  end
  object Edit3: TEdit
    Left = 45
    Top = 5
    Width = 11
    Height = 12
    BevelInner = bvNone
    BevelOuter = bvNone
    BorderStyle = bsNone
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 2
    OnKeyDown = Edit3KeyDown
  end
  object SaveDialog1: TSaveDialog
    InitialDir = 'c:\'
    Left = 8
    Top = 79
  end
  object OpenDialog1: TOpenDialog
    InitialDir = 'c:\   '
    Left = 8
    Top = 32
  end
end
