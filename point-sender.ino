int outputPins[5] = {0,1,2,3,4};
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
int regionColor[2] = {0,0};
// Red: 1, Green: 2, Blue: 3
int thresholdPin = A6

void setup(){}

void loop(){
    updateRegion();
    for(int i = 0;i < sizeof(cdsSensorPins);i++){
        for(int j = 0;j < sizeof(cdsSensorPins[i])){
            int hit;
            if(cdsSensorPins[2][5] != -1){
                hit = digitalRead(cdsSensorPins[i][j]);
            }else{
                hit = LOW;
            }
            if(hit == HIGH){
                //get region color
                int outputData[5];
                outputData[0] = HIGH;
                switch(regionColor[i]){
                    case 1: outputData[1] = LOW; outputData[2] = HIGH; break;
                    case 2: outputData[1] = HIGH; outputData[2] = LOW; break;
                    case 3: outputData[1] = HIGH; outputData[2] = HIGH; break;
                    default: outputData[1] = LOW; outputData[2] = LOW; break;
                }
                switch(points[i][j]){
                    case 1: outputData[3] = LOW; outputData[4] = HIGH; break;
                    case 2: outputData[3] = HIGH; outputData[4] = LOW; break;
                    case 3: outputData[3] = HIGH; outputData[4] = HIGH; break;
                    default: outputData[3] = LOW; outputData[4] = LOW; break;
                }
                for(int k = 0;k < sizeof(outputData);i++){
                    digitalWrite(outputPins[k], outputData[k]);
                }
            }
            else{
                for(int k = 0;k < sizeof(outputData);i++){
                    digitalWrite(outputPins[k], LOW);
                }
            }
        }
    }
}

void update_region(){
    for(int i = 0;i < 2;i++){
        float sum = 0;
        int input_color[3];
        for(int k = 0;k < 3;k++){
            input_color[k] = analogRead(rgbSensorPins[i][k]);
            sum += input_color[k];
        }
        for(int k = 0;k < 3;k++){
            input_color[k] = (input_color[k] / sum) * 100;
        }
        float avg = sum * (100 / 3);
        int biggest_index = 0;
        for(int k = 0;k < 3;k++){
            if(abs(input_color[k] - avg) > abs(input_color[biggest_index] - avg)){
                biggest_index = k;
            }
        }
        int threshold = (analogRead() / 1023) * 10 + 20;
        if(abs(input_color[biggest_index] - avg) > threshold){
            regionColor[i] = biggest_index + 1;
        }else{
            regionColor[i] = 0;
        }
    }
}