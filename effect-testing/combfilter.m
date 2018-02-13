classdef combfilter
    methods(Static=true)
        
        function y = universal(x, Fs, delay, feedforward, feedback, blend)
            %UNIVERSAL models a universal comb filter
            %   x               contains the samples
            %   Fs              contains the sample rate
            %   delay           contains the delay of the filter
            %   feedforward     determines the amount of feedforward of the filter
            %   feedback        determines the amount of feedback of the filter
            %   blend           determines the blend of original and delayed signal
                        
            % compute transfer function
            len = 2^(nextpow2(2 * size(x,1))); 
            f = Fs * (-len/2:len/2-1)' / len;
            z = exp(-1i*2*pi*delay*f);
            trans = (blend + feedforward * z) ./ (1 - feedback * z);
            
            % fourier transform of input
            x_fourier = fft(x,len);
            
            % calculate output in fourier domain
            y_fourier = trans .* x_fourier;
            
            % inverse transform
            y = real(ifft(y_fourier));
            y = y(1:size(x,1)+delay*Fs);
        end
        
        function y = fir(x, Fs, delayT, gain)
            %FIR implements a finite impulse response filter
            %   x           contains the input samples
            %   Fs          determines the sample rate
            %   delayT      determines the delay time
            %   gain        determines the strength of the echo
            
            % calculate amount of samples for delay
            dS = Fs * delayT;
            
            % calculate transfer function
            
        end
    end
end

