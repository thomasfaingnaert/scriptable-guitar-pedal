[y,Fs] = audioread('../samples/delaytest-guitar.wav');
delayT = 0.2; % In seconds
decay = 0.8;
amount = 4; % only for formula 2

if exist('formula', 'var') ~= 1
    disp('Please create a variable ''formula'' in the current workspace, set it to 1 or 2 and run this script again.');
    return;
end

switch formula
    case 1
        % Formula 1
        output = delay.makeOutputSamples(y, Fs, delayT, decay);
        sound(output,Fs);
    case 2
        % Formula 2
        output = delay.makeOutputSamples(y, Fs, delayT, decay, amount);
        sound(output,Fs);
    otherwise
        disp('Formula should be 1 or 2');
        return;
end
