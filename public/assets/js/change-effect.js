function changeEffect(box) {
    var id = box.id;
    var effect = $('#'+id).attr('data-effect');

    $('#effectModalLabel').html(effect);

    var form = displayForm(effect.toLowerCase());

    console.log(form);

    $('#effectModalBody').html(form);
}