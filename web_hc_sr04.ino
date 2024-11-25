#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi Ayarları
const char* ssid = "Wifi_Name";        // Wi-Fi ağ adı
const char* password = "Password";          // Wi-Fi şifresi

// Pin Tanımları
const int trigPin = 12;   // D6 pinine bağlı
const int echoPin = 14;   // D5 pinine bağlı
const int ledPin = 13;    // D7 (10 cm'den küçük mesafede yanacak LED)
const int led2Pin = 15;   // D8 (10-30 cm mesafede yanacak LED)
const int led3Pin = 4;    // 30 cm'den büyük mesafede yanacak LED
const int buzzerPin = 5;  // Buzzer pinini tanımla (örneğin D1)

// Sabitler
#define SOUND_VELOCITY 0.034
#define MEASURE_ATTEMPTS 5 // Birden fazla ölçüm denemesi

long duration;
float distanceCm;
bool validMeasurement = false; // Geçerli ölçüm kontrolü

// Web Server Tanımlama
ESP8266WebServer server(80);

// Mesafe mesajı
String distanceMessage = ""; // Mesafeye göre mesaj

void setup() {
  Serial.begin(115200); // Serial iletişimi başlat
  Serial.println();
  Serial.println("Connecting to Wi-Fi...");

  // Wi-Fi bağlantısını başlat
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  // Wi-Fi bağlanınca IP adresini yazdır
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP().toString()); // IP adresi
  delay(5000); // 5 saniye bekle

  // Pin Modları
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Web server yönlendirme
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started!");
  Serial.println(WiFi.localIP());
}

void loop() {
  measureDistance();
  controlLEDs();
  server.handleClient(); // Web server isteğini işle
}

// Mesafe Ölçümü
void measureDistance() {
  float totalDistance = 0;
  int successfulReads = 0;

  for (int i = 0; i < MEASURE_ATTEMPTS; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH, 30000); // Timeout 30 ms

    if (duration > 0) {
      float currentDistance = duration * SOUND_VELOCITY / 2;
      if (currentDistance > 0 && currentDistance < 400) { // Mantıklı mesafe aralığı
        totalDistance += currentDistance;
        successfulReads++;
      }
    }
    delay(50); // Ölçümler arasında kısa bekleme
  }

  if (successfulReads > 0) {
    distanceCm = totalDistance / successfulReads; // Ortalama mesafe
    validMeasurement = true;
  } else {
    validMeasurement = false; // Hiç geçerli ölçüm yok
  }

  Serial.print("Distance (cm): ");
  if (validMeasurement) {
    Serial.println(distanceCm);
  } else {
    Serial.println("Not Measured");
  }
}

// LED ve Buzzer Kontrolü
void controlLEDs() {
  if (validMeasurement) {
    if (distanceCm < 10.00) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(led2Pin, LOW);
      digitalWrite(led3Pin, LOW);
      digitalWrite(buzzerPin, HIGH);
      distanceMessage = "STOP! You are close enough.";
    } else if (distanceCm >= 10.00 && distanceCm <= 30.00) {
      digitalWrite(ledPin, LOW);
      digitalWrite(led2Pin, HIGH);
      digitalWrite(led3Pin, LOW);
      digitalWrite(buzzerPin, LOW);
      distanceMessage = "You can get closer.";
    } else if (distanceCm > 30.00) {
      digitalWrite(ledPin, LOW);
      digitalWrite(led2Pin, LOW);
      digitalWrite(led3Pin, HIGH);
      digitalWrite(buzzerPin, LOW);
      distanceMessage = "Distance is too much.";
    }
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, LOW);
    digitalWrite(buzzerPin, LOW);
    distanceMessage = "Measurement not valid.";
  }
}

// Web Sayfasını İşleme
void handleRoot() {
  String html = "<html><head><title>Distance Sensor</title>";
  html += "<meta http-equiv='refresh' content='2'>"; // Otomatik yenileme
  html += "</head><body>";
  html += "<h1>Distance Sensor Status</h1>";

  if (validMeasurement) {
    html += "<p><strong>Distance: </strong>" + String(distanceCm, 2) + " cm</p>";
    html += "<p><strong>Message: </strong>" + distanceMessage + "</p>"; // Mesaj ekleme
    html += "<p><strong>LED and Buzzer Status:</strong></p>";
    html += "<ul>";
    html += "<li>LED1 (<10 cm): " + String(digitalRead(ledPin) == HIGH ? "ON" : "OFF") + "</li>";
    html += "<li>LED2 (10-30 cm): " + String(digitalRead(led2Pin) == HIGH ? "ON" : "OFF") + "</li>";
    html += "<li>LED3 (>30 cm): " + String(digitalRead(led3Pin) == HIGH ? "ON" : "OFF") + "</li>";
    html += "<li>Buzzer: " + String(digitalRead(buzzerPin) == HIGH ? "ON" : "OFF") + "</li>";
    html += "</ul>";
  } else {
    html += "<p><strong>Distance:</strong> Not Measured</p>";
    html += "<p><strong>Message:</strong> Measurement not valid.</p>"; // Geçersiz mesaj
  }

  html += "</body></html>";

  server.send(200, "text/html", html);
}
