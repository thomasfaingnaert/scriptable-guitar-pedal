[x, Fs] = audioread('../samples/oasis-guitar.wav');

room_size = 1.5;
volume = 0.8;

y = reverb.makeOutputSamples1(x, Fs, room_size, volume);

sound(y, Fs);