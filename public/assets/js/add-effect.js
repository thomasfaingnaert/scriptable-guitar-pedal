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
        form = "<p>\n" +
            "                    <input type=\"radio\" name=\"type\" value=\"symmetric\"> Symmetric<br/>\n" +
            "                    <input type=\"radio\" name=\"type\" value=\"asymmetric\"> Asymmetric<br/>\n" +
            "                </p>\n" +
            "                <p>\n" +
            "                    Gain:<br/>\n" +
            "                    0\n" +
            "                    <input type=\"range\" name=\"gain\" min=\"0\" max=\"10\" step=\"0.1\"/>\n" +
            "                    10\n" +
            "                </p>\n" +
            "                <p>\n" +
            "                    Gain 2 (only Asymmetric):<br/>\n" +
            "                    0\n" +
            "                    <input type=\"range\" name=\"gain2\" min=\"0\" max=\"10\" step=\"0.1\"/>\n" +
            "                    10\n" +
            "                </p>\n" +
            "                <p>\n" +
            "                    Mix:<br/>\n" +
            "                    0\n" +
            "                    <input type=\"range\" name=\"mix\" min=\"0\" max=\"1\" step=\"0.01\"/>\n" +
            "                    1\n" +
            "                </p>\n" +
            "                <p>\n" +
            "                    Mix 2 (only Asymmetric):<br/>\n" +
            "                    0\n" +
            "                    <input type=\"range\" name=\"mix2\" min=\"0\" max=\"1\" step=\"0.01\"/>\n" +
            "                    1\n" +
            "                </p>\n" +
            "                <p>\n" +
            "                    Noise treshold:<br/>\n" +
            "                    0.0001\n" +
            "                    <input type=\"range\" name=\"threshold\" min=\"0.0001\" max=\"0.01\" step=\"0.001\"/>\n" +
            "                    0.01\n" +
            "                </p>";
    } else if (effect === 'delay') {
        form = "<p>\n" +
            "                    <input type=\"radio\" name=\"type\" value=\"fir\"/> One Echo<br/>\n" +
            "                    <input type=\"radio\" name=\"type\" value=\"iir\"/> Multiple Echoes\n" +
            "                </p>\n" +
            "                <p>\n" +
            "                    Delay Time:<br/>\n" +
            "                    0.05s\n" +
            "                    <input type=\"range\" name=\"delay\" min=\"0.05\" max=\"2\" step=\"any\"/>\n" +
            "                    2s\n" +
            "                </p>\n" +
            "                <p>\n" +
            "                    Decay:<br/>\n" +
            "                    0.1\n" +
            "                    <input type=\"range\" name=\"decay\" min=\"0.1\" max=\"0.9\" step=\"any\"/>\n" +
            "                    0.9\n" +
            "                </p>";
    } else if (effect === 'tremolo') {
        form = "<p>\n" +
            "                    Depth:<br/>\n" +
            "                    0.05\n" +
            "                    <input type=\"range\" name=\"depth\" min=\"0.05\" max=\"0.95\" step=\"any\"/>\n" +
            "                    0.95\n" +
            "                </p>\n" +
            "                <p>\n" +
            "                    Rate:<br/>\n" +
            "                    0.5 Hz\n" +
            "                    <input type=\"range\" name=\"rate\" min=\"0.5\" max=\"10\" step=\"any\"/>\n" +
            "                    10 Hz\n" +
            "                </p>";
    } else if (effect === 'conv') {
        form = " <p>\n" +
            "                    Impulse Response:<br/>\n" +
            "                    <input type=\"file\" name=\"impulse-response\" />\n" +
            "                </p>";
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

        var box = '<div class="jsplumb-box" id="box-' + numElements + '"' +
            'data-effect=\'' + formData + '\' ' +
            'onclick="$(\'#changeEffectModal\').modal(\'show\'); changeEffect(this);">\n' +
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

    $.map(unindexed, function(element, i) {
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