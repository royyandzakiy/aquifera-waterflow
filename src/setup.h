void initFlowSensor();
void initEeprom();
void WaktuRTC();

void setup()
{
  //delay(7000); //delay  detik untuk memastikan modul sim sudah siap 
  Serial.begin(9600);
  EspSerial.begin(115200);
  EspSerial.println("Setup initialize...");

  //MODUL SIM
  _buffer.reserve(50);
  Serial.println("Sistem Started...");
  sim.begin(9600);
  //ReceiveMode(); //masih percobaan nanti saja di V.3
  
  //MODUL RTC
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  //WaktuRTC(); //Perbaiki waktu

  //SENOR DEBIT
  initFlowSensor();
  //initEeprom(); //membuat semua isi eeprom bernilai 0
  delay(1000);

  //MODUL MIKROSD
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
 
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  EspSerial.println("initialization done.");
}

void initFlowSensor() 
{
  pinMode(FLOWSENSOR_PIN, INPUT);
  digitalWrite(FLOWSENSOR_PIN, HIGH);
  attachInterrupt(0,InterruptDebitAir,RISING); // di pin 2
  sei();
}

void initEeprom()
{
  for (int i=0; i<25; i++) //Eeprom dari 0 - 24 akan dikembalikan menjadi 0 
  {
    EEPROM16_Write(i, 0);
  }
}

void WaktuRTC()
{
  t.hour=12; 
  t.min=30;
  t.sec=0;
  t.mday=25;
  t.mon=12;
  t.year=2020;
  
  DS3231_set(t);
}
