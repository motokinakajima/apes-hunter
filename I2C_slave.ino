#include <Wire.h>

int outputPins[5] = {0, 1, 2, 3, 4};
int rgbSensorPins[2][3] = {
    {A0, A1, A2},
    {A3, A4, A5}
};
int cdsSensorPins[2][5] = {
    {5, 6, 7, -1, -1},
    {8, 9, 10, -1, -1}
};
int points[2][5] = {
    {1, 2, 3, 0, 0},
    {1, 2, 3, 0, 0}
};
int regionColor[2] = {0, 0};
int thresholdPin = A6;

int slaveAddress = 8;  // 各スレーブごとに異なるアドレスを設定

void setup() {
    Wire.begin(slaveAddress);  // スレーブアドレスを指定して開始
    Wire.onRequest(requestEvent);  // マスターからのリクエストがあったときの処理を設定
    for (int i = 0; i < 5; i++) {
        pinMode(outputPins[i], OUTPUT);
    }
}

void loop() {
    update_region();
    // 通常の処理は続ける（ただし、I2C通信はマスターがリクエストしたときにのみ行われる）
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

        int threshold = (analogRead(thresholdPin) / 1023.0) * 10 + 20;

        if (abs(input_color[biggest_index] - avg) > threshold) {
            regionColor[i] = biggest_index + 1;
        } else {
            regionColor[i] = 0;
        }
    }
}

// マスターからのリクエストを受け取ったときの処理
void requestEvent() {
    for (int i = 0; i < 2; i++) { // 2つの領域に対してデータを送信
        for (int j = 0; j < 5; j++) {
            if (cdsSensorPins[i][j] != -1 && digitalRead(cdsSensorPins[i][j]) == HIGH) {
                // データのフォーマット: !(RegionColor)<(Points)>
                String data = "!" + String(regionColor[i]) + "<" + String(points[i][j]) + ">";

                // I2Cバッファにデータを送信（文字列の最大長は32バイト以内）
                Wire.write(data.c_str());
            }
        }
    }
}
