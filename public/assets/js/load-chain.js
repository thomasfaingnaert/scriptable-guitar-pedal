/**
 * This function will load the chains from the server and return them to the user in a modal
 */
function loadChains() {
    // Send AJAX request for presets

    $.get('/chain/load', function (data) {
        // Construct a table
        var table = '<table class="table">\n' +
            '            <thead>\n' +
            '                <tr>\n' +
            '                    <th>#</th>\n' +
            '                    <th>Name</th>\n' +
            '                    <th class="hide">Data</th>\n' +
            '                    <th>Open</th>' +
            '                </tr>\n' +
            '            </thead>\n' +
            '            <tbody>\n';

        data.forEach(function (element, i) {
            table += '                <tr>\n' +
                '                    <td>' + (i + 1) + '</td>\n' +
                '                    <td>' + element.name + '</td>\n' +
                '                    <td class="hide" id="val-' + (i + 1) + '" data-chain=\'' + JSON.stringify(element.value) + '\'></td>\n' +
                '                    <td><button class="btn btn-light" onclick="drawChain(\'#val-' + (i + 1) + '\')" type="button" role="button">Open</button></td>\n' +
                '                </tr>\n';
        });

        table += '            </tbody>\n' +
            '        </table>';

        // Paste table into modal
        $('#modalLabel').html("Saved chains");
        $('#modalBody').html(table);
        $('#save-change-button').hide();

        // Show modal
        $('#modal').modal('show');
    });
}

/**
 * This function will draw the chain in the jsPlumb container
 *
 * @param  valueId Contains the id where the data can be found
 */
function drawChain(valueId) {
    var jsonData = $(valueId).attr('data-chain');
    var chain = JSON.parse(jsonData);

    // Clear the JsPlumbbox and fill with input and outputbox
    $('#jsplumb-container').html(
        '<div class="jsplumb-box" id="inputbox"><p>Input</p></div>\n' +
        '<div class="jsplumb-box" id="outputbox"><p>Output</p></div>'
    );

    addEndPoints($('#inputbox'));
    addEndPoints($('#outputbox'));

    chain.forEach(function (effect, i) {
        var box = '<div class="jsplumb-box" id="box-' + i + '" ' +
            'data-effect=\'' + JSON.stringify(effect) + '\' ' +
            'onclick="$(\'#modal\').modal(\'show\'); changeEffect(this);">\n' +
            effect.effect.charAt(0).toUpperCase() + effect.effect.substring(1).toLowerCase() + '\n' +
            '</div>';

        $('#jsplumb-container').append(box);

        var boxId = '#box-' + i + '';

        $(boxId).css({
            'left': (1 + i) * 15 + '%',
            'top': '30%'
        });

        addEndPoints($(boxId));
    });


}