
%% Converter Parameters
Vg      = 420;                 % Input voltage [V]
V       = 14;                  % Output voltage [V]
Vstage  = 80;                  % Intermediate stage voltage [V]
Rbat    = 0.008;               % Battery internal resistance [Ω]
Rlimit  = 0.1;                 % Precharge current limit resistor [Ω]
RlimitPass = 0.1;              % Passthrough current limit resistor [Ω]
F       = 200e3;               % Switching frequency [Hz]
Ts      = 1 / F;               % Switching period [s]
SOC = 100;                     % Battery state of charge
Pload = 500;                   % Load

% Ripple specifications
diL1     = 1.3;                % Inductor 1 current ripple [A]
diL2     = 7.4;                % Inductor 2 current ripple [A]
dVc     = 0.1;                 % Capacitor voltage ripple [V]

% Duty cycles
D1      = Vstage / Vg;         % Stage 1 duty cycle
D2      = V / Vstage;          % Stage 2 duty cycle

%% Component Sizing (Ideal)
L1 = ((Vg - Vstage) / diL1) * (D1 * Ts);                     % Stage 1 inductance [H]
C1 = ((Vg - Vstage) / (16 * L1 * dVc)) * (D1 * Ts)^2;        % Stage 1 capacitance [F]
L2 = ((Vstage - V) / diL2) * (D2 * Ts);                      % Stage 2 inductance [H]
C2 = ((Vstage - V) / (16 * L2 * dVc)) * (D2 * Ts)^2;         % Stage 2 capacitance [F]

%% Actual Component Values
% Inductors
L1_actual = 270e-6;     % Stage 1 inductance [H]
L1_esr    = 14e-3;      % Stage 1 ESR [Ω]
L2_actual = 12e-6;      % Stage 2 inductance [H]
L2_esr    = 2.08e-3;    % Stage 2 ESR [Ω]

% Capacitors
C1_actual = 470e-6;     % Stage 1 capacitance [F]
C1_esr    = 50e-3;      % Stage 1 ESR [Ω]
C2_actual = 470e-6;     % Stage 2 capacitance [F]
C2_esr    = 50e-3;      % Stage 2 ESR [Ω]

%% Semiconductor Parameters
Ron = 17e-3;            % MOSFET on-resistance [Ω]
Vf  = 5.1;              % Diode forward voltage [V]

Rdiode = 0.27;
Vfdiode = 0.3;

Rdiode_Charge = 0.0129;
Vfdiode_Charge = 0.47;

%% Setup
clc;

model = 'SolarPackBuckConverter';
simTime = 0.2;

Vg_values    = [420, 360, 270];
SOC_values   = [100,90,80,70];
Pload_values = [500,400,300,200]; 

results = [];
runCount = 0;
totalRuns = numel(Vg_values) * numel(SOC_values) * numel(Pload_values);

%% Helper function
getStats = @(ts) struct( ...
    'avg', mean(ts.Data), ...
    'max', max(ts.Data) ...
);

%% Sweep Loop
for vg = Vg_values
    for soc = SOC_values
        for pload = Pload_values
            runCount = runCount + 1;
            fprintf('Running %d of %d: Vg=%d, SOC=%d, Pload=%d\n', runCount, totalRuns, vg, soc, pload);

            try
                % Assign parameters
                assignin('base', 'Vg', vg);
                assignin('base', 'SOC', soc);
                assignin('base', 'Pload', pload);

                % Run simulation
                out = sim(model, 'StopTime', num2str(simTime));

                % Extract time-series signals

                %12V
                Ibat_ts   = out.Battery.Current__A_;
                Vbat_ts   = out.Battery.Voltage__V_;
                %Diodes
                Idel_ts   = out.Battery_Delivery_Diode.Diode_current;
                Vdel_ts   = out.Battery_Delivery_Diode.Diode_voltage;
                Ichg_ts   = out.Charge_Diode.Diode_current;
                Vchg_ts   = out.Charge_Diode.Diode_voltage;
                Ipass_ts  = out.Passthrough_Diode.Diode_current;
                Vpass_ts  = out.Passthrough_Diode.Diode_voltage;
                %Capacitors
                Vcap1_ts  = out.Capacitor_1_Voltage;
                Icap1_ts  = out.Capacitor_1_Current;
                Vcap2_ts  = out.Capacitor_2_Voltage;
                Icap2_ts  = out.Capacitor_2_Current;
                %Inductors
                Vind1_ts  = out.Inductor_1_Voltage;
                Iind1_ts  = out.Inductor_1_Current;
                Vind2_ts  = out.Inductor_2_Voltage;
                Iind2_ts  = out.Inductor_2_Current;
                %Mosfets
                Vfet1_ts  = out.FET1.MOSFET_voltage;
                Ifet1_ts  = out.FET1.MOSFET_current;
                Vfet2_ts  = out.FET2.MOSFET_voltage;
                Ifet2_ts  = out.FET2.MOSFET_current;
                Vfet3_ts  = out.FET3.MOSFET_voltage;
                Ifet3_ts  = out.FET3.MOSFET_current;
                Vfet4_ts  = out.FET4.MOSFET_voltage;
                Ifet4_ts  = out.FET4.MOSFET_current;   
                %Resistors
                Vr1_ts    = out.Charge_Limit_Voltage;
                Ir1_ts    = out.Charge_Limit_Current;
                Vr2_ts    = out.Passthrough_Limit_Resistor_Voltage;
                Ir2_ts    = out.Passthrough_Limit_Resistor_Current;
                %Input current
                Iin_ts    = out.Input_Current;


                % Compute stats
                Ibat   = getStats(Ibat_ts);
                Vbat   = getStats(Vbat_ts);
                Idel   = getStats(Idel_ts);
                Vdel   = getStats(Vdel_ts);
                Ichg   = getStats(Ichg_ts);
                Vchg   = getStats(Vchg_ts);
                Iin    = getStats(Iin_ts);
                Ipass  = getStats(Ipass_ts);
                Vpass  = getStats(Vpass_ts);
                Vcap1  = getStats(Vcap1_ts);
                Icap1  = getStats(Icap1_ts);
                Vcap2  = getStats(Vcap2_ts);
                Icap2  = getStats(Icap2_ts);
                Vind1  = getStats(Vind1_ts);
                Iind1  = getStats(Iind1_ts);
                Vind2  = getStats(Vind2_ts);
                Iind2  = getStats(Iind2_ts);
                Vfet1  = getStats(Vfet1_ts);
                Ifet1  = getStats(Ifet1_ts);
                Vfet2  = getStats(Vfet2_ts);
                Ifet2  = getStats(Ifet2_ts);
                Vfet3  = getStats(Vfet3_ts);
                Ifet3  = getStats(Ifet3_ts);
                Vfet4  = getStats(Vfet4_ts);
                Ifet4  = getStats(Ifet4_ts);
                Vr1    = getStats(Vr1_ts);
                Ir1    = getStats(Ir1_ts);
                Vr2    = getStats(Vr2_ts);
                Ir2    = getStats(Ir2_ts);

                %Calculations
                P_chg = (Ichg.avg)^2 * Rdiode;
                P_del = (Idel.avg)^2 * Rdiode;
                P_pass = (Ipass.avg)^2 * Rdiode;

                Pcap1 = (Icap1.avg)^2 * C1_esr;
                Pcap2 = (Icap2.avg)^2 * C2_esr;

                Pind1 = (Iind1.avg)^2 * L1_esr;
                Pind2 = (Iind2.avg)^2 * L2_esr;

                Pfet1 = (Ifet1.avg)^2 * Ron * D1;
                Pfet2 = (Ifet2.avg)^2 * Ron * (1-D1);
                Pfet3 = (Ifet3.avg)^2 * Ron * D2;
                Pfet4 = (Ifet4.avg)^2 * Ron * (1-D2);

                Pr1 = (Ir1.avg)^2 * Rlimit;
                Pr2 = (Ir2.avg)^2 * RlimitPass;

                % Store results
    results(end+1,:) = [vg, soc, pload, ...
    Ibat.avg, Vbat.avg, ...
    Idel.avg, Vdel.avg,  ...
    Ichg.avg, Vchg.avg, ...
    Ipass.avg, Vpass.avg,...
    Icap1.avg, Vcap1.avg, ...
    Icap2.avg, Vcap2.avg, ...
    Iind1.avg, Vind1.avg, ...
    Iind2.avg, Vind2.avg, ...
    Ifet1.avg, Vfet1.avg, ...
    Ifet2.avg, Vfet2.avg, ...
    Ifet3.avg, Vfet3.avg, ...
    Ifet4.avg, Vfet4.avg, ...
    Ir1.avg, Vr1.avg, ...
    Ir2.avg, Vr2.avg, ...
    P_chg, P_del, P_pass, ...
    Pcap1, Pcap2, ...
    Pind1, Pind2, ...
    Pfet1, Pfet2, Pfet3, Pfet4, ...
    Pr1, Pr2, Iin.avg];
                    

            catch ME
                warning('Simulation failed for Vg=%d, SOC=%d, Pload=%d: %s', vg, soc, pload, ME.message);
                continue;
            end
        end
    end
end

%% Display Results
resultTable = array2table(results, ...
'VariableNames', {'Vg','SOC','Pload', ...
    'Ibat_avg','Vbat_avg', ...
    'Idel_avg','Vdel_avg', ...
    'Ichg_avg','Vchg_avg', ...
    'Ipass_avg','Vpass_avg', ...
    'Icap1_avg','Vcap1_avg', ...
    'Icap2_avg','Vcap2_avg', ...
    'Iind1_avg','Vind1_avg', ...
    'Iind2_avg','Vind2_avg', ...
    'Ifet1_avg','Vfet1_avg', ...
    'Ifet2_avg','Vfet2_avg', ...
    'Ifet3_avg','Vfet3_avg', ...
    'Ifet4_avg','Vfet4_avg', ...
    'Ir1_avg','Vr1_avg', ...
    'Ir2_avg','Vr2_avg', ...
    'P_chg','P_del','P_pass', ...
    'Pcap1','Pcap2', ...
    'Pind1','Pind2', ...
    'Pfet1','Pfet2','Pfet3','Pfet4', ...
    'Pr1', 'Pr2', 'Iin'});

disp(resultTable);
%%
filename = 'Component_List_and_Testing.xlsx';
write (resultTable, filename, 'Sheet', 'Data');
