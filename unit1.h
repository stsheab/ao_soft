//---------------------------------------------------------------------------

#ifndef unit1H
#define unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Dialogs.hpp>
#include <IniFiles.hpp>

#include "ftd2xx.h"  
#include <Grids.hpp>
#include "CSPIN.h"
#include <DdeMan.hpp>
#include <AppEvnts.hpp>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel4;
        TButton *Button8;
        TButton *Button9;
        TButton *Button10;
        TGroupBox *GroupBox8;
        TPanel *Panel13;
        TButton *Button16;
        TButton *Button25;
        TGroupBox *GroupBox15;
        TButton *Button2;
        TButton *Button5;
        TGroupBox *GroupBox5;
        TGroupBox *GroupBox2;
        TSaveDialog *SaveDialog1;
        TButton *Button3;
        TEdit *Edit2;
        TTrackBar *TrackBar1;
        TCheckBox *CheckBox4;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TGroupBox *GroupBox1;
        TCheckBox *CheckBox5;
        TButton *Button6;
        TButton *Button7;
        TButton *Button11;
        TOpenDialog *OpenDialog1;
        TLabeledEdit *LabeledEdit2;
        TLabeledEdit *LabeledEdit1;
        TChart *Chart1;
        TPanel *Panel5;
        TPanel *Panel10;
        TPanel *Panel11;
        TPanel *Panel3;
        TPanel *Panel6;
        TPanel *Panel2;
        TLineSeries *Series1;
        TLineSeries *Series2;
        TLineSeries *Series5;
        TLineSeries *Series3;
        TPanel *Panel1;
        TButton *Button1;
        TButton *Button14;
        TGroupBox *GroupBox3;
        TCheckBox *CheckBox1;
        TGroupBox *GroupBox4;
        TLabeledEdit *LabeledEdit3;
        TLabeledEdit *LabeledEdit4;
        TGroupBox *GroupBox6;
        TGroupBox *GroupBox7;
        TPanel *Panel9;
        TEdit *Edit3;
        TCheckBox *CheckBox2;
        TLineSeries *Series4;
        TPanel *Panel7;
        TLabeledEdit *LabeledEdit5;
        TScrollBar *ScrollBar1;
        TButton *Button4;
        TEdit *Edit1;
        TCheckBox *CheckBox3;

        // my functions //////////////////////////////////////////////////////////////

        // FT status control
        void __fastcall FTStatContr(AnsiString Ind);

        // FT both buffers purge
        void __fastcall FifoPurge(AnsiString Ind);

        // FT RX purge
        void __fastcall RX_Purge(AnsiString Ind);

        // PCB purge
        void __fastcall AO_Purge(void);

        // write frame set
        void __fastcall Frame_Set(void);

        // write to USB
        void __fastcall WriteUSB(AnsiString Ind);

        // delay
        void __fastcall DelayF(void);

        // data reading
        bool __fastcall ReadData(unsigned char *pData, DWORD ToRead);

        // data decoder
        void __fastcall DecodeData(void);

        //////////////////////////////////////////////////////////////////////////
        
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall Chart1Zoom(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button16Click(TObject *Sender);
        void __fastcall Button25Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Chart1Scroll(TObject *Sender);
        void __fastcall Chart1Click(TObject *Sender);
        void __fastcall Chart1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Chart1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall CheckBox4Click(TObject *Sender);
        void __fastcall LabeledEdit1KeyPress(TObject *Sender, char &Key);
        void __fastcall LabeledEdit2KeyPress(TObject *Sender, char &Key);
        void __fastcall LabeledEdit1Change(TObject *Sender);
        void __fastcall LabeledEdit2Change(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button11Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button14Click(TObject *Sender);
        void __fastcall Edit3KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall CheckBox2Click(TObject *Sender);
        void __fastcall LabeledEdit5KeyPress(TObject *Sender, char &Key);
        void __fastcall ScrollBar1Change(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
        void __fastcall Edit2KeyPress(TObject *Sender, char &Key);

        
        
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};

//---------------------------------------------------------------------------

extern PACKAGE TForm1 *Form1;

typedef unsigned char uchar;


TIniFile *Ini;                          // TIniFile object


// define vars and constants

const unsigned int buf_size = 1024;     // arrays size

const float GCS_freq = 1100;            // GCS freq (MHz)
const float freq_range = 600;           // freq band (MHz)
float freq_range_calibr = 500;          // freq range (MHz) between calibr. points

const int q = 1023;                     // (q+1)= amount of really working pix of CCD
      //  q = 1014


// chart axes edges

double axemax;
double axemin; 

unsigned int AmpDist[1050]={0};         // Ampl distribution
float FreqMHz[1100]={0.0};              // freq distrib (MHz)
unsigned int maxA;                      // Ampl distrib max
unsigned int nmax;                      // pix number where is max
float Fmax;                             // freq of max
float nl05, nr05;                       // left & right nums by level 0.5   
//const float corr1=1.008;                // corrections for doubleline approx.
const float corr1=1.0;
const float corr2=1.0;
//float deltaF = 0.5;                   // discrete (MHz)
float center_shift = 510.0;             // center shift (on GCS freq 1100 MHz)
float center_shift2 = 910.0;            // center shift (on 1600 MHz)
float diodes_range = 900.0;             // pix num in freq band

unsigned int numb_signals = 0;          // signals counter


const int minb = 1000;                  // left edge of freq axis
const int maxb = 1700;                  // right edge of freq axis


unsigned int Thres = 100;               // threshold level
unsigned int Thres2 = 100;              // double of thres
int thres_on = 0;                       // show on/off the thres line
    

// USB

AnsiString Indicator;                   // FT status displayed in Panel4

FT_HANDLE ftHandle;                     // FT handle
FT_STATUS ftStatus;                     // FT status
                     
bool CycleBreak2 = true;

uchar txModeFrame[5];                   // modes frame

const uchar FrHead = 0xC0;              // frame head
const uchar FrDB = 0xDB;                // DB byte
const uchar FrDC = 0xDC;                // DC byte
const uchar FrDD = 0xDD;                // DD byte

const uchar FrIdn21 = 0x21;             // frame identifiers
const uchar FrIdn22 = 0x22;

// modes frame flags
const uchar MODE = 0x01;
const uchar DATA_RQ = 0x02;
const uchar CLB_B = 0x04;
const uchar CLB_F = 0x08;
const uchar CLOSE_SHF = 0x10;
const uchar TURN_GKS = 0x20;
const uchar AMPL_DIST = 0x40;
const uchar AUTO_CLB = 0x80;
const uchar EMPTY_BT = 0x00;

// and corresponding vars
uchar MODE_Set = 0x00;
uchar DATA_RQ_Set = 0x00;
uchar CLB_B_Set = 0x00;
uchar CLB_F_Set = 0x00;
uchar CLOSE_SHF_Set = 0x00;
uchar TURN_GKS_Set = 0x00;
uchar AMPL_DIST_Set = 0x00;
uchar AUTO_CLB_Set = 0x00;

// read modes frame flags
const uchar MODE_ACK = 0x01;
const uchar CLB_B_ACK = 0x04;
const uchar CLB_F_ACK = 0x08;
const uchar DATA_EXIST = 0x10;
const uchar OVER_FLOW = 0x20;
const uchar AMPL_DIST_ACK = 0x40;

// and corresponding vars
uchar MODE_ACK_v = 0x00;
uchar CLB_B_ACK_v = 0x00;
uchar CLB_F_ACK_v = 0x00;
uchar DATA_EXIST_v = 0x00;
uchar OVERFLOW_v = 0x00;
uchar AMPL_DIST_ACK_v = 0x00;

bool SlujKadr;                          // if serve frame
bool conf;                              // confirmation

uchar Threshold;
uchar Filter;

DWORD BWritten;                         // bytes written

uchar buf[10] = {0};                    // service data
uchar rxModeFrame[5];                   // read modes frame
uchar rxInfSFr[3];                      // semiframe (without the head) of info frame
uchar rxAmplSFr[6];                     // semiframe of data frame

uchar AByte;                            // single byte reading
uchar OneByte;                          //

DWORD BReturned;                        // bytes received
DWORD BytesToRead1;                     // bytes to read
DWORD RxBytes;                          // bytes in RX buf
bool read_OK;

bool CycleBreak;                        // reading & decoder cycle out
bool CycleBreak3 = true;                // auto break

//int TryToRead;                        // read tries
//const int MaxToRead = 5;              // read tries limit

unsigned long SleepInterv = 50;         // Sleep() interval


// cursors
float x_cursor1;                        // cursor1 coords(MHz)
float n_cursor1;                        // in pixels
float x_cursor2;                        // cursor2 coords(MHz)
float n_cursor2;                        // in pixels
float y_top;                            // vertical limit of cursors lines
float a_cursor1;                        // amp in cursor1 location
float a_cursor2;                        // amp in cursor2 location



// application design
const int gap1 = 10;                    // gap between graph & control region
const int dw1 = 35;                     // difference between form & graph heights
const int dw2 = 30;                     // difference between form & control region heights 

//---------------------------------------------------------------------------
#endif




