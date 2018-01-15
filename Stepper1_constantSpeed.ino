#define DIR_PIN          9
#define STEP_PIN         10
#define ENABLE_PIN       8

void setup() {
  pinMode(STEP_PIN,   OUTPUT);
  pinMode(DIR_PIN,    OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH);
}

void simpleMove(int steps) {
  int interval = 10;
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(interval);
  }
}

void loop() {
  digitalWrite(DIR_PIN, LOW);
  simpleMove(800);
  digitalWrite(DIR_PIN, HIGH);
  simpleMove(800);

  while (true);
}








