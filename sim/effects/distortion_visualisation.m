Fs = 1000;
l = 3;
t = (0:1/Fs:l-1/Fs)';

x = sin((4*pi/3)*t);

y = distortion.makeOutputSamples6(x,4,1);

plot(t,x);
hold on
plot(t,y);
title('Orgineel vs. distorted signaal');
xlabel('t');
ylabel('Amplitude');
legend('Origineel','Distorted');
hold off