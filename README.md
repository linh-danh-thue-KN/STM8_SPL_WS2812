📌 STM8S103F3P6 WS2812 Bit-Bang Control
🎯 Mục tiêu
Điều khiển dải LED WS2812 (NeoPixel) sử dụng vi điều khiển STM8S103F3P6, với phương pháp bit-banging thông qua chân PD4.

⚙️ Thông tin phần cứng
Thành phần	Thông tin
MCU	STM8S103F3P6 (STM8S Blue Pill)
LED	WS2812 hoặc WS2812B
Chân điều khiển	PD4
Điện áp hoạt động	5V (MCU + WS2812 cùng nguồn)

+----------------------+
|     STM8S103F3P6     |
|                      |
|         PD4 o--------+--------o DI     (WS2812)
|                      |        |
|        GND o------------------+--------o GND
|                      |        |
|        VDD o------------------+--------o VDD
+----------------------+


Lưu ý: WS2812 yêu cầu tín hiệu logic 5V. Nếu dùng nguồn 3.3V, cần mạch chuyển mức tín hiệu.

🧠 Nguyên lý hoạt động
Giao tiếp WS2812 yêu cầu tín hiệu PWM thời gian chính xác để biểu diễn bit 0 và 1. Do không có DMA hoặc PWM tốc độ cao trên STM8S103F3P6, dự án sử dụng bit-banging thông qua lệnh NOP để giữ thời gian đúng từng chu kỳ.

### ⏱️ Thời gian tín hiệu WS2812

| Bit | Thời gian HIGH (µs) | Thời gian LOW (µs) |
|:---:|:--------------------:|:------------------:|
| 0   | ~0.4 µs             | ~0.85 µs           |
| 1   | ~0.8 µs             | ~0.45 µs           |
