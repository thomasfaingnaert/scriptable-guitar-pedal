[y,Fs] = audioread('../samples/agc-test.wav');

input_power_treshold = -33;
output_power = -22;
block_size = 0.75 * Fs;

output = agc.makeOutputSamples(y, input_power_treshold, output_power, block_size);
sound(output, Fs);
