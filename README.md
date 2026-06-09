# Anggota Kelompok 5

* MUHAMMAD NABIL ZAEDAN AGESY - H1H024062
* FATHAH IKHWANSYAH - H1H024063
* LULA KHAISHA DELAVIA - H1H024064
* AURELLIA NABILA RAHMA PUTRI - H1H024066
* MUHAMMAD IMAM SALAFUDIN - H1H024067
* FIKRI RIZQIN FAIZUN - H1H024068

## 🔧 Komponen Hardware


| Komponen | Keterangan |
|----------|------------|
| ESP-32 | Mikrokontroler utama |
| Motor DC + Transistor TIP120 | Kipas, dikontrol PWM pin 3 |
| LCD 16x2 I2C | SDA/SCL Arduino |
| Breadboard + Kabel Jumper | Prototyping |
| Sensor Api KY-026 | Mendeteksi tanda ada Api |
| Sensor Gas MQ-2 | Mendeteksi tanda ada Gas |
| LED Merah | Mendeteksi tanda ada Bahaya |
| LED Hijau | Mendeteksi Aman |
| Buzzer | Untuk membunyikan alarm saat bahaya |
| PCB | Saat prototype nya aman akan di aplikasikan ke PCB |

---
 
## 📌 Deskripsi Proyek
 
Sistem ini mendeteksi potensi kebakaran secara dini menggunakan:
- **Sensor Api KY-026** → mendeteksi keberadaan nyala api
- **MQ-2** → mendeteksi konsentrasi gas mudah terbakar dan asap
- **Motor DC + TIP120** → kipas yang menyala otomatis
- **LCD 16x2 I2C** → menampilkan informasi status sistem secara real-time
- **Buzzer** → memberikan peringatan suara ketika terdeteksi kondisi bahaya
- **LED Merah** → indikator kondisi bahaya
- **LED Hijau** → menandakan kondisi aman
---
