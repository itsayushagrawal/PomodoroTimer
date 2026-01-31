unsigned long previousMillis = 0;
const unsigned long interval = 1000; // 1 second

int minutes = 5;
int seconds = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (minutes == 0 && seconds == 0) {
      Serial.println("Time's up!");
      while (1);   // stop
    }

    if (seconds == 0) {
      minutes--;
      seconds = 59;
    } else {
      seconds--;
    }

    // Print in MM:SS format
    if (minutes < 10)
      Serial.print("0");
    Serial.print(minutes);
    Serial.print(":");
    if (seconds < 10)
      Serial.print("0");
    Serial.println(seconds);
  }
}
