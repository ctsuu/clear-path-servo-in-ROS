#define X_ENABLE_PIN       8
#define X_DIR_PIN          9
#define X_STEP_PIN         10

#define Y_ENABLE_PIN       11
#define Y_DIR_PIN          12
#define Y_STEP_PIN         13


void setup() {
  pinMode(X_STEP_PIN,   OUTPUT);
  pinMode(X_DIR_PIN,    OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, HIGH);

  pinMode(Y_STEP_PIN,   OUTPUT);
  pinMode(Y_DIR_PIN,    OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, HIGH);
}

void simpleAccel(int steps) {

  int lowSpeed = 2000;
  int highSpeed = 1;
  int change = 2;

  int rampUpStop = (lowSpeed - highSpeed) / change;
  if ( rampUpStop > steps / 2 )
    rampUpStop = steps / 2;
  int rampDownStart = steps - rampUpStop;

  int d = lowSpeed;

  for (int i = 0; i < steps; i++) {
    digitalWrite(X_STEP_PIN, HIGH);
    digitalWrite(X_STEP_PIN, LOW);
    digitalWrite(Y_STEP_PIN, HIGH);
    digitalWrite(Y_STEP_PIN, LOW);
  
    delayMicroseconds(d);

    if ( i < rampUpStop )
      d -= change;
    else if ( i > rampDownStart )
      d += change;
  }
}

void loop() {
  digitalWrite(X_DIR_PIN, LOW);
  digitalWrite(Y_DIR_PIN, LOW);
  simpleAccel(32000);
  digitalWrite(X_DIR_PIN, HIGH);
  digitalWrite(Y_DIR_PIN, HIGH);
  simpleAccel(32000);

  while (true);

}
