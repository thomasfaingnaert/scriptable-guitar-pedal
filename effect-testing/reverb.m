classdef reverb
    methods(Static=true)
        
        function y = makeOutputSamples1(x, Fs, room_size, volume)
            %MAKEOUTPUTSAMPLES1 computes reverb for the input samples
            %   x           contains the input samples
            %   Fs          contains the rate of the input
            %   room_size   determines the room_size
            %   volume      determines the volume of the output
            
            
            ap1 = combfilter.allpass(x, Fs, 347 * room_size /Fs, 0.7);
            ap2 = combfilter.allpass(ap1, Fs, 113 * room_size /Fs, 0.7);
            ap3 = combfilter.allpass(ap2, Fs, 37 * room_size /Fs, 0.7);
            
            c1 = combfilter.fir(ap3, Fs, 1687 * room_size /Fs, 0.773);
            c2 = combfilter.fir(ap3, Fs, 1601 * room_size /Fs, 0.802);
            c3 = combfilter.fir(ap3, Fs, 2053 * room_size /Fs, 0.753);
            c4 = combfilter.fir(ap3, Fs, 2251 * room_size /Fs, 0.733);
            
            maxL = max(size(c1,1), max(size(c2,1), max(size(c3,1), size(c4,1))));
            c1(maxL,:) = 0;
            c2(maxL,:) = 0;
            c3(maxL,:) = 0;
            c4(maxL,:) = 0;
            
            y1 = c1 + c2 + c3 + c4;
            y2 = -y1;
            y3 = c1 - c2 + c3 + c4;
            y4 = -y3;
            
            y = volume * y1;
        end
        
    end
end

