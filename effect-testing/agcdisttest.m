[x,Fs] = audioread('../samples/smoke-on-the-water.wav');

input_power_treshold = -55;
output_power = -24;
block_size = Fs;

y = agc.makeOutputSamples(x, input_power_treshold, output_power, block_size);

formula = 3;

if exist('formula', 'var') ~= 1
    disp('Please create a variable ''formula'' in the current workspace, set it to 1, 2 or 3 and run this script again.');
    return;
end

switch formula
    case 1
        % Formula 1
        treshold = 0.05;
        vol = 0.6;
        output = distortion.makeOutputSamples1(y,treshold,vol);

        sound(output,Fs);
    case 2
        % Formula 2
        tau = 30;
        vol = 0.5;
        output = distortion.makeOutputSamples2(y,tau,vol);

        sound(output,Fs);
    case 3
        % Formula 3
        alpha = 15;
        vol = 0.8;
        output = distortion.makeOutputSamples3(y,alpha,vol);

        sound(output,Fs);
    otherwise
        disp('Formula should be 1, 2 or 3.');
        return;
end
