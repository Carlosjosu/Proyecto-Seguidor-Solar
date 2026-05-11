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