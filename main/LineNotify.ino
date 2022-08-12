#include <SPI.h>
#include <WiFiNINA.h>

static const char* token = ""; // Line Notifyのトークン
static const char ssid[] = "";
static const char pass[] = "";
static int status = WL_IDLE_STATUS;
static const char server[] = "notify-api.line.me";
static WiFiSSLClient client;

void setupSerial() {
	// シリアル初期化
	Serial.begin(9600);
	while (!Serial);
}
void setupWifi() {
	// WiFi初期化
	if (WiFi.status() == WL_NO_MODULE) {
		Serial.println("Communication with WiFi module failed!");
		while (true);
	}
	String fv = WiFi.firmwareVersion();
	if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
		Serial.println("Please upgrade the firmware");
	}
	while (status != WL_CONNECTED) {
		Serial.print("Attempting to connect to SSID: ");
		Serial.println(ssid);
		status = WiFi.begin(ssid, pass);
		delay(10000);
	}
	Serial.println("Connected to wifi");
	printWiFiStatus();
}
void printWiFiStatus() {
	// WiFi情報表示
	Serial.print("SSID: ");
	Serial.println(WiFi.SSID());
	Serial.print("IP Address: ");
	Serial.println(WiFi.localIP());
	Serial.print("signal strength (RSSI):");
	Serial.print(WiFi.RSSI());
	Serial.println(" dBm");
}
void sendMsg(String message) {
	Serial.println("\nStarting connection to server...");
	if (client.connect(server, 443)) {
		Serial.println("connected to server");
		String query = String("message=") + String(message);
		String request = String("") +
			"POST /api/notify HTTP/1.1\r\n" +
			"Host: " + server + "\r\n" +
			"Authorization: Bearer " + token + "\r\n" +
			"Content-Length: " + String(query.length()) + "\r\n" +
			"Content-Type: application/x-www-form-urlencoded\r\n\r\n" +
			query + "\r\n";
		client.print(request);
	}
	client.stop();
	while (client.connected());
	Serial.println("disconnecting from server.");
	Serial.println();
}
void readMsg() {
	while (client.available()) {
		char c = client.read();
		Serial.write(c);
	}
}