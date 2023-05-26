
char inputJsonBuffer[1024];
char outputJsonBuffer[1024];

String receivedString = "";
unsigned long startupTime = 0;

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();

  if (!sensors.getAddress(insideThermometer, 0)) {
    Serial.println("Unable to find address for Device 0");
  }
  
  while (!Serial.available()) {
    ; // wait for data to be sent over serial
  }
  
  receivedString = Serial.readStringUntil('\n');
  startupTime = receivedString.toInt();
}

void loop(void)
{ 
  float tempC = 69;

  unsigned long elapsedSeconds = (millis()/1000) + startupTime;
  snprintf(jsonMessage, sizeof(jsonMessage), "{\"time\":%lu,\"temperature\":%.2f}", elapsedSeconds, tempC);
  Serial.println(jsonMessage);
  
  delay(3000);
}
