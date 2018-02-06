[y,Fs]=audioread('..\samples\dry-guitar-cut.wav');

original = y;

% treshold
treshold=0.6;

% pre-gain
y = y./treshold;

% hard clipping to [-1,1]
y=min(1, max(y, -1));

% smoothing
y=-3/8.* y .^3 + 9/8 .* y;

audiowrite('distortion1.wav',y,Fs);

plot(y(1:500), 'r');
hold on;
plot(original(1:500), 'b');
