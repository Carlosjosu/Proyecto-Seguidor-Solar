# Proyecto Seguidor Solar ☀️

Un sistema automático de seguimiento solar basado en Arduino que utiliza sensores LDR para detectar la posición del sol y controla un servo motor para mantener un panel orientado hacia la fuente de luz. El proyecto incluye una pantalla LCD que muestra en tiempo real las lecturas de los sensores y el ángulo actual del servo.

## Descripción General

Este proyecto implementa un **seguidor solar de un eje** (eje horizontal) que:
- **Detecta la luz del sol** mediante dos sensores LDR (Light Dependent Resistor) posicionados a izquierda y derecha
- **Ajusta la orientación** del panel automáticamente con un servo motor de 180°
- **Monitorea en tiempo real** los valores de luz y el ángulo mediante una pantalla LCD 16x2
- **Implementa tolerancia y suavizado** para evitar movimientos innecesarios y oscilaciones

## Características

✅ **Seguimiento automático** del sol en tiempo real  
✅ **Pantalla LCD** que muestra lecturas de sensores y ángulo actual  
✅ **Sistema de tolerancia** (40 unidades) para evitar micro-movimientos  
✅ **Control de timing** con intervalo de 50ms para operaciones estables  
✅ **Rango de movimiento** de 0° a 180°  
✅ **Ajustes graduales** de 2° por ciclo para movimientos suave

---

## Hardware Requerido

| Componente | Especificación | Cantidad |
|-----------|----------------|----------|
| Microcontrolador | Arduino UNO | 1 |
| Servo Motor | SG90 o similar (180°) | 1 |
| Sensor LDR | Fotoresistencia 5MM | 2 |
| Resistencia | 10kΩ | 2 |
| Pantalla LCD | 16x2 (I2C opcional) | 1 |
| Fuente de Alimentación | 5V, 2A mínimo | 1 |
| Cable de conexión | Dupont/Jumper | varios |
| Protoboard | Estándar | 1 |

---

## Esquema de Conexiones

### Sensores LDR
```
LDR Izquierda ----[10kΩ]---- GND
      ↓
    A0 (Arduino)

LDR Derecha ----[10kΩ]---- GND
      ↓
    A1 (Arduino)
```

### Servo Motor
```
Servo Motor
├── VCC (Rojo) ----→ 5V
├── GND (Marrón) ---→ GND
└── Signal (Naranja) → PIN 9
```

### Pantalla LCD 16x2
```
LCD            Arduino
├── VSS (GND) → GND
├── VDD (5V) → 5V
├── VO → Potenciómetro 10kΩ (contraste)
├── RS → PIN 12
├── RW → GND
├── E → PIN 11
├── D4 → PIN 5
├── D5 → PIN 4
├── D6 → PIN 3
└── D7 → PIN 2
```


## Código Fuente

```cpp
#include <Servo.h>
#include <LiquidCrystal.h> 

Servo miServo;

//Configuración de Pines 
const int pinLdrIzq = A0;
const int pinLdrDer = A1;
const int pinServo = 9;

//Pines de la pantalla LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 

//Variables de Control del Servo
int anguloActual = 90;     
const int paso = 2;              
const int tolerancia = 40;       

//Variables para el control de tiempo 
unsigned long tiempoAnterior = 0;   
const long intervalo = 50;          

void setup() {
  Serial.begin(9600);
  
  // Configuración del Servo
  miServo.attach(pinServo);
  miServo.write(anguloActual); 
  
  // Inicialización de la pantalla LCD
  lcd.begin(16, 2);
  lcd.print("Iniciando..."); 
}

void loop() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoAnterior >= intervalo) {
    tiempoAnterior = tiempoActual;

    //Lógica Intacta del Seguidor Solar
    int luzIzq = analogRead(pinLdrIzq);
    int luzDer = analogRead(pinLdrDer);
    int diferencia = abs(luzIzq - luzDer);

    if (diferencia > tolerancia) {
      if (luzIzq > luzDer) {
        anguloActual += paso; 
      } 
      else if (luzDer > luzIzq) {
        anguloActual -= paso; 
      }

      anguloActual = constrain(anguloActual, 0, 180);
      miServo.write(anguloActual);
    }
    
   
    lcd.clear();
    
    // FILA 1: Muestra lecturas de los LDR
    lcd.setCursor(0, 0);
    lcd.print("I:");
    lcd.print(luzIzq);
    
    lcd.setCursor(8, 0); 
    lcd.print("D:");
    lcd.print(luzDer);

    // FILA 2: Muestra la diferencia y el ángulo actual
    lcd.setCursor(0, 1); 
    lcd.print("Dif:");
    lcd.print(diferencia);
    
    lcd.setCursor(8, 1); 
    lcd.print("Ang:");
    lcd.print(anguloActual);
  }
}
```

## Instalación

### 1. Preparar el Hardware
- Armar el circuito según el esquema de conexiones
- Verificar todas las conexiones antes de alimentar

### 2. Instalar las Librerías
El proyecto usa librerías estándar de Arduino:
- `Servo.h` → Incluida en Arduino IDE
- `LiquidCrystal.h` → Incluida en Arduino IDE

### 3. Cargar el Código
1. Abre Arduino IDE
2. Copia el código en un nuevo sketch
3. Selecciona la placa: **Tools → Board → Arduino UNO**
4. Selecciona el puerto COM correspondiente
5. Carga el código: **Upload**

---

## Uso

### Encendido Inicial
- Al encender, el servo se posiciona en 90° (centro)
- La pantalla muestra "Iniciando..."

### Operación
- El sistema busca automáticamente la máxima luminosidad
- **Fila 1 LCD**: Muestra lecturas de los sensores (I: izquierda, D: derecha)
- **Fila 2 LCD**: Muestra diferencia y ángulo actual

### Pantalla LCD
```
I:450   D:520     <- Lecturas de LDR
Dif:70  Ang:85    <- Diferencia y ángulo
```

---


