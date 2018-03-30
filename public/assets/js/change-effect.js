function changeEffect(box) {
    var id = box.id;
    var jsonData = $('#' + id).attr('data-effect');
    var parameterData = JSON.parse(jsonData);

    var effect = parameterData['effect'];

    $('#effectModalLabel').html(effect.charAt(0).toUpperCase() + effect.slice(1).toLowerCase());

    var form = '<form onsubmit="changeJSON; return false;" id="changeEffectForm">' +
        '   ' + displayForm(effect) +
        '</form>'

    $('#effectModalBody').html(form);

    // Set the parameter value to the previous one
    $.each(parameterData, function(name, value) {
        console.log(name + ': ' + value);
        $('#changeEffectForm [name ="' + name + '"]').val(value);
    });
}