#define NUM_BUMPERS 5

// indices into boolean array for each bumper
// and bumper array for each pin
#define FRONT_LEFT  0
#define FRONT_RIGHT 1
#define LEFT        2
#define RIGHT       3
#define BACK        4

// input pins for reading if a bumper is pressed

#define DEBOUNCE_TIME 10 // ms

#define INT0 0 // pin 2

static volatile int bumperHit[NUM_BUMPERS];
static int bumpers[] = { A0, A1, A4, A3, A2 };
static int leds[]    = { 38, 39, 40, 41, 42 };

void setup() { 
    attachInterrupt(INT0, detectCollision, RISING);

    for (int i = 0; i < NUM_BUMPERS; ++i) pinMode(bumpers[i], INPUT);
}

void loop()
{
    //check if any flags are set and if so, print to serial
    for (int i = 0; i < NUM_BUMPERS; ++i) {
        if (bumperHit[i]) {
            digitalWrite(leds[i], HIGH);
            bumperHit[i] = false;
        }
    }

    delay(1000);

    for (int i = 0; i < NUM_BUMPERS; ++i) digitalWrite(leds[i], LOW);
}


void detectCollision() {
    static unsigned long lastInterruptTimes[NUM_BUMPERS];

    unsigned long currTime = millis();

    // check each input for HIGH, setting flag indicating that bumper was hit
    for (int i = 0; i < NUM_BUMPERS; ++i) {
        if (currTime - lastInterruptTimes[i] > DEBOUNCE_TIME)
            // NOTE: potential bug -> HIGH might not map to true but I am
            //       making this assumption here
            bumperHit[i] = digitalRead(bumpers[i]);
    }
}
