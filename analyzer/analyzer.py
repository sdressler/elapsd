#!/usr/bin/python

from flask import Flask, render_template, g, request, jsonify

from analyzer import db_query

import os

app = Flask(__name__)

@app.after_request
def add_header(response):
    """
    Add headers to both force latest IE rendering engine or Chrome Frame,
    and also to cache the rendered page for 10 minutes.
    """
    response.headers['X-UA-Compatible'] = 'IE=Edge,chrome=1'
    response.headers['Cache-Control'] = 'public, max-age=0'
    return response

@app.route('/')
def hello_world():
    path = os.path.dirname(os.path.realpath(__file__))
    
    files = []
    for f in os.listdir(path + '/static/db'):
        if f.endswith('.db'):
            files.append(f)
    
    return render_template('index.html', dbs = files);

@app.route('/prediction')
def prediction():
    path = os.path.dirname(os.path.realpath(__file__))
    
    files = []
    for f in os.listdir(path + '/static/db'):
        if f.endswith('.db'):
            files.append(f)
    
    return render_template('prediction.html', dbs = files);


@app.route('/get_experiments')
def get_experiments():
    path = os.path.dirname(os.path.realpath(__file__))
    db_name = path + '/static/db/' + request.args.get('db', "", type=str)
    
    app.jinja_env.globals['db'] = db_name
    
    db = db_query.db_query(db_name)
    
    return jsonify(result=db.db_query_experiments())

@app.route('/get_prediction_experiments')
def get_prediction_experiments():
    path = os.path.dirname(os.path.realpath(__file__))
    db_name = path + '/static/db/' + request.args.get('db', "", type=str)
    
    app.jinja_env.globals['db'] = db_name
    
    db = db_query.db_query(db_name)
    
    return jsonify(result=db.db_query_prediction_experiments())


@app.route('/get_experiment_overview')
def get_experiment_overview():
    db_name = app.jinja_env.globals['db']
    db = db_query.db_query(db_name)
    
    e_id = request.args.get('id', "", type=str)
    return jsonify(result=db.db_query_experiment_overview(e_id))

@app.route('/get_data', methods=['POST'])
def get_data():
    db_name = app.jinja_env.globals['db']
    db = db_query.db_query(db_name)
    
    selection = []
    for x in request.form:
        selection.append(request.form.getlist(x))

    return jsonify(result=db.db_query_data_table(selection))

@app.route('/get_wall_time', methods=['POST'])
def get_wall_time():
    db_name = app.jinja_env.globals['db']
    db = db_query.db_query(db_name)
    
    selection = []
    for x in request.form:
        selection.append(request.form.getlist(x))

    return jsonify(result=db.db_query_wall_times(selection[0]))

if __name__ == '__main__':
    app.debug = True
    app.run(host='127.0.0.1', port=8080)
