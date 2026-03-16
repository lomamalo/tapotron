#include "LedControl.h" // Display library

LedControl lc = LedControl(7, 6, 5, 1); // Display connexions

// Display variables
int Var;
int Ones;
int Tens;
int Hundreds;

// Encoder variables
volatile int Counter = 60;
unsigned long Last_Run = 0;
unsigned long Current = 0;
unsigned long Previous = 0;

// Tempos variables
int Period = 1000;
byte State0 = 1;
byte State1 = 1;
byte State2 = 1;
byte State3 = 1;

// Button interruption variables
volatile bool ValButton = false;
unsigned long LastButton = 0;

// Sub-tempos selector variable
int Select = 0;

// Tap variables
int Piezo = 0;
int ThresP = 40;
int PState = 0;
int DebounceP = 50;
unsigned long StartTap = 0;
unsigned long EndTap = 0;
unsigned long Interval = 0;


// SETUP
void setup() {

  pinMode(3, INPUT);
  attachInterrupt(digitalPinToInterrupt(3), Encoder, LOW);

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), Button, FALLING);

  pinMode(8, OUTPUT);
  pinMode(A4, OUTPUT);

  // Display init
  lc.shutdown(0, false);   // WakeUp
  lc.setIntensity(0, 15);  // Brightness
  lc.clearDisplay(0);      // Reset
}


// Encoder interruption
void Encoder() {

  if (millis() - Last_Run > 10) {

    Last_Run = millis();

    if (digitalRead(4) == 1) {
      if (Counter < 241) Counter++;
      if (Counter == 241) Counter = 40;
    }

    if (digitalRead(4) == 0) {
      if (Counter > 39) Counter--;
      if (Counter == 39) Counter = 240;
    }
  }
}


// Button interruption (entering Tap mode)
void Button() {

  if (millis() - LastButton > 500) {
    LastButton = millis();
    ValButton = !ValButton;
  }
}


// Sub-tempos selection function
void SelecTempo() {

  if (millis() - Previous > 60)
    digitalWrite(A4, LOW); // Flash length

  Select = analogRead(A0) / 300;

  switch (Select) {

    case 0:
      Tempo0();
      break;

    case 1:
      Tempo1();
      break;

    case 2:
      Tempo2();
      break;

    case 3:
      Tempo3();
      break;
  }
}


// Tempos function

void Tempo0() {

  Period = 60000 / Counter;
  unsigned long Current = millis();

  if (Current - Previous >= Period) {

    Previous = Current;
    State0 = 1;

    tone(8, 1000, 20);
    digitalWrite(A4, HIGH);
  }
}


void Tempo1() {

  Period = 30000 / Counter;
  unsigned long Current = millis();

  if (Current - Previous >= Period) {

    Previous = Current;

    if (State1 == 1) {
      State1 = 2;
      tone(8, 1000, 20);
      digitalWrite(A4, HIGH);
    }

    else if (State1 == 2) {
      State1 = 1;
      tone(8, 250, 20);
    }
  }
}


void Tempo2() {

  Period = 20000 / Counter;
  unsigned long Current = millis();

  if (Current - Previous >= Period) {

    Previous = Current;

    if (State2 == 1) {
      State2 = 2;
      tone(8, 1000, 20);
      digitalWrite(A4, HIGH);
    }

    else if (State2 == 2) {
      State2 = 3;
      tone(8, 250, 20);
    }

    else if (State2 == 3) {
      State2 = 1;
      tone(8, 250, 20);
    }
  }
}


void Tempo3() {

  Period = 15000 / Counter;
  unsigned long Current = millis();

  if (Current - Previous >= Period) {

    Previous = Current;

    if (State3 == 1) {
      State3 = 2;
      tone(8, 1000, 20);
      digitalWrite(A4, HIGH);
    }

    else if (State3 == 2) {
      State3 = 3;
      tone(8, 250, 20);
    }

    else if (State3 == 3) {
      State3 = 4;
      tone(8, 250, 20);
    }

    else if (State3 == 4) {
      State3 = 1;
      tone(8, 250, 20);
    }
  }
}