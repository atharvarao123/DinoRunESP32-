# 🦖 DINO RUN with ESP32

This project is a recreation of Google's famous **Dino Run** game, developed in **C** and implemented on an **ESP32** microcontroller.  
It uses an LCD display and two buttons to control gameplay.  

---

## Game Description
- **Player**: Represented by the character `'o'`  
- **Obstacle**: Represented by the character `'i'`  
- The obstacle moves towards the player.  
- The player must **jump at the right time** to avoid colliding with the obstacle.  

---

## ⚙ ESP32 Usage

- **Display** → Shows:
  - Start screen  
  - Game screen  
  - Game Over screen  

- **Button 1** → Start the game / Restart after game over  
- **Button 2** → Make the player (`'o'`) jump  

---

##Circuit Connections

### Buttons
- **Button 1** → `GPIO 2` → Button → GND  
- **Button 2** → `GPIO 15` → Button → GND  

### Display (I²C)
- **SDA** → `GPIO 13`  
- **SCL** → `GPIO 14`  
- **Power** → `5V`  
- **Ground** → `GND`  

---

## Library Used
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)

---

[![Dino Run Demo](https://img.youtube.com/vi/15Ay1STQVcg/0.jpg)](https://www.youtube.com/watch?v=15Ay1STQVcg)

