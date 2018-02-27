[x,Fs] = audioread('../samples/delaytest-guitar.wav');

delayT = 0.18;
feedforward = 0.3;
feedback = 0.2;
blend = 1;

gain = 0.4;

a = 0.2;

if exist('formula', 'var') ~= 1
    disp('Please create a variable ''formula'' in the current workspace, set it to 1, 2, 3, 4 or 5  and run this script again.');
    return;
end

switch formula
    case 1
        % universal comb filter
        y = combfilter.universal(x, Fs, delayT, feedforward, feedback, blend);
    case 2
        % fir
        y = combfilter.fir(x, Fs, delayT, gain);
    case 3
        % iir
        y = combfilter.iir(x, Fs, delayT, gain);
    case 4
        % allpass
        y = combfilter.allpass(x, Fs, delayT, a);
    case 5
        % delay
        y = combfilter.delay(x, Fs, delayT);
    otherwise
        disp('Formula should be 1, 2, 3, 4 or 5.');
        return;
end

sound(y,Fs);