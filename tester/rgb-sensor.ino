const int BLUE_PIN = A0;
const int GREEN_PIN = A1;
const int RED_PIN = A2;
float input_color[3] = {0.0,0.0,0.0};
const int input_pins[3] = {BLUE_PIN,GREEN_PIN,RED_PIN};
String color_index[3] = {"BLUE","GREEN","RED"};
int biggest_index = 4;
//{B,G,R}

void setup() {
  //pinMode(5, OUTPUT);
  //pinMode(A5, INPUT_PULLUP);
  pinMode(BLUE_PIN, INPUT);
  pinMode(GREEN_PIN, INPUT);
  pinMode(RED_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Bingchilling");
  // put your setup code here, to run once:
}

void loop() {
  get_color();
  print_color();
  delay(100);

  // put your main code here, to run repeatedly:

}

void get_color(){
  float sum = 0;
  for(int i = 0;i < 3;i++){
    input_color[i] = analogRead(input_pins[i]);
    sum += input_color[i];
  }
  for(int i = 0;i < 3;i++){
    input_color[i] = (input_color[i] / sum) * 100;
  }
  float avg = sum * (100 / 3);
  biggest_index = 0;
  for(int i = 0;i < 3;i++){
    if(abs(input_color[i] - avg) > abs(input_color[biggest_index] - avg)){
      biggest_index = i;
    }
  }
}

void print_color(){
  String msg = "BGR = { ";
  msg += input_color[0];
  msg += " , ";
  msg += input_color[1];
  msg += " , ";
  msg += input_color[2];
  msg += " }  strongest color: ";
  msg += color_index[biggest_index];
  Serial.println(msg);
}