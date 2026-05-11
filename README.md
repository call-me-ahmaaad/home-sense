<img width="1127" height="229" alt="image" src="https://github.com/user-attachments/assets/3e5c67e3-9c57-4be4-bdd5-024a4b302293" />

> A smart home environment monitoring system that keeps tabs on temperature, humidity, rainfall, and gas levels — with remote LED control on top.

![Static Badge](https://img.shields.io/badge/STATUS-In--progress-%23E16B16?style=for-the-badge)
![Static Badge](https://img.shields.io/badge/LICENSE-MIT-blue?style=for-the-badge)

# 📌 What is this?

HomeSense is a smart home environment monitoring system that keeps tabs on temperature, humidity, rainfall, and gas levels — all in one place. There's also a section where you can control LED lights remotely, because what's a smart home without a little control over your own lights, right?
To keep things clean and reliable, the hardware side is split into two separate devices — each with its own job:

- Monitoring (ESP32) — reads the sensors and publishes data to MQTT topics. Two onboard LEDs act as status indicators for WiFi and MQTT connection.
- LED Control (ESP8266) — subscribes to MQTT topics and handles the on/off state of 3 LEDs. That's it. Just control.

The split is intentional. One device doing everything sounds fine on paper, but in practice it gets messy — too many components, too many responsibilities. Keeping them separate means each device stays focused, and the system stays manageable.

Now, full transparency — this isn't my first rodeo with this idea. HomeSense is a rebuild of my MSIB IoT Developer final project, originally called *Smart Home Monitoring*. That project worked. It got submitted, it got evaluated, and it even landed me the Best Graduate award. So yeah, it did its job.

But here's the thing: I relied *way* too much on AI to write that version. Not as a tool for brainstorming or decision-making — I mean actually letting it write the syntax for me. I knew what I wanted, I knew what the system should do, but I wasn't really the one building it. And that's been bugging me ever since.

So HomeSense is my way of going back and doing it right. Same concept, different approach. This time, every line of code is mine. Every decision is deliberate. Every bug is mine to fix — and honestly, that's the whole point.

That said — I'm not going completely off the grid here. AI is still part of the workflow, and I'm not going to pretend it isn't useful. But there's a difference between using AI as a crutch and using it as a tool. This time, AI is here for decision-making, syntax corrections, and suggestions on how to improve the code — not to build the whole thing from start to finish while I just watch. There's a line between "AI-assisted" and "AI-built", and HomeSense is firmly on the right side of it.

**What's changed under the hood:**
- Pure HTML, CSS, JS, and PHP — dropped Laravel to keep things lean and intentional
- XY-MD02 replaces the DHT11 for more reliable sensor readings
- MQTT instead of HTTP for sensor data transmission — cleaner, faster, more IoT-appropriate
- Fully responsive design across devices — because it should work on your phone too

Same idea. Better execution. Different mindset. Let's build this properly. 🛠️

## 🤝 How I Used AI

Let's be honest — AI was part of this project, and pretending otherwise would be hypocritical. It saved time, and I'm not going to apologize for that. But there's a difference between AI as a tool and AI as a replacement, and here's exactly where it fit in:

1. **Decision-making & brainstorming** — I used AI as a thinking partner when I wasn't sure which direction to take. Sometimes I agreed, sometimes I didn't — and went with my own call instead. Turns out AI can be wrong, and I learned that firsthand during this project.

2. **Project architecture** — Honestly, I had no idea where to start at first. Folder structure, code structure, what to build first — AI helped me get a rough sketch of how things should be organized. The final structure is still mine to own, but having that initial direction made a real difference.

3. **Code review** — My approach: write the code myself first, then ask AI to check if it's clean and efficient. Not "write this for me" — more like a second pair of eyes after I've already done the work.

4. **Documentation** — This README, the comments in the `.ino` files — yeah, AI helped with these. Documentation is time-consuming and honestly not where I want to spend most of my energy right now. So I leaned on AI here, and I'm fine with that call.

Where's the line between "AI-assisted" and "AI did it for you"? Genuinely, I'm not sure there's a clean answer. But my rule of thumb: I may not always nail the technical explanation, but I know what each part does and why it's there. That's the bar I set for myself — and so far, I'm meeting it.

---

# ✨ Features

- 🌡️ Real-time temperature and humidity monitoring
- 🌧️ Rainfall detection
- 💨 Gas level monitoring
- 💡 Remote LED light control
- 🗄️ Sensor data logging — data dari semua sensor tersimpan ke database
- 🔌 Device online/offline detection — monitors whether edge devices are reachable, powered by MQTT's Last Will and Testament (LWT)
- 📱 Responsive design — works on desktop, tablet, and mobile

---

# 🛠️ Tech Stack

**Software**

![Static Badge](https://img.shields.io/badge/HTML-%23E5532D?style=for-the-badge&logo=html5&logoColor=white)
![Static Badge](https://img.shields.io/badge/CSS-%230277BD?style=for-the-badge&logo=css&logoColor=white)
![Static Badge](https://img.shields.io/badge/JavaScript-%23F7E025?style=for-the-badge&logo=javascript&logoColor=black)
![Static Badge](https://img.shields.io/badge/PHP-%23787CB4?style=for-the-badge&logo=php&logoColor=white)
![Static Badge](https://img.shields.io/badge/MySQL-%23086590?style=for-the-badge&logo=mysql&logoColor=white)
![Static Badge](https://img.shields.io/badge/MQTT-%236B086B?style=for-the-badge&logo=mqtt&logoColor=white)
![Static Badge](https://img.shields.io/badge/Arduino-%23087885?style=for-the-badge&logo=arduino)
![Static Badge](https://img.shields.io/badge/Espressif-%23FF363A?style=for-the-badge&logo=espressif&logoColor=white)

**Hardware**
| Component | Description |
|-----------|-------------|
| ESP32 | Microcontroller for monitoring |
| ESP8266 | Microcontroller for LEDs control |
| XY-MD02 | Temperature & humidity sensor |
| Rainfall Sensor | Detects rain/water presence |
| MQ-5 | Gas sensor |
| LEDs | Controllable light output and indicator |

---

# 🎬 Preview

> A full walkthrough of HomeSense in action — from the hardware setup to the live dashboard.

[![YouTube](https://img.shields.io/badge/YouTube-Full_System_Demo-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=-2NnEMKvy1M)
[![YouTube](https://img.shields.io/badge/YouTube-Dashboard_Demo-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=Kpd37-ywLPE&t=5s)

## Screenshot

| Dashboard | Sensor Data Log | MQTT Explorer |
|:---------:|:---------------:|:-------------:|
| <img width="250" src="https://github.com/user-attachments/assets/520b9167-3032-40bb-b8da-86606445fc85"> | <img width="250" src="https://github.com/user-attachments/assets/82492334-8553-4e03-b96e-76d67db51cad"> | <img src="https://github.com/user-attachments/assets/24704d6c-a2d1-400e-a99d-0ac324e4770c" width="250"> |

## Hardware


| Active | Monitoring (ESP32) | LEDs Control (ESP8266) |
|:---------:|:---------------:|:--------------:|
| <img width="250" src="https://github.com/user-attachments/assets/6e3f4f17-a97f-4e5b-8194-db83198b3ff4"> | <img width="250" src="https://github.com/user-attachments/assets/164bd072-4944-4150-8944-c43bb2769636"> | <img width="250" src="https://github.com/user-attachments/assets/1b2f38f3-452f-480a-87e3-c0ba89281255"> |

> *Sorry for the camera quality 🙏*

---

# 🚀 Getting Started

## Prerequisites

- PHP 8.4
- Composer
- MySQL
- Mosquitto (MQTT Broker)
- Arduino IDE (for uploading to ESP32 & ESP8266)

### 1. Clone the Repository

```bash
git clone https://github.com/call-me-ahmaaad/home-sense.git
cd home-sense
```

### 2. Server Setup

**Install dependencies**
```bash
cd server
composer install
```

**Configure the server**

Copy the config files and fill in your environment details:
```bash
cp config/database.example.php config/database.php
cp config/mqtt.example.php config/mqtt.php
```

> **Note:** If your MQTT broker doesn't use authentication, set `username` and `password` to `null`.

**Run the API server**
```bash
php -S localhost:8000
```

**In a separate terminal, run the MQTT subscriber**
```bash
php index.php
```

### 3. Edge Setup

Applies to both devices (monitoring & LED control).

**Arduino Libraries**

Install the following libraries via Arduino IDE Library Manager before uploading:

| Library | Used In |
|---------|---------|
| PubSubClient | homeSense_monitoring, homeSense_ledControl |
| ModbusMaster | homeSense_monitoring |

**Upload sketch**

1. Open the appropriate folder in Arduino IDE:
   - `edge/homeSense_monitoring/` → ESP32
   - `edge/homeSense_ledControl/` → ESP8266
2. Copy the config file and fill in your WiFi & MQTT credentials:
   ```
   config.example.h → config.h
   ```
3. Upload the sketch to each device.

### 4. App

```bash
cd app
npx serve .
```

Open your browser and go to the address shown in the terminal.

> If you run into any issues getting this up and running, feel free to reach out to me directly. Sorry if this setup guide isn't as helpful as it could be — documentation is something I'm still working on. 🙏

---

# 📁 Project Structure

```
home-sense/
├── app/                        # Frontend
│   ├── assets/
│   │   ├── css/                # Stylesheets
│   │   ├── img/                # Images & icons
│   │   └── js/
│   │       ├── api/            # API call functions (sensor data, device status, LED control)
│   │       ├── components/     # UI components (sensor cards, LED control)
│   │       └── views/          # Page-level scripts
│   └── index.html              # Main entry point
│
├── edge/                       # Hardware (Arduino sketches)
│   ├── homeSense_monitoring/   # ESP32 — reads sensors, publishes to MQTT
│   │   ├── config.example.h
│   │   └── homeSense_monitoring.ino
│   └── homeSense_ledControl/   # ESP8266 — subscribes to MQTT, controls LEDs
│       ├── config.example.h
│       └── homeSense_ledControl.ino
│
└── server/                     # Backend
    ├── api/                    # API entry points
    ├── bootstrap/              # App initialization (DB & MQTT connections)
    ├── config/                 # Config files (database, mqtt, cors)
    ├── infrastructure/         # Low-level database connection
    ├── src/
    │   ├── Controllers/
    │   │   ├── API/            # Handles HTTP API requests
    │   │   └── MQTT/           # Handles incoming MQTT messages
    │   ├── Exceptions/         # Custom exception classes
    │   ├── Models/             # Data models
    │   ├── MQTT/               # MQTT connection, publish & subscribe logic
    │   ├── Repositories/
    │   │   ├── Read/           # Database read operations
    │   │   └── Write/          # Database write operations
    │   └── Services/           # Business logic & system logger
    ├── composer.json
    └── index.php               # MQTT subscriber entry point
```

---

# 📖 Documentation

## API Endpoints

Base URL: `http://localhost:8000`

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/SensorData.php` | Get the latest sensor data |
| GET | `/api/DeviceStatus.php` | Get the latest device status |
| POST | `/api/ControlLed.php` | Send a LED control command |

**GET** `/api/SensorData.php`

Response:
```json
{
  "success": true,
  "data": {
    "temperature": { "id": 1, "temperature": 28.5, "created_at": "2025-01-01 12:00:00" },
    "humidity":    { "id": 1, "humidity": 65.2,    "created_at": "2025-01-01 12:00:00" },
    "rain_status": { "id": 1, "is_raining": 0,     "created_at": "2025-01-01 12:00:00" },
    "gas_level":   { "id": 1, "gas_level": 320.0,  "created_at": "2025-01-01 12:00:00" }
  }
}
```

**GET** `/api/DeviceStatus.php`

Response:
```json
{
  "success": true,
  "data": { "id": 1, "status": "online", "created_at": "2025-01-01 12:00:00" }
}
```

**POST** `/api/ControlLed.php`

Request body:
```json
{
  "led": "red",
  "state": "ON"
}
```

Response:
```json
{
  "success": true,
  "message": { "led": "red", "state": "ON" }
}
```

> Valid values for `led`: `red`, `green`, `blue`. Valid values for `state`: `ON`, `OFF`.

## MQTT Topics

| Topic | Publisher | Subscriber | Description |
|-------|-----------|------------|-------------|
| `home_sense/data` | ESP32 | PHP Server | Sensor data payload |
| `home_sense/status` | ESP32 (LWT) | PHP Server | Device online/offline status |
| `home_sense/led/red` | PHP Server | ESP8266 | Red LED control |
| `home_sense/led/green` | PHP Server | ESP8266 | Green LED control |
| `home_sense/led/blue` | PHP Server | ESP8266 | Blue LED control |

**`home_sense/data`** — published every 2 seconds by ESP32:
```json
{
  "temperature": 28.5,
  "humidity": 65.2,
  "rain_status": 0,
  "gas_level": 320.0
}
```

> `gas_level` is in ppm, converted from ADC readings using a formula based on the MQ-5 datasheet calibrated for LPG (R0 = 10.0 kΩ). Note that the calibration is simulated — R0 should be re-calibrated in clean air for accurate real-world readings.

**`home_sense/status`** — published by ESP32 on connect/disconnect (LWT):
```json
{ "status": "online" }
{ "status": "offline" }
```

**`home_sense/led/<color>`** — published by PHP Server, received by ESP8266:
```
ON
OFF
```

## Database Schema

**`temperature`**
| Column | Type | Description |
|--------|------|-------------|
| id | INT, AUTO_INCREMENT, PK | — |
| temperature | FLOAT | Temperature value in °C |
| created_at | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

**`humidity`**
| Column | Type | Description |
|--------|------|-------------|
| id | INT, AUTO_INCREMENT, PK | — |
| humidity | FLOAT | Humidity value in % |
| created_at | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

**`rain_status`**
| Column | Type | Description |
|--------|------|-------------|
| id | INT, AUTO_INCREMENT, PK | — |
| is_raining | TINYINT(1) | `1` = raining, `0` = not raining |
| created_at | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

**`gas_level`**
| Column | Type | Description |
|--------|------|-------------|
| id | INT, AUTO_INCREMENT, PK | — |
| gas_level | FLOAT | Gas level value in ppm (simulated) |
| created_at | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

**`device_status`**
| Column | Type | Description |
|--------|------|-------------|
| id | INT, AUTO_INCREMENT, PK | — |
| status | ENUM('online', 'offline') | Device connection status |
| created_at | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP |

---

# 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgment

HomeSense started as a final project for the MSIB IoT Developer Program organized by **PT Arkatama Multi Solusindo**. A big thank you to everyone at Arkatama — especially the instructors who guided and equipped me with the knowledge that made this possible. To my friends who were there throughout the whole process, thank you for the company.

As for direct contributions to this project — that's all me. So, thanks to myself. :3

---

## 💬 Closing

This project is far from perfect, and I know that. The road to becoming a solid web developer, a solid IoT developer, and eventually someone who can merge the two seamlessly — it's still a long one. But I intend to use every opportunity to keep growing.

If you have feedback, ideas, or even want to collaborate on something — I'd genuinely love that. There's a lot I still have to learn, and I'm looking forward to every bit of it.
