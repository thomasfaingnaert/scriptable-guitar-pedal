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
            
            % make sure variables are not too big
            if abs(feedforward) > 1
                feedforward = 0.5;
            end
            
            if abs(feedback) > 1
                feedback = 0.5;
            end
            
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
            y = y(1:size(x,1)+ceil(delay*Fs));
        end
        
        function y = fir(x, Fs, delay, gain)
            %FIR implements a finite impulse response filter
            %   x           contains the input samples
            %   Fs          determines the sample rate
            %   delay      determines the delay time
            %   gain        determines the strength of the echo
                        
            % call universal comb filter with right parameters
            y = combfilter.universal(x,Fs,delay,gain,0,1);
        end
        
        function y = iir(x, Fs, delay, gain)
            %IIR implements a finite impulse response filter
            %   x           contains the input samples
            %   Fs          determines the sample rate
            %   delay       determines the delay time
            %   gain        determines the strength of the echo
                        
            % call universal comb filter with right parameters
            y = combfilter.universal(x,Fs,delay,0,gain,1);
        end
        
        function y = allpass(x, Fs, delay, a)
            %ALLPASS implements an allpass filter
            %   x           contains the input samples
            %   Fs          determines the sample rate
            %   delay       determines the delay time
            %   a           determines a multiplication factor
            
            % call universal comb filter with right parameters
            y = combfilter.universal(x,Fs,delay,1,-a,a);
        end
        
        function y = delay(x, Fs, delay)
            %DELAY implements a pure delay
            %   x           contains the input samples
            %   Fs          determines the sample rate
            %   delay       determines the delay time
            
            % call universal comb filter with right parameters
            y = combfilter.universal(x,Fs,delay,1,0,0);
        end
    end
end

