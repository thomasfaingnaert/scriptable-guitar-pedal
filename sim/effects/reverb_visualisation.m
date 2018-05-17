Fs = 1000;
l = 5;
t = (0:1/Fs:l-1/Fs)';

x = zeros(size(t,1),1);
x(1001) = 1;

y = reverb.makeOutputSamples1(x, Fs, 2, 1);
y = y ./ max(y);

t_ = (0:1/Fs:size(y,1)/Fs-1/Fs)';

plot(t_,y)
title('Reverb');
xlabel('t');
ylabel('Amplitude');