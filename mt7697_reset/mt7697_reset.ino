//mt7697 上传复位
//by xiaomi.cainiao

#define RTS_PIN 2
#define GPIO37_PIN 4
#define RST_PIN 5


volatile byte rts_state = LOW;
volatile byte state = LOW;

volatile long pre_ms = 0;
volatile long enter_ms = 0;
volatile long pulse_width = 0;
volatile bool has_pulse = false;
volatile bool waiting_pulse = false;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO37_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
//  attachInterrupt(digitalPinToInterrupt(RTS_PIN), rts_falling, FALLING);
  attachInterrupt(digitalPinToInterrupt(RTS_PIN), rts_changing, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, state);
  pinMode(RTS_PIN, INPUT);
  if(has_pulse){
    has_pulse = false;
    Serial.println(pulse_width);
    if(pulse_width > 120){
        Serial.println("I am entering normal mode");
        digitalWrite(GPIO37_PIN, LOW);
        digitalWrite(RST_PIN, LOW);
        delay(100);
        digitalWrite(RST_PIN, HIGH);
    }else{
        Serial.println("I am entering recovery mode");
        digitalWrite(GPIO37_PIN, HIGH);
        digitalWrite(RST_PIN, LOW);
        delay(100);
        digitalWrite(RST_PIN, HIGH);
    }
  }
//  Serial.println(digitalRead(RTS_PIN));
}

void rts_changing(){
  state = !state;
  if(millis() - pre_ms > 2 && millis() - pre_ms < 300){
    pulse_width = millis() - pre_ms;
    has_pulse = true;
  }
  pre_ms = millis();
//
  Serial.print(digitalRead(RTS_PIN) ? "rising: " : "falling: ");
  Serial.println(millis());
}

