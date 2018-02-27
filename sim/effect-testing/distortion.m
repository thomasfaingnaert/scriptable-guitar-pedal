classdef distortion
    methods(Static=true)

        function y = makeOutputSamples1(x,treshold, vol)
            %MAKEOUTPUTSAMPLES1 will add distortion to the samples
            %   x contains the samples
            %   treshold determines at which point the clipping starts
            %   vol determines outputvolume
            
            % eliminate noise
            noisegate = 5 * 10^(-4);
            x = x .* (abs(x) >= noisegate);
            
            % check if vol has valid value
            if vol > 1 || vol <= 0
                vol = 1;
            end

            % pre-gain
            y = x./treshold;

            % hard clipping to [-1,1]
            y=min(1, max(y, -1));

            % smoothing
            y=vol.*(-3/8.* y .^3 + 9/8 .* y);
        end

        function y = makeOutputSamples2(x,tau, vol)
            %MAKEOUTPUTSAMPLES2 adds distortion to the samples
            %   x contains the samples
            %   tau contains the 'steepness' of the function that
            %   vol regulates the volume
            %   calculates distortion
            
            % eliminate noise
            noisegate = 5 * 10^(-4);
            x = x .* (abs(x) >= noisegate);

            % check if vol has valid value
            if vol > 1 || vol <= 0
                vol = 1;
            end

            % sigmoid curve
            y = vol * sign(x) .* (1 - exp(-tau*abs(x)));
        end

        % This function creates a milder distortion because it uses soft
        % clipping instead of hard clipping.
        function y = makeOutputSamples3(x, alpha, vol)
            %MAKEOUTPUTSAMPLES3 adds distortion to the samples
            %   x contains the samples
            %   alpha affects the amount of clipping
            %   vol regulates the volume
            
            % eliminate noise
            noisegate = 5 * 10^(-4);
            x = x .* (abs(x) >= noisegate);

            if vol > 1 || vol <= 0
                vol = 1;
            end

            % soft clipping
            y = vol * 2/pi * atan(alpha * x);
        end

        
        function y = makeOutputSamples4(x,alpha,vol)
            %MAKEOUTPUTSAMPLES4 will add distortion to the samples
            %   x contains the samples
            %   treshold determines at which point the clipping starts
            %   vol determines outputvolume
            
            % eliminate noise
            noisegate = 2 * 10^(-3);
            
            if vol > 1 || vol <= 0
                vol = 1;
            end
            
            % asymmetric clipping
            y = vol * ((x.*(x >= noisegate)).^(1/alpha) + (x.*(x <= noisegate)));
            y = y.* (abs(y) >= noisegate);
        end
    end
end

