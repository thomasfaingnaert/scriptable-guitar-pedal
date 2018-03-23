/**
 * This function makes sure the right form-fields are displayed based on the selection list.
 */
$(document).ready(function () {
    console.log("in the ready");

    var effect="";
    var selection = $("#effect-select");

    effect = selection.find("option:selected").val();
    displayForm(effect);

    selection.change(function () {
        effect = selection.find("option:selected").val();
        displayForm(effect);
    });
});

/**
 * This function changes the content of the form based on the effect that is selected
 * @param effect a string containing the effect for which the form fields should be displayed
 */
function displayForm(effect) {
    if (effect === "distortion") {
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

        $("#parameter-form").html(form);
    } else if (effect === "delay") {
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

        $("#parameter-form").html(form);
    } else if (effect === "tremolo") {
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

        $("#parameter-form").html(form);
    } else if (effect === "conv") {
        form = " <p>\n" +
            "                    Impulse Response:<br/>\n" +
            "                    <input type=\"file\" name=\"impulse-response\"/>\n" +
            "                </p>";
        $("#parameter-form").html(form);
    } else {
        alert("Select only 1 effect at a time please.");
    }
}

/**
 * This function will add an effect box to the jsplumb container when the button is clicked
 */
function addEffect() {
}