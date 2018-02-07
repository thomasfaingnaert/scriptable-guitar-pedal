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
        
        function y = makeOutputSamples2(x,tau)
            %MAKEOUTPUTSAMPLES2 adds distortion to the samples
            %   x contains the samples
            %   tau contains the 'steepness' of the function that
            %   calculates distortion

            % sigmoid curve
            y = sign(x) .* (1 - exp(-tau*abs(x)));
        end
        
    end
end

