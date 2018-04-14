[x, Fs] = audioread('../samples/presentatie-sample.wav');

room_size = 6.5;
volume = 0.8;

y = reverb.makeOutputSamples1(x, Fs, room_size, volume);

sound(y, Fs);