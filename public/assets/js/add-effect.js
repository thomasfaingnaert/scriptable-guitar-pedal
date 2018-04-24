/**
 * This function makes sure the right form-fields are displayed based on the selection list.
 */
$(document).ready(function () {
    var effect = '';
    var selection = $('#effect-select');

    effect = selection.find('option:selected').val();
    var form = displayForm(effect);
    $('#parameter-form').html(form);

    selection.change(function () {
        effect = selection.find('option:selected').val();
        form = displayForm(effect);
        $('#parameter-form').html(form);
    });
});

/**
 * This function changes the content of the form based on the effect that is selected
 * @param effect A string containing the effect for which the form fields should be displayed
 */
function displayForm(effect) {
    var form = "";
    if (effect === 'distortion') {
        form = "<div class=\"form-group\">\n" +
            "                    <div class=\"form-check\">\n" +
            "                        <input type=\"radio\" class=\"form-check-input\" name=\"type\" id=\"type1\" value=\"symmetric\"/>\n" +
            "                        <label class=\"form-check-label\"> Symmetric </label>\n" +
            "                    </div>\n" +
            "                    <div class=\"form-check\">\n" +
            "                        <input type=\"radio\" class=\"form-check-input\" name=\"type\" id=\"type2\" value=\"asymmetric\"/>\n" +
            "                        <label class=\"form-check-label\"> Asymmetric </label>\n" +
            "                    </div>\n" +
            "                </div>\n" +
            "                <div class=\"form-group\">\n" +
            "                    <label for=\"gain1\">Gain</label> <br/>\n" +
            "                    0\n" +
            "                    <input type=\"range\" name=\"gain1\" id=\"gain1\" min=\"0\" max=\"10\" step=\"0.1\"/>\n" +
            "                    10\n" +
            "                </div>\n" +
            "                <div class=\"form-group\">\n" +
            "                    <label for=\"gain2\">Gain 2 (only Asymmetric)</label> <br/>\n" +
            "                    0\n" +
            "                    <input type=\"range\" name=\"gain2\" id=\"gain2\" min=\"0\" max=\"10\" step=\"0.1\"/>\n" +
            "                    10\n" +
            "                </div>\n" +
            "                <div class=\"form-group\">\n" +
            "                    <label for=\"mix1\">Mix</label> <br/>\n" +
            "                    0\n" +
            "                    <input type=\"range\" name=\"mix1\" id=\"mix1\" min=\"0\" max=\"1\" step=\"0.01\"/>\n" +
            "                    1\n" +
            "                </div>\n" +
            "                <div class=\"form-group\">\n" +
            "                    <label for=\"mix2\">Mix 2 (only Asymmetric</label> <br/>\n" +
            "                    0\n" +
            "                    <input type=\"range\" name=\"mix2\" id=\"mix2\" min=\"0\" max=\"1\" step=\"0.01\"/>\n" +
            "                    1\n" +
            "                </div>\n" +
            "                <div class=\"form-group\">\n" +
            "                    <label for=\"threshold\">Noise threshold</label> <br/>\n" +
            "                    0.0001\n" +
            "                    <input type=\"range\" name=\"threshold\" id=\"threshold\" min=\"0.0001\" max=\"0.01\" step=\"0.001\"/>\n" +
            "                    0.01\n" +
            "                </div>";
    } else if (effect === 'delay') {
        form = "<div class=\"form-group\">\n" +
            "                    <div class=\"form-check\">\n" +
            "                        <input type=\"radio\" class=\"form-check-input\" name=\"type\" id=\"type1\" value=\"fir\" />\n" +
            "                        <label class=\"form-check-label\">One echo</label>\n" +
            "                    </div>\n" +
            "                    <div class=\"form-check\">\n" +
            "                        <input type=\"radio\" class=\"form-check-input\" name=\"type\" id=\"type2\" value=\"iir\" />\n" +
            "                        <label class=\"form-check-label\">Multiple echoes</label>\n" +
            "                    </div>\n" +
            "                </div>\n" +
            "                <div class=\"form-group\">\n" +
            "                    <label for=\"delay\">Delay time</label> <br/>\n" +
            "                    0.05 <input type=\"range\" name=\"delay\" id=\"delay\" min=\"0.05\" max=\"2\" step=\"any\" /> 2\n" +
            "                </div>\n" +
            "                <div class=\"form-group\">\n" +
            "                    <label for=\"decay\">Decay</label> <br/>\n" +
            "                    0.1 <input type=\"range\" name=\"decay\" id=\"decay\" min=\"0.1\" max=\"0.9\" step=\"any\" /> 0.9\n" +
            "                </div>";
    } else if (effect === 'tremolo') {
        form = "<div class=\"form-group\">\n" +
            "                    <label for=\"depth\">Depth</label> <br/>\n" +
            "                    0.05\n" +
            "                    <input type=\"range\" name=\"depth\" id=\"depth\" min=\"0.05\" max=\"0.95\" step=\"any\"/>\n" +
            "                    0.95\n" +
            "                </div>\n" +
            "                <div class=\"form-group\">\n" +
            "                    <label for=\"rate\">Depth</label> <br/>\n" +
            "                    0.5 Hz\n" +
            "                    <input type=\"range\" name=\"rate\" id=\"rate\" min=\"0.5\" max=\"10\" step=\"any\"/>\n" +
            "                    10 Hz\n" +
            "                </div>";
    } else if (effect === 'conv') {
        form = "<div class=\"form-group\">\n" +
            "                    <label for=\"impulse-response\">Input file</label>\n" +
            "                    <input type=\"file\" class=\"form-control-file\" name=\"impulse-response\" id=\"impulse-response\" />\n" +
            "                </div>";
    } else {
        alert("Select only 1 effect at a time please.");
    }

    return form;
}

var numElements = 0;

/**
 * This function will add an effect box to the jsplumb container when the button is clicked
 */
function addEffect() {
    if (numElements < 5) {
        var effect = $('#effect-select').find('option:selected').val().toString().toLowerCase();

        // parse form data to JSON
        var formData = JSON.stringify(parseFormData($('#addEffectForm')));

        effect = effect.charAt(0).toUpperCase() + effect.slice(1).toLowerCase();

        var box = '<div class="jsplumb-box" id="box-' + numElements + '" ' +
            'data-effect=\'' + formData + '\' ' +
            'onclick="$(\'#modal\').modal(\'show\'); changeEffect(this);">\n' +
            '               <p>' + effect + '</p>\n' +
            '            </div>';

        $('#jsplumb-container').append(box);

        $('#box-' + numElements).css({
            'left': (1 + (numElements % 3)) * 150 + 'px',
            'top': (numElements <= 2 ? 0 : 1) * 100 + 'px'
        });

        addEndPoints($('#box-' + numElements));

        numElements++;
    } else {
        alert('Je kan maximaal 5 effecten tegelijk gebruiken!');
    }
}

/**
 * This function takes the data from the form and parses it to the desired format
 * @param form The selector of the form
 * @returns {{}} An array of all the parameters in the desired format
 */
function parseFormData(form) {
    var unindexed = form.serializeArray();
    var jsonFormData = {};

    $.map(unindexed, function (element, i) {
        jsonFormData[element['name']] = element['value'];
    });

    return jsonFormData;
}

/**
 * This function will add endpoints to the boxes in the jsplumb-container.
 * @param box the box for which we want to add the endpoints
 */
function addEndPoints(box) {
    var id = box.attr('id');
    var common = {
        isSource: true,
        isTarget: true,
        connector: ["Straight"]
    };

    if (id !== 'inputbox') {
        jsPlumb.addEndpoint(id, {anchor: 'Left'}, common);
    }
    if (id !== 'outputbox') {
        jsPlumb.addEndpoint(id, {anchor: 'Right'}, common);
    }
    if (id !== 'inputbox' && id !== 'outputbox') {
        jsPlumb.draggable(id, {containment: true});
    }
}

jsPlumb.ready(function () {
    // add endpoints to the input and output boxes
    addEndPoints($('#inputbox'));
    addEndPoints($('#outputbox'));
});