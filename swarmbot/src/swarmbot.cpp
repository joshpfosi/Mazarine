#include "pinmap.h"
#include "motor_control.h"
#include "photo_sensor.h"

// ----------------------------------------------------------------------------
// State variables
static bool isBot1  = true;
static bool hitWall = false;
// ----------------------------------------------------------------------------

void bot1(void);
void bot2(void);

// Helper routines
static inline void flashLed(int ledPin);

// ----------------------------------------------------------------------------
// collision.h
#define NUM_BUMPERS 5
#define DEBOUNCE_TIME 10000 // ms

// indices into boolean array for each bumper
// and bumper array for each pin
#define FRONT_LEFT  0
#define FRONT_RIGHT 1
#define LEFT        2
#define RIGHT       3
#define BACK        4

// input pins for reading if a bumper is pressed

bool bumperHit[NUM_BUMPERS];
volatile bool collisionHappened = false;
const int bumpers[] = { FL, FR, L, R, B };
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// collision.cpp
void detectCollision(void) {
    static unsigned long lastInterruptTime;
    unsigned long currTime = micros();

    if (currTime - lastInterruptTime > DEBOUNCE_TIME) {
        collisionHappened = true;
        lastInterruptTime = currTime;
    }
}

void setupCollision(void) {
    attachInterrupt(COLLISION_INT, detectCollision, RISING);
    for (int i = 0; i < NUM_BUMPERS; ++i) pinMode(bumpers[i], INPUT);
}
// ----------------------------------------------------------------------------

void setup() {
    // ------------------------------------------------------------------------
    // Set up general Bot pins
    //pinMode( GO_SWITCH,  INPUT);
    //pinMode( BOT_SWITCH, INPUT);
    pinMode( RED_LED,    OUTPUT);
    pinMode( YELLOW_LED, OUTPUT);
    // ------------------------------------------------------------------------

    Serial.begin(9600);

    setupPhotosensor();
    setupMotorControl();
    setupCollision();
}

void loop() {
    // TEST COLLISION
    //if (collisionHappened) {
    //    for (int i = 0; i < NUM_BUMPERS; ++i) {
    //        bumperHit[i] = digitalRead(bumpers[i]);
    //
    //        if (bumperHit[i]) {
    //            Serial.println(i);
    //            bumperHit[i] = false;
    //        }
    //    }
    //}

    // TEST MOTOR CONTROL
    //forward();
    //delay(1000);
    //backward();
    //delay(2000);
    //stop();
    //delay(500);

    // TEST PHOTOSENSOR
    //while (1) testPhotosensor();

    //while (!digitalRead(GO_SWITCH)) { delayMicroseconds(1); } // ON

    isBot1 = true; //digitalRead(BOT_SWITCH);

    // GO state
    if (isBot1) bot1();
    else        bot2();

    while (1) {};
}

void bot1(void) {
    // Puts bot in motion (`forward()`)
    forward();

    // Loops until collision (boolean is set in ISR)
    while (!hitWall) {
        for (int i = 0; i < NUM_BUMPERS; ++i) {
            if (digitalRead(bumpers[i])) {
                hitWall = true;
            }
        }
        delay(1);
    }
    Serial.println("Hit wall");


    // Assumes it hit the correct wall and turns a predetermined angle to the left

    stop(); delay(200); // resolve collision by backing up
    backward();
    delay(100);
    turn(-180); // negative to turn right

    // Moves forward until red and stops
    forwardUntilColor(RED);

    // Flash a red LED
    flashLed(RED_LED); // pauses for 1 second to flash

    // Follows red
    while (!followColorUntilColor(RED, YELLOW)) { delayMicroseconds(1); }
    Serial.println("Found yellow");

    // Stops on yellow, flashing yellow LED twice
    stop();
    flashLed(YELLOW_LED); // pauses for 1 second to flash
    flashLed(YELLOW_LED); // pauses for 1 second to flash

    // Moves forward until red
    forwardUntilColor(RED);

    // Follows red
    while (!followColorUntilColor(RED, YELLOW)) { delayMicroseconds(1); }

    // Stops on yellow, turns on yellow LED, turns 180 degrees
    stop();
//    // Moves forward until red
//    // Follows red
//    // Stops on yellow
//    // Communicates to Bot 2: `START`
//    // Waits for `ACK_START`
//    // Flashes green LED
//    // Moves forward until red
//    // Follows red
//    // TBD // we want this to be close to a specific location
//    // Turns on green LED
//    // Waits for `TOXIC`
//    // Flash yellow LED continuously
//    // Waits for `STOP_YELLOW`
//    // Turns off yellow LED
//    // Waits for `DONE`
//    // Flashes green LED
}

void bot2(void) {
//    // Waits for `START`
//    // Flash green LED
//    // Puts bot in motion (`forward()`)
//    // Loops until collision (boolean is set in ISR)
//    // Assumes it hit the correct wall and turns a predetermined angle to the left
//    // Moves forward 
//    // Stops on blue, flashing a blue LED
//    // Turns a predetermined angle to the right, and follows blue
//    // Stops on yellow, flashing yellow LED twice
//    // Moves forward
//    // Follows blue
//    // Stops on yellow, turns on yellow LED, turns 180 degrees
//    // Communicates to Bot 1: `TOXIC`
//    // Flash yellow continuously
//    // Moves forward until blue
//    // Follows blue
//    // Stops on yellow
//    // Communicates to Bot 1: `STOP_YELLOW`
//    // Stop flashing yellow LED
//    // Moves forward until blue
//    // Follows blue
//    // TBD // we want this to be close to a specific location
//    // Communicates to Bot 1: `DONE`
//    // Flashes green LED
}

// ----------------------------------------------------------------------------
// Helper Routines
// ----------------------------------------------------------------------------

static inline void flashLed(int ledPin) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
}
