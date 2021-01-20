//Volume air akan dihitung setiap 5 detik dan dijumlahkan secara langsung
//Tiap jam akan disimpan data berupa jumlah volume air pada jam tersebut sehingga
//dapat diketahui pengeluaran terbesar pada jam-jam berapa saja

#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <DS3231.h>
SoftwareSerial sim(10, 9);
#include <SD.h>
#include <SPI.h>
#include "credentials.h"
//======================================================================//

//MODUL SIM
int     _timeout;
int     Tsimpan1, Tsimpan2, Tsimpan3; //Hanya penyimpanan waktu sementara
String  _buffer;
String  number = dest_phone_no;
bool    DATASMS=1;
bool    DATASMS2=1;
bool    DATA1=1;
bool    DATA2=1;
int     SMSJam = 0;  //SMS dikirimkan tiap jam dan menit berapa?
int     SMSMenit = 1;
//char Received_SMS;
//short INFOVOL_OK=-1, INFOLAIN_OK=-1;

//SENSOR FLOW METER
const long WaktuDebitAir = 5; //pengambilan data setiap 5 detik
int K = 2.6; // Konstanta flow sensor
int VtiapJam[25];
float DebitAir;
float VolumeAir_10detik; //nama 10 detik bisa saja diganti tapi hoream. yg penting waktu debitnya jelas brp detik
float VolumeAir_1jam;
float VolumeAirSebelum;
float VolumeAir;
volatile long frekuensi_aliran;
#define FLOWSENSOR_PIN 2

//RTC
struct ts t;

//MICROSD CARD
File myFile;
char filename[13]; //untuk data per satu hari
char filename2[13]; //untuk data per satu bulan
void getFilename(int,int,int);
void getFilename2(int,int);
