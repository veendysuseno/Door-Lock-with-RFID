/* Program Door Look with RFID */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 
#define LED_R 4 
#define RELAY 3 
#define BUZZER 2 
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup(){
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();   // memulai MFRC522
    pinMode(LED_G, OUTPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(RELAY, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    noTone(BUZZER);
    digitalWrite(RELAY, LOW);
    Serial.println("Letakkan kartu anda ke pembaca...");
    Serial.println();
}

void loop(){
    if ( ! mfrc522.PICC_IsNewCardPresent())   {
        return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial())   {
        return;
    }
    //Menampilkan UID ke monitor
    Serial.print("UID tag :");
    String content = "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "83 23 38 BB") //ganti UID 83 23 38 BB dengan UID kartu anda untuk memberi akses
    {
        Serial.println("Authorized access"); //Apabila menggunakan RFID Tag yang benar
        Serial.println();
        delay(500);
        digitalWrite(RELAY, HIGH);
        digitalWrite(LED_G, HIGH);
        delay(ACCESS_DELAY);
        digitalWrite(RELAY, LOW);
        digitalWrite(LED_G, LOW);
    }

    else   {
        Serial.println(" Access denied");  //Apabila menggunakan RFID Tag yang Salah
        digitalWrite(LED_R, HIGH);
        tone(BUZZER, 300);
        delay(DENIED_DELAY);
        digitalWrite(LED_R, LOW);
        noTone(BUZZER);
    }
}
