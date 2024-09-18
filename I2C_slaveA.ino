#include <Wire.h>

int outputPins[2][5] = {
    {0, 1, 2, -1, -1},  // Group 1 of OUTPUT pins
    {3, 4, 5, -1, -1}   // Group 2 of OUTPUT pins
};

int rgbSensorPins[2][3] = {
    {A0, A1, A2},  // 光センサー用のピン
    {A3, A6, A7}   // 別の光センサー用のピン
};

int cdsSensorPins[2][5] = {
    {6, 7, 8, -1, -1},
    {9, 10, 11, -1, -1}
};

int points[2][5] = {
    {1, 2, 3, 0, 0},
    {1, 2, 3, 0, 0}
};

int regionColor[2] = {0, 0};
int slaveAddress = 10;  // スレーブのアドレス
int threshold = 20;

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
}

void loop() {
    update_region();

    // cdsSensorPinsの状態に基づいてOUTPUTピンを制御
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            if (cdsSensorPins[i][j] != -1 && digitalRead(cdsSensorPins[i][j]) == HIGH) {
                digitalWrite(outputPins[i][j], HIGH);  // センサーがHIGHのとき、OUTPUTピンをHIGHに
            } else if (cdsSensorPins[i][j] != -1 && digitalRead(cdsSensorPins[i][j]) == LOW) {
                digitalWrite(outputPins[i][j], LOW);
            }    
        }
    }
}

void update_region() {
    for (int i = 0; i < 2; i++) {
        float sum = 0;
        int input_color[3];
        for (int k = 0; k < 3; k++) {
            input_color[k] = analogRead(rgbSensorPins[i][k]);
            sum += input_color[k];
        }

        for (int k = 0; k < 3; k++) {
            input_color[k] = (input_color[k] / sum) * 100;
        }

        float avg = sum * (100 / 3);
        int biggest_index = 0;

        for (int k = 0; k < 3; k++) {
            if (abs(input_color[k] - avg) > abs(input_color[biggest_index] - avg)) {
                biggest_index = k;
            }
        }

        if (abs(input_color[biggest_index] - avg) > threshold) {
            regionColor[i] = biggest_index + 1;
        } else {
            regionColor[i] = 0;
        }
    }
}

// I2Cリクエスト時の処理
void requestEvent() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            if (cdsSensorPins[i][j] != -1 && digitalRead(cdsSensorPins[i][j]) == HIGH) {
                // regionColorとpointsを1バイトずつ送信
                Wire.write(regionColor[i]);  // 領域の色（1バイト）
                Wire.write(points[i][j]);    // ポイント（1バイト）

                Serial.print("A sent to master: ");
                Serial.print(regionColor[i]);
                Serial.print(", ");
                Serial.println(points[i][j]);
            }
        }
    }
}
