$(document).ready(function() {
    e = new elapsd();
    resizeDocument(e);

    $("#overlay").show();

    db_selector = $("#db_selector");
    db_selector
        .css('top', $(window).innerHeight() / 2 - db_selector.outerHeight() / 2)
        .css('left', $(window).innerWidth() / 2 - db_selector.outerWidth() / 2);

    /* Create event handler for thread interleaving */
    $("input:radio[name=thread_interleave]").change(function(ev) {
        e.setThreadInterleave(ev.target.id.split('_')[1]);
    });

    url_params = window.location.search.substring(1).split('&');
    $.each(url_params, function(index, value) {
        param = value.split('=');

        if (param[0] == '_debugmode' && param[1] == 'true') {
            //e.changeDB("GLAT_cuda_sebastian.db", db_selector);
            //changeDB(e, "elapsd_predict.db", db_selector);
            changeDB(e, "GLAT_predict.db", db_selector);
        }
    });

});

$(window).resize(function() {
    resizeDocument();
});

var colors = [
    '#6895E7',
    '#B468E7',
    '#7FF26D',
    '#FFCE73',

    '#27509B',
    '#6C279B',
    '#3DAC2B',
    '#BF8D30',

    '#002F86',
    '#500087',
    '#149500',
    '#A66C00',

    '#3A77E7',
    '#A13AE7',
    '#55F23D',
    '#FFBC40',

    '#0049CE',
    '#7B00CF',
    '#1FE600',
    '#FFA500'
];

var spinner = new Spinner({
    color:'#00B1E6',
    lines: 12,
    length: 0,
    width: 8,
    radius: 14
});

var experiments = $('#experiments');
var drawing     = $('#drawing');
var footer      = $('#footer');
var overlay     = $('#overlay');

function overlayToggle(newState, msg) {

    $("#overlay_msg").text(msg);
    $("#overlay_msg").show();

    if (newState == 'show') {
        overlay.show();
        spinner.spin(document.getElementById("overlay"));
        e._current_overlay_state = 'show';
    } else if (newState == 'hide') {
        overlay.fadeOut();
        spinner.stop();
        e._current_overlay_state = 'hide';
    }

};

function resizeDocument(obj) {

	var chart = d3.select('#drawing').append("svg:svg")	
		.attr("class", "chart")
		.attr("id", "chart")
		.attr("width", "100%");
    var $_chart = $("#chart");
    
    var chart_seconds = d3.select('#drawing').append("svg:svg")
                          .attr("class", "chart")
                          .attr("id", "chart_seconds")
                          .attr("width", "100%")
                          .attr("height", "50");
    
    var $_chart_seconds = $("#chart_seconds");
    
    var chart_top_space = 25;

    var body_height = $("body").height();
    var body_width  = $("body").width();

    if (typeof obj._statistics_width == 'undefined') {
        obj._statistics_width = 0;
    }

    $("#footer").css('top', body_height - $("#footer").outerHeight());
    $("#drawing").innerHeight(
        body_height -
        $("#footer").outerHeight() -
        $("#experiments").outerHeight() -
        parseInt($("#drawing").css('margin-top'),10) -
        parseInt($("#drawing").css('margin-bottom'),10));

    $("#statistics").innerHeight($("#drawing").innerHeight());
    $("#statistics").innerWidth(obj._statistics_width);

    $("#drawing").innerWidth(
        body_width - $("#statistics").innerWidth() -
        parseInt($("#drawing").css('margin-right'),10) -
        parseInt($("#statistics").css('margin-right'),10));

    $_chart.innerHeight(
        $("#drawing").innerHeight() -
        $_chart_seconds.innerHeight() -
        chart_top_space);

    $("#statistics").innerHeight($_chart.innerHeight());
    $("#statistics").css('top', $_chart.css('top'));

    $_chart.css('margin-top', chart_top_space);
    $("#statistics").css('margin-top', chart_top_space + 5);

    // Replace overlay message
    $("#overlay_msg").css('top', $(window).innerHeight() / 2 + 100).hide();

    // Update scales
    obj.updateScales('both');
    obj.replot();

};

function changeDB(obj, db, sender) {

    $(sender).hide();

    overlayToggle('show', 'Switching to ' + db);

    obj.db = db;

    // Trigger selection creation
    //this.createExpSelection();
    createExpSelection(obj);

};

function getPrecision(number) {
    if (number === 0) { number = 1; }
    return parseInt(Math.log(number) / Math.LN10 + 1, 10);
}

function getPaddedNumString(strnum,digits) {
    var pad = digits - strnum.length;
    return Array(pad + 1).join("0") + strnum;
}

function changeDisplay(obj, data) {
//this.changeDisplay = function() {

    overlayToggle('show', 'Changing View.');

    // Prepare Draw Data
    obj._full_draw_data = obj.prepareDrawData(data);
//    e._full_draw_data = e.prepareDrawData(db_data);

    // Update scales
    obj.updateScales('both');
    obj.replot();

    if (obj._statistics_width > 0) { obj.genStats(); }
    
    overlayToggle('hide');

};

function redraw(exp, exp_selection) {

    // Cleanup
    $('.experiment.container').remove();

    // Draw
    $.each(exp_selection, function(key,value) {
        $('<div>', {
            'class': 'experiment container',
            'id': 'exp-' + key
        }).appendTo(exp);

        $('<div>', {
            'class': 'experiment title',
            'text': exp_selection[key].exp_date
        }).appendTo("#exp-" + key);

        $('<div>', {
            'class': 'experiment title',
            'text': exp_selection[key].exp_name
        }).appendTo("#exp-" + key);

        $.each(exp_selection[key].exp_data, function(subkey,value) {

            var kid = subkey.split('-')[0];
            var did = subkey.split('-')[1];

            $('<div>', {
                'class': 'experiment selectable',
                'text': value.kname + " - " + value.dname,
                'eid': key,
                'kid': kid,
                'did': did
            }).appendTo("#exp-" + key)
              .click(function(sender) { e.triggerSelect(sender.target); });
        });
    });
}
