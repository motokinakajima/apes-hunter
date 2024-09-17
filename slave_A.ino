//This is a code for point-sender arduino A which has the 2 regions with smaller points and normal points.
int outputPins[5] = {0, 1, 2, 3, 4};
// clock | rgb[0] || rgb[1] || cds[0] || cds[1]
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
// Red: 1, Green: 2, Blue: 3
int thresholdPin = A6;

void setup() {
    // ピンのモード設定などをここで行う
    for (int i = 0; i < 5; i++) {
        pinMode(outputPins[i], OUTPUT);
    }
}

void loop() {
    update_region();
    for (int i = 0; i < 2; i++) { // 2つの領域に対応
        for (int j = 0; j < 5; j++) { // 各領域のセンサー数に対応
            int hit;
            if (cdsSensorPins[i][j] != -1) {
                hit = digitalRead(cdsSensorPins[i][j]);
            } else {
                hit = LOW;
            }
            
            // `outputData`をループ外で定義
            int outputData[5] = {LOW, LOW, LOW, LOW, LOW};

            if (hit == HIGH) {
                // get region color
                outputData[0] = HIGH; // Clockのためのピン

                switch (regionColor[i]) {
                    case 1: outputData[1] = LOW; outputData[2] = HIGH; break;
                    case 2: outputData[1] = HIGH; outputData[2] = LOW; break;
                    case 3: outputData[1] = HIGH; outputData[2] = HIGH; break;
                    default: outputData[1] = LOW; outputData[2] = LOW; break;
                }

                switch (points[i][j]) {
                    case 1: outputData[3] = LOW; outputData[4] = HIGH; break;
                    case 2: outputData[3] = HIGH; outputData[4] = LOW; break;
                    case 3: outputData[3] = HIGH; outputData[4] = HIGH; break;
                    default: outputData[3] = LOW; outputData[4] = LOW; break;
                }
            }

            // データの送信
            for (int k = 0; k < 5; k++) {
                digitalWrite(outputPins[k], outputData[k]);
            }

            // ヒットしない場合も少しの遅延を入れる
            delay(50);
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

        int threshold = (analogRead(thresholdPin) / 1023.0) * 10 + 20;

        if (abs(input_color[biggest_index] - avg) > threshold) {
            regionColor[i] = biggest_index + 1;
        } else {
            regionColor[i] = 0;
        }
    }
}
