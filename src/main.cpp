#include <Arduino.h>
#include "global.h"
#include "function.h"
#include "setup.h"

void loop() 
{
  DS3231_get(&t);
  bool Tiap10Detik = (t.sec + 1) % WaktuDebitAir == 0;
  if(Tiap10Detik==1 && DATA1==1) 
  {
    cli();
    HitungDebitAir();
    HitungVolumeAir();
    sei();
    Tsimpan1=t.sec + 1;
    getFilename(t.year,t.mon,t.mday);
    DATA1=0;
  }
  if(Tsimpan1 != t.sec + 1){DATA1=1;} //kasih jeda 2 detik
  
  //bool Tiap5min = (t.min+1)%5==0;
  //if (Tiap5min==1 && DATASMS==1)
  if(t.hour==SMSJam && t.min==SMSMenit && DATASMS==1) //KIRIM SMS TIAP HARI
  {
    DATASMS=0;
//    VolumeTerbesar();
    getFilename2(t.year,t.mon);
    SendMessage();
    VolumeAir=0;
    HapusEeprom();
    Tsimpan3=t.min+1;
    //ReceiveMode(); //masih percobaan
  }
  if(t.hour==SMSJam && t.min!=SMSMenit ){DATASMS=1;} //kasih jeda 1menit  
  //if(Tsimpan3 != t.min + 1){DATASMS=1;} //kasih jeda 2 detik
  
  Serial.print("Date : ");
  Serial.print(t.mday);
  Serial.print("/");
  Serial.print(t.mon);
  Serial.print("/");
  Serial.print(t.year); 
  Serial.print("\t Hour : ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(".");
  Serial.print(t.sec);
//  Serial.print("\t debit : ");
//  Serial.print(DebitAir);
  Serial.print("\t V10 : ");
  Serial.print(VolumeAir_10detik);
  Serial.print("\t V = ");
  Serial.println(VolumeAir); 

  if(Serial.available()) {
    String str = Serial.readStringUntil('\n');
    Serial.println(str);
    EspSerial.println(str);
    delay(10);
  }
  if(EspSerial.available()) {
    String str = EspSerial.readStringUntil('\n');
    Serial.println(str);
    delay(10);
  }
}


/* masih percobaan nanti saja
 *   String RSMS;
  while(sim.available()>0)
  {
   Received_SMS=sim.read();
   Serial.print(Received_SMS);
   RSMS.concat(Received_SMS);
   INFOVOL_OK=RSMS.indexOf("ASH");
   INFOLAIN_OK=RSMS.indexOf("DMH");
  }
  if(INFOVOL_OK!=-1)
  {
    Serial.println("ASANEMUH");
    SendMessage();
    ReceiveMode();
    INFOVOL_OK=-1;
    INFOLAIN_OK=-1;
  }
  //untuk pengiriman lain bebas we iyeumah optional
 */
