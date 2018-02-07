classdef distortion
    methods(Static=true)
        
        function y = makeOutputSamples1(x,treshold, vol)
            %MAKEOUTPUTSAMPLES1 will add distortion to the samples
            %   x contains the samples
            %   treshold determines at which point the clipping starts
            %   vol determines outputvolume
            
            % check if vol has valid value
            if vol > 1 || vol <= 0
                vol = 1;
            end
            
            % pre-gain
            y = x./treshold;

            % hard clipping to [-vol,vol]
            y=min(vol, max(y, -vol));

            % smoothing
            y=-3/8.* y .^3 + 9/8 .* y;
        end
        
        function y = makeOutputSamples2(x,tau, vol)
            %MAKEOUTPUTSAMPLES2 adds distortion to the samples
            %   x contains the samples
            %   tau contains the 'steepness' of the function that
            %   vol regulates the volume
            %   calculates distortion
            
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
            
            if vol > 1 || vol <= 0
                vol = 1;
            end
            
            % soft clipping
            y = vol * 2/pi * atan(alpha * x);
        end
        
    end
end

