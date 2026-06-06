const int pinGasMQ2   = 34;  
const int pinFlame    = 25;  
const int pinBuzzer   = 26;  
const int pinFanMotor = 27;  
const int pinRelayPump = 14; 

const int batasGas = 1500;   

void setup() {
  Serial.begin(115200); 

  pinMode(pinGasMQ2, INPUT);
  pinMode(pinFlame, INPUT);
  
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinFanMotor, OUTPUT);
  pinMode(pinRelayPump, OUTPUT);

  digitalWrite(pinBuzzer, LOW);
  digitalWrite(pinFanMotor, LOW);     
  digitalWrite(pinRelayPump, HIGH);   
}

void loop() {
  int nilaiGas = analogRead(pinGasMQ2);  
  int statusApi = digitalRead(pinFlame); 

  Serial.print("Gas: ");
  Serial.print(nilaiGas);
  Serial.print(" | Api: ");
  Serial.println(statusApi);

  if (nilaiGas > batasGas) {
    digitalWrite(pinFanMotor, HIGH); 
    digitalWrite(pinBuzzer, HIGH);
    delay(100);
    digitalWrite(pinBuzzer, LOW);
    delay(100);
  } else {
    digitalWrite(pinFanMotor, LOW); 
  }

  if (statusApi == LOW) { 
    digitalWrite(pinRelayPump, LOW); 
    digitalWrite(pinBuzzer, HIGH);   
  } else {
    digitalWrite(pinRelayPump, HIGH); 
  }

  if (nilaiGas <= batasGas && statusApi == HIGH) {
    digitalWrite(pinBuzzer, LOW);
  }

  delay(200); 
}