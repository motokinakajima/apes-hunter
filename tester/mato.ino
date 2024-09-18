int hit_counter = 0;
bool prev_hit = false;
void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  Serial.println("Testing censor");

}

void loop() {
  int hit = digitalRead(2);
  if(hit == HIGH) {
    digitalWrite(3, HIGH);
    if (prev_hit == false) {
      hit_counter ++;
      delay(10);
    }
    prev_hit = true;
  } else {
    digitalWrite(3, LOW);
    prev_hit = false;
  }
  String msg = "-1,2,";
  msg += hit;
  Serial.println(msg);
  //Serial.println(hit_counter); 

}
