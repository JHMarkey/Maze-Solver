#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Buzzer buzzer;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;

struct move{
  int time;
  int moveType;
};

#define NUM_LINE_SENSORS 3
#define MAX_MOVES 250
#define MAX_HOUSE 2
#define LINE_THRESHOLD 250
move moves[MAX_MOVES];
int movesCount = 0;
int hFound = 0;
unsigned int lineSensorValues[NUM_LINE_SENSORS];

int currentMove = 0;



void calibrateSensors() {
  for (uint16_t i = 0; i < 120; i++) {
    motors.setSpeeds(-200, 200);
    if (i > 30 && i <= 90) {
      motors.setSpeeds(200, -200);
    }
    lineSensors.calibrate();
  }
  Serial.println("Calibration Complete...");
  motors.setSpeeds(0, 0); 
}

void initializeLineSensors() {
  lineSensors.initThreeSensors();
}

void initializeProximitySensors() {
  proxSensors.initThreeSensors();
}

void setup() {
  initializeLineSensors();
  initializeProximitySensors();
  calibrateSensors();
}

void printReadingsToSerial(int s1, int s2, int s3) {
  static char buffer[80];
  sprintf(buffer, "%d %d %d %d %d %d\n",
          proxSensors.countsLeftWithLeftLeds(),
          proxSensors.countsLeftWithRightLeds(),
          proxSensors.countsFrontWithLeftLeds(),
          proxSensors.countsFrontWithRightLeds(),
          proxSensors.countsRightWithLeftLeds(),
          proxSensors.countsRightWithRightLeds()
         );

  Serial.print("Left Line:   ");
  Serial.print(s1);
  Serial.print("  Middle Line:   ");
  Serial.print(s2);
  Serial.print("  Right Line:   ");
  Serial.print(s3);
  Serial.print("  Prox Sensor Values:   ");
  Serial.print(buffer);
  Serial.println();
}

void turnAround() {
  for (int i = 0; i < 120; i++) {
    motors.setSpeeds(-100, 100);
  }
}

void turnLeft() {
  for (int i = 0; i < 60; i++) {
    motors.setSpeeds(-100, 100);
  }
}

void turnRight() {
  for (int i = 0; i < 60; i++) {
    motors.setSpeeds(100, -100);
  }
}

void reverse() {
  for (int i = 0; i < 60; i++) {
    motors.setSpeeds(-100, -100);
  }
}

void forward() {
  for (int i = 0; i < 60; i++) {
    motors.setSpeeds(100, 100);
  }
}

void loop() {

  while(hFound < MAX_HOUSE){
    
    static uint16_t lastSampleTime = 0;
    if ((uint16_t)(millis() - lastSampleTime) >= 100) {
      lastSampleTime = millis();
      lineSensors.readCalibrated(lineSensorValues);
      int s1 = lineSensorValues[0];
      int s2 = lineSensorValues[1];
      int s3 = lineSensorValues[2];
      proxSensors.read();

      printReadingsToSerial(s1, s2, s3);
      Serial.print("Moves:   ");
      Serial.println(movesCount);
      Serial.print("House Found:   ");
      Serial.println(hFound);

      if (proxSensors.countsFrontWithLeftLeds() > 5 || proxSensors.countsFrontWithRightLeds() > 5 || movesCount == MAX_MOVES) {
        
        motors.setSpeeds(0, 0);
        for(int i = 0; i < 3; i++){
          buzzer.playNote('E', 100, 100);
        }
            
        delay(1000);

        for (int i = movesCount - 1; i >= 0; i--) {
          switch (moves[i].moveType) {
            case 1:
              reverse();      
              Serial.println("Forward");  
              break;
            case 2:
              forward();
              Serial.println("Back");  
              ledRed(1);
              break;
            case 3:
              turnRight();
              Serial.println("Left");  
              ledYellow(1);
              break;
            case 4:
              turnLeft();
              Serial.println("Right");  
              ledGreen(1);
              break;
            default:
              break;           
                
          } 
          delay(90);       
          ledRed(0);
          ledYellow(0);
          ledGreen(0);
        }
        delay(1000);
        movesCount = 0;
        motors.setSpeeds(0, 0);
        if(movesCount != MAX_MOVES){
          hFound++;
          for(int i = 0; i <= hFound; i++){
          buzzer.playNote('E', 100, 100);
        }
        }
        
        } else {
          move m;
          m.time = lastSampleTime;
        if (s1 < LINE_THRESHOLD && s2 < LINE_THRESHOLD && s3 < LINE_THRESHOLD) {
          forward();        
          m.moveType = 1;
        } else if (s1 > LINE_THRESHOLD && s2 > LINE_THRESHOLD && s3 > LINE_THRESHOLD) {
          reverse();
          m.moveType = 2;
        } else if (s1 <= LINE_THRESHOLD && s2 <= LINE_THRESHOLD && s3 > LINE_THRESHOLD) {
          turnLeft();
          m.moveType = 3;
        } else if (s1 > LINE_THRESHOLD && s2 <= LINE_THRESHOLD && s3 <= LINE_THRESHOLD) {
          turnRight();
          m.moveType = 4;        
        }
        moves[movesCount++] = m;
      }
    }
  }
}