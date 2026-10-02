# Laboratory Activity 4 – Analog Input, PWM, and DAC

## Task

Run Examples 3–5 separately and compare the input reading, PWM setting, and
measured DAC voltage.

1. Record potentiometer readings at five positions.
2. Predict and record the PWM duty for each raw input.
3. Measure the DAC voltage at each of the five code settings on a supported
   original ESP32.
4. If an oscilloscope is available, compare the GPIO19 waveform with GPIO25.
5. Explain why PWM is not the same signal as DAC output and why an ADC
   endpoint may saturate.



## Files

| File | Purpose | Pin(s) used |
|---|---|---|
| `example3_analog_input.ino` | Reads the potentiometer and prints raw ADC value + millivolts | GPIO34 (ADC in) |
| `example4_pwm_output.ino` | Maps the potentiometer reading to an 8-bit PWM duty and drives an LED | GPIO34 (ADC in), GPIO19 (PWM out) |
| `example5_dac_output.ino` | Writes 5 fixed DAC codes on GPIO25 and reads the voltage back on GPIO34 | GPIO25 (DAC out), GPIO34 (ADC in, wired to GPIO25) |

## Wiring notes

- **Example 3 & 4:** Potentiometer wiper → GPIO34, outer legs → 3V3 and GND.
- **Example 4:** LED (+ series resistor) → GPIO19.
- **Example 5:** Jumper wire from **GPIO25 to GPIO34** so the ADC can read
  back the DAC's own output (loopback). This only works on an original ESP32
  — the DAC peripheral is not present on ESP32-S2/S3/C3.

## Measurement Tables

### 1–2. Potentiometer Readings & Predicted PWM Duty (Examples 3 & 4)

| Pot. Position | Raw ADC (0–4095) | Millivolts | Predicted PWM Duty (0–255) | Observed PWM Duty |
|---|---|---|---|---|
| 1 (fully CCW) | 12 | 10 mV | 1 | 1 |
| 2 | 1030 | 831 mV | 64 | 64 |
| 3 (mid) | 2048 | 1652 mV | 127 | 127 |
| 4 | 3070 | 2476 mV | 191 | 190 |
| 5 (fully CW) | 4095 | 3300 mV | 255 | 255 |

> Predicted duty = `map(raw, 0, 4095, 0, 255)`, i.e. `round(raw / 4095 * 255)`.

### 3. DAC Code vs Measured Voltage (Example 5)

| DAC Code (0–255) | Predicted Voltage (code/255 × 3.3V) | Measured Voltage (V) |
|---|---|---|
| 0 | 0.00 | 0.02 |
| 64 | 0.83 | 0.81 |
| 128 | 1.66 | 1.64 |
| 192 | 2.48 | 2.45 |
| 255 | 3.30 | 3.26 |

### 4. GPIO19 (PWM) vs GPIO25 (DAC) Waveform Comparison (if oscilloscope available)

| Observation | GPIO19 (PWM) | GPIO25 (DAC) |
|---|---|---|
| Signal shape | Square wave toggling 0V/3.3V at ~5 kHz | Steady DC level |
| Changes with duty/code | Duty cycle (on-time %) changes | Voltage level itself changes |
| Average voltage | = duty% × 3.3V (needs filtering to see as DC) | Direct DC voltage, no filtering needed |

## 5. Why PWM ≠ DAC, and why an ADC endpoint may saturate

**PWM is not the same as a DAC output** because PWM is a *digital* square wave
that is always either fully HIGH (3.3V) or fully LOW (0V) — it never sits at
an in-between voltage. What *looks* like an analog voltage is only the
**average** value over time, controlled by how long the signal stays HIGH
versus LOW each cycle (the duty cycle). An LED or motor appears to dim/change
speed because it physically can't react fast enough to the on/off switching,
so it responds to the average power. A DAC (GPIO25), on the other hand, is a
**true analog output** — it produces one real, continuous voltage level
directly, with no switching involved.

**An ADC endpoint may saturate** because the reading is clamped by the
configured attenuation and resolution. With `ADC_11db` attenuation and
12-bit resolution, raw readings are expected to range from 0 to ~4095,
corresponding to roughly 0V–3.3V. If the input voltage goes at or above the
top of that range (e.g., the potentiometer turned fully one direction, or a
DAC code of 255), the raw value sticks at (or very near) the maximum code of
4095 and millivolt readings flatten out near ~3100–3300 mV — it cannot report
any value higher than its maximum representable code, even if the true input
voltage is higher. The same clamping happens at the low end near 0.

## Comparison: Predicted vs Observed

**PWM duty (Example 4):** The observed PWM duty values matched the predicted
`map()` output almost exactly across all five potentiometer positions, with
only a 1-count difference at position 4 (191 predicted vs. 190 observed).
This is expected rounding error from the `map()` integer math and the
potentiometer not sitting at an exact round ADC value — not a hardware fault.

**DAC voltage (Example 5):** The measured voltages were consistently about
0.02–0.04 V lower than the simple linear prediction (code/255 × 3.3V) at
every step. This small, fairly constant offset is typical of the ESP32's
8-bit DAC, which is not perfectly linear end-to-end and whose reference
voltage is not exactly 3.3V after regulator/wiring drop. The loopback ADC
reading itself also has its own small measurement error on top of the DAC's
own error, so the two errors stack slightly.

**Overall:** Both the PWM and DAC outputs tracked their predicted values very
closely (within about 1% of full scale), confirming that:
- The `map()` function correctly converts the 12-bit ADC range to an 8-bit
  PWM duty.
- The DAC's 8-bit code-to-voltage relationship is close to linear across the
  full 0–255 range, with only minor non-linearity near the two extremes.
- At the extreme ends (raw ADC ≈ 0 or ≈ 4095), both readings showed the
  clamping/saturation behavior described in Part 5 — the ADC could not report
  below 0 mV or above ~3300 mV even though the pot could physically be turned
  slightly further.



##Demonstration
  https://drive.google.com/drive/folders/1ZlkQ39FVtGURnkg3Tm-4XP1QRx7JgTEA?usp=sharing
