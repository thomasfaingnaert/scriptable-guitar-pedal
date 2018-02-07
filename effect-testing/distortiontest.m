[y,Fs]=audioread('smoke-on-the-water.wav');


%{
% Formula 1
treshold = 0.05;
vol = 0.8; 
output = distortion.makeOutputSamples1(y,treshold,vol);

sound(output,Fs);
audiowrite('distortion1.wav',output,Fs);

plot(output(1:500), 'r');
hold on;
plot(y(1:500), 'b');
%}

% Formula 2
tau = 30;
vol = 0.5;
output = distortion.makeOutputSamples2(y,tau,vol);

sound(output,Fs);
audiowrite('distortion2.wav',output,Fs);

plot(output(1:500), 'r');
hold on;
plot(y(1:500), 'b');

%{
% Formula 3
alpha = 15;
vol = 0.8;
output = distortion.makeOutputSamples3(y,alpha,vol);

sound(output,Fs);
audiowrite('distortion3.wav',output,Fs);

plot(output(1:500),'r');
hold on;
plot(y(1:500),'b');
%}
