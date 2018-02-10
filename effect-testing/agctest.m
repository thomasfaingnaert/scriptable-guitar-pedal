[y,Fs] = audioread('../samples/smoke-on-the-water.wav');

input_power_treshold = -55;
output_power = -22;
block_size = 0.5 * Fs;

output = agc.makeOutputSamples(y, input_power_treshold, output_power, block_size);
sound(output, Fs);
