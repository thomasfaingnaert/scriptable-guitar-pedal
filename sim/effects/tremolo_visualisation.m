Fs = 1000;
l = 3;
t = (0:1/Fs:l-1/Fs)';

x = sin((4*pi/3)*t);

y = tremolo.makeOutputSamples(x,Fs,0.2,0.5);

plot(t,x);
hold on
plot(t,y);
title('Orginal vs. tremolo signal');
xlabel('t');
ylabel('Amplitude');
legend('Original','Tremolo');
hold off