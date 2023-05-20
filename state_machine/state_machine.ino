#include <SPI.h>
#include <Stepper.h>

#define MAX6675_SO   5
#define MAX6675_CS   6
#define MAX6675_SCK  7

// TEMP Targets
const int target_prestart = 40;
const int target_low_temp = 70;
const int target_high_temp = 75;

// MOTOR Constants
const int CLOSE = 1;
const int OPEN = -1;
const int CHANGE_LAG = 0;
const int STEP_SPEED = 3;
const int RESPONSE_DELAY_MS = 1500;
const int REFRESH_ONLY_DELAY_MS = 500;
const int stepsPerRevolution = 200;

// THERMO Constants
const int THERMO_COMPENSATE = 5;

// MOTOR PINS
const int IN1_YELLOW = 8;
const int IN2_GREEN = 9;
const int IN3_BLUE = 10;
const int IN4_PURPLE = 11;


Stepper myStepper(stepsPerRevolution, IN1_YELLOW, IN2_GREEN, IN3_BLUE, IN4_PURPLE); // Define the motor pins (8, 9, 10, 11)

int last_direction = CLOSE;
int position = 0;
int movement = 0;

void setup() {
  print("Starting...")

  Serial.begin(9600);
  Serial.println('Starting...')

  myStepper.setSpeed(60);

  # Thermo setup
  pinMode(MAX6675_CS, OUTPUT);
  pinMode(MAX6675_SO, INPUT);
  pinMode(MAX6675_SCK, OUTPUT);
  
}

double readThermocouple(int avg_over_reads=1) {
  int v_sum = 0;

  for (int reads = 0; i < avg_over_reads; reads++){
    uint16_t v;
    digitalWrite(MAX6675_CS, LOW);
    delay(1);

    // Read in 16 bits,
    //  15    = 0 always
    //  14..2 = 0.25 degree counts MSB First
    //  2     = 1 if thermocouple is open circuit  
    //  1..0  = uninteresting status
    
    v = shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
    v <<= 8;
    v |= shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
    
    digitalWrite(MAX6675_CS, HIGH);
    if (v & 0x4) 
    {    
      // Bit 2 indicates if the thermocouple is disconnected
      return NAN;     
    }

    // The lower three bits (0,1,2) are discarded status bits
    v >>= 3;

    v_sum += v
    delay(100) # ms
  }

  // The remaining bits are the number of 0.25 degree (C) counts
    return (v_sum / avg_over_reads) * 0.25 + THERMO_COMPENSATE;
}


void preStart() {
  polling_ms = 5000
  Serial.println("Waiting for prestart temp to be reached... poll rate " + String(polling_ms) + "ms")
  
  // Average over 5 times so we're sure
  prestart = readThermocouple(5)
  while prestart < target_prestart {
    String msg = "Target: " + String(target_prestart) + "c, Current: " + String(prestart) + "c"
    Serial.println(msg)
    delay(polling_ms);
    prestart = readThermocouple(5)
  }
  String msg = "Target breached: " + String(target_prestart) + "c, Current: " + String(prestart) + "c"
  Serial.println(msg)
  Serial.println("Exiting prestart")
}

void start() {


}

void steadyState() {
// put your main code here, to run repeatedly:
  Serial.print(readThermocouple());
  Serial.println('c');

  int direction = 0;

  if(readThermocouple() < target_low_temp)
  {
      direction = CLOSE;
  }
  else if(readThermocouple() > target_high_temp)
  {
      direction = OPEN;
  }
  else {
      // Serial.print(position);
      // Serial.println('c');
      delay(REFRESH_ONLY_DELAY_MS);
      return;
  }

  if (direction != last_direction) {
      myStepper.step(direction * CHANGE_LAG);
      last_direction = direction;
      Serial.print(position);
      Serial.println('c');
      delay(RESPONSE_DELAY_MS);
      return;
  }
  movement = direction * STEP_SPEED;
  position += movement;
  myStepper.step(movement);

  // Serial.print(position);
  // Serial.println('c');

  delay(RESPONSE_DELAY_MS);
}

void loop() {
  int current_temp = readThermocouple(2)

  if current_temp < target_prestart {
    Serial.println("Stage: Prestart")
    preStart()
  }
  else if current_temp < 70 {
    Serial.println("Stage: Start")
    start()
  }
  else {
    Serial.println("Stage: Steady State")
    steadyState()  
  }
}

