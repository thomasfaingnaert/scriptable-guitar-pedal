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
            
            % calculate the amount of samples for delay
            dS = Fs * delay;
            
            % calculate transfer function
            f = (Fs*(0:size(x,1)-1)/size(x,1))';
            z = exp(1i*2*pi.*f*dS);
            trans = (blend + feedforward * z) ./ (1 - feedback * z); 
            
            % fourier transform of input
            x_fourier = fft(x);
            
            % calculate output in fourier domain
            y_fourier = trans .* x_fourier;
            
            % inverse transform
            y = real(ifft(y_fourier));
        end
    end
end

