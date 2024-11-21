//---------------------------------------------------------------------------

#include <vcl.h>
#include <fstream.h>
#include <math.h>
#include <process.h>

#pragma hdrstop

#include "unit1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}                           

// FT status control --------------------------------------------

void __fastcall TForm1::FTStatContr(AnsiString Ind)
{

        if (ftStatus == FT_OK)
        {
                Indicator = Ind;
                Panel4->Font->Color = clBlue;
        }
        else
                { Panel4->Font->Color = clRed;
                switch (ftStatus)
                        {
                case FT_INVALID_HANDLE:
                        Indicator = "Неверный дескриптор";
                        break;
                case FT_DEVICE_NOT_FOUND:
                        Indicator = "Устройство не найдено";
                        break;
                case FT_DEVICE_NOT_OPENED:
                        Indicator = "Невозможно открыть";
                        break;
                default:
                        Indicator = "Ошибка открытия";

                        }
                }
        Panel4->Caption = Indicator;
        Panel13->Caption = "";

}

// FT both buffers purge -----------------------------------------------------

void __fastcall TForm1::FifoPurge(AnsiString Ind)
{

        ftStatus = FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);

        if (ftStatus != FT_OK)
                {
                Panel13->Font->Color = clRed;
                Panel13->Caption = Ind;
                }
}                                            

// FT RX purge -------------------------------------------------

void __fastcall TForm1::RX_Purge(AnsiString Ind)
{

        ftStatus = FT_Purge(ftHandle, FT_PURGE_RX);

        if (ftStatus != FT_OK)
                {
                Panel13->Font->Color = clRed;
                Panel13->Caption = Ind;
                }
}

// PCB purge -------------------------------------------------------------

void __fastcall TForm1::AO_Purge(void)

{

        txModeFrame[1] = 0x00;

        WriteUSB("Ошибка очистки блока");

        FT_GetQueueStatus(ftHandle,&RxBytes);

        while (RxBytes > 0)
        {
                FT_Read(ftHandle, buf, 1, &BReturned);
                FT_GetQueueStatus(ftHandle,&RxBytes);
                Application->ProcessMessages();
        }        

}

// write frame set -------------------------------------------------

void __fastcall TForm1::Frame_Set(void)
{
        txModeFrame[1] = MODE_Set | DATA_RQ_Set | CLB_B_Set | CLB_F_Set |
                         CLOSE_SHF_Set | TURN_GKS_Set | AMPL_DIST_Set | AUTO_CLB_Set;

        txModeFrame[2] = Threshold;
        txModeFrame[3] = Threshold;
        txModeFrame[4] = Filter;
}

// write to USB ---------------------------------------------------

void __fastcall TForm1::WriteUSB(AnsiString Ind)
{

        FT_Purge(ftHandle, FT_PURGE_TX);
        
        txModeFrame[0] = FrHead;    // Заголовок кадра управления режимами 0xC0

        ftStatus = FT_Write(ftHandle, txModeFrame,
                                sizeof(txModeFrame), &BWritten);

        if (ftStatus != FT_OK)
                {
                Panel13->Font->Color = clRed;
                Panel13->Caption = Ind;
                }

        else    Panel13->Caption = "OK";

}

// delay -----------------------------------------------------------------

void __fastcall TForm1::DelayF(void)
{
        for (int i=0; i<10; i++)
        {
                Application->ProcessMessages();
                Sleep(SleepInterv/10);
        }

}
               
// data reading -------------------------------------------------------------

bool __fastcall TForm1::ReadData(unsigned char *pData, DWORD ToRead)
{
         
        //RxBytes = 0;

        FT_GetQueueStatus(ftHandle,&RxBytes);
        //Panel7->Caption = RxBytes;

        if (RxBytes > 0)
        {
        ftStatus = FT_Read(ftHandle, pData, ToRead, &BReturned);

                if (ftStatus != FT_OK)
                {
                        Panel13->Font->Color = clRed;
                        Panel13->Caption = "Ошибка чтения";
                        return false;
                }
        }

        else
        {
                Panel13->Font->Color = clRed;
                //Panel13->Caption = "Буфер пуст";
                Panel13->Caption = "";
                return false;
        }

        return true;

}

// data decoder ------------------------------------------

void __fastcall TForm1::DecodeData(void)
{

        SlujKadr = false;
        AByte = 0x00;
        CycleBreak = false;

header:
        do
        {
        ReadData(&AByte,1);
        Application->ProcessMessages();
        }
        while (AByte != FrHead && !CycleBreak);


        //if (CycleBrake) {conf2 = true; return;}

wake:
        ReadData(&AByte, 1);

        switch (AByte)
        {
                case FrHead: goto wake;
                case FrDB:
                        {
                        ReadData(&AByte, 1);
                        switch (AByte)
                                {
                                        case FrDC:
                                                {
                                                OneByte = FrHead;
                                                break;
                                                }

                                        case FrDD:
                                                {
                                                OneByte = FrDB;
                                                break;
                                                }
                                        
                                        default: goto header;
                                };

                        break;
                        }

                default:  OneByte = AByte;
        }

        switch (OneByte)
        {
                case FrIdn21:
                {
                        SlujKadr = true;
                        for (int i=0; i<3; i++)
                        {
                                ReadData(&AByte,1);
                                rxInfSFr[i] = AByte;
                        };

                        break;
                }


                case FrIdn22:
                {
                        for (int i=0; i<6; i++)
                        {
                                ReadData(&AByte,1);
                                switch (AByte)
                                {
                                        case FrHead: goto wake;
                                        case FrDB:
                                        {
                                                ReadData(&AByte, 1);
                                                switch (AByte)
                                                {
                                                        case FrDC:
                                                        {
                                                        rxAmplSFr[i] = FrHead;
                                                        break;
                                                        }

                                                        case FrDD:
                                                        {
                                                        rxAmplSFr[i] = FrDB;
                                                        break;
                                                        }
                                        
                                                        default: goto header;
                                        };

                                        break;
                                        }

                                        default:  rxAmplSFr[i] = AByte;
                                }
                        };

                        break;
                }

        }


        MODE_ACK_v       = (rxInfSFr[0] & MODE_ACK);
        CLB_B_ACK_v      = (rxInfSFr[0] & CLB_B_ACK);
        CLB_F_ACK_v      = (rxInfSFr[0] & CLB_F_ACK);
        DATA_EXIST_v     = (rxInfSFr[0] & DATA_EXIST);
        OVERFLOW_v       = (rxInfSFr[0] & OVER_FLOW);
        AMPL_DIST_ACK_v  = (rxInfSFr[0] & AMPL_DIST_ACK);

}             

// USB open -----------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{

        Threshold = 0;
        Filter = 0;     
        
        ftStatus = FT_OpenEx("", FT_OPEN_BY_SERIAL_NUMBER, &ftHandle);

        FTStatContr("Устройство инициализировано");

        // 8 bit format set
        FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);

        DelayF();

        // FT & PCB purge

        //AO_Purge();

        FifoPurge("Ошибка очистки буфера");
        if (ftStatus != FT_OK) return;

        // write frame init
        txModeFrame[0] = FrHead;
        txModeFrame[1] = EMPTY_BT;
        txModeFrame[2] = Threshold;
        txModeFrame[3] = Threshold;
        txModeFrame[4] = Filter;   

}

// USB close --------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{

        ftStatus = FT_Close(ftHandle);

        FTStatContr("Устройство закрыто");

}

// USB reset ------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{

        ftStatus = FT_ResetDevice(ftHandle);

        FTStatContr("Устройство перезагружено");

        // FT & PCB purge

        //AO_Purge();

        FifoPurge("Ошибка очистки буфера");
        if (ftStatus != FT_OK) return;

        Threshold = 0;
        Filter = 0;

        // write frame init
        txModeFrame[0] = FrHead;
        txModeFrame[1] = EMPTY_BT;
        txModeFrame[2] = Threshold;
        txModeFrame[3] = Threshold;
        txModeFrame[4] = Filter;  
               
}

// graph and so on clear -------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
        
        Series1->Clear();
        Series3->Clear();
        Series5->Clear(); 

        Panel13->Caption = "";

        // cursors initials


                // 1050-1650

                x_cursor1=1050;
                x_cursor2=1650;


        Series5->AddXY(x_cursor1, 0, "");
        Series5->AddXY(x_cursor1, y_top, "");

        Series3->AddXY(x_cursor2, 0, "");
        Series3->AddXY(x_cursor2, y_top, "");

        Chart1->UndoZoom();
        
        Chart1->LeftAxis->Logarithmic=false;

                

        n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift; 
        n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;


        Panel10->Caption = " F1: "+FormatFloat("###0.0",x_cursor1);
        Panel11->Caption = " n1: "+FormatFloat("###0.0",n_cursor1);
        //Panel8->Caption = " A1: "+FormatFloat("###0.0",0);

        Panel3->Caption = " F2: "+FormatFloat("###0.0",x_cursor2);
        Panel6->Caption = " n2: "+FormatFloat("###0.0",n_cursor2);
        //Panel7->Caption = " A2: "+FormatFloat("###0.0",0);

        TrackBar1->Position = Thres2;

        RadioButton1->Checked = true; 
                 
}

// thres change -----------------------------------------------------------

void __fastcall TForm1::TrackBar1Change(TObject *Sender)
{

        Series2->Clear();

        Thres = TrackBar1->Position;   
                                        
        Series2->AddXY(minb, Thres, "");
        Series2->AddXY(maxb, Thres, "");

        Edit2->Text = Thres;

}

// save distr into a file ------------------------------------------------------
       
void __fastcall TForm1::Button5Click(TObject *Sender)
{

        if(SaveDialog1->Execute())
     {
        char* FileOut = (SaveDialog1->FileName).c_str();

        // open a file
        ofstream out1(FileOut);

        // write file with 1024 values
        for(int i=0; i<buf_size; i++)
        {                              
                out1 << AmpDist[i] << endl;
        }
        out1.close();
     }
     
}

// form close ------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{

        CycleBreak = true;

        if (!CycleBreak3)
        {
        Application->MessageBoxA("Остановите Амплитудное распределение", "", MB_OK);
        Action = caNone;
        return;
        }    
          
        delete Ini;
        Ini = NULL;

        ftStatus = FT_Close(ftHandle);  

} 

// dark signal calibr -------------------------------------------------

void __fastcall TForm1::Button16Click(TObject *Sender)
{


        TrackBar1->Position = Thres2;

        
        // 1. FT & PCB purge

        //AO_Purge();

        FifoPurge("Ошибка очистки буфера");
        if (ftStatus != FT_OK) return;

        // 2. set PCB into calibr mode                  (CLB_B = 1)

        CLB_B_Set = CLB_B;                              // флаг 0000 0100

        Frame_Set();

        WriteUSB("Ошибка команды");
        if (ftStatus != FT_OK) return;
                                       

        // 3. get the acknowledge                         (CLB_B_ACK = 1)

        CycleBreak = false;
        read_OK = false;
        //TryToRead = 0;
           
        do
        {
                read_OK = ReadData(rxModeFrame, 5);
                Application->ProcessMessages(); 
        }
        while (CycleBreak==false && read_OK != true);


        // test head & ident of frame
        if ((rxModeFrame[0] != FrHead)||((rxModeFrame[1]) != FrIdn21))
               {
                Panel13->Font->Color = clRed;
                Panel13->Caption = "Ошибка структуры кадра";
                return;
               }

        // test of calibr ack
        if ((rxModeFrame[2]&CLB_B_ACK) != CLB_B_ACK)
               {
                Panel13->Font->Color = clRed;
                Panel13->Caption = "Ошибка подтверждения калибровки";
                return;
               }


        // 4. remove calibr command                     (CLB_B = 0)

        CLB_B_Set = EMPTY_BT;                           // remove flag 0000 0100

        Frame_Set();

        WriteUSB("Ошибка команды");
        if (ftStatus != FT_OK) return;

        // 5. get ack (CLB_B_ACK = 0)

        CycleBreak = false;
        read_OK = false;
        //TryToRead = 0;
           
        do
        {
                read_OK = ReadData(rxModeFrame, 5);
                Application->ProcessMessages();
        }
        while (CycleBreak==false && read_OK==false);
                                           
        // test head & ident of frame
        if ((rxModeFrame[0] != FrHead)||((rxModeFrame[1]) != FrIdn21))
               {
                Panel13->Font->Color = clRed;
                Panel13->Caption = "Ошибка структуры кадра";
                return;
               }

        // test of calibr cancel ack
        if ((rxModeFrame[2]&CLB_B_ACK) != EMPTY_BT)
               {
                Panel13->Font->Color = clRed;
                Panel13->Caption = "Ошибка подтверждения отмены калибровки";
                return;
               }


        // 6. display messages

        Panel13->Font->Color = clBlue;
        Panel13->Caption = "OK";

        Application->MessageBoxA("Калибровка по темновому сигналу произведена", "", MB_OK);

}

// freq axis precise calibration on 1300 MHz --------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{

        // max neighborhood distrib center
        maxA=0;
             for(int i=0; i<=200; i++)
                {
                   //if (AmpDist[q-i]>=maxA)
                   if (AmpDist[i]>=maxA)
                   {
                        //maxA = AmpDist[q-i];
                        maxA = AmpDist[i];
                        nmax = i;
                   }
                }


                // middle PhD strictly
                

                int i = nmax;
                while (AmpDist[i]>=0.5*maxA) i++;
                nr05 = (i-1)+((float)(AmpDist[i-1]-0.5*AmpDist[nmax])/
                                                        (AmpDist[i-1]-AmpDist[i]));
                //Panel12->Caption = FormatFloat("###0.0",nr05);

                i = nmax;
                while (AmpDist[i]>=0.5*maxA) i--;
                nl05 = (i+1)-((float)(AmpDist[i+1]-0.5*AmpDist[nmax])/
                                                        (AmpDist[i+1]-AmpDist[i]));
                //Panel9->Caption = FormatFloat("###0.0",nl05);

                center_shift = (nl05+nr05)/2;

                Thres2 = Thres;
                TrackBar1->Position = 0.5*maxA;                                                               


        LabeledEdit1->Text = FormatFloat("###0.0",center_shift);           
        
}

// emergent exit of read cycles -----------------------------------------------

void __fastcall TForm1::Button25Click(TObject *Sender)
{

        CycleBreak = true;
        
}

// form creation  ------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{


        Chart1->BottomAxis->Maximum = 1650;
        Chart1->BottomAxis->Minimum = 1050;
                                                
        //Series5->Clear();

        // cursors initials
        x_cursor1=1050;
        x_cursor2=1650;

        y_top=Chart1->LeftAxis->Maximum;

        Series5->AddXY(x_cursor1, 0, "");
        Series5->AddXY(x_cursor1, y_top, "");

        Series3->AddXY(x_cursor2, 0, "");
        Series3->AddXY(x_cursor2, y_top, "");


        // objects structure & behavior
        Chart1->Height = Form1->Height - dw1;
        Chart1->Width = Form1->Width - GroupBox2->Width - gap1;
        GroupBox2->Height = Form1->Height - dw2;
        Panel5->Left = 0.9905*(Chart1->Width) - Panel5->Width; 

        Ini = new TIniFile(ExtractFilePath(Application->ExeName)
                                                +"settings.ini");

        // read ini file  

        center_shift = Ini->ReadFloat("Configs","shift",511.0);
        diodes_range = Ini->ReadFloat("Configs","range",910.0);
        //Thres = Ini->ReadInteger("Configs","threshold",50);
        thres_on = Ini->ReadInteger("Configs","thres_on",1);

            
        TrackBar1->Position = Thres2;
        CheckBox4->Checked = thres_on;

        LabeledEdit1->Text = FormatFloat("###0.0",center_shift);
        LabeledEdit2->Text = FormatFloat("###0.0",diodes_range);


        Series1->Clear();
        Series3->Clear();
        Series5->Clear();


        Chart1->BottomAxis->Maximum = maxb;
        Chart1->BottomAxis->Minimum = minb;

        Series5->AddXY(x_cursor1, 0, "");
        Series5->AddXY(x_cursor1, y_top, "");

        Series3->AddXY(x_cursor2, 0, "");
        Series3->AddXY(x_cursor2, y_top, "");

        n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift; 
        n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;

        

        Panel10->Caption = " F1: "+FormatFloat("###0.0",x_cursor1);
        Panel11->Caption = " n1: "+FormatFloat("###0.0",n_cursor1);
        //Panel8->Caption = " A1: "+FormatFloat("###0.0",0);

        Panel3->Caption = " F2: "+FormatFloat("###0.0",x_cursor2);
        Panel6->Caption = " n2: "+FormatFloat("###0.0",n_cursor2);
        //Panel7->Caption = " A2: "+FormatFloat("###0.0",0);



        // open test file
        ifstream in2("test.000");  

        // get ampl distribution
                                           
        for(int i=0; i<buf_size; i++)
        {

         in2 >> AmpDist[i];
          
        }

        in2.close();


        // create freq array and draw the plot

        Series1->Clear();


        for(int i=0; i<(int)center_shift; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr1;
           Series1->AddXY(FreqMHz[i], AmpDist[i], "");
        }
        for(int i=(int)center_shift; i<buf_size; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr2;
           Series1->AddXY(FreqMHz[i], AmpDist[i], "");
        }


        // open USB AOP
        Button8Click(Sender);

}
  
// when Chart1 OnZoom not descending bottom edge below zero ------------------------------

void __fastcall TForm1::Chart1Zoom(TObject *Sender)
{

                                      
        if (Chart1->LeftAxis->Minimum < 0)
        {
            Chart1->LeftAxis->Minimum = 0;
        }
        
}

// when Chart1 OnScroll not descending bottom edge below zero -----------------------------

void __fastcall TForm1::Chart1Scroll(TObject *Sender)
{

        if (Chart1->LeftAxis->Minimum < 0)
        {
                Chart1->LeftAxis->Minimum = 0;
        }

}

// when mouse click Chart1 pass the focus to Edit1  ---------------------------------------

void __fastcall TForm1::Chart1Click(TObject *Sender)
{

        Edit3->SetFocus();
        
}

// converts coords when left mouse button down --------------------------------------------
            
void __fastcall TForm1::Chart1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

        
        //if (Shift.Contains(ssCtrl)&&!Shift.Contains(ssShift)) Chart1->AllowZoom = false;
        //else Chart1->AllowZoom = true;

        if (Shift.Contains(ssLeft)&&!Shift.Contains(ssCtrl))
        {

        if (Shift.Contains(ssShift)) Chart1->AllowZoom = false;
        else Chart1->AllowZoom = true;

        //Chart1->AllowZoom = false;

        // converts object coords in pix to coords of bottom axis
        x_cursor1 = Series5->XScreenToValue(X);
        Series5->Clear();
        Series5->AddXY(x_cursor1, 0, "");
        Series5->AddXY(x_cursor1, y_top, "");

                

        if (x_cursor1<GCS_freq)
        n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift;
        else n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;



        if (n_cursor1<0) n_cursor1 = 0;
        if (n_cursor1>1023) n_cursor1 = 1023;


        //a_cursor1 = (float)AmpDist[(int)n_cursor1]+((float)AmpDist[(int)n_cursor1+1]-
        //                (float)AmpDist[(int)n_cursor1])*(n_cursor1-(int)n_cursor1);


        Panel10->Caption = " F1: "+FormatFloat("###0.0",x_cursor1);
        Panel11->Caption = " n1: "+FormatFloat("###0.0",n_cursor1);
        //Panel8->Caption = " A1: "+FormatFloat("###0.0",a_cursor1);
        
        }

             
        if (Shift.Contains(ssLeft)&&Shift.Contains(ssCtrl))
        {

        Chart1->AllowZoom = false;

        // converts object coords in pix to coords of bottom axis
        x_cursor2 = Series3->XScreenToValue(X);
        Series3->Clear();
        Series3->AddXY(x_cursor2, 0, "");
        Series3->AddXY(x_cursor2, y_top, "");


        if (x_cursor2<GCS_freq)
        n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift; 
        else n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;
         
                         
        if (n_cursor2<0) n_cursor2 = 0;
        if (n_cursor2>1023) n_cursor2 = 1023;


        //a_cursor2 = (float)AmpDist[(int)n_cursor2]+((float)AmpDist[(int)n_cursor2+1]-
        //                (float)AmpDist[(int)n_cursor2])*(n_cursor2-(int)n_cursor2);

        Panel3->Caption = " F2: "+FormatFloat("###0.0",x_cursor2);
        Panel6->Caption = " n2: "+FormatFloat("###0.0",n_cursor2);
        //Panel7->Caption = " A2: "+FormatFloat("###0.0",a_cursor2);
        
        }


}

// converts coords also when mouse moves (not only when it's down) ---------------------------------------------------------------------------

void __fastcall TForm1::Chart1MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{

        //if (Shift.Contains(ssCtrl)&&!Shift.Contains(ssShift)) Chart1->AllowZoom = false;
        //else Chart1->AllowZoom = true;

        if (Shift.Contains(ssLeft)&&Shift.Contains(ssCtrl))
        {

        //Chart1->AllowZoom = false;

        // converts object coords in pix to coords of bottom axis
        x_cursor2 = Series3->XScreenToValue(X);
        Series3->Clear();
        Series3->AddXY(x_cursor2, 0, "");
        Series3->AddXY(x_cursor2, y_top, "");

                    

        if (x_cursor2<GCS_freq)
        n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift;
        else n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;


        if (n_cursor2<0) n_cursor2 = 0;
        if (n_cursor2>1023) n_cursor2 = 1023;


        //a_cursor2 = (float)AmpDist[(int)n_cursor2]+((float)AmpDist[(int)n_cursor2+1]-
        //                (float)AmpDist[(int)n_cursor2])*(n_cursor2-(int)n_cursor2);

        Panel3->Caption = " F2: "+FormatFloat("###0.0",x_cursor2);
        Panel6->Caption = " n2: "+FormatFloat("###0.0",n_cursor2);
        //Panel7->Caption = " A2: "+FormatFloat("###0.0",a_cursor2);

        }


        //if (Shift.Contains(ssShift)&&!Shift.Contains(ssCtrl)) Chart1->AllowZoom = false;
        //else Chart1->AllowZoom = true;

        if (Shift.Contains(ssLeft)&&Shift.Contains(ssShift))
        {

        //Chart1->AllowZoom = false;
        
        // converts object coords in pix to coords of bottom axis
        x_cursor1 = Series5->XScreenToValue(X);
        Series5->Clear();
        Series5->AddXY(x_cursor1, 0, "");
        Series5->AddXY(x_cursor1, y_top, "");

       
        if (x_cursor1<GCS_freq)
        n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift;
        else n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;


        if (n_cursor1<0) n_cursor1 = 0;
        if (n_cursor1>1023) n_cursor1 = 1023;


        //a_cursor1 = (float)AmpDist[(int)n_cursor1]+((float)AmpDist[(int)n_cursor1+1]-
        //                (float)AmpDist[(int)n_cursor1])*(n_cursor1-(int)n_cursor1);

        Panel10->Caption = " F1: "+FormatFloat("###0.0",x_cursor1);
        Panel11->Caption = " n1: "+FormatFloat("###0.0",n_cursor1);
        //Panel8->Caption = " A1: "+FormatFloat("###0.0",a_cursor1);

        }

}

// when form resizing ---------------------------------------------------------------------------
 
void __fastcall TForm1::FormResize(TObject *Sender)
{

        Chart1->Height = Form1->Height - dw1;
        Chart1->Width = Form1->Width - GroupBox2->Width - gap1;

        GroupBox2->Height = Form1->Height - dw2;

        Panel5->Left = 0.9905*(Chart1->Width) - Panel5->Width;
        Panel2->Left = 0.9905*(Chart1->Width) - Panel2->Width;

        Panel9->Height = GroupBox2->Height - 600;

}

// turn on / off the threshold level ---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox4Click(TObject *Sender)
{

        if (CheckBox4->Checked) Series2->Active = true;
        else Series2->Active = false;
        thres_on = CheckBox4->Checked; 
        Ini->WriteInteger("Configs","thres_on",thres_on);

}

// force LabeledEdit1 taking only digits ---------------------------------------------------------------------------
    
void __fastcall TForm1::LabeledEdit1KeyPress(TObject *Sender, char &Key)
{

        Set <char, '0', '9'> dig;
        dig << '0' << '1' << '2' << '3' << '4' << '5' << '6' << '7' << '8' << '9';
        if (!dig.Contains(Key)) Key = 0;
                
}

// force LabeledEdit2 taking only digits ---------------------------------------------------------------------------

void __fastcall TForm1::LabeledEdit2KeyPress(TObject *Sender, char &Key)
{

        Set <char, '0', '9'> dig;
        dig << '0' << '1' << '2' << '3' << '4' << '5' << '6' << '7' << '8' << '9';
        if (!dig.Contains(Key)) Key = 0;

}

// when center shift changing ---------------------------------------------------------------------------

void __fastcall TForm1::LabeledEdit1Change(TObject *Sender)
{

        center_shift = StrToFloat(LabeledEdit1->Text);
        if (center_shift>=900)  center_shift=900;
        if (center_shift<=100 || LabeledEdit1->Text == "")  center_shift=100;
        //LabeledEdit1->Text = FormatFloat("###0.0",center_shift);
        Ini->WriteFloat("Configs","shift",StrToFloat(FormatFloat("###0.0",center_shift)));
        


        Series1->Clear();


        for(int i=0; i<(int)center_shift; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr1;
           Series1->AddXY(FreqMHz[i], AmpDist[i], ""); 
        }
        for(int i=(int)center_shift; i<buf_size; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr2;
           Series1->AddXY(FreqMHz[i], AmpDist[i], "");
        }


        if (x_cursor1<GCS_freq)
        n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift;
        else n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;


        if (n_cursor1<0) n_cursor1 = 0;
        if (n_cursor1>1023) n_cursor1 = 1023;  


        //a_cursor1 = (float)AmpDist[(int)n_cursor1]+((float)AmpDist[(int)n_cursor1+1]-
        //                (float)AmpDist[(int)n_cursor1])*(n_cursor1-(int)n_cursor1);

        Panel10->Caption = " F1: "+FormatFloat("###0.0",x_cursor1);
        Panel11->Caption = " n1: "+FormatFloat("###0.0",n_cursor1);
        //Panel8->Caption = " A1: "+FormatFloat("###0.0",a_cursor1);

       
        if (x_cursor2<GCS_freq)
        n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift;
        else n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;


        if (n_cursor2<0) n_cursor2 = 0;
        if (n_cursor2>1023) n_cursor2 = 1023;


        //a_cursor2 = (float)AmpDist[(int)n_cursor2]+((float)AmpDist[(int)n_cursor2+1]-
        //                (float)AmpDist[(int)n_cursor2])*(n_cursor2-(int)n_cursor2);

        Panel3->Caption = " F2: "+FormatFloat("###0.0",x_cursor2);
        Panel6->Caption = " n2: "+FormatFloat("###0.0",n_cursor2);
        //Panel7->Caption = " A2: "+FormatFloat("###0.0",a_cursor2);

}

// when pix range changing ---------------------------------------------------------------------------

void __fastcall TForm1::LabeledEdit2Change(TObject *Sender)
{

        diodes_range = StrToFloat(LabeledEdit2->Text);
        if (diodes_range==0 || LabeledEdit1->Text == "") diodes_range=1;
        Ini->WriteFloat("Configs","range",StrToFloat(FormatFloat("###0.0",diodes_range)));
        

        Series1->Clear();


        for(int i=0; i<(int)center_shift; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr1;
           Series1->AddXY(FreqMHz[i], AmpDist[i], ""); 
        }
        for(int i=(int)center_shift; i<buf_size; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr2;
           Series1->AddXY(FreqMHz[i], AmpDist[i], "");
        }

        
       
        if (x_cursor1<GCS_freq)
        n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift;
        else n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;



        if (n_cursor1<0) n_cursor1 = 0;
        if (n_cursor1>1023) n_cursor1 = 1023;  
        

        //a_cursor1 = (float)AmpDist[(int)n_cursor1]+((float)AmpDist[(int)n_cursor1+1]-
        //                (float)AmpDist[(int)n_cursor1])*(n_cursor1-(int)n_cursor1);

        Panel10->Caption = " F1: "+FormatFloat("###0.0",x_cursor1);
        Panel11->Caption = " n1: "+FormatFloat("###0.0",n_cursor1);
        //Panel8->Caption = " A1: "+FormatFloat("###0.0",a_cursor1);



        if (x_cursor2<GCS_freq)
        n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift;
        else n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;


        if (n_cursor2<0) n_cursor2 = 0;
        if (n_cursor2>1023) n_cursor2 = 1023;
        

        //a_cursor2 = (float)AmpDist[(int)n_cursor2]+((float)AmpDist[(int)n_cursor2+1]-
        //                (float)AmpDist[(int)n_cursor2])*(n_cursor2-(int)n_cursor2);
                        
        Panel3->Caption = " F2: "+FormatFloat("###0.0",x_cursor2);
        Panel6->Caption = " n2: "+FormatFloat("###0.0",n_cursor2);
        //Panel7->Caption = " A2: "+FormatFloat("###0.0",a_cursor2);
                
}

// linear vertical axis scale ---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton1Click(TObject *Sender)
{

        Chart1->LeftAxis->Logarithmic = false;

        //RadioButton1->Enabled = false;
        //RadioButton2->Enabled = true;

}

// logarithmic vertical axis scale ---------------------------------------------------------------------------

void __fastcall TForm1::RadioButton2Click(TObject *Sender)
{
              
        Chart1->LeftAxis->Logarithmic = true;

        //RadioButton2->Enabled = false;
        //RadioButton1->Enabled = true;
        
}

// start ---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{


        if(ftStatus != FT_OK)
        {
        Application->MessageBoxA("Инициализируйте АОАС", "Ошибка", MB_OK);
        return;
        }

        //TrackBar1->Position = Thres2;

        // FT & PCB purging

        //AO_Purge();

        FifoPurge("Ошибка очистки буфера");
        if (ftStatus != FT_OK) return;


        if (CheckBox5->Checked)
        {

        CycleBreak3 = false;
        
        Button2->Enabled = false;
        Button6->Enabled = false;
        GroupBox4->Enabled = false;
        GroupBox8->Enabled = false;
        GroupBox15->Enabled = false;
                               
        }

        else CycleBreak3 = true;


        do
        {

        // counter
        __int64 start, stop, f;
        QueryPerformanceCounter((_LARGE_INTEGER*)&start);
        QueryPerformanceFrequency((_LARGE_INTEGER*)&f);


        Application->ProcessMessages();


        // 2. set PCB into AmplDist reading mode        (AMPL_DIST=1)

        AMPL_DIST_Set = AMPL_DIST;                      // flag 0100 0000

        Frame_Set();         

        WriteUSB("Ошибка команды");
        if (ftStatus != FT_OK) return;

                 
        // 3. set PCB into measure & accumulate         (MODE=1)

        MODE_Set = MODE;                                // flag 0000 0001

        Frame_Set();

        WriteUSB("Ошибка команды");
        if (ftStatus != FT_OK) return;


        // 4. get ack mode                              (MODE_ACK = 1)

        CycleBreak = false;
        conf = false;

        do
        {
        DecodeData();
        if(MODE_ACK_v) conf = true;
        Application->ProcessMessages();
        }
        while (CycleBreak==false && conf==false);

        if (CycleBreak==true) {Button7Click(Sender); return;}

        // 5. get ack overflow                          (OVERFLOW = 1)
                                    
        CycleBreak = false;
        conf = false;

        do
        {
        DecodeData();
        if(OVERFLOW_v) conf = true;
        Application->ProcessMessages();
        }
        while (CycleBreak==false && conf==false);

        if (CycleBreak==true) {Button7Click(Sender); return;}


        // 6. remove read signal                        (AMPL_DIST=0)

        AMPL_DIST_Set = EMPTY_BT;                       // take off flag 0100 0000

        Frame_Set();

        WriteUSB("Ошибка команды");
        if (ftStatus != FT_OK) return;


        // 7. set PCB into data reading mode            (MODE=0)

        MODE_Set = EMPTY_BT;                            // take off flag 0000 0001

        Frame_Set();

        WriteUSB("Ошибка команды");
        if (ftStatus != FT_OK) return;


        // 8. get ack                                   (MODE_ACK = 0)

        CycleBreak = false;
        conf = false;

        do
        {
                DecodeData();
                if(!MODE_ACK_v) conf = true;
                Application->ProcessMessages();
        }
        while (CycleBreak==false && conf==false);

        if (CycleBreak==true) {Button7Click(Sender); return;}


        // 9. send command to read of FIFO              (DATA_RQ=1)

        DATA_RQ_Set = DATA_RQ;                          // flag 0000 0010

        Frame_Set();     

        WriteUSB("Ошибка команды");
        if (ftStatus != FT_OK) return;


        // 10. reading while DATA_EXIST != 0


        CycleBreak = false;
        int i = 0;

        do
        {
                DecodeData();
                if(SlujKadr && DATA_EXIST_v) continue;
                if(!DATA_EXIST_v) break;
                //Application->ProcessMessages();
                //AmpDist[1023-(2*i+1)] = ((int)rxAmplSFr[0]+((int)rxAmplSFr[1]&0x0F)*256);
                //AmpDist[1023-2*i] = ((int)rxAmplSFr[2]+((int)rxAmplSFr[3]&0x0F)*256);
                AmpDist[2*i+1] = ((int)rxAmplSFr[0]+((int)rxAmplSFr[1]&0x0F)*256);
                AmpDist[2*i] = ((int)rxAmplSFr[2]+((int)rxAmplSFr[3]&0x0F)*256);
                i++;
                //if(i >= 512) break;
        }
        while (CycleBreak==false);

        if (CycleBreak==true) {Button7Click(Sender); return;}


        // 11. set DATA_RQ = 0 (stop reading)

        DATA_RQ_Set = EMPTY_BT;                  // remove flag 0000 0010

        Frame_Set();            

        WriteUSB("Ошибка команды");
        if (ftStatus != FT_OK) return;


        //AmpDist[32] = 0;
        //AmpDist[33] = 0;

        AmpDist[990] = 0;
        AmpDist[991] = 0;


        // create freq array and draw the plot

                  
        Series1->Clear();


        for(int i=0; i<(int)center_shift; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr1;
           Series1->AddXY(FreqMHz[i], AmpDist[i], "");
        }
        for(int i=(int)center_shift; i<buf_size; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr2;
           Series1->AddXY(FreqMHz[i], AmpDist[i], "");
        }

             
                   
        // change ampl by cursors
        /*
        a_cursor1 = (float)AmpDist[(int)n_cursor1]+((float)AmpDist[(int)n_cursor1+1]-
                        (float)AmpDist[(int)n_cursor1])*(n_cursor1-(int)n_cursor1);

        Panel8->Caption = " A1: "+FormatFloat("###0.0",a_cursor1);

        
        a_cursor2 = (float)AmpDist[(int)n_cursor2]+((float)AmpDist[(int)n_cursor2+1]-
                        (float)AmpDist[(int)n_cursor2])*(n_cursor2-(int)n_cursor2);
                        
        Panel7->Caption = " A2: "+FormatFloat("###0.0",a_cursor2);

                                                                     */

        if (CheckBox2->Checked)
        {
                maxA=0;

                for(int i=0; i<buf_size; i++)

                if (AmpDist[i]>=maxA)  maxA = AmpDist[i];

                Chart1->LeftAxis->Maximum = maxA;

        }

                 

        if (CheckBox1->Checked)
        {

             // max neighborhood distrib center
             maxA=0;

             for(int i=0; i<buf_size; i++)
                {

                   if (AmpDist[i]>=maxA)
                   {    
                        maxA = AmpDist[i];
                        nmax = i;
                   }
                }



                if (maxA>Thres)
                {

                numb_signals ++;
                Panel7->Caption = numb_signals;


                if (CheckBox3->Checked)
                {

                // open a file
                ofstream out1((LabeledEdit5->Text + "\\" + IntToStr(numb_signals)).c_str());

                // write file with 1024 values
                for(int i=0; i<buf_size; i++)
                        {
                                out1 << AmpDist[i] << endl;
                        }
                out1.close();

                }


                // middle PhD strictly
                      
                int i = nmax;
                while (AmpDist[i]>=0.5*maxA) i++;
                nr05 = (i-1)+((float)(AmpDist[i-1]-0.5*AmpDist[nmax])/
                                                        (AmpDist[i-1]-AmpDist[i]));

                i = nmax;
                while (AmpDist[i]>=0.5*maxA) i--;
                nl05 = (i+1)-((float)(AmpDist[i+1]-0.5*AmpDist[nmax])/
                                                        (AmpDist[i+1]-AmpDist[i]));
                float middlePhD = (nl05+nr05)/2;

                if(middlePhD < center_shift)
                Fmax = GCS_freq + (middlePhD-center_shift)*(freq_range/diodes_range)*corr1;
                else Fmax = GCS_freq + (middlePhD-center_shift)*(freq_range/diodes_range)*corr2;



                LabeledEdit4->Text = maxA;
                LabeledEdit3->Text = FormatFloat("###0.0",Fmax);


                //Series4->Active = true;
                //Series4->Clear();
                //Series4->AddXY(Fmax, 0, "");
                //Series4->AddXY(Fmax, 4000, "");

                }

        }

        else
        {

        numb_signals = 0;
        Panel7->Caption = numb_signals;
        //LabeledEdit3->Text = "";
        //LabeledEdit4->Text = "";
        Series4->Clear();
        //Series4->Active = false;

        }




        QueryPerformanceCounter((_LARGE_INTEGER*)&stop);
        // time in ms
        double time = 1000*(((double)(stop-start))/(double)f);
        Panel2->Caption = FormatFloat("###0.0",time);


        }
        while (!CycleBreak3);


        Panel13->Font->Color = clBlue;
        Panel13->Caption = "OK";     
        
}

// stop ---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{

        CycleBreak3 = true;
                                       

        //TrackBar1->Position = Thres2;
             
        Button2->Enabled = true;
        Button6->Enabled = true;
        GroupBox4->Enabled = true;
        GroupBox8->Enabled = true;
        GroupBox15->Enabled = true;

        
        Panel13->Color = clBtnFace;
        Panel13->Font->Color = clBlue;
        Panel13->Caption = "OK";

        //AO_Purge(); 
        
}

// open test distrib ---------------------------------------------------------------------------

void __fastcall TForm1::Button11Click(TObject *Sender)
{


        if (LabeledEdit5->Text=="")
        {
                Panel7->Caption="out";
                return;
        }

        if (!DirectoryExists(LabeledEdit5->Text))
        {
                if (!CreateDir(LabeledEdit5->Text))
                Application->MessageBoxA("Не возможно создать папку", "", MB_OK);
                Panel7->Caption="";
        }


}

// open any distrib ---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{

        if(OpenDialog1->Execute())
        {
                char* FileIn=(OpenDialog1->FileName).c_str();
        
                // открываем файл
                ifstream in2(FileIn);


                // считываем из файла 1024 значения амплитуд
                                           
                for(int i=0; i<buf_size; i++)
                {
                        in2 >> AmpDist[i];
                }

                in2.close();

                Series1->Clear();


                      

        for(int i=0; i<(int)center_shift; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr1;
           Series1->AddXY(FreqMHz[i], AmpDist[i], "");
        }
        for(int i=(int)center_shift; i<buf_size; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr2;
           Series1->AddXY(FreqMHz[i], AmpDist[i], "");
        }


        }
                 
        TrackBar1->Position = Thres2;
        
}    

// calibrate diodes_range on 1600 MHz ---------------------------------------------------------------------------

void __fastcall TForm1::Button14Click(TObject *Sender)
{

        // max neighborhood distrib center
        maxA=0;
             for(int i=800; i<=1000; i++)
                {
                   //if (AmpDist[q-i]>=maxA)
                   if (AmpDist[i]>=maxA)
                   {
                        //maxA = AmpDist[q-i];
                        maxA = AmpDist[i];
                        nmax = i;
                   }
                }


                //Panel14->Caption = maxA;

                // middle PhD strictly
                

                int i = nmax;
                while (AmpDist[i]>=0.5*maxA) i++;
                nr05 = (i-1)+((float)(AmpDist[i-1]-0.5*AmpDist[nmax])/
                                                        (AmpDist[i-1]-AmpDist[i]));
                //Panel12->Caption = FormatFloat("###0.0",nr05);

                i = nmax;
                while (AmpDist[i]>=0.5*maxA) i--;
                nl05 = (i+1)-((float)(AmpDist[i+1]-0.5*AmpDist[nmax])/
                                                        (AmpDist[i+1]-AmpDist[i]));
                //Panel9->Caption = FormatFloat("###0.0",nl05);

                center_shift2 = (nl05+nr05)/2;

                diodes_range = (center_shift2 - center_shift)*freq_range/freq_range_calibr;

                Thres2 = Thres;
                TrackBar1->Position = 0.5*maxA;


        LabeledEdit2->Text = FormatFloat("###0.0",diodes_range);
        
}

// moves cursor2 +/-0.5 by left/right keys and +/-10 when Ctrl pressed -----------------------

void __fastcall TForm1::Edit3KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        if (Key == 37)
        {
                if (Shift.Contains(ssCtrl))  x_cursor2-=5;
                if (Shift.Contains(ssShift)&&!Shift.Contains(ssCtrl)) x_cursor2-=0.1;
                Series3->Clear();
                Series3->AddXY(x_cursor2, 0, "");
                Series3->AddXY(x_cursor2, y_top, "");

                if (!Shift.Contains(ssShift)&&!Shift.Contains(ssCtrl))  x_cursor1-=0.1;
                Series5->Clear();
                Series5->AddXY(x_cursor1, 0, "");
                Series5->AddXY(x_cursor1, y_top, "");

        }

        if (Key == 39)
        {
                if (Shift.Contains(ssCtrl))  x_cursor2+=5;
                if (Shift.Contains(ssShift)&&!Shift.Contains(ssCtrl)) x_cursor2+=0.1;
                Series3->Clear();
                Series3->AddXY(x_cursor2, 0, "");
                Series3->AddXY(x_cursor2, y_top, "");

                if (!Shift.Contains(ssShift)&&!Shift.Contains(ssCtrl))  x_cursor1+=0.1;
                Series5->Clear();
                Series5->AddXY(x_cursor1, 0, "");
                Series5->AddXY(x_cursor1, y_top, "");

        }

                                                                                      
        if (x_cursor2<GCS_freq)
        n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift;
        else n_cursor2 = (x_cursor2-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;


        if (n_cursor2<0) n_cursor2 = 0;
        if (n_cursor2>1023) n_cursor2 = 1023;


        //a_cursor2 = (float)AmpDist[(int)n_cursor2]+((float)AmpDist[(int)n_cursor2+1]-
        //                (float)AmpDist[(int)n_cursor2])*(n_cursor2-(int)n_cursor2);

        Panel3->Caption = " F2: "+FormatFloat("###0.0",x_cursor2);
        Panel6->Caption = " n2: "+FormatFloat("###0.0",n_cursor2);
        //Panel7->Caption = " A2: "+FormatFloat("###0.0",a_cursor2);



        if (x_cursor1<GCS_freq)
        n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr1/diodes_range) + center_shift;
        else n_cursor1 = (x_cursor1-GCS_freq)/(freq_range*corr2/diodes_range) + center_shift;


        if (n_cursor1<0) n_cursor1 = 0;
        if (n_cursor1>1023) n_cursor1 = 1023;
        

        //a_cursor1 = (float)AmpDist[(int)n_cursor1]+((float)AmpDist[(int)n_cursor1+1]-
        //                (float)AmpDist[(int)n_cursor1])*(n_cursor1-(int)n_cursor1);

        Panel10->Caption = " F1: "+FormatFloat("###0.0",x_cursor1);
        Panel11->Caption = " n1: "+FormatFloat("###0.0",n_cursor1);
        //Panel8->Caption = " A1: "+FormatFloat("###0.0",a_cursor1);
}

// autoscale ---------------------------------------------------------------------------
     
void __fastcall TForm1::CheckBox2Click(TObject *Sender)
{
        if (CheckBox2->Checked) Chart1->LeftAxis->Maximum = maxA;
        else Chart1->LeftAxis->Maximum = 4000;
}

//---------------------------------------------------------------------------
         
void __fastcall TForm1::LabeledEdit5KeyPress(TObject *Sender, char &Key)
{
        //Set <char, '0', '9'> dig;
        //dig << '0' << '1' << '2' << '3' << '4' << '5' << '6' << '7' << '8' << '9';
        //if (!dig.Contains(Key)) Key = 0;
}

//---------------------------------------------------------------------------
        
void __fastcall TForm1::ScrollBar1Change(TObject *Sender)
{


        // counter
        __int64 start, stop, f;
        QueryPerformanceCounter((_LARGE_INTEGER*)&start);
        QueryPerformanceFrequency((_LARGE_INTEGER*)&f);

        //Panel13->Font->Color = clBlue;
        //Panel13->Caption = ScrollBar1->Position;
        //Panel7->Caption = ScrollBar1->Position;
        Edit1->Text = ScrollBar1->Position;

        ScrollBar1->Max = ScrollBar1->Position + 50;
        if (ScrollBar1->Position<50) ScrollBar1->Min = 0;
        else ScrollBar1->Min = ScrollBar1->Position - 50;


        // opening file
        ifstream in2((LabeledEdit5->Text + "\\" + IntToStr(ScrollBar1->Position)).c_str());

        // считываем из файла 1024 значения амплитуд
                                           
        for(int i=0; i<buf_size; i++)
                {
                        in2 >> AmpDist[i];
                }

        //closing file
        in2.close();


        //drawing AmpDist
        Series1->Clear();

        for(int i=0; i<(int)center_shift; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr1;
           Series1->AddXY(FreqMHz[i], AmpDist[i], "");
        }
        for(int i=(int)center_shift; i<buf_size; i++)
        {
           FreqMHz[i] = GCS_freq + (i-center_shift)*(freq_range/diodes_range)*corr2;
           Series1->AddXY(FreqMHz[i], AmpDist[i], "");
        }



        if (CheckBox1->Checked)
        {

             // max neighborhood distrib center
             maxA=0;

             for(int i=0; i<buf_size; i++)
                {

                   if (AmpDist[i]>=maxA)
                   {    
                        maxA = AmpDist[i];
                        nmax = i;
                   }
                }



                if (maxA>Thres)
                {


                // middle PhD strictly
                      
                int i = nmax;
                while (AmpDist[i]>=0.5*maxA) i++;
                nr05 = (i-1)+((float)(AmpDist[i-1]-0.5*AmpDist[nmax])/
                                                        (AmpDist[i-1]-AmpDist[i]));

                i = nmax;
                while (AmpDist[i]>=0.5*maxA) i--;
                nl05 = (i+1)-((float)(AmpDist[i+1]-0.5*AmpDist[nmax])/
                                                        (AmpDist[i+1]-AmpDist[i]));
                float middlePhD = (nl05+nr05)/2;

                if(middlePhD < center_shift)
                Fmax = GCS_freq + (middlePhD-center_shift)*(freq_range/diodes_range)*corr1;
                else Fmax = GCS_freq + (middlePhD-center_shift)*(freq_range/diodes_range)*corr2;
                            

                LabeledEdit4->Text = maxA;
                LabeledEdit3->Text = FormatFloat("###0.0",Fmax);


                //Series4->Active = true;
                Series4->Clear();        
                Series4->AddXY(Fmax, 0, "");
                Series4->AddXY(Fmax, 4000, "");

                }

        }


        else
        {

                LabeledEdit3->Text = "";
                LabeledEdit4->Text = "";
                Series4->Clear();
                
        }




         QueryPerformanceCounter((_LARGE_INTEGER*)&stop);
        // time in ms
        double time = 1000*(((double)(stop-start))/(double)f);
        Panel2->Caption = FormatFloat("###0.0",time);


}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
         /*  
        if (StrToInt(Edit1->Text)>ScrollBar1->Max)
                {
                ScrollBar1->Max = StrToInt(Edit1->Text) + 50;
                ScrollBar1->Min = StrToInt(Edit1->Text);
                ScrollBar1->Position = StrToInt(Edit1->Text);
                return;
                }
        if (StrToInt(Edit1->Text)<ScrollBar1->Min)
                {
                ScrollBar1->Min = StrToInt(Edit1->Text);
                ScrollBar1->Max = StrToInt(Edit1->Text) + 50;
                ScrollBar1->Position = StrToInt(Edit1->Text);
                return;
                }
        ScrollBar1->Position = StrToInt(Edit1->Text);
                                                      */

}

//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1KeyPress(TObject *Sender, char &Key)
{
        Set <char, '0', '9'> dig;
        dig << '0' << '1' << '2' << '3' << '4' << '5' << '6' << '7' << '8' << '9';
        if (!dig.Contains(Key) && Key != 13) Key = 0;
        if (Key == 13)
        {
                if (StrToInt(Edit1->Text)>ScrollBar1->Max)
                {
                ScrollBar1->Max = StrToInt(Edit1->Text) + 50;
                ScrollBar1->Min = StrToInt(Edit1->Text);
                ScrollBar1->Position = StrToInt(Edit1->Text);
                return;
                }
        if (StrToInt(Edit1->Text)<ScrollBar1->Min)
                {
                ScrollBar1->Min = StrToInt(Edit1->Text);
                ScrollBar1->Max = StrToInt(Edit1->Text) + 50;
                ScrollBar1->Position = StrToInt(Edit1->Text);
                return;
                }
        ScrollBar1->Position = StrToInt(Edit1->Text);
        }
         
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Edit2KeyPress(TObject *Sender, char &Key)
{
        Set <char, '0', '9'> dig;
        dig << '0' << '1' << '2' << '3' << '4' << '5' << '6' << '7' << '8' << '9';
        if (!dig.Contains(Key) && Key != 13) Key = 0;
        if (Key == 13) TrackBar1->Position = StrToInt(Edit2->Text);

}
//---------------------------------------------------------------------------



