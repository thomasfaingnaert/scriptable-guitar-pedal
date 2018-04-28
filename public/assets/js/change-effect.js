/**
 * This function will display a modal with the form parameters of the effect as they were originally
 * set. The user can then modify the values and save them.
 *
 * @param box The html element containing the effect data
 */
function changeEffect(box) {
    var id = box.id;
    var jsonData = $('#' + id).attr('data-effect');
    var parameterData = JSON.parse(jsonData);

    var effect = parameterData['effect'];

    $('#modalLabel').html(effect.charAt(0).toUpperCase() + effect.slice(1).toLowerCase());

    var form = '<form onsubmit="changeJSON($(\'#effectID\').val()); $(\'#modal\').modal(\'toggle\'); return false;" id="changeEffectForm">' +
        '    <input type="hidden" name= "effect" id="effect" value="' + effect + '"/>' +
        '    ' + displayForm(effect) +
        '    <input type="hidden" id="effectID" value="' + id + '"/>' + // No name so it doesn't get parsed
        '</form>';

    $('#modalBody').html(form);
    $('#save-change-button').show();

    // Set the parameter value to the previous one
    $.each(parameterData, function (name, value) {
        if (name === 'type') {
            $('#changeEffectForm [name="' + name + '"]').val([value]);
        }
        else {
            $('#changeEffectForm [name ="' + name + '"]').val(value);
        }
    });
}

/**
 * This function will take the data-effect attribute of the effect being changed, and will change its content
 * according to the form.
 */
function changeJSON(effectID) {
    var jsonData = parseFormData($('#changeEffectForm'));
    $('#' + effectID).attr('data-effect', JSON.stringify(jsonData));
}