x = rand([1 3*29*1000]);
h = rand([1 30]);
y = overlapSave(x, h);
c = conv(x, h);
if isequal(c(1:length(x)), y) 
   disp('Output OK'); 
end

function y = overlapSave(x, h)
    M = length(h);
    
    % number of samples to remember of previous block
    overlap = M - 1;
    
    % buffer which remembers previous samples
    buf = zeros([1 overlap]);
    
    % process input per block_size samples
    block_size = 3 * overlap;
    
    % current output and input position
    pos = 0;
    
    % initialize output
    y = [];
    
    while pos+block_size <= length(x)
       % create input segment from previous samples and next block
       input_segment = [buf x(pos+1:pos+block_size)];
       
       % calculate corresponding output segment
       output_segment = conv(input_segment, h);
       
       % append output
       y = [y output_segment(M:M+block_size-1)];
       
       % remember input samples
       buf = x(pos+block_size-overlap+1:pos+block_size);
       
       % process next block
       pos = pos + block_size;
    end
end
