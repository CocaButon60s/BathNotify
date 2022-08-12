#define MAX 10
#define THRESHOLD 500

void setup() {
	setupSerial();
	setupWifi();
}
int check() {
	static int count = 0;
	static int pre = (analogRead(A0) >= THRESHOLD) ? 1 : 0;

	int now = (analogRead(A0) >= THRESHOLD) ? 1 : 0;
	if (pre == now) {
		count++;
	} else {
		count = 0;
		pre = now;
	}

	Serial.print(String(count) + " ");
	
	if (count >= MAX) {
		count = MAX;
		return pre;
	} else {
		return -1;
	}

	return pre;
}
void loop() {
	static bool is_fulled = false;
	int result = check();
	if (result == 1) {
		if (!is_fulled) {
			sendMsg("お風呂が沸きました");
			is_fulled = true;
		}
	} else if (result == 0) {
		is_fulled = false;
	}
	delay(500);
}
