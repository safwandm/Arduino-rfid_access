#define BLYNK_PRINT Serial

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <MFRC522.h>
#include <Servo.h>

char auth[] = "BBFF-6ZJVSjFu5ZpexEpMcuT70PgkwnpgFf"; // token yang dikirim dari blynk ke email masing masing
char ssid[] = "namahotspot";
char pass[] = "passwordhotspot";

#define SDA_PIN D2 // mendefinisikan SDA pin ke D2
#define RST_PIN D1 // mendefinisikan SDA pin ke D1
#define motor D4

Servo servo;

MFRC522 mfrc522(SDA_PIN, RST_PIN);

WidgetLCD lcd(V1);

void buka() { // membuat fungsi untuk menggerakkan servo
  servo.attach(motor);
  servo.write(90);
  delay(2000);
  servo.write(0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  Blynk.begin(auth, ssid, pass);
  mfrc522.PCD_Init();
  pinMode(motor, OUTPUT);
  Serial.println("Aproximate your card to the reader"); // pesan untuk men tap kartu
  Serial.println();

}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // menampilkan UID di serial Monitor
  Serial.println("UID tag :");
  String content = "";
  byte letter;

  // berfungsi untuk mengubah ID dari kartu menjadi kode Hexa seperti D4 EA FO 2B
  for (byte i = 0 ; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  Serial.print("message : ");
  content.toUpperCase(); // UID dijadikan huruf besar semua

  if (content.substring(1) == "04 EA F0 2B") { // masukkan ID kartu / E-KTP
    lcd.clear();
    lcd.print(0, 0/* baris pertama*/, "UID for card is"); // menamppilkan UID di ,cd blynk
    lcd.print(0, 1, content);
    delay(1000);
    buka(); // fungsi menggerakkan servo
  } else {
    lcd.clear();
    lcd.print(0, 0, "Invalid Access"); //menampilkan pesan ditolak blynk
    lcd.print(0, 1/* baris kedua */, "Access Denied");
  }

}
