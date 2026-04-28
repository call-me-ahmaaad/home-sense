![1777341580580](image/README/1777341580580.png)

> A smart home environment monitoring system that keeps tabs on temperature, humidity, rainfall, and gas levels — with remote LED control on top.

![Status](https://img.shields.io/badge/status-in%20progress-yellow)
![License](https://img.shields.io/badge/license-MIT-blue)


## 📌 What is this?

HomeSense is a smart home environment monitoring system that keeps tabs on temperature, humidity, rainfall, and gas levels — all in one place. There's also a section where you can control LED lights remotely, because what's a smart home without a little control over your own lights, right?

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

---

## ✨ Features

- 🌡️ Real-time temperature and humidity monitoring
- 🌧️ Rainfall detection
- 💨 Gas level monitoring
- 💡 Remote LED light control
- 📱 Responsive design — works on desktop, tablet, and mobile

---

## 🛠️ Tech Stack

**Software**
![HTML](https://img.shields.io/badge/HTML-E34F26?style=flat&logo=html5&logoColor=white)
![CSS](https://img.shields.io/badge/CSS-1572B6?style=flat&logo=css3&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-F7DF1E?style=flat&logo=javascript&logoColor=black)
![PHP](https://img.shields.io/badge/PHP-777BB4?style=flat&logo=php&logoColor=white)
![MySQL](https://img.shields.io/badge/MySQL-4479A1?style=flat&logo=mysql&logoColor=white)
![MQTT](https://img.shields.io/badge/MQTT-660066?style=flat&logo=mqtt&logoColor=white)

**Hardware**
| Component | Description |
|-----------|-------------|
| ESP32 | Main microcontroller |
| XY-MD02 | Temperature & humidity sensor |
| Rainfall Sensor | Detects rain/water presence |
| MQ-5 | Gas sensor |
| LED | Controllable light output |