/**
 * This function will check if the fields are okay
 */
function addIR() {
    if ($('#ir-file')[0].files.length === 0) {
       alert("Please select 1 or more files");
       return false;
    }
    return true;
}

/**
 * This function will load and display a list of available impulse responses for reverb
 */
function loadResponses() {
    // Send get-request for impulse responses
    $.get('/conv/list', function(data) {
        var options = '';
        $.each(data, function(i, name) {
            var index = name.indexOf('.wav');
            if (index >= 0) {
                options += '<option>' + name.substring(0,index) + '</option>\n';
            }
        });

        // Append options to form
        $('.response-list').append(options);
    })
}