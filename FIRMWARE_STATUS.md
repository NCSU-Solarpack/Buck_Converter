# Buck Converter Firmware - Where We're At

## Overview

Two-stage buck converter for SolarPack's solar EV. Stage 1 takes the 250-420V solar bus down to 80V, Stage 2 takes 80V down to 13.7V to float-charge a 12V SLA battery.

The MCU is a dsPIC33CK Curiosity Nano. There's a part number mismatch in the project - nbproject/configurations.xml says dsPIC33CK64MC105 but the MCC config (.mc3 and mcc-manifest) says dsPIC33CK256MC105. The MCC files are more recent so the 256 variant is likely correct, but Estyn should confirm.

Switching at 200 kHz (not 50 kHz like the README claims). Built with MPLAB X + XC16 compiler + MCC Melody.

---

## Architecture and control approach

**Voltage-mode control, single loop per stage.** There's no current sensing hardware or current feedback anywhere - the ADC only reads two voltage channels (AN2 for Stage 1, AN3 for Stage 2) through resistor dividers. The MATLAB simulation measures currents for power loss analysis but doesn't feed them back for control. The PID controllers are straightforward voltage-error-to-duty-cycle loops.

**Clock/oscillator config is handled by MCC Melody.** The .mc3 file includes the pic24-dspic33-clock-sib and dspic33-configuration-bits modules. No #pragma config lines needed in source - MCC generates those internally. The code assumes 100 MHz Fosc / 400 MHz HSPWM and that's what MCC is set up for.

**Gate driver is IR2101STRPBF** (from the component spreadsheet). Dead time is currently set to 75ns (30 counts at 400 MHz). The IR2101 has propagation delays around 100-150ns, so 75ns is on the tight side. Should probably bump to 100-120ns (40-48 counts) for margin against shoot-through once we're testing on hardware.

---

## What's implemented now

### Original scaffold (was already in the repo)
- **System init (Init.c)** - sets up voltage divider ratios from resistor values in macros.h
- **ADC init (Init.c)** - configures AN2 for Stage 1 sensing and AN3 for Stage 2, 12-bit resolution, shared core
- **ADC reads (adc.c)** - ADC_Read_AN2() and ADC_Read_AN3() do single conversions and return from the right buffers
- **Voltage scaling (Control.c)** - raw ADC counts -> pin voltage in mV -> full rail voltage through divider ratio
- **Error calc (Control.c)** - setpoint minus measured for each stage
- **PWM duty setting (pwm.c)** - PWM_SetDuty_Stage1/2 write to PGxDC with clamping
- **Macros (macros.h)** - all defines and externs

### New: PID controller (pid.h / pid.c)
Integer-only PID (no floats, runs on dsPIC without FPU overhead). Takes voltage error in millivolts, outputs duty cycle counts (0 to 2000). Features:
- Proportional, integral, derivative terms
- Gains scaled by PID_SCALE (1000) for fine control without floats - e.g. Kp=2 means effective gain of 0.002
- Integral anti-windup: integral term clamped to +/- int_max
- Output clamping to [out_min, out_max] (set to [0, PWM_Period_Counts])
- PID_Reset() zeros integral and previous error (used during soft-start)

Current gains are conservative starting points, will need tuning on hardware:
- Stage 1 (250-420V -> 80V): Kp=2, Ki=1, Kd=0
- Stage 2 (80V -> 14V): Kp=5, Ki=2, Kd=0

### New: Timer1 ISR with soft-start (timer.h / timer.c)
20 kHz Timer1 interrupt runs the full control loop at a fixed timestep (50us). This is what PID math requires - Ki and Kd only make sense with a known dt.

**Soft-start state machine:**
- STATE_SOFTSTART: duty ramps linearly from 0 to the ideal steady-state duty over 100ms (2000 ticks). Stage 1 ramps to 380 counts (D=80/420=0.19), Stage 2 ramps to 350 counts (D=14/80=0.175). PID is held in reset during this phase.
- STATE_RUN: PID takes over for closed-loop voltage regulation.

The ramp prevents inrush current that would happen if we jumped straight to operating duty with a 420V input.

### Updated: main.c
Stripped the control loop out. main() now just does init (System_Init, ADC_Init, PWM_Init, Timer1_Init) and sits in an idle while(true). All real-time control happens in the Timer1 ISR.

---

## Bugs found and fixed

### BUG 1 - Infinite recursion in pwm.c (would crash instantly)
The first function in pwm.c was calling itself before doing anything else. Instant stack overflow on boot. It was supposed to be PWM_Init() - renamed it and it now correctly calls Period_Calc() first.

### BUG 2 - Two functions named Period_Calc (wouldn't compile)
Two Period_Calc() definitions in pwm.c. Fixed by renaming the first one to PWM_Init() which is what main.c and pwm.h expect.

### BUG 3 - PWM_Period_Counts was never computed
Period_Calc() calculated Target_Duty_Period_us but never set PWM_Period_Counts. PG1PER and PG2PER would load with 0 - no PWM output. Added PWM_Period_Counts = HSPWM_CLOCK_HZ / FREQUENCY (= 2000 counts).

### BUG 4 - adc.h didn't match adc.c
Header declared ADC_READ() in all caps with wrong param style. Fixed to match the actual ADC_Read_AN2() and ADC_Read_AN3() implementations. Also fixed the header guard.

### BUG 5 - Typo in pwm.h
PWM_SetDuty_STage2 (capital T) - fixed to PWM_SetDuty_Stage2.

### BUG 6 - PWM_Init() declared but never defined
Fixed by the rename in Bug 2.

---

## File map

```
Buck_Converter.X/
  main.c        - init sequence + idle loop
  Init.c/.h     - system init (divider ratios) + ADC hardware config
  Control.c/.h  - ADC read wrappers, voltage scaling, error calc
  pwm.c/.h      - PWM generator config, period calc, duty setters
  adc.c/.h      - raw ADC read functions for AN2 and AN3
  pid.c/.h      - integer PID controller with anti-windup     [NEW]
  timer.c/.h    - Timer1 ISR, control loop, soft-start ramp   [NEW]
  macros.h      - all defines, externs, includes
```

---

## What's left to do

1. **Tune PID gains on hardware** - current values are just starting points
2. **Dead time adjustment** - bump from 75ns to ~100ns for IR2101 margin during board bring-up
3. **Fault protection** - OV/UV shutdown, duty cycle hard limits
4. **Fix README** - wrong switching freq, wrong filenames, missing directories
5. **Clarify MCU part number** with Estyn

---

## README stuff that's wrong

- Says switching freq is 50 kHz - code and MATLAB both use 200 kHz
- References SolarPackBuckConverter.m - actual file is Buck_Design.m
- References components/ComponentList.csv - actual file is Component_List_and_Testing.xlsx at root
- References a hardware/ directory that doesn't exist

---

## Remaining question

The MCC config (.mc3) says dsPIC33CK256MC105 but nbproject says 64MC105. Which one is on the Curiosity Nano board we have? Doesn't change any firmware logic but affects flash size.
