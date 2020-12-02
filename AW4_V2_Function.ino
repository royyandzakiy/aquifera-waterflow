/*****************************************************
 * DAFTAR FUNGSI
 * InterruptDebitAir() = interrupt dari pin 2
 * HitungDebitAir() = hitung debit tiap detik (L/s)
 * HitungVolumeAir() = hitung volume tiap 10 detik (mL) dan menyimpan data tiap jam dalam eeprom
 * SendMessage() = kirim sms hasil perhitungan volume
 * EEPROM16_Write(....) = menyimpan nilai dalam eeprom > kelemahan akan dibulatkan. tetapi selama arduino tidak mati hasil Volume terakhir tidak akan dibulatkan
 * HapusEeprom() = mengubah isi eeprom dengan nilai 0
 * getFilename(..) = membuat file baru tiap hari yang isinya data dari perhitungan volume tiap sekian detik
 * getFilename2(..) = membuat file data tiap bulan isinya data volume akhir tiap hari
 */

void InterruptDebitAir()
{
  frekuensi_aliran++;
}

void HitungDebitAir()
{
    DebitAir = (frekuensi_aliran/7.5)*K/60.0/WaktuDebitAir; //L/s
    frekuensi_aliran = 0;
}

void HitungVolumeAir()
{
  VolumeAir_10detik = DebitAir*(WaktuDebitAir); //L
  if(VolumeAir>EEPROM16_Read(0)) {VolumeAirSebelum=VolumeAir;}
  else {VolumeAirSebelum = EEPROM16_Read(0);}  
  VolumeAir = VolumeAirSebelum+VolumeAir_10detik;
  EEPROM16_Write(0, VolumeAir);

  bool Tiap1Jam = (t.hour+1) % 1 == 0; //Simpan data tiap jam
  if(Tiap1Jam==1 && DATA2==1) 
  {
    EEPROM16_Write(t.hour+1, VolumeAir-VolumeAirSebelum); //total
    Tsimpan2=t.hour + 1;
    DATA2=0;
  }
  if(Tsimpan2 != t.hour + 1){DATA2=1;} //kasih jeda 1 jam  
}

void SendMessage()
{ 
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Malam Aquifers, Kode[007], Volume: " + (String) round(VolumeAir) + " L";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  Serial.println(SMS);
  Serial.println("SMS DIKIRIMKAN");
}

void ReceiveMode(){       //Set the SIM800L Receive mode  
  sim.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  sim.println("AT+CMGF=1"); // Configuring TEXT mode
  sim.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
}

void EEPROM16_Write(uint8_t a, uint16_t b){
  EEPROM.write(a, lowByte(b));
  EEPROM.write(a + 1, highByte(b));
}
uint16_t EEPROM16_Read(uint8_t a){
  return word(EEPROM.read(a + 1), EEPROM.read(a));
}

void HapusEeprom() {
  for (int j=0; j<25; j++) //Eeprom dari 0 - 24 akan dikembalikan menjadi 0 
  {
    EEPROM16_Write(j, 0);
  }
}

void getFilename2(int year,int month)
{
      sprintf(filename2, "%04d%02d", year, month);  
      myFile = SD.open(filename2, FILE_WRITE);
      // if the file opened okay, write to it:
      if (myFile) {
        Serial.println(filename2);
        Serial.println("Hari ke-"+String(t.mday)+" | "+String(VolumeAir)+" | mL");
        myFile.println("Hari ke-"+String(t.mday)+" | "+String(VolumeAir)+" | mL");
        myFile.close();
        } 
      else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
      }
      myFile.close();
}

void getFilename(int year,int month, int day )
     {         
      sprintf(filename, "%04d%02d%02d", year, month, day);
        myFile = SD.open(filename, FILE_WRITE);
 
      // if the file opened okay, write to it:
      if (myFile) {
        Serial.println(filename);
        Serial.println(String(t.hour)+":"+String(t.min)+":"+String(t.sec)+" | "+String(VolumeAir)+" | mL");
        myFile.println(String(t.hour)+":"+String(t.min)+":"+String(t.sec)+" | "+String(VolumeAir)+" | mL");
        myFile.close();
        } 
      else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
      }
      myFile.close();
     }

void VolumeTerbesar()
{
  for (int k=1; k<25; k++) 
  { 
    EEPROM16_Write(0, 0);
    VtiapJam[k] = round((EEPROM16_Read(k)-EEPROM16_Read(k-1))); 
    Serial.print("Jam ke-");
    Serial.print(k);
    Serial.print(" : ");
    Serial.println(VtiapJam[k]);
  } 
}
