function process(input)
    result = {}
    noiseTreshold = 1e-3
    steepness = 30

    for i = 1, #input do
        if math.abs(input[i]) < noiseTreshold then
            result[i] = input[i]
        else
            result[i] = 2 / math.pi * math.atan(steepness * input[i])
        end
    end

    return result
end
