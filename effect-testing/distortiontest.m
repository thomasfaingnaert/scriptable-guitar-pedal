[y,Fs]=audioread('dry-guitar-cut.wav');

% Formula 1
treshold = 0.2;
vol = 0.1; 
output = distortion.makeOutputSamples1(y,treshold, vol);

sound(output,Fs);
audiowrite('distortion1.wav',output,Fs);

plot(output(1:500), 'r');
hold on;
plot(y(1:500), 'b');

%{
% Formula 2
tau = 10;
output = distortion.makeOutputSamples2(y,tau);

%sound(output,Fs);
audiowrite('distortion1.wav',output,Fs);

plot(output(1:500), 'r');
hold on;
plot(y(1:500), 'b');
%}
