[y,Fs] = audioread('tremolotest-guitar.wav');

depth = 0.4;
rate = 0.2;
output = tremolo.makeOutputSamples(y,Fs,depth,rate);

sound(output,Fs);
audiowrite('tremolo1.wav',output,Fs);

plot(output(1:500), 'r');
hold on;
plot(y(1:500), 'b');