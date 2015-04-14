#define NUM_BUMPERS 6

// indices into boolean array for each bumper
// and bumper array for each pin
#define FRONT_LEFT  0
#define FRONT_RIGHT 1
#define LEFT        2
#define RIGHT       3
#define BACK        4

// input pins for reading if a bumper is pressed

#define DEBOUNCE_TIME 10000 // ms

#define INT0 0 // pin 2

static volatile bool collisionHappened = false;
static volatile bool bumperHit[NUM_BUMPERS];
static int bumpers[] = { 43, 44, 45, 46, 47, 48 };
static int leds[]    = { 38, 39, 40, 41, 42 };

void setup() {
    Serial.begin(9600);
    attachInterrupt(INT0, detectCollision, RISING);

    for (int i = 0; i < NUM_BUMPERS; ++i) {
        pinMode(bumpers[i], INPUT);
        pinMode(leds[i],    OUTPUT);
    }
}

void loop() {
    // check each input for HIGH, setting flag indicating that bumper was hit
    if (collisionHappened) {
        for (int i = 0; i < NUM_BUMPERS; ++i) {
            bumperHit[i] = digitalRead(bumpers[i]);

            if (bumperHit[i]) {
                Serial.println(i);
                bumperHit[i] = false;
            }
        }
    }
}


void detectCollision() {
    static unsigned long lastInterruptTime;
    unsigned long currTime = micros();

    if (currTime - lastInterruptTime > DEBOUNCE_TIME) {
        collisionHappened = !collisionHappened;
        lastInterruptTime = currTime;
    }
}

