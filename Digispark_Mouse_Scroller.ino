#include <DigiMouse.h>

#define UP_BTN 0
#define DN_BTN 2
#define LED_PIN 1

unsigned long lastScrollTime;
unsigned long autoScrollPeriod;
int8_t scrollAmount;

int8_t buttonStatus;
int8_t prevStatus;


void setup() {
  DigiMouse.begin(); //start or reenumerate USB - BREAKING CHANGE from old versions that didn't require this

  // Set Pins to input
  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DN_BTN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // init variables
  lastScrollTime = millis();
  autoScrollPeriod = 400;
  scrollAmount = 5;
  buttonStatus = 0; // Indicates button status, +1/-1 indicates direction.
  prevStatus = 0;
}

void loop() {
  // The main loop polls the buttons frequently to improve response time.
  // Scrolling is activated on a status change to prevent rapid scrolling.
  // Scrolling repeats automatically if button held down. Set autoScrollPeriod.
  // The mouse will scroll in the correct direction an amount Set by scrollAmount.
  
  pollButtons();

  // auto-repeat if button held down.
  if((millis() - lastScrollTime) >= autoScrollPeriod) {  
    scrollMouse();
  }

  //Slight delay, also allows USB sync/idle signals to update.  DigiMouse.delay(5);
}

void pollButtons() {
  // Polls both buttons. Falling edge trigger.
  // +1 for Up, 0 for none, -1 for Down, Up + Down cancel each other out.
  
  buttonStatus = (digitalRead(UP_BTN) == LOW) - (digitalRead(DN_BTN) == LOW);
  
  // Toggle Indicating LED
  buttonStatus == 0 ? digitalWrite(LED_PIN, LOW) : digitalWrite(LED_PIN, HIGH);

  // If a button is pressed, and buttonStatus has changed state since last poll.
  if(prevStatus != buttonStatus) {
    scrollMouse();
  }

  prevStatus = buttonStatus;
}

void scrollMouse() { // Just to reduce duplicates.
  DigiMouse.scroll(buttonStatus*scrollAmount);
  lastScrollTime = millis();
}






// fgggg
