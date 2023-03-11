clear; clc;

t(1, :) = 7 * 11025:1:(11025 * 14);
t(2, :) = 7:1/11025:14;
[channels, Fs] = audioread("Estes_J_orig.wav", "native");

left_channel = channels(:, 1);
right_channel = channels(:, 2);

% LEFT CHANNEL 
f_l = 1810;
a_l = 300;
phase_l = pi/4;

% RIGHT CHANNEL
f_r = 1990;
a_r = 250;
phase_r = 3*pi/4;

left_channel(t(1, :)) = a_l.*toDegrees("radians", sin(2*pi*f_l*t(2, :) + phase_l));
right_channel(t(1, :)) = a_r.*toDegrees("radians", sin(2*pi*f_r*t(2, :) + phase_r));

channels(:, 1) = left_channel;
channels(:, 2) = right_channel;

audiowrite("ESTES_J_mod.wav", channels, Fs);

figure(1)
plot(left_channel)
