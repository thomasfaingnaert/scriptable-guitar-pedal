/**
 * This function will take the effects from the jsplumb container, check if they are well connected,
 * and then send a JSON string to the server for it to be processed over there
 */
function sendEffect() {
    var sources = [];
    var targets = [];

    jsPlumb.getConnections().forEach(function (conn) {
        sources.push(conn.sourceId);
        targets.push(conn.targetId);
    });


    if (checkConnections(sources, targets) || $('#input').val() === "") {
        sources.splice(sources.indexOf('inputbox'), 1);
        var jsonString = makeJSON(sources); // sources always contains 'inputbox' at this point

        // Set value for hidden fields
        $('#effect-info').val(jsonString);

        return true;
    } else {
        alert("Please make sure your effects are chained correctly and you have selected a file!");
        return false;
    }

}

/**
 * This function will check if the connections are valid:
 * sources should at least contain 'inputbox'
 * targets should at least contain 'outputbox'
 * other boxes should be in both
 * @param sources list containing all sources
 * @param targets list containing all targets
 */
function checkConnections(sources, targets) {
    if ((sources.indexOf('inputbox') < 0) || (targets.indexOf('outputbox') < 0)) {
        return false;
    }

    sources.forEach(function (el) {
        if (el !== 'inputbox') {
            if (targets.indexOf(el) < 0) {
                return false;
            }
        }
    });

    return true;
}

/**
 * This function takes an array of effectsID's and will make a JSON string containing all effects chained
 * after one another.
 * @param effects is an array of ID's
 */
function makeJSON(effects) {
    console.log(effects);
    var jsonArray = [];
    effects.forEach(function (id) {
        var effectParams = $('#' + id).attr('data-effect');
        jsonArray.push(JSON.parse(effectParams));
    });

    return JSON.stringify(jsonArray);
}