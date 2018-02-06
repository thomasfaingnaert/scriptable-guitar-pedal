[y,Fs]=audioread('..\samples\dry-guitar-cut.wav');

original = y;

% 'steepness'
tau=10;

% sigmoid curve
y = sign(y) .* (1 - exp(-tau*abs(y)));

audiowrite('distortion2.wav',y,Fs);

plot(y(1:500), 'r');
hold on;
plot(original(1:500), 'b');
