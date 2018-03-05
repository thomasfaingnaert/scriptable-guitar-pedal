Fs = 1000;
l = 3;
t = (0:1/Fs:l-1/Fs)';

x = sin((4*pi/3)*t);

y = distortion.makeOutputSamples5(x,4,1);

plot(t,x);
hold on
plot(t,y);
title('Orginal vs. distorted signal');
xlabel('t');
ylabel('Amplitude');
legend('Original','Distorted');
hold off