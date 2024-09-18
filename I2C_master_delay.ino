#include <Wire.h>

void setup() {
  Wire.begin();  // マスターとして初期化
  Serial.begin(9600);
}

void loop() {
  requestDataFromSlave(8);  // スレーブ1からデータをリクエスト
  requestDataFromSlave(9);  // スレーブ2からデータをリクエスト
  requestDataFromSlave(10); // スレーブ3からデータをリクエスト

  delay(500);  // 少し待機
}

void requestDataFromSlave(int slaveAddress) {
  Wire.requestFrom(slaveAddress, 2);  // 2バイトのリクエスト

  // データが受信できるか確認
  if (Wire.available() >= 2) {  // 必要なバイト数が受信できているかチェック
    int regionColor = Wire.read();  // 1バイト目: regionColor
    int points = Wire.read();       // 2バイト目: points

    Serial.print("Slave Address: ");
    Serial.print(slaveAddress);
    Serial.print(", Region Color: ");
    Serial.print(regionColor);
    Serial.print(", Points: ");
    Serial.println(points);
  } else {
    Serial.print("Error: Not enough data from Slave Address ");
    Serial.println(slaveAddress);
  }
}
