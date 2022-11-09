#define VRX A3
#define VRY A2

#define LEDB 7
#define LEDG 6
#define LEDR 11
#define LEDY 8

bool isGameStarted;
bool LeftSideDone;
bool SideCounting;
bool scoreCounted;

unsigned long sideTime;
unsigned long startTime;
unsigned long lightTime;

int score;
int randNumber;

void setup() {
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDY, OUTPUT);

  randomSeed(analogRead(0));

  Serial.begin(9600);
}

void loop() {
  if (!isGameStarted) {
    if (analogRead(VRX) > 900 && !SideCounting && !LeftSideDone) {
      sideTime = millis();
      SideCounting = true;
    }

    else if (analogRead(VRX) < 900 && !LeftSideDone)  SideCounting = false;

    if (sideTime + 1000 < millis() && SideCounting && !LeftSideDone) {
      LeftSideDone = true;
      SideCounting = false;
      Serial.println("Left Side OK.");
    }

    if (LeftSideDone && !SideCounting && analogRead(VRX) < 100) {
      sideTime = millis();
      SideCounting = true;
    }

    else if (analogRead(VRX) > 100 && LeftSideDone)   SideCounting = false;

    if (sideTime + 1000 < millis() && SideCounting && LeftSideDone) {
      isGameStarted = true;
      startTime = millis();
      Serial.println("Game has been started.");
      delay(800);
    }
  }

  if (isGameStarted) { // 0 = UP, 1 = LEFT, 2 = DOWN, 3 = RIGHT

    while (startTime + 20000 > millis()) { //  20s

      randNumber = random(0, 4);

      if (randNumber == 0)  digitalWrite(LEDB, HIGH);
      if (randNumber == 1)  digitalWrite(LEDG, HIGH);
      if (randNumber == 2)  digitalWrite(LEDR, HIGH);
      if (randNumber == 3)  digitalWrite(LEDY, HIGH);

      lightTime = millis();

      while (lightTime + 2000 > millis()) { 
        if (lightTime + 1000 < millis()) { 
          turnOFFleds();
        }

        if (analogRead(VRX) < 100 && !scoreCounted) { // right
          turnOFFleds();
          scoreCounted = true;
          if (randNumber == 3) score++;
        }

        if (analogRead(VRX) > 900 && !scoreCounted) { // left
          turnOFFleds();
          scoreCounted = true;
          if (randNumber == 1) score++;
        }

        if (analogRead(VRY) > 900 && !scoreCounted) {   // up
          turnOFFleds();
          scoreCounted = true;
          if (randNumber == 0) score++;
        }

        if (analogRead(VRY) < 100 && !scoreCounted) { // down
          turnOFFleds();
          scoreCounted = true;
          if (randNumber == 2) score++;
        }

      }
      scoreCounted = false;
    }

    Serial.println("Game Done.");
    Serial.print("Total score: ");
    Serial.print(score);
    Serial.println("/10");

    isGameStarted = false;
    score = 0;
  }
}

void turnOFFleds() {
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDY, LOW);
}
