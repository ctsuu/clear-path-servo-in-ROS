#define DIR_PIN          9
#define STEP_PIN         10
#define ENABLE_PIN       8

#define STEP_HIGH        PORTF |=  0b00000001;
#define STEP_LOW         PORTF &= ~0b00000001;

#define TIMER1_INTERRUPTS_ON    TIMSK1 |=  (1 << OCIE1A);
#define TIMER1_INTERRUPTS_OFF   TIMSK1 &= ~(1 << OCIE1A);

unsigned int c0;

void setup() {
  pinMode(STEP_PIN,   OUTPUT);
  pinMode(DIR_PIN,    OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  Serial.begin(115200);
  
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 10000;                             
  TCCR1B |= (1 << WGM12);
  TCCR1B |= ((1 << CS11) | (1 << CS10));
  interrupts();

  c0 = 160; // was 2000 * sqrt( 2 * angle / accel )
}

volatile int dir = 0;
volatile unsigned int maxSpeed = 10;
volatile unsigned long n = 0;
volatile float d;
volatile unsigned long stepCount = 0;
volatile unsigned long rampUpStepCount = 0;
volatile unsigned long totalSteps = 0;
volatile int stepPosition = 0;

volatile bool movementDone = false;

ISR(TIMER1_COMPA_vect)
{
  if ( stepCount < totalSteps ) {
    digitalWrite(STEP_PIN, HIGH);
    digitalWrite(STEP_PIN, LOW);
    //STEP_HIGH
    //STEP_LOW
    stepCount++;
    stepPosition += dir;
  }
  else {
    movementDone = true;
    TIMER1_INTERRUPTS_OFF
  }

  if ( rampUpStepCount == 0 ) { // ramp up phase
    //Serial.println("ramp up phase");
    n++;
    d = d - (2 * d) / (4 * n + 1);
    if ( d <= maxSpeed ) { // reached max speed
      d = maxSpeed;
      rampUpStepCount = stepCount;
    }
    if ( stepCount >= totalSteps / 2 ) { // reached halfway point
      rampUpStepCount = stepCount;
    }
  }
  else if ( stepCount >= totalSteps - rampUpStepCount ) { // ramp down phase
    //Serial.println("ramp down phase");
    n--;
    d = (d * (4 * n + 1)) / (4 * n + 1 - 2);
  }

  OCR1A = d;
}

void moveNSteps(long steps) {

  digitalWrite(DIR_PIN, steps < 0 ? HIGH : LOW);
  Serial.println(steps);
  dir = steps > 0 ? 1 : -1;
  totalSteps = abs(steps);
  d = c0;
  OCR1A = d;
  stepCount = 0;
  n = 0;
  rampUpStepCount = 0;
  movementDone = false;

  TIMER1_INTERRUPTS_ON
}

void moveToPosition(long p, bool wait = true) {
  //Serial.println("Starting...toposition");
  moveNSteps(p - stepPosition);
  while ( wait && ! movementDone );
}

void loop() {
  digitalWrite(ENABLE_PIN, HIGH);
  Serial.println("Starting...");
  maxSpeed = 40;
  moveToPosition(  1600 );
  moveToPosition( -1600 );
  moveToPosition( 0 );

  moveToPosition( 200 );
  moveToPosition( 400 );
  moveToPosition( 600 );
  moveToPosition( 800 ); 

  moveToPosition( 4000 );
  moveToPosition( 600 );
  moveToPosition( 200 );
  moveToPosition( 400 );
  moveToPosition( 0 );

  maxSpeed = 1600;
  moveToPosition( 200 );
  moveToPosition( 400 );

  maxSpeed = 400;
  moveToPosition( 1600 );
  moveToPosition( 800 );

  maxSpeed = 3200;
  moveToPosition( 1000 );
  moveToPosition( 1200 );

  maxSpeed = 10;
  moveToPosition( 1000 );
  Serial.println("Finished everything...");
  while (true);

}








