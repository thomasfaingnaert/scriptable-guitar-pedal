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
            
            maxL = max([size(c1,1),size(c2,1),size(c3,1),size(c4,1)]);
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
        
        function y = makeOutputSamples2(x, Fs, room_size, volume)
            %MAKEOUTPUTSAMPLES1 computes reverb for the input samples
            %   x           contains the input samples
            %   Fs          contains the rate of the input
            %   room_size   determines the room_size
            %   volume      determines the volume of the output
            
            % Delay in samples for iir
            S = [1557, 1617, 1491, 1422, 1277, 1356, 1188, 1116];
            S = S .* room_size/Fs; % in seconds
            
            % Delay in samples for allpass
            A = [225, 556, 441, 341];
            A = A .* room_size/Fs; % in seconds
            
            c1 = combfilter.iir(x, Fs, S(1), 0.2);
            c2 = combfilter.iir(x, Fs, S(2), 0.2);
            c3 = combfilter.iir(x, Fs, S(3), 0.2);
            c4 = combfilter.iir(x, Fs, S(4), 0.2);
            c5 = combfilter.iir(x, Fs, S(5), 0.2);
            c6 = combfilter.iir(x, Fs, S(6), 0.2);
            c7 = combfilter.iir(x, Fs, S(7), 0.2);
            c8 = combfilter.iir(x, Fs, S(8), 0.2);
            
            maxL = max([size(c1,1),size(c2,1),size(c3,1),size(c4,1),size(c5,1),size(c6,1),size(c7,1),size(c8,1)]);
            c1(maxL,:) = 0;
            c2(maxL,:) = 0;
            c3(maxL,:) = 0;
            c4(maxL,:) = 0;
            c5(maxL,:) = 0;
            c6(maxL,:) = 0;
            c7(maxL,:) = 0;
            c8(maxL,:) = 0;
            inter = c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8;
            
            ap1 = combfilter.allpass(inter, Fs, A(1), 0.5);
            ap2 = combfilter.allpass(ap1, Fs, A(2), 0.5);
            ap3 = combfilter.allpass(ap2, Fs, A(3), 0.5);
            ap4 = combfilter.allpass(ap3, Fs, A(4), 0.5);
            
            x(size(ap4,1),:) = 0;
            y = x + volume * ap4;
        end        
    end
end

