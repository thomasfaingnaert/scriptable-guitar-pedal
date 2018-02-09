[y,Fs] = audioread('../samples/dry-guitar-cut.wav');
delayT = 0.2; % In seconds
decay = 0.8;


output = delay.makeOutputSamples(y, Fs, delayT, decay);
sound(output,Fs);
