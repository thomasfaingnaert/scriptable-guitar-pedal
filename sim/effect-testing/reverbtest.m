[x, Fs] = audioread('../samples/dry-guitar-cut.wav');

room_size = 3.5;
volume = 0.8;

y = reverb.makeOutputSamples1(x, Fs, room_size, volume);

sound(y, Fs);