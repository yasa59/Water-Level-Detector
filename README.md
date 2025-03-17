### **HC-SR04 Water Level Detector**  

This project is a **PIC16F887-based** system that uses an **HC-SR04 ultrasonic sensor** to measure water levels. The **LCD** displays "HIGH," "MID," or "LOW," and a **buzzer** alerts when the water level is high. It is suitable for monitoring tanks or reservoirs.  

---

## **Features**  
- Measures water level in real time  
- Displays water level on a **16x2 LCD**  
- Activates a **buzzer** when the water level is high  
- Uses a **PIC16F887** microcontroller  
- Works with an **8MHz internal oscillator**  

---

## **Components Used**  
- **PIC16F887** microcontroller  
- **HC-SR04** ultrasonic sensor  
- **16x2 LCD** (4-bit mode)  
- **Buzzer**  
- Resistors and connecting wires  

---

## **Circuit Connections**  
| Component | Pin Connection |  
|-----------|---------------|  
| **HC-SR04 Trig** | RC0 |  
| **HC-SR04 Echo** | RC1 |  
| **LCD RS** | RD0 |  
| **LCD EN** | RD1 |  
| **LCD D4-D7** | RD2-RD5 |  
| **Buzzer** | RB0 |  

---

## **Installation & Usage**  
1. **Compile the Code** using **MPLAB X IDE** with **XC8 Compiler**.  
2. **Upload the HEX file** to the **PIC16F887** using a programmer.  
3. **Connect components** as per the circuit.  
4. **Power the system** and observe LCD readings.  
5. The **buzzer will activate** when the water level is high.  

---

## **Code Explanation**  
- **LCD_Init()** – Initializes the **16x2 LCD** in **4-bit mode**.  
- **Measure_Distance()** – Uses **HC-SR04** to calculate distance.  
- **Main Loop** – Reads the water level, updates LCD, and controls the buzzer.  

---

## **License**  
This project is open-source. Feel free to modify and use it.
