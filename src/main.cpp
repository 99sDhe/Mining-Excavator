#include <Arduino.h>

int mid = 0;
int mn = 0;
int mx = 0;

#define IN1 PB0
#define IN2 PB1
#define IN3 PA8
#define IN4 PA11

#define J1CW A3
#define J1CCW A2
#define J2CW A6
#define J2CCW A7

int sen0 = 0;
int sen1 = 0;
int sen2 = 0;
int sen3 = 0;
int sen4 = 0;

// static int Sensor_Count = 0;
// static long Sensor_prev = millis();
// static long Sensor_now = millis();

// static int action_now = 0;
// static int action_prev = 0;
// static int action_next = 0;

const int onWhite= 100;
const int LEDPin = PC13;
char inputData = 0;

void maju();
void kiri();
void kanan();
void stop();
// void stop_keruk();
void keruk();
void lepas();
void toleh_kanan();
void toleh_kiri();

void setup()
{
    Serial.begin(9600);

    pinMode(J1CW, OUTPUT);
    pinMode(J1CCW, OUTPUT);
    pinMode(J2CW, OUTPUT);
    pinMode(J2CCW, OUTPUT);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    Serial.println("IROSTECH");
    Serial.println("HC-05 Bluetooth with STM32");

    for(int i=0; i<5000; i++)
    {
        digitalWrite(PC13, HIGH);

        int val = 0;
        for(int j=0; j<=5; j++) // kalibrasi nilai maximum dan minimum untuk jadi titik tengah 
        {                      
            val = analogRead(j);
            if(val >= mx)
            mx = val;
            if(val <= mn)
            mn = val;
        }
        delay(1);
    }    

    mid = (((mx + mn)/2)+250);
    pinMode(LEDPin, OUTPUT);


}

void loop()
{
    digitalWrite(LEDPin, HIGH);


    delay(1);

    int sen0 = analogRead(A0);
    int sen1 = analogRead(A1);
    int sen2 = analogRead(PB4);
    int sen3 = analogRead(A4);
    int sen4 = analogRead(A5);

    Serial.print("Mid");
    Serial.print(mid); 
    Serial.print(" ");

    Serial.print("Sens0 :");
    Serial.print(sen0);
    Serial.print(" ");

    Serial.print("Sens1 :");
    Serial.print(sen1);
    Serial.print(" ");

    Serial.print("Sens2 :");
    Serial.print(sen2);
    Serial.print(" ");

    Serial.print("Sens3 :");
    Serial.print(sen3);
    Serial.print(" ");

    Serial.print("Sens4 :");
    Serial.print(sen4);
    Serial.print(" ");
    Serial.println();

    if(((sen0 + sen1 + sen2)/2) < (((sen2 + sen3 + sen4)/2)-200))//Belok Kiri
    {
        kiri();
        Serial.print("KIRI");
        delay(abs((((sen0 + sen1 + sen2)/3)-((sen2 + sen3 + sen4)/3))/2));
    }
    
    if((((sen0 + sen1 + sen2)/2) > (((sen2 + sen3 + sen4)/2)+200)))//Belok Kanan
    {
        kanan();
        Serial.print("KANAN");
        delay(abs((((sen0 + sen1 + sen2)/3)-((sen2 + sen3 + sen4)/3))/2));
    }

    if((sen0 < sen2)&&(sen4 < sen2))
    {
        maju();
        Serial.print("maju");
        delay(abs((((sen0 + sen1 + sen2)/3)-((sen2 + sen3 + sen4)/3))/2));
    }

    if((sen0 < onWhite)&&(sen4 < onWhite)&&(sen2 < onWhite))
    {
        //sensor mencari garis berwarna hitam
        //tapi nanti aja deh :v
        kanan();
        delay(abs((((sen0 + sen1 + sen2)/3)-((sen2 + sen3 + sen4)/3))/2));
    }

    //Kondisi Line Follower berhenti ketika sensor 1 dan sensor 4 lebih tinggi dari nilai 1023
    if((sen0 > mid)&&(sen1 > mid)&&(sen3 > mid)&&(sen4 > mid)) 
    {                         
        stop();
        Serial.print(" STOP");

        for(int k=0; k<50; k++)
        {

        //Manual Excavating

        if(Serial.available() > 0){
        inputData = Serial.read();
            //Keruk Pasir
            if(inputData == 'f')
                {
                    keruk();
                    Serial.println("Keruk Pasir");
                    // inputData = "n";
                }

            //lepas
            else if(inputData == 'g')
                {
                    lepas();
                    Serial.println("Narok Pasir");
                    // inputData = "n";
                }
            //CCW
            else if(inputData == 'j')
                {
                    toleh_kiri();
                    Serial.println("Kiri");
                }

            //Stop
            // else  if(inputData == 'i')
            //     {
            //         stop_keruk();
            //         Serial.println("Stop");
            //     }

            //CW
            else  if(inputData == 'h')
                {
                    toleh_kanan();
                    Serial.println("Kanan!");
                }
            }
            digitalWrite(PC13, HIGH);
            delay(100);
            digitalWrite(PC13, LOW);
            delay(100);
        }
        delay(5000);
    }

}

void maju(){
    analogWrite(IN1, 0);
    analogWrite(IN2, 150);
    analogWrite(IN3, 150);
    analogWrite(IN4, 0);
}

void kiri(){
    analogWrite(IN1, 0);
    analogWrite(IN2, 180);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
}

void kanan(){
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 180);
    analogWrite(IN4, 0);
}

void stop(){
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
}

void keruk(){
    analogWrite(J1CW, 180);
    analogWrite(J1CCW, 0);
    analogWrite(J2CW, 0);
    analogWrite(J2CCW, 0);
    delay(2500);
    analogWrite(J1CW, 0);
    analogWrite(J1CCW, 0);
    analogWrite(J2CW, 0);
    analogWrite(J2CCW, 0);
}

void toleh_kanan(){
    analogWrite(J1CW, 0);
    analogWrite(J1CCW, 0);
    analogWrite(J2CW, 150);
    analogWrite(J2CCW, 0);
    delay(2000);
    analogWrite(J1CW, 0);
    analogWrite(J1CCW, 0);
    analogWrite(J2CW, 0);
    analogWrite(J2CCW, 0);

}

// void stop_keruk(){
//     analogWrite(J1CW, 0);
//     analogWrite(J1CCW, 0);
//     analogWrite(J2CW, 0);
//     analogWrite(J2CCW, 0);
// }

void toleh_kiri(){
    analogWrite(J1CW, 0);
    analogWrite(J1CCW, 0);
    analogWrite(J2CW, 0);
    analogWrite(J2CCW, 150);
    delay(1900);
    analogWrite(J1CW, 0);
    analogWrite(J1CCW, 0);
    analogWrite(J2CW, 0);
    analogWrite(J2CCW, 0);
}

void lepas(){
    analogWrite(J1CW, 0);
    analogWrite(J1CCW, 180);
    analogWrite(J2CW, 0);
    analogWrite(J2CCW, 0);
    delay(2000);
    analogWrite(J1CW, 0);
    analogWrite(J1CCW, 0);
    analogWrite(J2CW, 0);
    analogWrite(J2CCW, 0);
}
