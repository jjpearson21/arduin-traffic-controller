/****************************************************************************
 * Project: Non-Blocking Traffic Light
 * Name: Jared Pearson
 * Description:
 *    Uses millis() timing (no delay) to cycle through GREEN -> YELLOW -> RED
 ****************************************************************************/

// Pins
const int red_pin = 13;
const int yellow_pin = 12;
const int green_pin = 11;

const int walk_pin = 10;
const int walk_button_pin = 2;

// Timing
const unsigned long time_red = 5000;
const unsigned long time_yellow = 2000;
const unsigned long time_green = 5000;

const unsigned long time_walk = 3000;
const unsigned long time_walk_flash = 1500;
const unsigned long flash_period = 250;

// Debounce
const unsigned long debounce_ms = 40;

// Constraints
const unsigned long min_green_before_walk = 2000;

// FSM State
enum TrafficState {red, yellow, green};
enum PedState {idle, walk, flash};

TrafficState traffic_state = red;
PedState ped_state = idle;
unsigned long traffic_state_start_ms = 0;
unsigned long ped_state_start_ms = 0;

// Pedestrian Button
bool ped_request = false;
int last_button_reading = HIGH;
int stable_button_state = HIGH;
unsigned long last_debounce_change_ms = 0;

// Hardware Helper Functions
void set_traffic_lights(bool r, bool y, bool g) {
  digitalWrite(red_pin, r);
  digitalWrite(yellow_pin, y);
  digitalWrite(green_pin, g);
}

void set_walk_light(bool on) {
  digitalWrite(walk_pin, on);
}

// State Helper Functions
void traffic_enter_state(TrafficState s) {
  traffic_state = s;
  traffic_state_start_ms = millis();

  switch (traffic_state) {
    case red: set_traffic_lights(true, false, false); break;
    case yellow: set_traffic_lights(false, true, false); break;
    case green: set_traffic_lights(false, false, true); break;
  }
}

void ped_enter_state(PedState s) {
  ped_state = s;
  ped_state_start_ms = millis();

  switch (ped_state) {
    case idle: set_walk_light(false); break;
    case walk: set_walk_light(true); break;
    case flash: break;
  }
}

void update_button() {
  int reading = digitalRead(walk_button_pin);

  if (reading != last_button_reading) {
    last_debounce_change_ms = millis();
    last_button_reading = reading;
  }

  if ((millis() - last_debounce_change_ms) >= debounce_ms) {
    if (reading != stable_button_state) {
      stable_button_state = reading;

      if (stable_button_state == LOW) {
        ped_request = true;
      }
    }
  }
}

// Traffic FSM Update
void traffic_update( unsigned long now) {
  unsigned long elapsed = now - traffic_state_start_ms;

  switch (traffic_state) {
    case green:
      if (ped_request && elapsed >= min_green_before_walk) {
        traffic_enter_state(yellow);
      } else if (elapsed >= time_green) {
        traffic_enter_state(yellow);
      }
      break;
    case yellow:
      if (elapsed >= time_yellow) traffic_enter_state(red); 
      break;
    case red:
      if (elapsed >= time_red) traffic_enter_state(green);
      break;
  }
}

// Ped FSM Update
void ped_update(unsigned long now) {
  unsigned long ped_elapsed = now - ped_state_start_ms;

  switch (ped_state) {
    case idle:
      if (ped_request && traffic_state == red) {
        ped_enter_state(walk);
      }
      break;
    case walk:
      if (ped_elapsed >= time_walk) {
        ped_enter_state(flash);
      }
      break;
    case flash:
      unsigned long flash_elapsed = now - ped_state_start_ms;
      bool on = ((flash_elapsed / flash_period) % 2) == 0;
      set_walk_light(on);

      if (flash_elapsed >= time_walk_flash) {
        set_walk_light(false);
        ped_request = false;
        ped_enter_state(idle);
      }
      break;
  }
}

void setup() {
  pinMode(red_pin, OUTPUT);
  pinMode(yellow_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);

  pinMode(walk_pin, OUTPUT);
  pinMode(walk_button_pin, INPUT_PULLUP);

  traffic_enter_state(red);
  ped_enter_state(idle);
}

void loop() {
  update_button();

  unsigned long now = millis();

  traffic_update(now);
  ped_update(now);
}
