
// Bibliotheken werden eingebunden  


 
#include <WiFiClientSecure.h>
#include <PubSubClient.h> // install with Library Manager, I used v2.6.0
#include <OneWire.h>  
#include <math.h> 
#include <SPI.h>
#include <WiFi.h>
#include <DallasTemperature.h>  
#include "time.h"


// Hier wird der Eingangs-Pin deklariert, an dem das Sensor-Modul angeschlossen ist
#define KY001_Signal_PIN 4



// Libraries werden konfiguriert
OneWire oneWire(KY001_Signal_PIN);          
DallasTemperature sensors(&oneWire);




//WLAN Verbindung: Variablen werden deklariert
const char* ssid = "only4dankmemes";
const char* password = "17051996";

const char* awsEndpoint = "a2xcttado1dq90-ats.iot.eu-west-1.amazonaws.com"; //AWS-Endpoint wird deklariert

// Variablen Deklaration MacAdresse:
#define BUFFER_LEN 512
#define BUFFER_TI 256
long lastMsg = 0;
char msg[BUFFER_LEN];
char ti[BUFFER_TI];
int value = 0;
byte mac[6];
char mac_Id[18];
char timee[18];

// Zeitvariablen
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

// AWS Zertifikate deklarieren und zuweisen 
// Anführungszeichen zu Beginn jeder Zeile und  \n" \ am Ende jeder Zeile 

// xxxxxxxxxx-certificate.pem.crt
const char* certificate_pem_crt = \

"-----BEGIN CERTIFICATE-----\n" \
"MIIDWjCCAkKgAwIBAgIVALXo1mX8Og5i86uK/RhnRrjrwaa+MA0GCSqGSIb3DQEB\n" \
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n" \
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDAzMjcwOTEw\n" \
"MDBaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n" \
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC52PQw+SWAzAgF9Ue8\n" \
"LJ7WOddNqcJjJZwLmQXJx2fvPy5j3cU7B2JGfMg0TH0+OBZ3q3yKCoYLoduaUFwJ\n" \
"tkd9hh5V5i/XbRwKdinE4cGvwB4Z9fufF3HrOKVz1HxU1nmzq7Pbm7vAdzOKPNci\n" \
"+88/1Jqvyne0c/KfLoN9nbsxVLdeAgRmMd5egIBjbVq/w0G3uGz1ZIgK80gGYrxV\n" \
"wY1zaJF2N+ovnd9N+in5+SUY6tQwswus0LKKv4tzfnhmeKDASDSAnfIed5BVhwiE\n" \
"tyJkHHuucrFYUX5pCg40iQTMlNxNMQFkjx+FLIbo+6cW1PP7cWDu8tw6NlqPNSj1\n" \
"6/LTAgMBAAGjYDBeMB8GA1UdIwQYMBaAFP/GPK1ichI0rjIoSC1vBjaqbUgdMB0G\n" \
"A1UdDgQWBBSYZvQe3b7u+lNmw5kvEl4IPNtJTzAMBgNVHRMBAf8EAjAAMA4GA1Ud\n" \
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEALtKYllDakiX2C4+k3gakMPkF\n" \
"x36BSAOv5jLOAS/PD7AzHrEj+ZMK9imZrHz4C2u0XZ10ET2TyXYx0ym9Qsslwfhi\n" \
"MQIBfjZYOXqTPAgV2llAmrJ9RcUzNco8uRpe54GHpzcum3CCbIihPOBPTNn8DPYe\n" \
"VzdRh+6w4Htnuz97XM69ClvEhevEQMGpKiuHvWKv6pVQj2s914XMb6nX9yI+h2DY\n" \
"dXizjk5a2FJVNCHva9ln4srLim610nYGvKwK2UuUOVkq8hGVkpTAT+1r1+7pLeOm\n" \
"lWH4bSUwict5QsQFqiUFSCbeAYlF5cAZbTvqgOb46H02Y3YF5UDI1FvzZwhvvQ==\n" \
"-----END CERTIFICATE-----\n";

// xxxxxxxxxx-private.pem.key
const char* private_pem_key = \

"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEpAIBAAKCAQEAudj0MPklgMwIBfVHvCye1jnXTanCYyWcC5kFycdn7z8uY93F\n" \
"OwdiRnzINEx9PjgWd6t8igqGC6HbmlBcCbZHfYYeVeYv120cCnYpxOHBr8AeGfX7\n" \
"nxdx6zilc9R8VNZ5s6uz25u7wHczijzXIvvPP9Sar8p3tHPyny6DfZ27MVS3XgIE\n" \
"ZjHeXoCAY21av8NBt7hs9WSICvNIBmK8VcGNc2iRdjfqL53fTfop+fklGOrUMLML\n" \
"rNCyir+Lc354ZnigwEg0gJ3yHneQVYcIhLciZBx7rnKxWFF+aQoONIkEzJTcTTEB\n" \
"ZI8fhSyG6PunFtTz+3Fg7vLcOjZajzUo9evy0wIDAQABAoIBAQCP4BWrlDYkb9AG\n" \
"TM0sMJ8G2eCEj1edyg/0KpMAkw8zeIZBaf62pBDA4PldA5oyANnj6y6O73a2qqeH\n" \
"345hNmxCEQKLHZkL/bwnnTLY6K05klG7Z2NU1qGqqD/uXCGkzCWLovGqofXIGoAc\n" \
"FN24h5j6oWzfiq8BVIIQGcJSDTpOBSX1Z/lG7IeUxU9Pb895wjo9TcVQHQ7/zVAk\n" \
"ZTnnCxep9nHlEzoPIUXEBLODU/TPEEuDZ1CX0EgrKIx1V936OGdR82OdzyjoPgWn\n" \
"8eUXhks9hzuU1AfWnfKGcPOtzgkl/wDP2RAjYnM66r78t2TPKsrvJow8BMUvtNSV\n" \
"qiBlBoCBAoGBANr5aPixdSfDeM02kLiEqSNEOlteh06p/muHwUM3n3gMtfw8YT4o\n" \
"7AoWLyl97RPvWKDbwsyYd2N/yMeAvC3FRwKqmZQZKRkij+vJ/1+Z2/cE21iUkOml\n" \
"TkBrhCD08dzNzSpaPxl829/zro3ELhvI2P1XhF2VeKhE+BLSRDon6DJFAoGBANlF\n" \
"m3ltBKcZOrOivAYMMDggTGfSRUqr0DWdHsb2OfyXTr0hRTKss9OTnN26vOizWq88\n" \
"dmb3Hp3wzaJ0jbfuBKRpBhyQNj8nJF0RQ542mw33a8xVoBTTMgU2voyecQQMn/DY\n" \
"QPE1By0DaiTc1r1n5q4X2VZ9eoUSGDvl9ZWuae43AoGAHruOfgj59mC63aJg4WTN\n" \
"EEQNJLG5r9KwAIuHFlYCLakrCYG37C92IVxHtj0UPw4ClGEDkA9+Ahr3JPjucEYV\n" \
"10v2ThDR8dMl++PX0aUYeCzYVDBVvlUTDHuKic2W/HRt7RnJbVxWx7G9t+vmxMOF\n" \
"oNjdnPzTeFplrs6QANGFBzkCgYEAsJO/jWmqVFtOt9tTz95r+dFWBzJ+mfcUmd39\n" \
"u1k1OzGFDnRRlqOA6jyXSiSPjtbkpnIPEBNaUKX5mUP8QTUxlKS+EMyrKZwYE4MK\n" \
"XzSlwy4H7+LO3N3fUxlwtzu1QcM44TX/YYjlMhIKW7cW1ovwkCBVw9LqledkxplX\n" \
"g7Rh5vUCgYAOo24xAwjph5zJUGfLhyMmWeqc4qtXWgQUBe4DElSTdmfdrwV9X2xi\n" \
"ktFBnZAD7SrB6QxfNbiZ1nFHP160cHL4k/Y6OhpLY7+Y087kVSAwywDy/lOOVK6R\n" \
"Liq3UR2yUY/S+Ht5Of7idV2xGExQGb9EfTZQwyQTuOgOUudwXfqR2Q==\n" \
"-----END RSA PRIVATE KEY-----\n";
/* root CA can be downloaded in:
  https://www.symantec.com/content/en/us/enterprise/verisign/roots/VeriSign-Class%203-Public-Primary-Certification-Authority-G5.pem
*/
const char* rootCA = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----\n";



// Der WifiClientSecure unterstützt bei der sicheren Verbindung über TLS (SSL)
WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient);



void setup() {
  
  // Ausgabe von Baudrate im seriellen Monitor 
  Serial.begin(115200); delay(50); Serial.println();
  // Sensor wird initialisiert
    sensors.begin(); 
  Serial.println("ESP32 AWS IoT Example");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());
  
  // WLAN Verbindung und Ausgabe der IP-Adresse im seriellen Monitor, wenn die Verbindung funktioniert hat
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  WiFi.macAddress(mac);  // Holt Mac Adresse vom ESP
snprintf(mac_Id, sizeof(mac_Id), "%02x:%02x:%02x:%02x:%02x:%02x",
mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
Serial.print(mac_Id); //Umwandlung in String

 // Zertifikate einbinden
  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);

 //init and get the time
 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);


}

// Variablen deklarieren
unsigned long lastPublish;
int msgCount;


void loop() {


delay(4000);
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");



 // Temperaturmessung wird gestartet...
    sensors.requestTemperatures(); 
    // ... und gemessene Temperatur ausgeben
    Serial.print("Temperatur: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.write(176); // UniCode-Angabe eines char-Symbols für das "°-Symbol"
    Serial.println("C");
         
    delay(4000);    // 5s Pause bis zur nächsten Messung

  pubSubCheckConnect(); 

 int month = (timeinfo.tm_mon + 1);
 int year = (timeinfo.tm_year + 1900);

  String macIdStr = mac_Id;  //Umwandlung in String - Übergabe ins Ausgabevariable

snprintf (ti, BUFFER_TI, "%02d.%02d.%d, %02d:%02d:%02d", timeinfo.tm_mday, month, year, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
snprintf (msg, BUFFER_LEN, "{\"mac_Id\":\"%s\",\"uptime\":%lu,\"temp\":%f,\"time\":\"%s\"}", macIdStr.c_str(), millis() / 1000, sensors.getTempCByIndex(0), ti); //Speichern der MacID, Uptime, Temperatur in msg-Variable

  
  if (millis() - lastPublish > 60000) { //Festlegen wie oft die Nachricht an AWS gesendet wird
      lastPublish = millis();
 
  boolean rc = pubSubClient.publish("Test0805", msg);  //mqttClient.publish("outTopic", msg); Ausgabe von msg im AWS-Topic


  //Ausgabe von msg im seriellen Monitor
  Serial.print("Publish message: ");
  Serial.println(msg);

  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// AWS Verbindung und Ausgabe im seriellen Monitor, wenn die Verbindung funktioniert hat
void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) { //checkt, ob der Client noch mit dem Server verbunden ist 
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint); //Ausgabe des AWS Endpoints im seriellen Monitor
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthingXXXX");
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe("Test0805"); // abonniert das angegebene Thema
  }
  pubSubClient.loop(); // ermöglicht, dass der Client eingehende Nachrichten verarbeiten und die Verbindung zum Server aufrechterhalten kann
}
