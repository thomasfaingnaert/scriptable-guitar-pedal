[y,Fs] = audioread('tremolotest-guitar.wav');

depth = 0.4;
rate = 0.2;
output = tremolo.makeOutputSamples(y,Fs,depth,rate);

sound(output,Fs);
