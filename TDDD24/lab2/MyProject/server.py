from flask import Flask, render_template, request, jsonify
app = Flask(__name__)
import sys
import database_helper
import hashlib
import os,binascii


loggedInUsers = {}

@app.route('/')
def hello_world():
    print("Sending template")
    return render_template("client.html", title = 'Home')




@app.route('/getUserMessagesByToken', methods=['GET'])
def getUserMessagesByToken():
    token = request.args.get('token')
    if (token in loggedInUsers.keys()):
        email = loggedInUsers[token]
        return getUserMessages(email)
    else:
        return jsonify(success = False, message = "User not logged in")

@app.route('/getUserMessagesByEmail', methods=['GET'])
def getUserMessagesByEmail():
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
    token = request.form['token']
    if (token in loggedInUsers.keys()):
        
        toEmail = request.form['email']
        fromEmail = loggedInUsers[token] 
        message = request.form['message']

        database_helper.postMessage(toEmail, fromEmail, message)
        return jsonify(success = True, message = "Message posted")
    else:
        return jsonify(success = False, message = "User not logged in")


@app.route('/getUserDataByEmail', methods=['GET'])
def getUserDataByEmail():
    email = request.args.get('email')
    token = request.args.get('token')
    if (token in loggedInUsers.keys()):
        return getUserData(email)

@app.route('/getUserDataByToken', methods=['GET'])
def getUserDataByToken():
    token = request.args.get('token')
    if (token in loggedInUsers.keys()):
        email = loggedInUsers[token]
        return getUserData(email)
    else:
        return jsonify(success = False, message = "Not logged in")

def getUserData(email):
    dbRet = database_helper.getUser(email)
    if (dbRet == []):
        return jsonify(success = False, message = "User does not exist")
    else:
        return jsonify(success = True, message = "Got user data", data = dbRet[0])
                      

@app.route('/signout', methods=['POST'])
def signOut():
    token = request.form['token']
    if (token in loggedInUsers.keys()):
        del loggedInUsers[token]
        return jsonify(success = True, message = "Successfully logged out")
    else:        
        return jsonify(success = False, message = "Not logged in")

@app.route('/signup', methods=['POST'])
def signUp():
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
    email = request.form['email']
    password = request.form['password']
    user = database_helper.getUser(email)
    
    success = True
    message = "User is logged in"
    
    if (user == []):
        success = False
        message = "User does not exist"
        return jsonify(message = message, success = success)
    else:
        if (user[0][1] == hashlib.sha224(password).hexdigest()):
            token = binascii.b2a_hex(os.urandom(15))
            loggedInUsers[token] = email
            return jsonify(message = message, success = success, token = token)
        else:
            success = False
            message = "Incorrect password"
            return jsonify(message = message, success = success)


@app.route('/changePassword', methods=['POST'])
def changePassword():
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
                       
    

if __name__ == '__main__':
    app.run(debug=True)
