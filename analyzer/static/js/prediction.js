function lagrangePoly(x, points) {
    
    y = 0.0;

    for (i = 0; i < points.length; i++) {
    
        tmp = 1.0;

        for (j = 0; j < points.length; j++) {
            if (j == i) { continue; }
            tmp *= ((x - points[j].x) / (points[i].x - points[j].x));
        }

        y += points[i].y * tmp;

    }

    return y;

}

function elapsd() {

    var e = this;

    var experiments = $('#experiments');
    var drawing     = $('#drawing');
    var footer      = $('#footer');
    var overlay     = $('#overlay');

    /*
     *var bisect_l = d3.bisector(function(d) { return d[0]; }).left;
     *var bisect_r = d3.bisector(function(d) { return d[1]; }).right;
     */
    
    var chart_seconds = d3.select('#drawing').append("svg:svg")
                          .attr("class", "chart")
                          .attr("id", "chart_seconds")
                          .attr("width", "100%")
                          .attr("height", "50");
    
    var $_chart_seconds = $("#chart_seconds");

    var spinner = new Spinner({
        color:'#00B1E6',
        lines: 12,
        length: 0,
        width: 8,
        radius: 14
    });


    overlayToggle = function(newState, msg) {
/*
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
*/

        overlay.hide();

    };

    this.setThreadInterleave = function(interleave) {
        this._threadInterleave = interleave;
        this.changeDisplay();
    };

    var min_width   = 2;
    var min_height  = 2;
    var num_ticks   = 15;

    var chart_top_space = 25;

    this.precision = function(value) {

        // Initialize
        if (typeof this._precision == 'undefined') {
            this._precision = 3;
        }

        if (typeof value != 'undefined') {
            if (value < 3) {
                this._precision = 3;
            } else if (value > 9) {
                this._precision = 9;
            } else {
                this._precision = value;
            }
        }

        return this._precision;
    };

    var active_markers = {};
    var cursor_sec;

    var current_selection = {};

	var chart = d3.select('#drawing').append("svg:svg")	
		.attr("class", "chart")
		.attr("id", "chart")
		.attr("width", "100%");
    var $_chart = $("#chart");
   
    d3.select("#drawing").append('div')
                         .attr('class', 'chart tag')
                         .attr('id', 'tag');

    $_tag = $("#tag");

    var plot_empty = true;

    this.colors = [
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

    this.resizeDocument = function() {

        var body_height = $("body").height();
        var body_width  = $("body").width();

        if (typeof this._statistics_width == 'undefined') {
            this._statistics_width = 0;
        }

        $("#footer").css('top', body_height - $("#footer").outerHeight());
        $("#drawing").innerHeight(
            body_height -
            $("#footer").outerHeight() -
            $("#experiments").outerHeight() -
            parseInt($("#drawing").css('margin-top'),10) -
            parseInt($("#drawing").css('margin-bottom'),10));

        $("#statistics").innerHeight($("#drawing").innerHeight());
        $("#statistics").innerWidth(this._statistics_width);

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
        this.updateScales('both');

    };

    this.changeDB = function(db, sender) {

        $(sender).hide();

        overlayToggle('show', 'Switching to ' + db);

        this.db = db;

        // Trigger selection creation
        this.createExpSelection();
    };

    var x, y;
    var db_data = {
        'data': {},
        'min_x': 0,
        'max_x': 0,
        'max_y': 0,
        'min_y': 0
    };

    this.createExpSelection = function() {
        
        this.exp_selection = {}; // Re-Initialize experiments object

        var exp           = experiments;
        var exp_selection = this.exp_selection;

        var exp_to_load   = 0;
        var exp_loaded    = 0;

        $.ajax({
            type: 'GET',
            url: '/get_experiments',
            dataType: 'json',
            data: { db: this.db },
            async: true,
            success: function(data) {

                exp_to_load = data.result.length;

                $.each(data.result, function(index, value) {

                    var exp_id = value[0];

                    exp_selection[exp_id] = {
                        'exp_date': new Date(value[1] * 1000),
                        'exp_name': value[2],
                        'exp_start': value[3],
                        'exp_stop': value[4],
                        'exp_data': {}
                    };

                    // Call to get details of experiments
                    $.ajax({
                        type: 'GET',
                        url: '/get_experiment_overview',
                        dataType: 'json',
                        data: { id: exp_id },
                        async: true,
                        success: function(data) {
                            $.each(data.result, function(index, value) {
                                exp_selection[exp_id].exp_data[value[0] + '-' + value[2]] = {
                                    'kname': value[1],
                                    'dname': value[3],
                                    'selected': false
                                };
                            });
                            
                            exp_loaded++;

                            if (exp_loaded == exp_to_load) {
                                redrawExperiments(exp, exp_selection);
                                e.resizeDocument();

                                overlayToggle('hide');

                            }

                        }
                    });
                });
            }
        });
    };

    function redrawExperiments(exp, exp_selection) {

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

    var color_assignments = {};

    this.triggerSelect = function(_caller) {

        var caller = $(_caller);

        var eid = caller.attr('eid');
        var sid = caller.attr('kid') + '-' + caller.attr('did');

        if (caller.hasClass('selected')) {
            // Already selected, undo    
            caller.removeClass('selected');

            this.exp_selection[eid].exp_data[sid].selected = false;

            // Remove entry and color
            delete db_data.data[sid];
            this.colors.push(color_assignments[sid]);

        } else {

            overlayToggle('show', 'Loading selected data.');

            // Select it
            caller.addClass('selected');
            this.exp_selection[eid].exp_data[sid].selected = true;

            color_assignments[sid] = this.colors.pop();

            caller.css('border-left-color', color_assignments[sid]);

        }

        this.redrawFromSelection();

    };

    this.redrawFromSelection = function() {
        
        /* Create the selection */
        var new_selection = {};
        $.each(this.exp_selection, function(ekey,evalue) {
            $.each(evalue.exp_data, function(dkey,dvalue) {
                if (dvalue.selected) {
                    new_selection[ekey + "-" + dkey] = true;
                }
            });
        });

        var load_selection = [];
        $.each(new_selection, function(key,value) {

            if (!(key in current_selection)) {
                sel_key = key.split('-');
                load_selection.push([
                    parseInt(sel_key[0], 10),
                    parseInt(sel_key[1], 10),
                    parseInt(sel_key[2], 10)    
                ]);
            }

        });

        /* Store the current selection */
        current_selection = new_selection;

        /* Maybe nothing was selected ?! */
        if (Object.keys(current_selection).length === 0) {
            this.clearPlot();
            return;
        }

        /* Nothing to reload, but we should trigger a redraw */
        if (load_selection.length === 0) {
            this.changeDisplay();
            return;
        }

        /* Trigger the AJAX request to receive the data */
        overlayToggle(null, 'Data request pending.');
        var xhr = $.ajax({
            type: 'POST',
            url: '/get_wall_time',
            dataType: 'json',
            data: { sel: load_selection },
            async: true,
            success: function(data) {

                main_key = data['result'][0];
                values = data['result'][1];

                if (typeof db_data.data[main_key] == "undefined") {
                    db_data.data[main_key] = [];
                }

                $.each(values, function(index,value) {
                    db_data.data[main_key].push({
                        'x': value[0],
                        'y': value[1],
                        'enabled': true
                    });
                })

                db_data.max_x = 0;
                db_data.max_y = 0;

                db_data.min_x = Number.MAX_VALUE;
                db_data.min_y = Number.MAX_VALUE;

                $.each(db_data.data, function(key, entry) {
                    $.each(entry, function(index, value) {
                        db_data.max_y = Math.max(db_data.max_y, value.y);
                        db_data.max_x = Math.max(db_data.max_x, value.x);

                        db_data.min_x = Math.min(db_data.min_x, value.x);
                        db_data.min_y = Math.min(db_data.min_y, value.y);
                    });
                });

                e.changeDisplay();
            }
        });

    };

    this.replot = function() {
    
        var data = db_data.data;

        d3.selectAll(".drawings").remove();

        var method = $("#interpolation").find('option:selected').val();

        var line = d3.svg.line()
            .x(function(d) { return x(d.x); })
            .y(function(d) { return y(d.y); })
            .interpolate(method);

        $.each(data, function(key, entry) {

            var line_points = [];
            $.each(entry, function(index, value) {
                if (value.enabled) { line_points.push(value); }
            });
            
            var extend = 0;

            interp_points = [];
            for (n = 1.0; n < entry.length + extend; n += 0.05) {
                 interp_points.push({
                     'x': n,
                     'y': lagrangePoly(n, line_points)
                 });
            }

//            db_data.max_x = entry.length + extend;
//            db_data.max_y = lagrangePoly(db_data.max_x, line_points);
//            e.updateScales('both');

            /*
             *chart
             *    .append("svg:path")
             *    .attr("class", "drawings")
             *    .attr("d", line(line_points))
             *    .attr("stroke", color_assignments[key])
             *    .attr("stroke-width", "2pt")
             *    .attr("fill", "none");
             */

            chart
                .selectAll("#drawings")
                .data(entry)
                .enter()
                .append("circle")
                .attr("class", "drawings")
                .attr("cx", function(d) { return x(d.x); })
                .attr("cy", function(d) { return y(d.y); })
                .attr("r", "6")
                .attr("fill", color_assignments[key])
                .attr("fill-opacity", function(d) {
                    if (d.enabled) {
                        return 1.0;
                    } else {
                        return 0.4;
                    }
                })

            chart
                .selectAll("#drawings")
                .data(entry)
                .enter()
                .append("circle")
                .attr("class", "drawings point-helper")
                .attr("cx", function(d) { return x(d.x); })
                .attr("cy", function(d) { return y(d.y); })
                .attr("r", "18")
                .attr("fill-opacity", 0.0)
                .attr("stroke", color_assignments[key])
                .attr("stroke-dasharray", "2,2")
                .attr("stroke-width", "2")
                .on("click", function(d) {
                    if (d.enabled) {
                        d.enabled = false;
                    } else {
                        d.enabled = true;
                    }
                    e.replot();
                });

            chart
                .selectAll("#drawings")
                .data(interp_points)
                .enter()
                .append("circle")
                .attr("class", "drawings")
                .attr("cx", function(d) { return x(d.x); })
                .attr("cy", function(d) { return y(d.y); })
                .attr("r", "1")
                .attr("fill", color_assignments[key]);
        });

        var x_axis = d3.svg.axis()
            .scale(x)
            .tickFormat(d3.format(".3f"))
//            .tickSize(-$_chart.innerHeight())
            .ticks(20)
            .orient("bottom");

        var y_axis = d3.svg.axis()
            .scale(y)
            .tickFormat(d3.format(".3f"))
//            .tickSize(-$_chart.innerWidth())
            .ticks(20)
            .orient("left");

        chart.append("g")
            .attr("transform", "translate(0," + ($_chart.innerHeight() - 25) + ")")
            .attr("class", "drawings axes")
            .call(x_axis);

        chart.append("g")
            .attr("transform", "translate(100,0)")
            .attr("class", "drawings axes")
            .call(y_axis);

    };

    this.changeDisplay = function() {

        overlayToggle('show', 'Changing View.');

        // Prepare Draw Data
        //e._full_draw_data = e.prepareDrawData(db_data);

        // Update scales
        e.updateScales('both');
        e.replot();

        overlayToggle('hide');
    
    };

    function getPrecision(number) {
        if (number === 0) { number = 1; }
        return parseInt(Math.log(number) / Math.LN10 + 1, 10);
    }

    function getPaddedNumString(strnum,digits) {
        var pad = digits - strnum.length;
        return Array(pad + 1).join("0") + strnum;
    }


    this.updateScales = function(scale) {
    
	    // Only if data is available
    	if (typeof db_data == "undefined") { return; }

        if (scale == 'x' || scale == 'both') {

            x = d3.scale
                  .linear()
                  .domain([db_data.min_x, db_data.max_x])
                  .rangeRound([115, $_chart.innerWidth() - 20]);

        }

        if (scale == 'y' || scale == 'both') {
            
            y = d3.scale
                  .linear()
                  .domain([db_data.max_y, db_data.min_y])
                  .rangeRound([40, $_chart.innerHeight() - 40]);
        }

    };

    this.clearPlot = function() {
        // Cleanup everything
        d3.selectAll(".drawings").remove();
    };

    this.numberToString = function(value) {
        return String(value.toFixed(e.precision()));
    };

    $("#interpolation").change(function(ev) { e.replot(); });

}
