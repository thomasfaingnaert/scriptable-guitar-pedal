A = load('audiotest'); % Hallelujah from Handel with 8192 Hz
Fs = 8192;
delay = 0.5; % In seconds
decay = 0.6;
lengthlim = 3; % In seconds

y = getfield(A,'y');


% Only 1 echo
%{
buf = Delay.readSamples(y); % Makes sure that input is in right format
output = Delay.makeOutputSamples1(buf, Fs, delay, decay);
sound(output);
%}

% Multiple echoes
buf = Delay.readSamples(y); % Makes sure that input is in right format
output = Delay.makeOutputSamples2(buf, Fs, delay, decay, lengthlim);
sound(output);
