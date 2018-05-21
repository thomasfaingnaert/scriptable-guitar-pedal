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
                '                    <td><button class="btn btn-light" onclick="drawChain(\'#val-' + (i + 1) + '\');" data-dismiss="modal" type="button" role="button">Open</button></td>\n' +
                '                </tr>\n';
        });

        table += '            </tbody>\n' +
            '        </table>';

        // Paste table into modal
        $('#modalLabel').html("Saved chains");
        $('#modalBody').html(table);
        $('#save-change-button').hide();
        $('#delete-effect-button').hide();

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

    var jsPlumbContainer = $('#jsplumb-container');

    // Clear the JsPlumbbox and fill with input and outputbox
    jsPlumb.empty(jsPlumbContainer);

    jsPlumbContainer.html(
        '<div class="jsplumb-box" id="inputbox"><p>Input</p></div>\n' +
        '<div class="jsplumb-box" id="outputbox"><p>Output</p></div>'
    );

    addEndPoints($('#inputbox'));
    addEndPoints($('#outputbox'));

    chain.forEach(function (effect, i) {
        var box = '<div class="jsplumb-box" id="box-' + i + '" ' +
            'data-effect=\'' + JSON.stringify(effect) + '\' ' +
            'ondblclick="$(\'#modal\').modal(\'show\'); changeEffect(this);">\n' +
            effect.effect.charAt(0).toUpperCase() + effect.effect.substring(1).toLowerCase() + '\n' +
            '</div>';

        $('#jsplumb-container').append(box);

        var boxId = '#box-' + i + '';

        $(boxId).css({
            'left': (1 + (i % 3)) * 20 + '%',
            'top': (20 * (1 + 3 * Math.floor(i / 3))) + '%'
        });

        addEndPoints($(boxId));
    });

    // Draw connections
    drawConnections();
}

/**
 * This function will draw connections between the different effects in the chain
 */
function drawConnections() {
    var boxes = $('#jsplumb-container .jsplumb-box');
    var numEffects = boxes.length - 2;

    boxes.each(function (i, element) {
        if (element.id === 'inputbox') {
            jsPlumb.connect({
                source: 'inputbox',
                target: 'box-0',
                uuids: ['ep-right-0', 'ep-left-1']
            });
        } else if (element.id !== 'outputbox') {
            var j = parseInt(element.id.charAt(4)); // box-j
            if (j < numEffects - 1) {
                jsPlumb.connect({
                    source: 'box-' + j,
                    target: 'box-' + (j + 1),
                    uuids: ['ep-right-' + (j + 1), 'ep-left-' + (j + 2)]
                });
            } else {
                jsPlumb.connect({
                    source: 'box-' + j,
                    target: 'outputbox',
                    uuids: ['ep-right-' + (j + 1), 'ep-left-0']
                });
            }
        }
    });
}

/**
 * This function will send a request to the server for the active chain and if there is one, draw it.
 */

function loadActiveChain() {
    $.get('/chain/load/active', function (data) {
        if (data.length > 0) {
            $('#current-chain').attr('data-chain', JSON.stringify(data));

            // Draw chain
            drawChain('#current-chain');
        }
    });
}
