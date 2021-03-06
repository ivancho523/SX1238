
#include <SPI.h>           //included with Arduino IDE install (www.arduino.cc)

// **********************************************************************************
// SX1238 Internal registers addresses
//**************************************************
#define REG_FIFO          0x00
#define REG_OPMODE        0x01
#define REG_BITRATEMSB    0x02
#define REG_BITRATELSB    0x03
#define REG_FDEVMSB       0x04
#define REG_FDEVLSB       0x05
#define REG_FRFMSB        0x06
#define REG_FRFMID        0x07
#define REG_FRFLSB        0x08
#define REG_PACONFIG      0x09
#define REG_PARAMP        0x0A
#define REG_OCP           0x0B
#define REG_LNA           0x0C
#define REG_RXCONFIG      0x0D
#define REG_RSSICONFIG    0x0E
#define REG_RSSICOLLISION 0x0F
#define REG_RSSITHRESH    0x10
#define REG_RSSIVALUE     0x11
#define REG_RXBW          0x12
#define REG_AFCBW         0x13
#define REG_OOKPEAK       0x14
#define REG_OOKFIX        0x15
#define REG_OOKAVG        0x16
#define REG_AFCFEI        0x1A
#define REG_AFCMSB        0x1B
#define REG_AFCLSB        0x1C
#define REG_FEIMSB        0x1D
#define REG_FEILSB        0x1E
#define REG_PREAMBLEDETECT  0x1F
#define REG_RXTIMEOUT1    0x20
#define REG_RXTIMEOUT2    0x21
#define REG_RXTIMEOUT3    0x22
#define REG_RXDELAY       0x23
#define REG_OSC           0x24
#define REG_PREAMBLEMSB   0x25
#define REG_PREAMBLELSB   0x26
#define REG_SYNCCONFIG    0x27
#define REG_SYNCVALUE1    0x28
#define REG_SYNCVALUE2    0x29
#define REG_SYNCVALUE3    0x2A
#define REG_SYNCVALUE4    0x2B
#define REG_SYNCVALUE5    0x2C
#define REG_SYNCVALUE6    0x2D
#define REG_SYNCVALUE7    0x2E
#define REG_SYNCVALUE8    0x2F
#define REG_PACKETCONFIG1 0x30
#define REG_PACKETCONFIG2 0x31
#define REG_PAYLOADLENGTH 0x32
#define REG_NODEADRS      0x33
#define REG_BROADCASTADRS 0x34
#define REG_FIFOTHRESH    0x35
#define REG_SEQCONFIG1    0x36
#define REG_SEQCONFIG2    0x37
#define REG_TIMERRESOL    0x38
#define REG_TIMER1COEF    0x39
#define REG_TIMER2COEF    0x3A
#define REG_IMAGECAL      0x3B
#define REG_TEMP          0x3C
#define REG_LOWBAT        0x3D
#define REG_IRQFLAGS1     0x3E
#define REG_IRQFLAGS2     0x3F
#define REG_DIOMAPPING1   0x40
#define REG_DIOMAPPING2   0x41
#define REG_VERSION       0x42
#define REG_AGCREF        0x43
#define REG_AGCTHRESH1    0x44
#define REG_AGCTHRESH2    0x45
#define REG_AGCTHRESH3    0x46
#define REG_TCXO          0x58
#define REG_PADAC         0x5A
#define REG_PLL           0x5C
#define REG_PLLLOWPN      0x5E
#define REG_FORMERTEMP    0x6C
#define REG_BITRATEFRAC   0x70

#define SERIAL_BAUD       115200
#define SS                4
#define TX_EN             7
#define RX_EN             9
#define MODE              6
#define MAX_DATA_LEN      61   //may not need later
#define TX_LIMIT_MS       1000 //may not need later
#define ADDRESS           2   //may not need later
#define DIO0_INTERRUPT    3
#define RESET             2 



// TWS: define CTLbyte bits
#define CTL_SENDACK   0x80     //may not need later
#define CTL_REQACK    0x40     //may not need later

// RegIrqFlags1
#define RF_IRQFLAGS1_MODEREADY            0x80
#define RF_IRQFLAGS1_RXREADY              0x40
#define RF_IRQFLAGS1_TXREADY              0x20
#define RF_IRQFLAGS1_PLLLOCK              0x10
#define RF_IRQFLAGS1_RSSI                 0x08
#define RF_IRQFLAGS1_TIMEOUT              0x04
#define RF_IRQFLAGS1_PREAMBLEDETECT       0x02
#define RF_IRQFLAGS1_SYNCADDRESSMATCH     0x01

// RegIrqFlags2
#define RF_IRQFLAGS2_FIFOFULL             0x80
#define RF_IRQFLAGS2_FIFOEMPTY            0x40
#define RF_IRQFLAGS2_FIFOLEVEL            0x20
#define RF_IRQFLAGS2_FIFOOVERRUN          0x10
#define RF_IRQFLAGS2_PACKETSENT           0x08
#define RF_IRQFLAGS2_PAYLOADREADY         0x04
#define RF_IRQFLAGS2_CRCOK                0x02
#define RF_IRQFLAGS2_LOWBAT               0x01 

// RegDioMapping1
#define RF_DIOMAPPING1_DIO0_00            0x00  // Default
#define RF_DIOMAPPING1_DIO0_01            0x40
#define RF_DIOMAPPING1_DIO0_10            0x80
#define RF_DIOMAPPING1_DIO0_11            0xC0

#define RF_DIOMAPPING1_DIO1_00            0x00  // Default
#define RF_DIOMAPPING1_DIO1_01            0x10
#define RF_DIOMAPPING1_DIO1_10            0x20
#define RF_DIOMAPPING1_DIO1_11            0x30

#define RF_DIOMAPPING1_DIO2_00            0x00  // Default
#define RF_DIOMAPPING1_DIO2_01            0x04
#define RF_DIOMAPPING1_DIO2_10            0x08
#define RF_DIOMAPPING1_DIO2_11            0x0C

#define RF_DIOMAPPING1_DIO3_00            0x00  // Default
#define RF_DIOMAPPING1_DIO3_01            0x01
#define RF_DIOMAPPING1_DIO3_10            0x02
#define RF_DIOMAPPING1_DIO3_11            0x03


//transceiver modes
#define SX1238_MODE_SLEEP         0 //none
#define SX1238_MODE_STANDBY       1 // Top regulator and crystal oscillator
#define SX1238_MODE_SYNTH_TX      2 // Frequency synthesizer at Tx frequency (Frf)
#define SX1238_MODE_TX            3 // Frequency synthesizer and transmitter
#define SX1238_MODE_SYNTH_RX      4 // Frequency synthesizer at frequency for reception (Frf-IF)
#define SX1238_MODE_RX            5 // Frequency synthesizer and receiver

//public
static volatile uint8_t _mode;
static volatile uint8_t DATALEN;
static volatile uint8_t SENDERID;
static volatile uint8_t TARGETID; // should match ADDRESS
static volatile uint8_t PAYLOADLEN;
static volatile uint8_t ACK_REQUESTED;
static volatile uint8_t ACK_RECEIVED; // should be polled immediately after sending a packet with ACK request
static volatile int16_t RSSI; // most accurate RSSI during reception (closest to the reception)
static volatile uint8_t DATA[MAX_DATA_LEN]; // recv/xmit buf, including header & crc bytes


void setup() {
  //setup serial
  Serial.begin(SERIAL_BAUD);

  //setup control pins
  pinMode(TX_EN, OUTPUT);
  pinMode(RX_EN, OUTPUT);
  pinMode(MODE, OUTPUT);

  // Determine the interrupt number that corresponds to the interruptPin
  int interruptNumber = digitalPinToInterrupt(DIO0_INTERRUPT);

  attachInterrupt(interruptNumber, handleInterrupt, RISING);

  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, LOW);


  // manual reset
  digitalWrite(RESET, HIGH);
  delay(10);
  digitalWrite(RESET, LOW);
  delay(10);

  //setup cs pin
  digitalWrite(SS, HIGH);
  pinMode(SS, OUTPUT);

  //setup spi
  SPI.begin();

  writeRegister(REG_PACONFIG, 0x00); //output power to 0 for receive
  
  writeRegister(REG_FIFOTHRESH, 0x8f); //fifo start condition not empty

  writeRegister(REG_PACKETCONFIG1, 0x80); //turn off crc
  writeRegister(REG_PACKETCONFIG2, 0x40); //packet mode

  writeRegister(REG_PREAMBLEMSB, 0x00); //preamble length
  writeRegister(REG_PREAMBLELSB, 0x03);

  writeRegister(REG_FRFMSB, 0xe4); //frequency 915MHz
  writeRegister(REG_FRFMID, 0xc0);
  writeRegister(REG_FRFLSB, 0x00);

  writeRegister(REG_SYNCCONFIG, 0x91); //auto restart, sync on, fill auto, sync size 2 bytes
  writeRegister(REG_SYNCVALUE1, 0x5A);
  writeRegister(REG_SYNCVALUE2, 0x5A);

  writeRegister(REG_BITRATEMSB, 0x1a); //bit rates etc...
  writeRegister(REG_BITRATELSB, 0x0b); 

  writeRegister(REG_FDEVMSB, 0x00); //frequency deviation (deviation in Hz = fdev * 61)
  writeRegister(REG_FDEVLSB, 0x52); //see datasheet for max fdev limits (https://www.semtech.com/uploads/documents/sx1238.pdf page 22)

  writeRegister(REG_RXBW, 0x05); 
  //writeRegister(REG_AFCBW, 0xe0); 


  //setup our node address
  Serial.print("Setting Node Address to: ");
  Serial.println(ADDRESS);
  writeRegister(REG_NODEADRS, ADDRESS);

  
  setMode(SX1238_MODE_STANDBY);

  setMode(SX1238_MODE_RX); //start out in rx mode

  writeRegister(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00); // DIO0 is "Payload Ready"
}

//reads the register value of supplied address
uint8_t readRegister(uint8_t addr)
{
  select();
  SPI.transfer(addr & 0x7F);
  uint8_t regval = SPI.transfer(0);
  unselect();
  return regval;
}

//writes register value to supplied address
void writeRegister(uint8_t addr, uint8_t value)
{
  select();
  SPI.transfer(addr | 0x80);
  SPI.transfer(value);
  unselect();
}

// select the transceiver (save SPI settings, set CS low)
void select() {
 
  // set SPI settings
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0)); //2MHz clock, MSB first
  digitalWrite(SS, LOW);
}

// unselect the transceiver (set CS high)
void unselect() {
  digitalWrite(SS, HIGH);

}

void loop(){
  //nothing needed here since all the hard work is done in the interrupt handler...
}

void setMode(uint8_t newMode)
{
  if (newMode == _mode) //if it's the same mode, just return.
    return;

  Serial.print("Setting mode to ");
  
  writeRegister(REG_OPMODE, newMode | 0x08); //adding gausian filter too...
  
  while ((readRegister(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady
  _mode = newMode;
  
  switch (newMode) {
    case SX1238_MODE_TX:
      Serial.println("Transmit");
      digitalWrite(RX_EN, LOW); //disable rx
      digitalWrite(MODE, LOW);
      digitalWrite(TX_EN, HIGH); //enable power amp transmitter
      break;
    case SX1238_MODE_RX:
      Serial.println("Receive");
      digitalWrite(RX_EN, HIGH); //enable rx
      digitalWrite(MODE, HIGH); //low gain mode for now
      digitalWrite(TX_EN, LOW); //disable power amp transmitter
      break;
    case SX1238_MODE_SLEEP:
      Serial.println("Sleep");
      digitalWrite(RX_EN, LOW); //disable rx
      digitalWrite(MODE, LOW); //low gain mode for now
      digitalWrite(TX_EN, LOW); //disable power amp transmitter
      break;
    case SX1238_MODE_STANDBY:
      Serial.println("Standby");
      digitalWrite(RX_EN, LOW); //disable rx
      digitalWrite(MODE, LOW); //low gain mode for now
      digitalWrite(TX_EN, LOW); //disable power amp transmitter
      break;
    default:
      return;
  }
}

//receive a packet of data
void receiveSomething(){
 
  setMode(SX1238_MODE_RX); //start out in rx mode

  writeRegister(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00); // DIO0 is "Payload Ready"
}

void handleInterrupt()
{
    // Get the interrupt cause
    uint8_t irqflags2 = readRegister(REG_IRQFLAGS2);
    
    // Must look for PAYLOADREADY
    if (_mode == SX1238_MODE_RX && (irqflags2 & RF_IRQFLAGS2_PAYLOADREADY))
    {
      // A complete message has been received.

      Serial.println("Packet received!");
      setMode(SX1238_MODE_STANDBY);
      // Save it in our buffer
      readFifo();
    }
}

void readFifo(){
    select();
    SPI.transfer(REG_FIFO & 0x7F);
    PAYLOADLEN = SPI.transfer(0);
    Serial.print("PAYLOADLEN: ");
    Serial.println(PAYLOADLEN);
    
    PAYLOADLEN = PAYLOADLEN > 66 ? 66 : PAYLOADLEN; // precaution
    TARGETID = SPI.transfer(0);
    Serial.print("TARGETID: ");
    Serial.println(TARGETID);

    DATALEN = PAYLOADLEN - 3;
    SENDERID = SPI.transfer(0);
    Serial.print("SENDERID: ");
    Serial.println(SENDERID);
    
    uint8_t CTLbyte = SPI.transfer(0);

    Serial.print("PAYLOAD: ");
    for (uint8_t i = 0; i < DATALEN; i++)
    {
      DATA[i] = SPI.transfer(0);
      Serial.print((char)DATA[i]);
    }
    Serial.println();
    if (DATALEN < MAX_DATA_LEN) DATA[DATALEN] = 0; // add null at end of string
    unselect();
    Serial.print("RSSI: ");
    Serial.println(readRSSI());
    setMode(SX1238_MODE_RX);
}

// get the received signal strength indicator (RSSI)
int16_t readRSSI() {
  int16_t rssi = 0;
  
  rssi = -readRegister(REG_RSSIVALUE);
  rssi >>= 1;
  return rssi;
}

