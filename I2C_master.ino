#include <Wire.h>

unsigned long previousMillis = 0;  // 最後にデータをリクエストした時間
unsigned long pointsResetMillis = 0; // 最後にポイントをリセットした時間
const long interval = 500;         // リクエスト間隔（ミリ秒）
const long point_interval = 1000;  // 1秒ごとに表示

int totalPointsRed = 0;
int totalPointsGreen = 0;
int totalPointsBlue = 0;

int resetPins[3] = {2, 3, 4};  // 赤、緑、青の得点をリセットするためのデジタルピン

void setup() {
  Wire.begin();  // マスターとして初期化
  Serial.begin(9600);
  
  // リセット用のピンを入力に設定
  for (int i = 0; i < 3; i++) {
    pinMode(resetPins[i], INPUT);
  }
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

  // 1秒ごとにポイントの集計と表示
  if (currentMillis - pointsResetMillis >= point_interval) {
    pointsResetMillis = currentMillis;  // リセット時間を更新

    // 色ごとの得点を表示
    Serial.print("赤: ");
    Serial.print(totalPointsRed);
    Serial.print(" 緑: ");
    Serial.print(totalPointsGreen);
    Serial.print(" 青: ");
    Serial.println(totalPointsBlue);

    // リセットピンの状態を確認し、HIGHならポイントをリセット
    if (digitalRead(resetPins[0]) == HIGH) totalPointsRed = 0;
    if (digitalRead(resetPins[1]) == HIGH) totalPointsGreen = 0;
    if (digitalRead(resetPins[2]) == HIGH) totalPointsBlue = 0;
  }
}

void requestDataFromSlave(int slaveAddress) {
  Wire.requestFrom(slaveAddress, 2);  // 2バイトのリクエスト

  // データが受信できるか確認
  if (Wire.available() >= 2) {  // 必要なバイト数が受信できているかチェック
    int regionColor = Wire.read();  // 1バイト目: regionColor
    int points = Wire.read();       // 2バイト目: points

    // 色ごとのポイントを追加
    switch (regionColor) {
      case 1:  // 赤
        totalPointsRed += points;
        break;
      case 2:  // 緑
        totalPointsGreen += points;
        break;
      case 3:  // 青
        totalPointsBlue += points;
        break;
    }

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
