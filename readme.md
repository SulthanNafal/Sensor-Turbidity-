# 🌊 Water Turbidity Monitoring System (Arduino Uno)

Sistem pemantauan kekeruhan air (*turbidity sensor*) berbasis **Arduino Uno** yang dilengkapi fitur kalibrasi tombol mandiri serta indikator LED tiga warna untuk mengklasifikasikan tingkat kejernihan air secara langsung (*real-time*).

---

## 🚀 Fitur Utama

- **Pembacaan Stabil:** Menggunakan metode *oversampling* (30 sampel) untuk menyaring *noise* pembacaan tegangan analog.
- **Fitur Kalibrasi Tombol:** Proses offset kalibrasi 0 NTU secara cepat hanya dengan menekan tombol saat sensor dicelupkan ke air jernih.
- **Indikator Visual LED:**
  - 🟢 **LED Hijau:** Air Jernih ($< 5\text{ NTU}$)
  - 🟡 **LED Kuning:** Air Agak Keruh ($5 - 50\text{ NTU}$)
  - 🔴 **LED Merah:** Air Keruh ($> 50\text{ NTU}$)
- **Koreksi Matematik Formulasi NTU:** Menggunakan fungsi polinomial untuk konversi tegangan sensor ke skala NTU.

---

## 🛠️ Alat dan Bahan

1. **Arduino Uno**
2. Modul Sensor Turbiditas (Analog Output)
3. 1x Push Button (Tombol Kalibrasi)
4. 3x LED (Hijau, Kuning, Merah)
5. Resistor secukupnya (misal: 220Ω untuk LED, 10kΩ untuk pembagi tegangan jika diperlukan)
6. Breadboard & Kabel Jumper

---

## 🔌 Pinout & Rangkaian

> ⚠️ **PENTING:** **Sesuaikan pin nya** pada tabel di bawah ini jika kamu menghubungkan kabel ke pin yang berbeda pada papan Arduino Uno milikmu.

| Komponen | Pin Komponen | Pin Arduino Uno | Keterangan |
| :--- | :--- | :--- | :--- |
| **Sensor Turbiditas** | AO (Analog Out) | `A0` | Input Analog pembacaan sensor |
| **Push Button** | Terminal 1 | `10` | Mode `INPUT_PULLUP` (Tombol Kalibrasi) |
| **LED Hijau** | Anoda (+) | `7` | Indikator Status: JERNIH |
| **LED Kuning** | Anoda (+) | `8` | Indikator Status: AGAK KERUH |
| **LED Merah** | Anoda (+) | `9` | Indikator Status: KERUH |
| **GND / VCC** | Power | `GND` / `5V` | Jalur Daya Utama |

---

## 💻 Panduan Penggunaan

1. **Persiapan:** 
   - Rakit seluruh komponen sesuai dengan tabel konfigurasi pin di atas.
   - Buka file program di **Arduino IDE**, pastikan papan terpilih sebagai **Arduino Uno**.
2. **Upload:** Unggah kode ke papan Arduino Uno kamu.
3. **Serial Monitor:** Buka Serial Monitor dan atur *baud rate* ke `115200`.
4. **Proses Kalibrasi:**
   - Celupkan probe sensor kekeruhan ke dalam **air jernih/bersih**.
   - Tekan **Tombol Kalibrasi** (Pin 10).
   - Ketiga LED akan berkedip 3 kali menandakan nilai `vOffset` berhasil disimpan dan titik acuan 0 NTU sudah tersetel.

---

## 📐 Persamaan & Konversi

Konversi tegangan pembacaan ($V_c$) ke nilai NTU dihitung menggunakan persamaan polinomial berikut:

$$NTU = -1120.4 \times V_c^2 + 5742.3 \times V_c - 4352.9$$

Di mana:
- $V_s$ = Tegangan sensor hasil pembacaan analog.
- $V_{off}$ = Tegangan offset kalibrasi ($V_{s,\text{jernih}} - 4.2\text{V}$).
- $V_c = V_s - V_{off}$ (Tegangan terolesi/terkoreksi).

---

## 📄 Lisensi

Proyek ini dibuat untuk keperluan edukasi dan bersifat *open-source*. Bebas untuk dimodifikasi dan dikembangkan lebih lanjut.