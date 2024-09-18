#include <Wire.h>

void setup() {
  Wire.begin();  // マスターとして初期化
  Serial.begin(9600);
}

void loop() {
  // スレーブ1からデータをリクエスト
  requestDataFromSlave(8);
  
  // スレーブ2からデータをリクエスト
  requestDataFromSlave(9);
  
  // スレーブ3からデータをリクエスト
  requestDataFromSlave(10);

  delay(500);  // 少し待機
}

void requestDataFromSlave(int slaveAddress) {
  Wire.requestFrom(slaveAddress, 2);  // 2バイトのリクエスト

  while (Wire.available()) {
    int regionColor = Wire.read();  // 1バイト目: regionColor
    int points = Wire.read();       // 2バイト目: points
    Serial.print("Slave Address: ");
    Serial.print(slaveAddress);
    Serial.print(", Region Color: ");
    Serial.print(regionColor);
    Serial.print(", Points: ");
    Serial.println(points);
  }
}
