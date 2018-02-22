classdef tremolo
    methods(Static=true)
        
        function y = makeOutputSamples(x,Fs,depth,rate)
            %MAKEOUTPUTSAMPLES adds the tremolo effect to the samples
            %   x contains the samples
            %   depth determines the depth of the change
            %   rate determines the speed of the effect
            
            % construct sinewave vector
            t = (1:size(x,1))' ./ Fs;
            sw = depth * sin(2*pi*t / rate) + 1;
            
            % calculate output
            y = sw .* x;
        end

    end
end

