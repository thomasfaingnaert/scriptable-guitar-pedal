[y,Fs]=audioread('smoke-on-the-water.wav');


%{
% Formula 1
treshold = 0.05;
vol = 0.8; 
output = distortion.makeOutputSamples1(y,treshold,vol);

sound(output,Fs);
%}

% Formula 2
tau = 30;
vol = 0.5;
output = distortion.makeOutputSamples2(y,tau,vol);

sound(output,Fs);

%{
% Formula 3
alpha = 15;
vol = 0.8;
output = distortion.makeOutputSamples3(y,alpha,vol);

sound(output,Fs);
%}
