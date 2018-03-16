[y,Fs] = audioread('../samples/presentatie-sample.wav');

depth = 0.8;
rate = 0.2;
output = tremolo.makeOutputSamples(y,Fs,depth,rate);

sound(output,Fs);
