Fs = 1000;
l = 5;
t = (0:1/Fs:l-1/Fs)';

x = zeros(size(t,1),1);
x(1001) = 1;

y = combfilter.iir(x,Fs,1,0.5);

t_ = (0:1/Fs:size(y,1)/Fs-1/Fs)';

plot(t_,y)
title('IIR');
xlabel('t');
ylabel('Amplitude');