void setupFlowSensor();
void resetEeprom();
void WaktuRTC();

void setup()
{
  //delay(7000); //delay  detik untuk memastikan modul sim sudah siap 
  Serial.begin(9600);
  EspSerial.begin(115200);

  while (!Serial);
  Serial.println("Setup: Initialize...");
  EspSerial.println("Setup: Initialize...");


  //MODUL SIM
  _buffer.reserve(50);
  sim.begin(9600);
  //ReceiveMode(); //masih percobaan nanti saja di V.3
  
  //MODUL RTC
  setupRTC();
  //WaktuRTC(); //Perbaiki waktu

  //SENOR DEBIT
  setupFlowSensor(); // initializes flow sensor through pin connections
  //resetEeprom(); // resets all eeprom values to 0, turn on if ...?
  delay(1000);

  //MODUL MIKROSD
  setupSDCard();

  // Setup done
  Serial.println("Setup: Initialization done.");
  EspSerial.println("Setup: Initialization done.");
}

void setupSDCard() {
  Serial.print("Setup: Initializing SD Card...");
 
  if (!SD.begin(4)) {
    Serial.println("Setup: Initialization SD Card failed!");
    while (1);
  }
}

void setupRTC() {
  // initializes RTC
  Serial.print("Setup: Initializing RTC...");
  
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
}

void setupFlowSensor() 
{
  // initializes flow sensor
  Serial.print("Setup: Initializing Flow Sensor...");
 
  pinMode(FLOWSENSOR_PIN, INPUT);
  digitalWrite(FLOWSENSOR_PIN, HIGH);
  attachInterrupt(0,InterruptDebitAir,RISING); // di pin 2
  sei();
}

void resetEeprom()
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
