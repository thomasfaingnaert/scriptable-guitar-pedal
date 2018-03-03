%x = rand([1 3*29*1000]);
%h = rand([1 30]);
x = 1:36;
h = [1 0 0 0 0];
y = overlapSave(x, h);
c = conv(x, h);
if all(abs(c(1:length(x)) - y) < 1e-12) 
   disp('Output OK'); 
end

function y = overlapSave(x, h)
    M = length(h);
    
    % number of samples to remember of previous block
    overlap = M - 1;
    
    % buffer which remembers previous samples
    buf = zeros([1 overlap]);
    
    % period for FFT
    N = 4 * overlap;
    
    % process input per block_size samples
    block_size = N - overlap;
    
    % current output and input position
    pos = 0;
    
    % calculate frequency response
    H = fft(h, N);
    
    % initialize output
    y = [];
    
    while pos+block_size <= length(x)
       % create input segment from previous samples and next block
       input_segment = [buf x(pos+1:pos+block_size)];
       
       % calculate corresponding output segment
       output_segment = ifft(fft(input_segment, N) .* H, N);
       
       % append output
       y = [y output_segment(M:M+block_size-1)];
       
       % remember input samples
       buf = x(pos+block_size-overlap+1:pos+block_size);
       
       % process next block
       pos = pos + block_size;
    end
end
