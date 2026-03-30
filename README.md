# 🌞 SolarPack Buck Converter
[![MATLAB](https://img.shields.io/badge/MATLAB-R2024b-orange?logo=mathworks)](https://www.mathworks.com/)
[![Simulink](https://img.shields.io/badge/Simulink-Model-blue?logo=mathworks)](https://www.mathworks.com/products/simulink.html)
[![License](https://img.shields.io/badge/License-NC--State--SolarPack-lightgrey)](#-license)
[![Status](https://img.shields.io/badge/Status-Active-success)](#)

### ⚡ Efficient High-Input Buck Converter for Solar-Powered Electric Vehicles

---

### 📘 Overview
The SolarPack Buck Converter is a high-voltage DC-DC converter that steps down input voltages between 250 V – 420 V to a regulated 13.7 V output for float-charging a 12 V SLA battery in a solar-powered electric vehicle. The design focuses on converter stability, reliable regulation, low output ripple, and clean transitions suitable for long-term battery health and EV integration.

This repository currently contains MATLAB/Simulink models and a component list, plus guidance for PCB and hardware implementation.

---

### 🧾 Design Goals and Requirements

- Primary goal: Deliver a regulated 13.7 V float charge from a 250 V–420 V PV/bus with efficiency > 80% under expected operating conditions.
- Measured efficiency: Current measured converter efficiency is 82–85%, varying with input voltage and operating point.
- Peak current target: **7 A** (design constrained by inductor).
- Current ripple requirement: **10%** of nominal current at the chosen operating point.
- Voltage ripple requirement: **1%** of Vout (≈ 0.137 V).
- Inductance requirement: **L > 850 µH** desired for ripple control.
- Maintainability: Simulink model parameterization and a component list to speed iteration.
- PCB design: Detailed PCB layout and thermal considerations are required before hardware prototyping.

---

### 🧠 Key Design Specifications

Parameter | Symbol | Value Range | Description
---|---:|---|---
Input Voltage | Vg | 250 V – 420 V | Solar bus input range
Output Voltage | Vout | 13.7 V | Float charge target
Output Current | Iout | up to 7 A (peak) | Design constrained by inductor
Current Ripple | ΔI / I | 10% | Peak-to-peak current ripple spec (relative to nominal)
Voltage Ripple | ΔV / Vout | 1% | Peak-to-peak voltage ripple spec (≈ 0.137 V)
Inductance | L | > 850 µH | Target inductance to meet ripple spec
Output Capacitor | C | calculated per ripple goal | Sized with ESR target to meet ΔV ≤ 1%
Switching Frequency | Fs | 50 kHz nominal | Tradeoff between losses, EMI, and passive size
Efficiency Goal | η | > 80% | Measured 82–85% depending on input voltage
Control | Type | Voltage-mode or current-mode PWM | Includes feedforward for wide input range

---

### 🧮 Design Methodology and Key Equations

- Duty cycle approximation at steady-state ideal converter:
  D ≈ Vout / Vg

- Inductor ripple current (peak-to-peak):
  ΔIL = ((Vg − Vout) / L) · D_on · (1 / Fs)
  design for ΔIL such that ΔIL ≤ 0.10 · I_nom where I_nom is the nominal load current.

- With L > 850 µH, verify ΔIL across Vg = 250–420 V and Fs = 50 kHz meets the 10% ripple spec.

- Output capacitor selection from voltage ripple:
  ΔVC ≈ ΔIL / (8 · Fs · C)
  include ESR contribution: ΔVESR = I_ripple · ESR
  choose C and ESR so that (ΔVC + ΔVESR) ≤ 0.01 · Vout (≈ 0.137 V).

- Efficiency estimate:
  η = Pout / Pin = (Vout · Iout) / (Vg · Ig)
  include conduction and switching loss terms when refining.

---

### 🔌 Battery-as-Load Implementation Details

- Load strategy: The vehicle battery is used as the primary load during testing and normal operation.
- Series current limiting: A current-limiting resistor is placed in series with the battery to limit inrush and control charge current during connection and transient events.
- Backflow prevention: A protection diode (or ideal diode controller) in series prevents the battery from discharging back into the buck converter input or upstream PV/bus during low-input or reverse conditions.
- Operational notes: Soft-start and controlled ramping of the duty cycle reduce stress on the series resistor and diode during battery connect; monitor resistor and diode temperature during extended operation.

---

### 🪧 Inductor Design — Challenges and Requirements

Design challenges:
- Wide duty-cycle range: D varies roughly from 13.7/250 to 13.7/420; the inductor must tolerate large volt-second variation without saturating.
- Ripple vs. size tradeoff: Achieving 10% current ripple at up to 7 A peak with Fs = 50 kHz requires L > 850 µH to meet the spec without excessive winding losses or impractical core size.
- Core saturation under DC bias: Must select a core and geometry that maintain required inductance at 7 A DC plus ripple current.
- Losses and heating: High RMS currents cause I^2R losses; AC losses (proximity, eddy) in the winding and core-loss at switching frequency must be minimized.
- EMI contribution: Winding layout, shielding, and magnetics placement affect conducted and radiated emissions.
- Mechanical and packaging constraints: Volume, weight, and mounting options in an EV limit achievable inductance and cooling.
- Manufacturing variability: Tolerances in inductance, wire resistance, and core properties across batches and temperature must be accounted for.

Requirements to achieve success:
- Target inductance: Obtain and validate L > 850 µH with margin for tolerance and temperature drift.
- Flux margin: Choose core material with adequate saturation current so peak flux density remains below saturation at 7 A plus ripple margin.
- Winding strategy: Use appropriate conductor (thicker gauge, multiple parallel strands, or litz) to reduce DC resistance and AC losses.
- Thermal management: Define acceptable winding/core temperature rise and implement cooling or heat sinking as needed.
- Verification: Prototype and measure ΔIL across Vg = 250–420 V, confirm ΔIL ≤ 10% of nominal current, verify no core saturation at 7 A, measure temperature rise at rated duty cycle, and perform EMI scans.
- Contingency tradeoffs: If L > 850 µH cannot be achieved within size/weight/thermal constraints, consider increasing Fs, relaxing ripple spec, adding active ripple cancellation, or redesigning the power stage (parallel inductors or switched-capacitor techniques).

---

### 🔧 Files and Components Present in This Repo

- MATLAB / Simulink
  - SolarPackBuckConverter.slx — primary system-level model with averaged measurements and switch-level options
  - SolarPackBuckConverter.m — initialization and batch-run script
- Component list
  - components/ComponentList.csv or .xlsx — current BOM and candidate parts (MOSFETs, diodes/ideal-diode controllers, sense resistors, passive components)
- PCB and Schematic
  - hardware/ — notes and requirements for PCB layout, thermal paths, high-voltage creepage, and component placement

---

### 🚀 How to Run the Simulation

1. Clone the repository:
   git clone https://github.com/<your-username>/SolarPack-Buck-Converter.git
   cd SolarPack-Buck-Converter

2. Open MATLAB and set the working directory to the repository root.

3. Run the initialization and simulation script:
   SolarPackBuckConverter.m

4. The script will load the Simulink model, run simulations across configured operating points, and output summary plots and a results table showing Vout, Iout, Pin, Pout, and η.

Simulation outputs include averaged measurements to reduce switching noise and present long-term behavior.

---

### 🛠 PCB Design Notes and Requirements

- High-voltage spacing and creepage: Ensure PCB clearances meet the maximum operating voltage (420 V) plus margin and conform to applicable automotive/EV guidelines for creepage and clearance.
- Thermal management: Define thermal vias, copper pours, and heat sinks for MOSFETs, diode(s), and the inductor; place high-loss parts to optimize heat flow to chassis or dedicated heat spreaders.
- Power loop minimization: Keep switching node loop areas small; mount input and output capacitors close to switching devices to limit EMI.
- Grounding strategy: Separate power and signal grounds; provide a single-point star or controlled return to avoid ground loops affecting control circuitry.
- EMI suppression: Reserve space for common-mode choke, Y-cap placement if needed, and snubbers across switching nodes; plan for filter test points.
- Test and debug: Include test pads for switching node, gate signals, sense resistor nodes, thermistor terminals, and an isolated programming header for controller firmware.
- Mechanical mounting: Provide secure mounting for the inductor with mechanical support and consider potting or conformal coating for vibration tolerance.

---

### 🗺️ Next Steps and Roadmap

- Finalize component selection and BOM with verified footprints
- Complete KiCad schematic and PCB layout focusing on thermal paths and EMI containment
- Build prototype and validate magnetics, thermal behavior, and regulation in the lab
- Integrate telemetry and fault logging for in-vehicle diagnostics when hardware validation begins

---

### 🧾 License
NC State SolarPack internal license. See LICENSE.md for details.

---

### 📬 Contributing

- Fork the repo, create a feature branch, and submit PRs with evidence for changes to the models or component list
- For major design changes, open an issue describing objective, expected impact, and verification plan

---

### Contact
Project maintained by NC State SolarPack engineering team. For design questions, open an issue or start a discussion in the repo.
