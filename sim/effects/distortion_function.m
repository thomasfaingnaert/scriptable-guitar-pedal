x = 0:0.001:1;

y0 = x; % Unchanged
y5 = 2/pi * atan(5 * x);
y10 = 2/pi * atan(10 * x);

plot(x,y0);
hold on
plot(x,y5);
plot(x,y10);
title('Distorsiefuncties');
xlabel('Samplewaarden');
ylabel('Afbeelding');
ylim([0 1.1]);
legend('Origineel', 'Gain=5', 'Gain=10', 'Location', 'southeast');
hold off