classdef delay
 methods(Static=true)

     % Function that reads signal from buffer and adds delay effect
     %  This implementation will cause multiple echoes.
     function samples = makeOutputSamples(buf, rate, delayT, decay)
         % MAKEOUTPUTSAMPLES reads signal from buf at a rate and adds a
         % delayed signal with a certain decay
         %  buf buffer with original samples
         %  rate determines rate of samples in buf
         %  delayT determines the time for the delay
         %  decay determines the relative volume of the echo ( 0 < decay < 1)
         
        % Determine amount of samples delay
        dS = delayT * rate;
        
        % Make sure decay has a valid value.
        if decay >= 1 || decay <= 0
            decay = 0.5;
        end
        
        %Prepare delayed sound
        delS = [zeros(dS,size(buf,2)) ; buf];
        
        samples = [buf ; zeros(dS,size(buf,2))] + (1 - decay) * delS;
     end
     
     function y = makeOutputSamples2(buf, rate, delayT, decay, amount)
         %MAKEOUTPUTSAMPLES2 will implement a tapped delay line with
         %multiple echoes.
         %  buf         contains the input samples
         %  rate        determines the rate of samples in the buffer
         %  delayT      determines the time for the delay
         %  decay       determines the relative volume of the echo (0 < decay < 1)
         %  amount      determines the amount of echoes
         
         inter = buf;
         
         for i=1:amount
            inter = delay.makeOutputSamples(inter, rate, delayT, decay^(1/i)); 
         end
         
         y = inter;
     end
 end
end

