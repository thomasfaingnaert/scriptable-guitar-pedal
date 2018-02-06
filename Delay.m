classdef Delay
 methods(Static=true)
     
     % Function that reads samples into a buffer
     function buf = readSamples(samples)
         buf = [];
         samples = reshape(samples, [], 1); % make one column
         buf = [buf; samples];
     end
     
     % Function that reads signal from buffer and adds delay effect
     %  This implementation will only cause 1 echo.
     function samples = makeOutputSamples1(buf, rate, delay, decay)
         % MAKEOUTPUTSAMPLES1 reads signal from buf at a rate and adds a
         % delayed signal with a certain decay
         %  buf buffer with original samples
         %  rate determines rate of samples in buf
         %  delay determines the time for the delay
         %  decay determines the relative volume of the echo ( 0 < decay < 1)
         
        % Determine amount of samples delay
        dS = delay * rate;
        
        % Make sure decay has a valid value.
        if decay >= 1 || decay <= 0
            decay = 0.5;
        end
        
        samples = [];
        
        for i=1:(size(buf,1) + dS)
            if i > size(buf,1)
                sample = (1 - decay) * buf(i - dS); % No more 'dry' samples left
            elseif dS < i
                sample = buf(i) + (1 - decay) * buf(i-dS); % 'Dry' samples left
            else
                sample = buf(i); % Not enough samples passed
            end
            
            samples = [samples ; sample]; % Add to result
        end
     end
     
     % Function that reads signal from buffer and adds delay effect
     %  This implementation will cause multiple echoes.
     function samples = makeOutputSamples2(buf, rate, delay, decay, lengthlim)
         % MAKEOUTPUTSAMPLES2 reads signal from buf at a rate and adds a
         % delayed signal with a certain decay
         %  buf buffer with original samples
         %  rate determines rate of samples in buf
         %  delay determines the time for the delay
         %  decay determines the relative volume of the echo ( 0 < decay < 1)
         %  lengthlim determines how long the echoing can go on after the
         %              end of the original track
         
        % Determine amount of samples delay
        dS = delay * rate;
        
        % Make sure decay has a valid value.
        if decay >= 1 || decay <= 0
            decay = 0.5;
        end
        
        for i=1:(size(buf,1) + (lengthlim * rate)) % To make sure echoing doesn't continue forever
            if i > size(buf,1)
                buf(i) = (1 - decay) * buf(i - dS); % No more 'dry' samples left
            elseif dS < i
                buf(i) = buf(i) + (1 - decay) * buf(i-dS); % 'Dry' samples left
            else
                buf(i) = buf(i); % Not enough samples passed
            end
        end
        
        samples = buf;
     end     
 end
end

