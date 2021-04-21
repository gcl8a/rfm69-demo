#define USE_TRADITIONAL_SPI

#if defined(ARDUINO_SAMD_FEATHER_M0) // Feather M0 w/Radio
  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
  #define LED           13
  #define SerialUSB Serial
  #define MYNODEID    0  // My node ID (0 to 255)
  #define SENDTOID    1  // Where we'll send to
#else 
  #define RFM69_CS      10
  #define RFM69_INT     3
  #define RFM69_RST     4
  #define LED           2
  #define MYNODEID    1  // My node ID (0 to 255)
  #define SENDTOID    0  // Where we'll send to
#endif
//RFM69 radio(10, 3, false, digitalPinToInterrupt(3));

#include <RFM69.h>

// Address for this node
#define NETWORKID   100   // Must be the same for all nodes (0 to 255)

// RFM69 frequency:
#define FREQUENCY     RF69_915MHZ

// AES encryption (or not):
#define ENCRYPT       false // Set to "true" to use encryption
#define ENCRYPTKEY    "TOPSECRETPASSWRD" // Use the same 16-byte key on all nodes

// Use ACKnowledge when sending messages (or not):
#define USEACK        false // Request ACKs or not

// Create a library object for our RFM69HCW module:
RFM69 radio(RFM69_CS, RFM69_INT, false, digitalPinToInterrupt(RFM69_INT));

void setup()
{
  // Open a serial port so we can send keystrokes to the module:
  SerialUSB.begin(115200);
  //while(!SerialUSB) {} //COMMENT OUT TO RUN WITHOUT REQUIRING SERIAL
  SerialUSB.println("Hello");

  SerialUSB.println(SCK);
  SerialUSB.println(MOSI);
  SerialUSB.println(MISO);
  SerialUSB.println(SS);

  SerialUSB.println(RF69_SPI_CS);
  SerialUSB.println(RF69_IRQ_PIN);
  SerialUSB.println(RF69_IRQ_NUM);
    
  // Initialize the RFM69HCW:
  radio.initialize(FREQUENCY, MYNODEID, NETWORKID);
  radio.setHighPower(); // Always use this for RFM69HCW
  //radio.setPowerLevel(0);
  radio.promiscuous(true);
  
  // Turn on encryption if desired:
  if (ENCRYPT)
    radio.encrypt(ENCRYPTKEY);
}

void loop()
{
  //We're just going to listen here
  if(radio.receiveDone()) // Got one!
  {
    for (byte i = 0; i < radio.DATALEN; i++)
      SerialUSB.print((char)radio.DATA[i]);
  }

  static unsigned long lastSend = 0;
  if(millis() - lastSend > 2000)
  {
    SerialUSB.println(lastSend);

    lastSend = millis();
    char sendStr1[] = "214:5.00:219\n";
    radio.send(SENDTOID, sendStr1, strlen(sendStr1));
  }
}