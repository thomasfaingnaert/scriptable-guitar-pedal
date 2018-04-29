/**
 * This function will check if the fields are okay
 */
function addIR() {
    if ($('#ir-file')[0].files.length === 0) {
       alert("Please select 1 or more files");
       return false;
    }
    return true;
}