#include <ModbusMaster.h>
#include <Arduino.h>
#include <ArduinoJson.h>


#define RS485_DIR D1

ModbusMaster node;

void preTransmission()  { digitalWrite(RS485_DIR, HIGH); }
void postTransmission() { digitalWrite(RS485_DIR, LOW);  }


// ---------- U_WORD (16 bits sin signo)
uint16_t readUWORD(uint16_t reg) {
  uint8_t rc = node.readHoldingRegisters(reg, 1);
  if (rc != node.ku8MBSuccess) {
    return 0;
  }
  return node.getResponseBuffer(0);
}

// ---------- U_DWORD (32 bits sin signo)
uint32_t readUDWORD(uint16_t reg) {
  uint8_t rc = node.readHoldingRegisters(reg, 2);
  if (rc != node.ku8MBSuccess) {
    return 0;
  }

  uint16_t high = node.getResponseBuffer(0);
  uint16_t low  = node.getResponseBuffer(1);

  return ((uint32_t)high << 16) | low;
}

// ---------- S_DWORD (32 bits con signo)
int32_t readSDWORD(uint16_t reg) {
  uint8_t rc = node.readHoldingRegisters(reg, 2);
  if (rc != node.ku8MBSuccess) {
    return 0;
  }

  uint16_t high = node.getResponseBuffer(0);
  uint16_t low  = node.getResponseBuffer(1);

  return ((int32_t)high << 16) | low;
}

// ---------- S_WORD (16 bits con signo)
int16_t readSWORD(uint16_t reg) {

  uint8_t rc = node.readHoldingRegisters(reg, 1);
  if (rc != node.ku8MBSuccess) {
    return 0;
  }

  return (int16_t)node.getResponseBuffer(0);
}

void innitModbus(){
	
  pinMode(RS485_DIR, OUTPUT);
  digitalWrite(RS485_DIR, LOW);

  Serial.begin(115200, SERIAL_8N1);

  node.begin(1, Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

}

String readJKBMS(){

	uint16_t batteryAuxP = readUWORD(0x12A6);
  delay(100);

  uint32_t batteryAuxV = readUDWORD(0x1290);
  delay(100);

  int32_t batteryAuxA = readSDWORD(0x1298);
  delay(100);

  int16_t BatTempAux1 = readSWORD(0x129C);
  delay(100);
  
  int16_t BatTempAux2 = readSWORD(0x129E);
  delay(100);

  int16_t BatTempAux = readSWORD(0x128A);
	delay(100);

  JsonDocument doc;
  String output;

	doc["batteryAuxP"] = batteryAuxP;
	doc["batteryAuxV"] = batteryAuxV * 0.001f;
	doc["batteryAuxA"] = batteryAuxA  * 0.001f;
	doc["BatTempAux1"] = BatTempAux1 * 0.1f;
	doc["BatTempAux2"] = BatTempAux2 * 0.1f;
	doc["BatTempAux"] = BatTempAux * 0.1f;

  serializeJson(doc, output);

  return output;
}