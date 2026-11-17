# ADXL345 Accelerometer – Test Code

A simple sketch to check that your **ADXL345 3-axis accelerometer** (GY-291 module) works. It runs on **Arduino Uno / Nano / Mega, ESP32 and ESP8266** and needs **no external library**: it uses only the built-in `Wire` (I2C) library.

Upload it, open the Serial Monitor, and tilt the sensor. The X/Y/Z readings and the pitch/roll angles should change.

---

## Features

- No library to install
- Finds the sensor automatically at either I2C address (`0x53` or `0x1D`)
- Confirms the chip ID (`0xE5`), so you know it's a real, working ADXL345
- Scans the I2C bus and prints what it finds if the sensor isn't detected
- Shows acceleration in **g** plus **pitch and roll** in degrees
- Works with the **Serial Plotter** for live graphs

---

## Hardware Required

| Item | Qty |
|---|---|
| ADXL345 accelerometer module (GY-291) | 1 |
| Arduino Uno / Nano / Mega, ESP32 or ESP8266 board | 1 |
| Jumper wires | 6 |
| USB cable for your board | 1 |

🛒 Get the ADXL345 module from **[techiesms.com](https://techiesms.com/product/adxl345-tripple-axis-accelerometer-board/)**

---

## Wiring (I2C)

| ADXL345 Pin | Arduino Uno / Nano | Arduino Mega | ESP32 | ESP8266 (NodeMCU) |
|---|---|---|---|---|
| VCC | 3.3V / 5V* | 3.3V / 5V* | 3.3V | 3.3V |
| GND | GND | GND | GND | GND |
| SDA | A4 | 20 | GPIO 21 | D2 (GPIO 4) |
| SCL | A5 | 21 | GPIO 22 | D1 (GPIO 5) |
| CS | 3.3V | 3.3V | 3.3V | 3.3V |
| SDO | GND | GND | GND | GND |

\* Use 5V only if your module has an onboard 3.3V regulator (most GY-291 modules do). If you're not sure, use 3.3V.

**Notes**
- **CS → 3.3V** puts the sensor in I2C mode. If CS is left floating, the sensor may not be detected.
- **SDO** sets the I2C address: **GND → 0x53**, **3.3V → 0x1D**. The code checks both.
- To use other I2C pins on an ESP32, change `Wire.begin();` to `Wire.begin(SDA_PIN, SCL_PIN);`

---

## How to Upload

1. Download or clone this repository:
   ```bash
   git clone https://github.com/techiesms/ADXL345-Test-Code.git
   ```
2. Open `ADXL345_Test/ADXL345_Test.ino` in the **Arduino IDE**.
3. Select your board under **Tools → Board**.
   - For **ESP32** or **ESP8266**, install the board package first from **Tools → Board → Boards Manager** (search for `esp32` or `esp8266`).
4. Select the correct port under **Tools → Port**.
5. Click **Upload**.
6. Open **Tools → Serial Monitor** and set the baud rate to **115200**.

---

## Expected Output

```
=== ADXL345 Test ===
ADXL345 found at address 0x53
Sensor ready. Reading data...

X(g):0.02	Y(g):-0.01	Z(g):1.00	Pitch:-1.1	Roll:-0.6
X(g):0.03	Y(g):-0.02	Z(g):0.99	Pitch:-1.7	Roll:-1.2
X(g):0.51	Y(g):0.04	Z(g):0.86	Pitch:-30.6	Roll:2.7
```

| Position | X | Y | Z |
|---|---|---|---|
| Lying flat (chip facing up) | ~0 g | ~0 g | ~+1 g |
| Tilted along X axis | changes | ~0 g | decreases |
| Tilted along Y axis | ~0 g | changes | decreases |

For live graphs, close the Serial Monitor and open **Tools → Serial Plotter** (115200 baud).

---

## Troubleshooting

| Problem | Fix |
|---|---|
| `ERROR: ADXL345 not found!` and `No I2C devices found` | Check VCC, GND, SDA and SCL. Make sure SDA and SCL aren't swapped. |
| A device is found, but not at 0x53 or 0x1D | That device isn't an ADXL345, or the module is faulty. |
| Sensor is detected only some of the time | Connect **CS to 3.3V** and **SDO to GND**; don't leave them floating. |
| Serial Monitor shows garbage characters | Set the baud rate to **115200**. |
| Readings are stuck at 0 | Press RESET on the board, and check that the module is powered. |
| ESP32 upload fails | Hold the **BOOT** button while uploading starts. |

---

## Sensor Settings Used

| Setting | Value |
|---|---|
| Interface | I2C |
| Range | ±2g |
| Resolution | Full resolution (3.9 mg/LSB) |
| Output data rate | 100 Hz |
| Serial baud rate | 115200 |

---

## About Techiesms

- 🛒 Store: [techiesms.com](https://techiesms.com)
- 📺 YouTube: [youtube.com/@techiesms](https://www.youtube.com/@techiesms)
- 🎓 Courses: [techiesms.graphy.com](https://techiesms.graphy.com)

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
