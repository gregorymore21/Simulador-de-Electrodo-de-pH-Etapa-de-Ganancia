// Laboratorio 1 - Instrumentación Biomédica III
// Acondicionamiento de señales de alta impedancia
// Simulación de electrodo de pH mediante ESP32

const int DAC_PIN = 25;

// Parámetros de la ecuación de Nernst a 25 °C
const float SLOPE = 0.05916;   // V/pH

// Factor de escala para hacer visible la señal
const float FACTOR_ESCALA = 5.0;

// Offset para mantener la señal dentro de 0 - 3.3 V
const float OFFSET = 1.65;

// Rango del DAC
const float V_DAC_MIN = 0.0;
const float V_DAC_MAX = 3.3;

void setup() {

  Serial.begin(115200);

  // Pequeña espera para iniciar el Monitor Serial
  delay(1000);

  Serial.println("====================================");
  Serial.println(" SIMULADOR DE ELECTRODO DE pH");
  Serial.println(" Laboratorio 1 - IB III");
  Serial.println("====================================");
  Serial.println("Ingrese un valor de pH entre 0 y 14:");
}

void loop() {

  // Verificar si se ingresó un dato por el Monitor Serial
  if (Serial.available() > 0) {

    // Leer el valor de pH
    float pH = Serial.parseFloat();

    // Limpiar datos restantes del Monitor Serial
    while (Serial.available() > 0) {
      Serial.read();
    }

    // Verificar que el pH esté dentro del rango permitido
    if (pH < 0.0 || pH > 14.0) {

      Serial.println("ERROR: El pH debe estar entre 0 y 14.");
      Serial.println("Ingrese nuevamente el valor de pH:");

      return;
    }

    // ------------------------------------------------
    // ECUACIÓN DE NERNST
    // E = E0 - 0.05916*(pH - 7)
    //
    // Para esta simulación se toma E0 = 0 V
    // en pH = 7.
    // ------------------------------------------------

    float E_nernst = -SLOPE * (pH - 7.0);

    // Aplicar factor de escala y offset
    float voltaje = OFFSET + (E_nernst * FACTOR_ESCALA);

    // Limitar el voltaje al rango del DAC: 0 - 3.3 V
    voltaje = constrain(voltaje, V_DAC_MIN, V_DAC_MAX);

    // Convertir el voltaje a un valor digital de 8 bits
    // DAC: 0 -> 0 V
    // DAC: 255 -> aproximadamente 3.3 V
    int dacValue = round((voltaje / 3.3) * 255.0);

    // Asegurar que el valor esté entre 0 y 255
    dacValue = constrain(dacValue, 0, 255);

    // Enviar el valor al DAC del GPIO25
    dacWrite(DAC_PIN, dacValue);

    // ------------------------------------------------
    // MOSTRAR INFORMACIÓN EN EL MONITOR SERIAL
    // ------------------------------------------------

    Serial.println("------------------------------------");

    Serial.print("pH ingresado: ");
    Serial.println(pH, 2);

    Serial.print("Voltaje Nernst: ");
    Serial.print(E_nernst, 4);
    Serial.println(" V");

    Serial.print("Voltaje entregado por DAC: ");
    Serial.print(voltaje, 4);
    Serial.println(" V");

    Serial.print("Valor digital DAC: ");
    Serial.println(dacValue);

    Serial.println("------------------------------------");
    Serial.println("Ingrese otro valor de pH (0 - 14):");
  }
}
