#include <GSM.h>

#define PINNUMBER ""//write the pinnumber 

// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;
int button = 5;
int value;
int led = 4;
bool smsToTechnicianSent = false;
bool smsToTeamLeaderSent = false;

void setup() {
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("SMS Messages Sender");

  bool notConnected = true;
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM initialized");
}

void loop() {
  value = digitalRead(button);
  if (value == LOW && !smsToTechnicianSent) {  // Button pressed
    digitalWrite(led, HIGH);  // Turn on LED as an indicator
    char Numtech[20] = "";//write the technicien num

    Serial.print("Sending message to technician: ");
    sms.beginSMS(Numtech);
    sms.print("Il y a un defaut dans la machine de presse d'etain. Veuillez verifier!");
    sms.endSMS();
    Serial.println("\nMessage sent to technician!\n");

    smsToTechnicianSent = true;
    //delay(900000);// Wait for 15 minutes before sending the next SMS
    delay(10000);//Wait for 10 minutes before sending the next SMS  
    char NumLeader[20] = "";//write the leader num

    Serial.print("Sending message to team leader: ");
    sms.beginSMS(NumLeader);
    sms.print("Le technicien a ete averti. Veuillez suivre l'etat.");
    sms.endSMS();
    Serial.println("\nMessage sent to team leader!\n");

    smsToTeamLeaderSent = true;
  }

  // Check for incoming SMS messages
  if (sms.available()) {
    Serial.print("Message received from: ");
    char senderNumber[20];
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);

    
    char c;
    String message = "";
    while ((c = sms.read()) != '\0') {
      message += c;
    }
    Serial.println("Message content: " + message);

    // Check if the message contains "ok" (case insensitive)
    message.toLowerCase();  
    if (message.indexOf("ok") != -1) {
      digitalWrite(led, LOW);  // Turn off the LED
      Serial.println("LED turned off by SMS command.");
    }

    sms.flush();  
  }
}
