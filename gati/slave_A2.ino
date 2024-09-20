//This program is for gati
//2 cds for small points and 3 cds for normal points
#include <Wire.h>

int outputPins[2][5] = {
    {2, 3, -1, -1, -1},  // Group 1 of OUTPUT pins
    {4, 5, 6, -1, -1}   // Group 2 of OUTPUT pins
};

int rgbSensorPins[2][3] = {
    {A0, A1, A2},  // 光センサー用のピン
    {A3, A6, A7}   // 別の光センサー用のピン
};

int cdsSensorPins[2][5] = {
    {7, 8, -1, -1, -1},
    {9, 10, 11, -1, -1}
};

int points[2][5] = {
    {3, 3, 0, 0, 0},
    {5, 5, 5, 0, 0}
};

int regionColor[2] = {0, 0};
int slaveAddress = 10;  // スレーブのアドレス
int threshold = 20;

// センサーの前回の状態を保存するための配列
int previousSensorState[2][5] = { 
    {LOW, LOW, LOW, LOW, LOW}, 
    {LOW, LOW, LOW, LOW, LOW} 
};

// I2C送信用の一時保存領域
int lastRegionColor = 0;
int lastPoints = 0;

void setup() {
    Wire.begin(slaveAddress);  // スレーブアドレスを指定して開始
    Wire.onRequest(requestEvent);  // マスターからのリクエストがあったときの処理
    Serial.begin(9600);  // シリアルモニターを使うための設定

    // OUTPUTピンを初期化
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            if (outputPins[i][j] != -1) {
                pinMode(outputPins[i][j], OUTPUT);
            }
        }
    }
    
    // CDSセンサー用のピンを初期化
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            if (cdsSensorPins[i][j] != -1) {
                pinMode(cdsSensorPins[i][j], INPUT_PULLUP);
            }
        }
    }
}

void loop() {
    update_region();
    /*
    Serial.println("Bingchilling");
    Serial.println(regionColor[0]);
    Serial.println(regionColor[1]);
    */

    // cdsSensorPinsの状態に基づいてOUTPUTピンを制御
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            if (cdsSensorPins[i][j] != -1) {
                int currentSensorValue = digitalRead(cdsSensorPins[i][j]);
                //lastPoints = 0;
                /*
                Serial.print("CdS Sensor Pin: ");
                Serial.print(cdsSensorPins[i][j]);
                Serial.print(" Value: ");
                Serial.println(currentSensorValue);
                */

                // 前回の状態と現在の状態を比較し、立ち上がりエッジを検出
                if (currentSensorValue == HIGH && previousSensorState[i][j] == LOW) {
                    // センサーがLOWからHIGHに変わった時
                    digitalWrite(outputPins[i][j], HIGH);  // OUTPUTピンをHIGHに

                    // データを一時保存（I2Cリクエストが来たときに送信）
                    lastRegionColor = regionColor[i];
                    lastPoints = points[i][j];

                    /*
                    Serial.print("Data ready to send: Region Color = ");
                    Serial.print(lastRegionColor);
                    Serial.print(", Points = ");
                    Serial.print(lastPoints);
                    Serial.print(", Hit pin: ");
                    Serial.println(outputPins[i][j]);
                    */
                } else if (currentSensorValue == LOW) {
                    // センサーがLOWの場合、OUTPUTピンをLOWに
                    digitalWrite(outputPins[i][j], LOW);
                }

                // 前回のセンサーの状態を更新
                previousSensorState[i][j] = currentSensorValue;
            }
        }
    }
}

void update_region() {
    for (int i = 0; i < 2; i++) {
        float sum = 0;
        float input_color[3];
        float avg = 100 / 3;
        int biggest_index = 0;
        for (int k = 0; k < 3; k++) {
            input_color[k] = analogRead(rgbSensorPins[i][k]);
            sum += input_color[k];
        }
        if (sum > 0) {
            for (int k = 0; k < 3; k++) {
                input_color[k] = (input_color[k] / sum) * 100;
                Serial.println(input_color[k]);
            }
            for (int k = 0; k < 3; k++) {
                if (abs(input_color[k] - avg) > abs(input_color[biggest_index] - avg)) {
                    biggest_index = k;
                }
            }

            if (abs(input_color[biggest_index] - avg) > threshold) {
                regionColor[i] = biggest_index + 1;
                Serial.println("Cyka blyat");
            }
        }
        /*
        Serial.print("Region detected: ");
        Serial.print(i);
        Serial.print("  Color detected: ");
        Serial.println(regionColor[i]);
        */

    }
}

// I2Cリクエスト時の処理
void requestEvent() {
    // マスターからのリクエストに応じて、最後に保存したregionColorとpointsを送信
    Wire.write(lastRegionColor);  // 領域の色（1バイト）
    Wire.write(lastPoints);       // ポイント（1バイト）
    /*
    Serial.print("Sent to master: Region Color = ");
    Serial.print(lastRegionColor);
    Serial.print(", Points = ");
    Serial.println(lastPoints);
    */
    lastPoints = 0;
}
