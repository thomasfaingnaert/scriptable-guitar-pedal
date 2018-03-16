if exist('fragment','var') ~= 1
    disp('Please create a variable ''fragment'' in the current workspace, set it to a 1 or 2 and run this script again.');
    return;
end

switch fragment
    case 1
        [x,Fs] = audioread('../samples/my-propeller.wav');
        N = size(x,1);
    case 2
        Fs = 8192;
        len = 3; % seconds
        t = 0:1/Fs:len -1/Fs;
        t = t';
        N = size(t,1);
        
        freq = [150, 600, 2400];
        
        x = sin(2*pi*freq(1)*t) + sin(2*pi*freq(2)*t) + sin(2*pi*freq(3)*t);
    otherwise
        disp('Fragment should be 1 or 2');
        return;
end

f = -Fs/2:Fs/N:Fs/2 - Fs/N;

y = equalizer.makeOutputSamples1(x,Fs,-5,-8,-6);
z = equalizer.makeOutputSamples1(x,Fs,-15,0,5);

X = abs(fftshift(fft(x)/N));
Y = abs(fftshift(fft(y)/N));
Z = abs(fftshift(fft(z)/N));

subplot(2,2,1);
plot(f,X);
title('X');
subplot(2,2,2);
plot(f,Y);
title('Y');
subplot(2,2,3);
plot(f,Z);
title('Z');