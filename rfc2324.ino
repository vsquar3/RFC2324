#include <EtherCard.h>

static byte mymac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

byte myipfornow[] = { 10,0,0,177 };
byte gwipfornow[] = { 10,0,0,138 };

byte Ethernet::buffer[700];

int brewPin = 2;
int warmPin = 4;

void setup(){
	Serial.begin(9600);
	if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    	Serial.println( "Failed to access Ethernet controller");

	if (!ether.dhcpSetup()){
    	Serial.println("DHCP failed");
    	Serial.println("Using static addresses");
    	ether.staticSetup(myipfornow, gwipfornow);
    } else {
    	Serial.println("DHCP lease aquired");
    }

	ether.printIp("IP:  ", ether.myip);
	ether.printIp("GW:  ", ether.gwip);
	ether.printIp("DNS: ", ether.dnsip);

	pinMode(brewPin, OUTPUT);
	pinMode(warmPin, OUTPUT);

}

void startBrew(){
	if (!digitalRead(brewPin)) {
		Serial.println("Starting to brew");
		digitalWrite(brewPin, HIGH);
	} else {
		Serial.println("Already brewing");
	}
}

void stopBrew(){
	if (!digitalRead(brewPin)) {
		Serial.println("Not brewing");
	} else {
		Serial.println("Stopping brew");
		digitalWrite(brewPin, LOW);
	}
}

void startWarming(){
	if (!digitalRead(warmPin)) {
		Serial.println("Starting to warm coffee up");
		digitalWrite(warmPin, HIGH);
	} else {
		Serial.println("Already warming");
	}
}

void stopWarming(){
	if (!digitalRead(warmPin)) {
		Serial.println("Not warming");
	} else {
		Serial.println("Stoping the hot plate");
		digitalWrite(warmPin, LOW);
	}
}

const char cofffee_reponse_BREW[] PROGMEM =
"COFFEE/1.0 200 OK\r\n"
"Content-Type: application/coffee-pot-response\r\n"
"Status: brewing\r\n\r\n";

const char cofffee_response_WHEN[] PROGMEM =
"COFFEE/1.0 200 OK\r\n"
"Content-Type: application/coffee-pot-response\r\n"
"Status: stopped\r\n\r\n";

const char cofffee_response_WARM[] PROGMEM =
"COFFEE/1.0 200 OK\r\n"
"Content-Type: application/coffee-pot-response\r\n"
"Status: warming\r\n\r\n";

void loop(){
	/*if (!STATIC && ether.dhcpExpired()) {
	    Serial.println("Acquiring DHCP lease again");
	    ether.dhcpSetup();
  	}*/

	// Removed because apparently this ethercard library doesnt have the dhcpExpited function anymore :(

  	word len = ether.packetReceive();
	word pos = ether.packetLoop(len);

	if(pos){
		BufferFiller buff = ether.tcpOffset();
		char *data = (char *) Ethernet::buffer + pos;
	    if (!strncmp("BREW /1", data, 7)) {
	    	startBrew();
	    	startWarming();
	    	buff.emit_p(cofffee_reponse_BREW);
	    } else if (!strncmp("WHEN /1", data, 7)) {
	    	stopWarming();
	    	stopBrew();
	    	buff.emit_p(cofffee_response_WHEN);
	    } else if (!strncmp("GET /1", data, 6)) {
	    	if (digitalRead(brewPin)) {
	    		buff.emit_p(cofffee_reponse_BREW);
	    	} else if (!digitalRead(brewPin) && digitalRead(warmPin)) {
	    		buff.emit_p(cofffee_response_WARM);
	    	} else {
	    		buff.emit_p(cofffee_response_WHEN);
	    	}
	    }

	    ether.httpServerReply(buff.position());
	}
}