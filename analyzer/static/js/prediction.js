function elapsd() {

    var e = this;

    var chart_seconds = d3.select('#drawing').append("svg:svg")
                          .attr("class", "chart")
                          .attr("id", "chart_seconds")
                          .attr("width", "100%")
                          .attr("height", "50");
    
    var $_chart_seconds = $("#chart_seconds");

    overlayToggle = function(newState, msg) {
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

    var x, y;
    var db_data = {
        'data': {},
        'min_x': 0,
        'max_x': 0,
        'max_y': 0,
        'min_y': 0
    };

    this.getDBData = function() { return db_data; }

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
            colors.push(color_assignments[sid]);

        } else {

            overlayToggle('show', 'Loading selected data.');

            // Select it
            caller.addClass('selected');
            this.exp_selection[eid].exp_data[sid].selected = true;

            color_assignments[sid] = colors.pop();

            caller.css('border-left-color', color_assignments[sid]);

        }

        current_selection = redrawFromSelection(this, '/get_wall_time', current_selection);

    };

    this.redrawFromSelectionCallback = function(data) {   

        main_key = data['result'][0];
        values = data['result'][1];

        if (typeof db_data.data[main_key] == "undefined") {
            db_data.data[main_key] = [];
        }

        tmp = {};

        $.each(values, function(index, value) {

            if (typeof tmp[value[1]] == "undefined") {
                tmp[value[1]] = {
                    'key'   : value[0],
                    'y'     : value[2],
                    'count' : 1
                }
            } else {
                tmp[value[1]].y += value[2];
                tmp[value[1]].count++;
            }
        });

        $.each(tmp, function(key,value) {
            db_data.data[main_key].push({
                'key'     : value.key,
                'x'       : key,
                'y'       : value.y / value.count,
                'enabled' : true
            });
        })

        max_x = 0;
        max_y = 0;

        min_x = Number.MAX_VALUE;
        min_y = Number.MAX_VALUE;

        $.each(db_data.data[main_key], function(key, entry) {

            max_y = Math.max(max_y, entry.y);
            max_x = Math.max(max_x, entry.x);

            min_x = Math.min(min_x, entry.x);
            min_y = Math.min(min_y, entry.y);

        });

        db_data.data[main_key].max_x = max_x;
        db_data.data[main_key].min_x = min_x;
        db_data.data[main_key].max_y = max_y;
        db_data.data[main_key].min_y = min_y;
        
        // Update overall min/max
        max_x = 0;
        max_y = 0;

        min_x = Number.MAX_VALUE;
        min_y = Number.MAX_VALUE;


        $.each(db_data.data, function(key, entry) {

            max_x = Math.max(max_x, entry.max_x);
            max_y = Math.max(max_y, entry.max_y);

            min_x = Math.min(min_x, entry.min_x);
            min_y = Math.min(min_y, entry.min_y);
        });
        
        db_data.max_x = max_x;
        db_data.max_y = max_y;

        db_data.min_x = min_x;
        db_data.min_y = min_y;

        changeDisplay(e, db_data);
    }

    this.prepareDrawData = function(data) { return data; }

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

            for (xx = entry.min_x; xx < entry.max_x; xx += 0.05) {

                 interp_points.push({
                     'x': xx,
                     'y': lagrangePoly(xx, line_points)
                 });

            }

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
            .tickFormat(d3.format("d"))
//            .tickSize(-$_chart.innerHeight())
//            .ticks(20)
            .orient("bottom");

        var y_axis = d3.svg.axis()
            .scale(y)
//            .tickFormat(d3.format(".3f"))
//            .tickSize(-$_chart.innerWidth())
//            .ticks(20)
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

}

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

function createExpSelection(obj) {
   
    obj.exp_selection = {}; // Re-Initialize experiments object

    var exp           = experiments;
    var exp_selection = obj.exp_selection;

    var exp_to_load   = 0;
    var exp_loaded    = 0;

    $.ajax({
        type: 'GET',
        url: '/get_prediction_experiments',
        dataType: 'json',
        data: { db: obj.db },
        async: true,
        success: function(data) {

            console.log(data);

            var exp_id = 0;

            for (i = 0; i < data.result.length; i++) {

                exp_selection[i] = {
                    'exp_date'  : '',
                    'exp_name'  : '',
                    'exp_start' : '',
                    'exp_stop'  : '',
                    'exp_data'  : {}
                }
                
                value = data.result[i];
                
                exp_selection[i].exp_data[value[0] + '-' + value[2]] = {
                    'kname': value[1],
                    'dname': value[3],
                    'selected': false
                };
            
            }

            redraw(exp, exp_selection);
            resizeDocument(e);

            overlayToggle('hide');

        }
    });
};
