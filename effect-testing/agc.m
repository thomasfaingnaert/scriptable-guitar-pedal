classdef agc
    methods(Static=true)

    function y = makeOutputHelper(x, input_power_treshold, output_power, block_size)
        % MAKEOUTPUTHELPER is like MAKEOUTPUTSAMPLES, but expects its input x to have block_size rows
        assert(size(x,1) == block_size);

        % calculate power of input
        p_in = sum(x.^2) / block_size;

        % calculate desired output power
        p_out = 10 ^ (0.1 * output_power);

        % determine scaling factor for each block
        scaling_factor = sqrt(repmat(p_out, size(p_in)) ./ p_in);

        % set scaling to 1 for input power < treshold
        p_tresh = 10 ^ (0.1 * input_power_treshold);
        scaling_factor(:,p_in < p_tresh) = 1;

        % determine scaling matrix
        scaling_matrix = repmat(scaling_factor, [block_size 1]);

        y=scaling_matrix .* x;
    end

    function y = makeOutputSamples(x, input_power_treshold, output_power, block_size)
        % MAKEOUTPUTSAMPLES applies automatic gain control (AGC) to the input samples
        %   x                       contains the input samples
        %   input_power_treshold    if the power of the input is lower than this treshold (in dB), the signal will not be amplified
        %   output_power            desired output power
        %   block_size              the input is processed in blocks of this size (in number of samples)

        % process full blocks at beginning
        num_blocks = floor(size(x,1) / block_size);
        x_block_part = x(1:num_blocks*block_size,:);
        y_block_part = agc.makeOutputHelper(reshape(x_block_part, [block_size numel(x_block_part)/block_size]), input_power_treshold, output_power, block_size);
        y_block_part = reshape(y_block_part, [numel(y_block_part)/size(x,2) size(x,2)]);

        % process the remaining input
        x_remaining_part = x(num_blocks*block_size+1:end,:);
        y_remaining_part = agc.makeOutputHelper(x_remaining_part, input_power_treshold, output_power, size(x_remaining_part, 1));
        y_remaining_part = reshape(y_remaining_part, [numel(y_remaining_part)/size(x,2) size(x,2)]);

        y = [y_block_part; y_remaining_part];
    end
    end
end
