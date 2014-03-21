from flask import Flask, render_template, request, jsonify, json
from flask.ext.socketio import SocketIO, emit
app = Flask(__name__)
import sys
import database_helper
import hashlib
import os,binascii
from gevent.wsgi import WSGIServer
from geventwebsocket.handler import WebSocketHandler

loggedInUsers = {}
loggedInWebSockets = {}

def connect(ws):
    print "Client connected"
    data = json.loads(ws.receive())
    token = data["token"]
    if not token in loggedInWebSockets:
        loggedInWebSockets[token] = ws
    ws.receive()
            
def my_app(environ, start_response):  
    path = environ["PATH_INFO"]  
    if path == "/":  
        return app(environ, start_response)  
    elif path == "/connectWS":  
        connect(environ["wsgi.websocket"])   
    else:  
        return app(environ, start_response)

def send_update():
    for token in loggedInWebSockets:
        try:
            ws = loggedInWebSockets[token]
            ws.send(json.dumps({'message': "Update"}))
        except e:
            ws.close()
            del loggedInWebSockets[token]      

@app.route('/')
def index():
    print("Sending template")
    return render_template("client.html", title = 'Home')

@app.route('/getUserMessagesByToken', methods=['GET'])
def getUserMessagesByToken():
    print "Get messages"
    token = request.args.get('token')
    if (token in loggedInUsers.keys()):
        email = loggedInUsers[token]
        return getUserMessages(email)
    else:
        return jsonify(success = False, message = "User not logged in")

@app.route('/getUserMessagesByEmail', methods=['GET'])
def getUserMessagesByEmail():
    print "Get messages"
    email = request.args.get('email')
    token = request.args.get('token')
    if (token in loggedInUsers.keys()):
        return getUserMessages(email)
    else:
        return jsonify(success = False, message = "User not logged in")
    
def getUserMessages(email):
    dbRet = database_helper.getMessages(email)
    return jsonify(success = True, message = "Returning messages", data = dbRet)

@app.route('/postMessage', methods=['POST'])
def postMessage():
    print "Post message"
    token = request.form['token']
    if (token in loggedInUsers.keys()):
        
        toEmail = request.form['email']
        fromEmail = loggedInUsers[token] 
        message = request.form['message']

        database_helper.postMessage(toEmail, fromEmail, message)
        send_update()
        return jsonify(success = True, message = "Message posted")
    else:
        return jsonify(success = False, message = "User not logged in")


@app.route('/getUserDataByEmail', methods=['GET'])
def getUserDataByEmail():
    print "Get user data"
    email = request.args.get('email')
    token = request.args.get('token')
    if (token in loggedInUsers.keys()):
        return getUserData(email)

@app.route('/getUserDataByToken', methods=['GET'])
def getUserDataByToken():
    print "Get user data"
    token = request.args.get('token')
    if (token in loggedInUsers.keys()):
        email = loggedInUsers[token]
        return getUserData(email)
    else:
        return jsonify(success = False, message = "Not logged in")

def getUserData(email):
    dbRet = database_helper.getUser(email)
    if not dbRet:
        return jsonify(success = False, message = "User does not exist")
    else:
        #dbRet[0] = dbRet[0][:1] + dbRet[0][2:]
        del dbRet['password']
        return jsonify(success = True, message = "Got user data", data = dbRet)
                      

@app.route('/signout', methods=['POST'])
def signOut():
    print "Signout"
    token = request.form['token']
    if (token in loggedInUsers.keys()):
        del loggedInUsers[token]
        del loggedInWebSockets[token]
        return jsonify(success = True, message = "Successfully logged out")
    else:        
        return jsonify(success = False, message = "Not logged in")

@app.route('/signup', methods=['POST'])
def signUp():
    print "Signup"
    firstname = request.form['firstname']
    familyname = request.form['familyname']
    gender = request.form['gender']
    email = request.form['email']
    city = request.form['city']
    country = request.form['country']
    password = hashlib.sha224(request.form['password']).hexdigest()

    dbRet = database_helper.createUser(email, password, firstname, familyname, gender, city, country)
    if (dbRet):
        message = "User was successfully added"
    else:
        message = "User already exists for that email"
    return jsonify(message = message, success = dbRet)

@app.route('/signin', methods=['POST'])
def signIn():
    print "Signin"
    email = request.form['email']
    password = request.form['password']
    user = database_helper.getUser(email)
    
    success = True
    message = "User is logged in"
    
    if not user:
        success = False
        message = "User does not exist"
        return jsonify(message = message, success = success)
    else:
        if (user['password'] == hashlib.sha224(password).hexdigest()):
            token = binascii.b2a_hex(os.urandom(15))
            loggedInUsers[token] = email
            return jsonify(message = message, success = success, token = token)
        else:
            success = False
            message = "Incorrect password"
            return jsonify(message = message, success = success)


@app.route('/changePassword', methods=['POST'])
def changePassword():
    print "Change password"
    token = request.form['token']
    oldPassword = hashlib.sha224(request.form['oldPassword']).hexdigest()
    newPassword = hashlib.sha224(request.form['newPassword']).hexdigest()
    if (token in loggedInUsers.keys()):
        dbRet = database_helper.getUser(loggedInUsers[token])
        if (dbRet == []):
            return jsonify(success = False, message = "User does not exist")
        else:
            email = loggedInUsers[token]
            dbRet = database_helper.changePassword(email, oldPassword, newPassword)
            if (dbRet):
                return jsonify(success = True, message = "Password changed")
            else:
                return jsonify(success = False, message = "Incorrect password")
    else:
        return jsonify(success = False, message = "Not logged in")

    
server = WSGIServer(('', 5000), my_app, handler_class=WebSocketHandler)
server.serve_forever()
