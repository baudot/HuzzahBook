#include "AdafruitIO_WiFi.h"

#define BUTTON_LIGHT 2
#define BUTTON 12
#define IO_USERNAME "Your Adafruit IO account name goes here."
#define IO_KEY "Your Adafruit IO account key goes here. Look for it on the settings page of io.adafruit."

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, "YOUR NETWORK NAME", "YOUR WIFI PASSWORD");
AdafruitIO_Feed *chair_occupied = io.feed("ChairOccupied");
AdafruitIO_Feed *trigger_trap = io.feed("TriggerTrap");

void publish_status() {
  boolean button_pressed = !digitalRead(BUTTON);
  static boolean button_previously_pressed = true;
  if (button_pressed && !button_previously_pressed) {
    trigger_trap->save(true);
    button_previously_pressed = button_pressed;
  } else if (!button_pressed && button_previously_pressed) {
    trigger_trap->save(false);
    button_previously_pressed = button_pressed;    
  }
}

void change_light(AdafruitIO_Data *data) {
  int light_on = strcmp(data->value(),"Empty");
  if (light_on) {
    digitalWrite(BUTTON_LIGHT, HIGH);
    Serial.println("Chair: Occupied");
  } else {
    digitalWrite(BUTTON_LIGHT, LOW);
    Serial.println("Chair: Empty");
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial) ;
  pinMode(BUTTON_LIGHT, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.print("Attempting to connect to wifi...");
  io.connect();
  while(io.status() < AIO_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected! ");
  digitalWrite(BUTTON_LIGHT, LOW);
  chair_occupied->onMessage(change_light);
  Serial.println("Listening for chair status.");
}

void loop() {
  io.run();
  publish_status();
}
