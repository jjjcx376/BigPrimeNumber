from flask import Flask, render_template, request, redirect, url_for
from flask_wtf import FlaskForm
from wtforms import StringField, SubmitField
from wtforms.validators import DataRequired
import MillerRabbin
import os
import threading
import inspect
import ctypes
import platform
import time
from multiprocessing import Process, Queue

app = Flask(__name__)
app.secret_key = 'admin'

q = Queue()


class require_form(FlaskForm):
    width = StringField('Large prime digit width:', validators=[DataRequired()])
    times = StringField('Number of Miller-Rabin tests:', validators=[DataRequired()])
    file = StringField('Specify the file name:', validators=[DataRequired()])
    stop = False
    submit = SubmitField('Start')
    submit1 = SubmitField('Stop')


class stop_form(FlaskForm):
    submit1 = SubmitField('Stop')


def run(width, times, file):
    newfile = open('.\\' + file, 'w')
    newfile.close()
    result = []
    while 1:
        if q.qsize() > 0:
            break
        MillerRabbin.Miller_Rabin(width, times, file, result)
        time.sleep(0.5)


@app.route('/', methods=['GET', 'POST'])
def index():
    form = require_form()
    form1 = stop_form()

    submit = form.data['submit']
    submit1 = form.data['submit1']

    if q.empty():
        print("kong")
    else:
        print("man")

    # t1 = threading.Thread(target=run)
    if not q.empty() and request.form:
        q.get()
        if request.form.get("submit") and request.form.get("width") and request.form.get(
                "times") and request.form.get("file"):
            width = form.width.data
            times = form.times.data
            file = form.file.data
            # t1 = threading.Thread(target=run)
            t1 = Process(target=run, args=(width, times, file))
            # q.put(t1)
            t1.run()
    else:
        if request.form.get("submit") and request.form.get("width") and request.form.get(
                "times") and request.form.get("file"):
            width = form.width.data
            times = form.times.data
            file = form.file.data
            # t1 = threading.Thread(target=run)
            t1 = Process(target=run, args=(width, times, file))
            # q.put(t1)
            t1.run()
            # print(1)
            # time.sleep(2)

    if request.form.get("submit1"):
        # print(2)
        q.put(1)
        # stop_thread(t1)

        # request.form.setdefault("submit",None)
        return redirect(url_for('index'))
        # request.form.clear(form)
        # break
        # while(form.data):
        # MillerRabbin.Miller_Rabin(width,times,file)

        # form.stop=True
    return render_template('index.html', form=form, form1=form1)


if __name__ == '__main__':
    app.run()
