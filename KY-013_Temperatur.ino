#include <math.h>
#include "WiFi.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>

#ifndef certs_h
#define certs_h

// Amazon's root CA. This should be the same for everyone.
const char AWS_CERT_CA[] = "-----BEGIN CERTIFICATE-----\n" \
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

// The private key for your device
const char AWS_CERT_PRIVATE[] = "-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEowIBAAKCAQEA1JJOP02t6yjOnDisyVrVGDcNogWC5RZC13FIzO7Bo8Tn/KOe\n" \
"YuHqlhWIeTifNUFcY2j5HpSXuuMICddtVNOsCjNu/OnK09hpveJvgNLZbW+DfxSB\n" \
"Ew6APtRgy4SMePhusgOaaBSjUkbgAdfkbBqnraBUa1gRDVkNLqcVId1kgXXCv0qH\n" \
"50LElZMVSBVAUonTXQVv3OjvDbO6Xqg3FHpOBUvcyjKvwIA+HMYgote0QMtAD3ql\n" \
"/gSai7uAABO9RQOcldj5CBCi9/xr3O04AY8cUcxosGT1/8CAsL2TasLd13ma1xVI\n" \
"MMTMpJRT+L06mkXzrOpCxHFXLoFxPX5rbT9MsQIDAQABAoIBABsrzFapHjb101sC\n" \
"n4tpUd2zg8mW/ME9lDOnpk+ToSrGy4RugZtQF0Ux4WzlQOx0u1N1ooo3IiHgTTG8\n" \
"2JvEJrRzGJe5ZpSnj/SWkfwcd3BWla9Ttp/x9ZCSYwEAo1f/IV8duMnfAXVN2+pA\n" \
"Z1EuIwq9eKzz59IV9OpmrvvSKbumTv0IGNYco0VDBEhnPkMteYAQ95nzaCg2xSBw\n" \
"wwpzX4NUMI+GYLYGg8calyKmfTkiDAAcbEJBlPLvwxwVWqMNdMvrEYUtQ5aQEq7+\n" \
"f4msXF/08/pefKwStPVa2EbEjT67jFVOj9LEUKY1fLbSn9dxxZQ9MrvSHGVU7Rgu\n" \
"f6akz1UCgYEA+4rj6cICOK8P7vlCwYcP/YaKd0H9egWulxX3QQ0C6fxHgSO3H2Q5\n" \
"Jub7uhs1YgO38jj7/GvJsSvC064DivYFtLl1S5DwNhEjgf2E8MN2tY0sGGFAci4l\n" \
"12/ZkkesukjCfMgddCs82K4Xcq0zt0e6Prd7+IhWxm/UfCkgPGlGKZMCgYEA2Fag\n" \
"EwfWzDOTVGjvUs5q7pMkhwRCZYyPeNCIhAD5m4KvWAW9WiKLD9OHhzi1m78hRA3L\n" \
"kf0iJuBFQTaDwH2+3YUs9kgxfCgKt/dkKsFqBn6qJ+4CWQSRANYPMmF+tuC+ipyO\n" \
"Hh6GWeUA3tc/HCZ1rCg6ZnDiqjAK+h5fmDK/CysCgYBpsxRQfwDV9uI4Ji/iCLI0\n" \
"c5utv2IXK1oRDN7oS+Jklmkpn+tsWn4VlcxDiPlmpW9fboicqmKHe38T2jSM6RK+\n" \
"vYvHZDE8VvlXv0NktI4mfhsAZ6VgFXo7TDnd2RaA9OAc9S5DSYP4/TbHLnbj1lUx\n" \
"04JZjlkAa6+bOODd4dLk1wKBgHSP3/nA/IPTuir9ywzzuPvIfiLdMaUjYd43hYaS\n" \
"O+sFM9KabRaWjBS/5UL/VCKRjavsPuXu3KrTpZzocX6Sgfc7HSmpZjg+HuBWRDsh\n" \
"MS5EqwuOmuc5paMsss1zaGHCoAdxKMWjrsHauGhY02Eoe7iBiE3PvuDxt8hh+yG3\n" \
"HeY3AoGBAO9Se4GkG4JijJAhcIcBjeAjAfUohTinPGbcv7QmhTDA4BWrDc/OVrC2\n" \
"OyZn99G6EInFE94RN6HPwDo20HbLXk3u565AUF9ETdmjzc3EpQd5XyfJ0xWT8Rgn\n" \
"YMYLNyJcX2F5Mwyzq7CjE+C4pLbOoJHgSanE5DT17qOEtuAb0XzW\n" \
"-----END RSA PRIVATE KEY-----\n";

// The certificate for your device
const char AWS_CERT_CRT[] = "-----BEGIN CERTIFICATE-----\n" \
"MIIDWTCCAkGgAwIBAgIUQcpZkB/1jwu8iFuzrh03PVoWJ3YwDQYJKoZIhvcNAQEL\n" \
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n" \
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMDMwNDE3Mzcz\n" \
"N1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n" \
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANSSTj9Nresozpw4rMla\n" \
"1Rg3DaIFguUWQtdxSMzuwaPE5/yjnmLh6pYViHk4nzVBXGNo+R6Ul7rjCAnXbVTT\n" \
"rAozbvzpytPYab3ib4DS2W1vg38UgRMOgD7UYMuEjHj4brIDmmgUo1JG4AHX5Gwa\n" \
"p62gVGtYEQ1ZDS6nFSHdZIF1wr9Kh+dCxJWTFUgVQFKJ010Fb9zo7w2zul6oNxR6\n" \
"TgVL3Moyr8CAPhzGIKLXtEDLQA96pf4Emou7gAATvUUDnJXY+QgQovf8a9ztOAGP\n" \
"HFHMaLBk9f/AgLC9k2rC3dd5mtcVSDDEzKSUU/i9OppF86zqQsRxVy6BcT1+a20/\n" \
"TLECAwEAAaNgMF4wHwYDVR0jBBgwFoAUw90NXB0drBTrI6Gq4gawCxCCgAUwHQYD\n" \
"VR0OBBYEFIRqt+7KTUfsmSHCe54g07y26g/kMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n" \
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQDHs9KBfRsJUW2TwYITx//r8Qrh\n" \
"rxwC+bfLxU7OnHksXsaxxzAVXqdB/7mTjVTMm0RrO7ESnV/6NI5H9rSNKykx8C+i\n" \
"x6vDFLAP4M+Zbqjn3cMPkn5WJUVHKBXVL9DI2dfjtmNHHrjDrPJ3IyQDi9lRThLg\n" \
"cuOzoqQM7U5nMUnXkCIaWn4MQol310bC3hQiR2+RIr7J7zSbNVSdJfHeNWPBZ8yX\n" \
"p06+T3OgdiejbNj/sy6vunIsUsJojQUM33MXz33Z/VECgb9MhXLXD4KeEWTWs/XD\n" \
"nTCuatVpoBZrN6q5wLaJjA/Fj3nwH0+iTA8/hv7w2FZWQx78CjcUQIumvFD2\n" \
"-----END CERTIFICATE-----\n";

#endif

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient();

// Wifi credentials
const char *WIFI_SSID = "UPC9599591";
const char *WIFI_PASSWORD = "8fznhRjhU2wf";

void setup(void) {
{ 
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Wifi connected");
  connectToAWS();

  // Only try 15 times to connect to the WiFi
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 15){
    delay(500);
    Serial.print(".");
    retries++;
  }

}

// The name of the device. This MUST match up with the name defined in the AWS console
#define DEVICE_NAME "Temp-Sensor1"

// The MQTTT endpoint for the device (unique for each AWS account but shared amongst devices within the account)
#define AWS_IOT_ENDPOINT "a2xcttado1dq90-ats.iot.eu-central-1.amazonaws.com"

// The MQTT topic that this device should publish to
#define AWS_IOT_TOPIC "$aws/things/Temp-Sensor1/shadow/update"

// How many times we should attempt to connect to AWS
#define AWS_MAX_RECONNECT_TRIES 50
 
}

void connectToAWS()
{
  // Configure WiFiClientSecure to use the AWS certificates we generated
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Try to connect to AWS and count how many times we retried.
  int retries = 0;
  Serial.print("Connecting to AWS IOT");

  while (!client.connect(DEVICE_NAME) && retries < AWS_MAX_RECONNECT_TRIES) {
    Serial.print(".");
    delay(100);
    retries++;
  }

  // Make sure that we did indeed successfully connect to the MQTT broker
  // If not we just end the function and wait for the next loop.
  if(!client.connected()){
    Serial.println(" Timeout!");
    return;
  }

  // If we land here, we have successfully connected to AWS!
  // And we can subscribe to topics and send messages.
  Serial.println("Connected to AWS!");
}

int sensorPin = 33; // Hier wird der Eingangs-Pin deklariert

// Diese Funktion übersetzt den aufgenommenen analogen Messwert
// in die entsprechende Temperatur in °C und gibt diesen aus
double Thermistor(int RawADC)
{
	int Temp;
  
	Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
	Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
	Temp = Temp - 273.15;            // Konvertierung von Kelvin in Celsius
	return Temp;
}

// Das Programm misst den aktuellen Spannungswert am NTC
// und übersetzt das Ergebnis in °C für die serielle Ausgabe

void loop()
{
  int readVal = analogRead(sensorPin);
  double temp =  Thermistor(readVal);
  
  

  // Ausgabe auf die serielle Schnittstelle
  Serial.print("Aktuelle Temperatur ist:");
  Serial.println(int(Thermistor(analogRead(0))));  
        Serial.print(char(186)); //Ausgabe <°> Symbol
  Serial.println("C");
  Serial.println("---------------------------------------");

  delay(500);

sendJsonToAWS();
  client.loop();
  delay(1000);


}


void sendJsonToAWS()
{
  StaticJsonDocument<512> jsonDoc;
  JsonObject stateObj = jsonDoc.createNestedObject("state");
  JsonObject reportedObj = stateObj.createNestedObject("reported");
  
  // Write the temperature & humidity. Here you can use any C++ type (and you can refer to variables)
  
  reportedObj["temperature"] = ("temp");
  


  Serial.println("Publishing message to AWS...");
  //serializeJson(doc, Serial);
  char jsonBuffer[512];
  serializeJson(jsonDoc, jsonBuffer);

  client.publish("Temp-Sensor1", jsonBuffer);
}
