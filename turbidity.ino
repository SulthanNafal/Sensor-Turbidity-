const int PIN_SENSOR = A0;   // Pin Analog sensor turbidity
const int PIN_BTN    = 10;   // Pin Tombol untuk Kalibrasi
const int PIN_HIJAU  = 7;    // Pin LED Hijau
const int PIN_KUNING = 8;    // Pin LED Kuning
const int PIN_MERAH  = 9;    // Pin LED Merah

const float V_REF         = 5.0;   // Tegangan referensi Arduino (5V)
const float DIVIDER_RATIO = 2.0;   // R1 = R2 = 10k (Pembagi tegangan)
const float V_JERNIH_REF  = 4.2;   // Tegangan acuan 0 NTU di 5V

// Batas status disesuaikan dengan Slide 2 PPT
const float BATAS_JERNIH = 5.0;    // NTU < 5    -> JERNIH
const float BATAS_AGAK   = 50.0;   // NTU 5 - 50 -> AGAK KERUH, selebihnya KERUH

float vOffset = 0.0;               // Sesuai PPT: vOffset

float bacaTegangan() {
  long total = 0;
  for (int i = 0; i < 30; i++) {
    total += analogRead(PIN_SENSOR);
    delay(5);
  }
  float rataAdc = total / 30.0;

  // Konversi ADC (0-1023) ke Tegangan Pin Arduino (0 - 5V)
  float vPin = (rataAdc / 1023.0) * V_REF * DIVIDER_RATIO;
  return vPin;
}

float hitungNTU(float vs) {
  float vc = vs - vOffset;                            // Koreksi tegangan (V' = Vs - Voff)
  if (vc < 2.5) return 3000;

  // Perbaikan rumus NTU sesuai PPT (operator perkalian '*' ditambahkan)
  float ntu = -1120.4 * (vc * vc) + (5742.3 * vc) - 4352.9;
  return ntu < 0 ? 0 : ntu;
}

void kalibrasi() {
  Serial.println(F(">> Kalibrasi... pastikan sensor di AIR JERNIH"));
  float vsJernih = bacaTegangan();
  vOffset = vsJernih - V_JERNIH_REF;                    // Voff = Vs,jernih - 4,2
  Serial.print(F(">> Vs Jernih: ")); Serial.print(vsJernih, 3);
  Serial.print(F(" V | vOffset: ")); Serial.print(vOffset, 3);
  Serial.println(F(" V"));
  Serial.println(F(">> Kalibrasi selesai (0 NTU)"));

  // Kedip semua LED sebagai tanda
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_HIJAU, HIGH); digitalWrite(PIN_KUNING, HIGH); digitalWrite(PIN_MERAH, HIGH);
    delay(150);
    digitalWrite(PIN_HIJAU, LOW); digitalWrite(PIN_KUNING, LOW); digitalWrite(PIN_MERAH, LOW);
    delay(150);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_HIJAU, OUTPUT);
  pinMode(PIN_KUNING, OUTPUT);
  pinMode(PIN_MERAH, OUTPUT);

  Serial.println(F("Tekan tombol untuk kalibrasi (sensor di air jernih)"));
}

void loop() {
  if (digitalRead(PIN_BTN) == LOW) {
    delay(30);                      // debounce
    if (digitalRead(PIN_BTN) == LOW) {
      kalibrasi();
      while (digitalRead(PIN_BTN) == LOW);      // tunggu tombol dilepas
    }
  }

  float v   = bacaTegangan();
  float ntu = hitungNTU(v);

  String status = ""; // Perbaikan deklarasi String status
  
  // Matikan semua LED terlebih dahulu sebelum pengecekan kondisi
  digitalWrite(PIN_HIJAU, LOW); 
  digitalWrite(PIN_KUNING, LOW); 
  digitalWrite(PIN_MERAH, LOW);

  // Klasifikasi status berdasarkan Slide 2 PPT dan pengaturan LED
  if (ntu < BATAS_JERNIH) { 
    status = "JERNIH";     
    digitalWrite(PIN_HIJAU, HIGH); // LED Hijau menyala
  }
  else if (ntu <= BATAS_AGAK) { 
    status = "AGAK KERUH"; 
    digitalWrite(PIN_KUNING, HIGH); // LED Kuning menyala
  }
  else { 
    status = "KERUH";      
    digitalWrite(PIN_MERAH, HIGH);  // LED Merah menyala
  }

  Serial.print(F("Vs: ")); Serial.print(v, 2);
  Serial.print(F(" V | NTU: ")); Serial.print(ntu, 1);
  Serial.print(F(" | ")); Serial.println(status);

  delay(500);
}