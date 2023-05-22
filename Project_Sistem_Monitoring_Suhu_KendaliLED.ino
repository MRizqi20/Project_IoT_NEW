Kode Program :
// Library yang dibutuhkan
#include "DHT.h"
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
// Mendefinisikan pin dan tipe sensor DHT
#define DHTPIN D6
#define DHTTYPE DHT11
DHT dht11(DHTPIN, DHTTYPE);
#define led D4
// Isikan sesuai pada Firebase
#define FIREBASE_HOST "https://monitoring-pengukuran-suhu-defaultrtdb.firebaseio.com/"
#define FIREBASE_AUTH "pKt7NGXSm1y1R9P741DGdRJ7qLTVJ7HPnCxEcAUk"
//Nama Wifi
#define WIFI_SSID "TIARA KS"
#define WIFI_PASSWORD "ayuta100601"
// mendeklarasikan objek data dari FirebaseESP8266
FirebaseData firebaseData;
void setup() {
  Serial.begin(115200);
dht11.begin();
pinMode(led, OUTPUT);
digitalWrite(led, LOW); //nilai awal relay akan off
// Koneksi ke Wifi
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("connecting");
while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}
Serial.println();
Serial.print("Connected with IP: ");
Serial.println(WiFi.localIP());
Serial.println();
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
void loop(){
// Sensor DHT11 membaca suhu dan kelembaban
float t = dht11.readTemperature();
float h = dht11.readHumidity();
// Memeriksa apakah sensor berhasil mambaca suhu dan kelembaban
if (isnan(t) || isnan(h)) {
Serial.println("Gagal membaca sensor DHT11");
delay(1000);
return;
}
// Menampilkan suhu dan kelembaban kepada serial monitor
Serial.print("Suhu: ");
Serial.print(t);
Serial.println(" *C");
Serial.print("Kelembaban: ");
Serial.print(h);
Serial.println(" %");
Serial.println();
// Memberikan status suhu dan kelembaban kepada firebase
if (Firebase.setFloat(firebaseData, "/suhu", t)){
Serial.println("Suhu terkirim");
} else{
Serial.println("Suhu tidak terkirim");
Serial.println("Karena: " + firebaseData.errorReason());
}
if (Firebase.setFloat(firebaseData, "/kelembaban", h)){
Serial.println("Kelembaban terkirim");
Serial.println();
} else{
Serial.println("Kelembaban tidak terkirim");
Serial.println("Karena: " + firebaseData.errorReason());
}
Firebase.getString(firebaseData, "/lampu");
String value = firebaseData.stringData();
if (value == "1") {
digitalWrite(led, HIGH);
} else {
digitalWrite(led, LOW);
}
delay(1000);
}