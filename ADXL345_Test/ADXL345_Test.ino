/*
  ADXL345 Accelerometer - Test Code
  ---------------------------------
  Works with: Arduino Uno / Nano / Mega, ESP32, ESP8266 (and any board with Wire/I2C)
  No external library needed - uses only the built-in Wire library.

  WIRING (I2C)
  ------------------------------------------------------------
  ADXL345    Arduino Uno/Nano   Arduino Mega   ESP32      ESP8266 (NodeMCU)
  VCC        3.3V / 5V*         3.3V / 5V*     3.3V       3.3V
  GND        GND                GND            GND        GND
  SDA        A4                 20             GPIO 21    D2 (GPIO 4)
  SCL        A5                 21             GPIO 22    D1 (GPIO 5)
  CS         3.3V (selects I2C mode)
  SDO        GND -> address 0x53  |  3.3V -> address 0x1D
  ------------------------------------------------------------
  * Use 5V only if your module has an onboard regulator (most GY-291 modules do).

  WHAT YOU SHOULD SEE (Serial Monitor @ 115200 baud)
  - Board lying flat: X ~ 0 g, Y ~ 0 g, Z ~ +1 g
  - Tilt the board and watch the X/Y values and pitch/roll change.
  - Open Tools > Serial Plotter to see live graphs.

  Techiesms - https://techiesms.com
*/

#include <Wire.h>

// ADXL345 registers
#define REG_DEVID        0x00
#define REG_BW_RATE      0x2C
#define REG_POWER_CTL    0x2D
#define REG_DATA_FORMAT  0x31
#define REG_DATAX0       0x32

#define ADXL345_DEVID    0xE5
#define SCALE_G          0.0039f   // 3.9 mg per LSB in full-resolution mode
#define GRAVITY          9.80665f

uint8_t adxlAddr = 0;

void writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(adxlAddr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

uint8_t readReg(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return 0;
  Wire.requestFrom(addr, (uint8_t)1);
  return Wire.available() ? Wire.read() : 0;
}

bool readAxes(int16_t &x, int16_t &y, int16_t &z) {
  Wire.beginTransmission(adxlAddr);
  Wire.write(REG_DATAX0);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(adxlAddr, (uint8_t)6) != 6) return false;
  x = (int16_t)(Wire.read() | (Wire.read() << 8));
  y = (int16_t)(Wire.read() | (Wire.read() << 8));
  z = (int16_t)(Wire.read() | (Wire.read() << 8));
  return true;
}

void scanI2C() {
  Serial.println(F("Scanning I2C bus..."));
  uint8_t found = 0;
  for (uint8_t a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) {
      Serial.print(F("  Device found at 0x"));
      if (a < 16) Serial.print('0');
      Serial.println(a, HEX);
      found++;
    }
  }
  if (!found) Serial.println(F("  No I2C devices found. Check wiring and power."));
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("\n=== ADXL345 Test ==="));

  Wire.begin();   // Default pins: ESP32 SDA=21 SCL=22 | ESP8266 SDA=D2 SCL=D1 | Uno A4/A5
  // For custom ESP32 pins use: Wire.begin(SDA_PIN, SCL_PIN);

  // Auto-detect address (0x53 when SDO=GND, 0x1D when SDO=3.3V)
  const uint8_t addrs[2] = {0x53, 0x1D};
  for (uint8_t i = 0; i < 2; i++) {
    if (readReg(addrs[i], REG_DEVID) == ADXL345_DEVID) {
      adxlAddr = addrs[i];
      break;
    }
  }

  if (!adxlAddr) {
    Serial.println(F("ERROR: ADXL345 not found!"));
    scanI2C();
    Serial.println(F("Fix the wiring and press RESET."));
    while (true) delay(1000);
  }

  Serial.print(F("ADXL345 found at address 0x"));
  Serial.println(adxlAddr, HEX);

  writeReg(REG_BW_RATE, 0x0A);       // 100 Hz output data rate
  writeReg(REG_DATA_FORMAT, 0x08);   // Full resolution, +/-2g range
  writeReg(REG_POWER_CTL, 0x08);     // Start measurement mode
  delay(20);

  Serial.println(F("Sensor ready. Reading data...\n"));
}

void loop() {
  int16_t rx, ry, rz;
  if (!readAxes(rx, ry, rz)) {
    Serial.println(F("Read failed - check connections."));
    delay(500);
    return;
  }

  float gx = rx * SCALE_G;
  float gy = ry * SCALE_G;
  float gz = rz * SCALE_G;

  float pitch = atan2(-gx, sqrt(gy * gy + gz * gz)) * 180.0 / PI;
  float roll  = atan2(gy, gz) * 180.0 / PI;

  // Format also works with Serial Plotter (label:value pairs)
  Serial.print(F("X(g):"));    Serial.print(gx, 2);
  Serial.print(F("\tY(g):"));  Serial.print(gy, 2);
  Serial.print(F("\tZ(g):"));  Serial.print(gz, 2);
  Serial.print(F("\tPitch:")); Serial.print(pitch, 1);
  Serial.print(F("\tRoll:"));  Serial.println(roll, 1);

  delay(200);
}
