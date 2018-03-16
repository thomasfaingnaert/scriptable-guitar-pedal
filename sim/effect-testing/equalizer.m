classdef equalizer
    methods(Static=true)
        
        function [A,B] = makeFilter(Fc, Fs, gain, Q)
            %MAKEFILTER makes a second order filter
            %   Fc      determines the center frequency
            %   Fs      determines the sample rate
            %   gain    determines how much to boost/cut
            %   Q       Q factor determines the width
                        
            omega = 2*pi*Fc/Fs;
            
            k=tan(omega/2);
                        
            b0 =  1 + gain*k/Q + k^2;
            b1 =  2*(k^2 - 1);
            b2 =  1 - gain*k/Q + k^2;
            
            a0 =  1 + k/Q + k^2;
            a1 =  2*(k^2 - 1);
            a2 =  1 - k/Q + k^2;
            
            A = [a0 a1 a2] / a0;
            B = [b0 b1 b2] / a0;           
        end
        
        function y = makeOutputSamples1(x, Fs, bass, mid, trebble)
            %MAKEOUTPUTSAMPLES1 will apply EQ to the signal
            %   x           contains the input samples
            %   Fs          determines the sample rate
            %   bass        determines gain for bass frequency in dB
            %   mid         determines gain for mid frequency in dB
            %   trebble     determines gain for high frequency in dB
            
            % Set bass, mid and trebble frequency
            bf = 150;
            mf = 600;
            hf = 2400;
            
            % Calculate peaking filters
            [bA, bB] = equalizer.makeFilter(bf, Fs, bass, 0.5);
            [mA, mB] = equalizer.makeFilter(mf, Fs, mid, 0.5);
            [hA, hB] = equalizer.makeFilter(hf, Fs, trebble, 0.5);
            
            % Calculate functions
            len = 2^(nextpow2(2 * size(x,1))); 
            f = Fs * (-len/2:len/2-1)' / len;
            f = repmat(f,1,size(x,2));
            z = exp(-1i*2*pi*f/Fs);
            
            bassFilter = (1 + bB(1) ./ z + bB(2) ./ z.^2) ./ (1 + bA(1) ./ z + bA(2) ./ z.^2);
            midFilter = (1 + mB(1) ./ z + mB(2) ./ z.^2) ./ (1 + mA(1) ./ z + mA(2) ./ z.^2);
            highFilter = (1 + hB(1) ./ z + hB(2) ./ z.^2) ./ (1 + hA(1) ./ z + hA(2) ./ z.^2);

            % Fourier transform original signal
            x_fourier = fft(x, len);
            
            % Apply filters
            y_fourier = bassFilter .* midFilter .* highFilter .* x_fourier;
            
            % Inverse Fourier
            y = ifft(y_fourier);
            y = real(y);
            y = y(1:size(x,1),:);
        end
    end
end

