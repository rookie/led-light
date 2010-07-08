int ledPin = 0;                 // LED connected to digital pin 13

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
}

void loop()
{
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(2000);                  // waits for a second
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(2000);                  // waits for a second
}


int main(void)
{
  init();
  setup();

  for (;;){
    loop();
  }
  //return 0;
}

