/**
 * This function will load the chains from the server and return them to the user in a modal
 */
function loadChains() {
    // Send AJAX request for presets



    $.get('/chain/load', function(data) {
        console.log(data);

        // Construct a table
        var table = '<table class="table">\n' +
            '            <thead>\n' +
            '                <tr>\n' +
            '                    <th>#</th>\n' +
            '                    <th>Name</th>\n' +
            '                    <th class="hide">json</th>\n' +
            '                </tr>\n' +
            '            </thead>\n' +
            '            <tbody>\n';

        data.forEach(function(element, i) {
            table += '                <tr>\n' +
                '                    <td>' + (i + 1) + '</td>\n' +
                '                    <td>' + element.name + '</td>\n' +
                '                    <td class="hide">' + element.value + '</td>\n' +
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