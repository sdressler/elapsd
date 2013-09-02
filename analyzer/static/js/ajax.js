function redrawFromSelection(obj, url, current_selection) {
    
    /* Create the selection */
    var new_selection = {};
    $.each(obj.exp_selection, function(ekey,evalue) {
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

        /* Needed to update current_selection */
        //current_selection[key] = value;

    });
    
    /* Maybe nothing was selected ?! */
    if (Object.keys(new_selection).length === 0) {
        obj.clearPlot();
        return new_selection;
    }

    /* Nothing to reload, but we should trigger a redraw */
    if (load_selection.length === 0) {
        changeDisplay(obj, obj.getDBData());
        return new_selection;
    }

    /* Trigger the AJAX request to receive the data */
    overlayToggle(null, 'Data request pending.');
    var xhr = $.ajax({
        type: 'POST',
        url: url,
        dataType: 'json',
        data: { sel: load_selection },
        async: true,
        success: obj.redrawFromSelectionCallback
    });

    return new_selection;

};

function createExpSelection(obj) {
    
    obj.exp_selection = {}; // Re-Initialize experiments object

    var exp           = experiments;
    var exp_selection = obj.exp_selection;

    var exp_to_load   = 0;
    var exp_loaded    = 0;

    $.ajax({
        type: 'GET',
        url: '/get_experiments',
        dataType: 'json',
        data: { db: obj.db },
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
                            redraw(exp, exp_selection);
                            resizeDocument(e);

                            overlayToggle('hide');

                        }

                    }
                });
            });
        }
    });
};

