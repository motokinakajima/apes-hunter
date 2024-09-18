#include <Wire.h>

unsigned long previousMillis = 0;  // 最後にデータをリクエストした時間
const long interval = 500;         // リクエスト間隔（ミリ秒）
const long point_interval = 1000;

void setup() {
  Wire.begin();  // マスターとして初期化
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  
  // リクエスト間隔が経過した場合
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // 最後にデータをリクエストした時間を更新

    // スレーブ1からデータをリクエスト
    requestDataFromSlave(8);

    // スレーブ2からデータをリクエスト
    requestDataFromSlave(9);

    // スレーブ3からデータをリクエスト
    requestDataFromSlave(10);
  }
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
