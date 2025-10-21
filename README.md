# Stress-pad
Blynk Iot integrated Wokwi project

# 🧠 Stress Pad Monitor v2

**IoT-Based Stress Monitoring Simulation**

## 📖 Overview

**Stress Pad** is an **IoT-based simulation project** designed to demonstrate a stress monitoring system using the **Blynk IoT platform** and the **ESP32 microcontroller**.
Instead of using actual sensors, this project **generates random values** to simulate physiological and behavioral data such as heart rate, temperature, and motion — which are then used to determine a student's stress level.

When the simulated readings exceed a certain threshold, the system automatically detects **“High Stress”**, triggers visual indicators, and sends notifications through **Blynk IoT** and **email alerts**.

---

## ⚙️ Features

* 🔴 **High Stress Detection:** Simulated stress values trigger alerts when exceeding the threshold (e.g., 0.75).
* 📲 **Blynk Integration:** Displays the simulated stress level and sends notifications in real-time.
* 📧 **Email Alerts:** Automatically sends an email when “High Stress” is detected.
* 💡 **LED and OLED Display Simulation:** Shows system status visually within Wokwi.
* 🧠 **Educational Demonstration:** Ideal for learning IoT concepts, logic flow, and Blynk integration without physical hardware.

---

## 🧩 Components Used (Simulation Only)

| Component                  | Description                                                           |
| -------------------------- | --------------------------------------------------------------------- |
| **ESP32**                  | Main microcontroller used to run the simulation and connect to Blynk. |
| **OLED Display (SSD1306)** | Displays the simulated stress status (“High Stress Detected”).        |
| **LEDs (Red/Yellow)**      | Indicators for normal and high stress states.                         |
| **Push Button**            | Used to simulate a trigger event or data reading in Wokwi.            |
| **Wokwi Simulator**        | Platform used to emulate the ESP32 and its peripherals.               |

> 🧪 *Note: No physical sensors are used — random values are generated in the code to mimic sensor readings.*

---

## 🖥️ System Interface

### **Blynk Dashboard**

* Displays **stress level** (0–1 scale).
* Shows “HIGH STRESS DETECTED” when the simulated value exceeds 0.75.
* Sends both **push notifications** and **email alerts** via Blynk Cloud.

---

## 📷 Screenshots

### **1. Blynk Notifications**

![](./Messenger_creation_1C679CC3-DF46-4C54-A872-261D9C2C2605.png)

### **2. Email Alerts**

![](./Messenger_creation_41A79841-C613-45CB-B138-E1851ACA6D1A.png)

### **3. Blynk Dashboard Interface**

![](./Messenger_creation_A24CA9CC-1821-4EEF-84FC-4FE7A39A435F.png)
![](./Messenger_creation_69314B72-4367-4DFD-B491-FC79E54D16B5.png)

### **4. Wokwi Simulation (ESP32 Circuit)**

![](./Screenshot_20251020-224247.jpg)

---

## 🧮 How It Works

1. The ESP32 generates a **random number** between 0 and 1 to simulate a stress reading.
2. The value is sent to **Blynk Cloud** and displayed on the dashboard.
3. If the value exceeds **0.75**, the system triggers:

   * Red LED ON
   * OLED display shows **“HIGH STRESS DETECTED”**
   * Blynk sends **push and email notifications**
4. If below threshold, the LED remains OFF and the status stays normal.

---

## 🚀 Future Improvements

* 🧘 Connect real sensors for heart rate, motion, and temperature.
* 📊 Add data logging and graph history on Blynk.
* 🔈 Add sound or vibration alerts for stress detection.
* 🧩 Integrate with student profiles and report analytics.

---

## 📄 Documentation

Additional technical documentation, source code explanation, and wiring diagram notes will be available soon.

---

## 👨‍💻 Developer

**Created by:** Gino Villamor Badana
**Email:** [ginobadana369@gmail.com](mailto:ginobadana369@gmail.com)
**Project Name:** *Stress Pad Monitor v2*
**Platform:** ESP32 + Blynk IoT + Wokwi Simulator
