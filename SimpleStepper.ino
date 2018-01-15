#define DIR_PIN          9
#define STEP_PIN         10
#define ENABLE_PIN       8

void setup() {
  pinMode(DIR_PIN,    OUTPUT);
  pinMode(STEP_PIN,   OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH);
}

void loop() {
  digitalWrite(STEP_PIN, HIGH);
  digitalWrite(STEP_PIN, LOW);
  delay(1);
}

