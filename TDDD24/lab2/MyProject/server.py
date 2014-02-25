from flask import Flask, render_template, request, jsonify
app = Flask(__name__)
import sys
import database_helper


storedToken = "UIHEWFISUHF%/&/)((F/Y/&"

@app.route('/')
def hello_world():
    print("Sending template")
    return render_template("client.html", title = 'Home')




@app.route('/getUserMessagesByToken', methods=['GET'])
def getUserMessagesByToken():
    email = "nicsi918@student.liu.se" ##mail by token
    token = storedToken ##token
    getUserMessages(email, token)

@app.route('/getUserMessagesByEmail', methods=['GET'])
def getUserMessagesByEmail():
    email = "nicsi918@student.liu.se" ##mail by request
    token = storedToken ##token
    getUserMessages(email, token)
    

def getUserMessages(email, token):
    if (token == storedToken): ##
        dbRet = database_helper.getMessages(email)
        return jsonify(success = True, message = "Returning messages", data = dbRet)
    else:
        return jsonify(success = False, message = "User not logged in")
    


@app.route('/postMessage', methods=['POST'])
def postMessage(token, content, toEmail):
    token = storedToken ##token
    toEmail = request.form['email']
    fromEmail = "nicsi918@student.liu.se" ##get email of token
    message = request.form['message']

    if (token == storedToken):
        database_helper.postMessage(toEmail, fromEmail, message)
        return jsonify(success = True, message = "Message posted")
    else:
        return jsonify(success = False, message = "User not logged in")


@app.route('/getUserDataByEmail', methods=['GET'])
def getUserDataByEmail():
    email = "nicsi918@student.liu.se" ##Searched email
    token = storedToken ##Token

@app.route('/getUserDataByToken', methods=['GET'])
def getUserDataByToken():
    email = "nicsi918@student.liu.se" ##Users own email
    token = storedToken ##Token
    return getUserData(email, token)

def getUserData(email, token):
    if (token == storedToken): ##
        dbRet = database_helper.getUser(email)
        print(dbRet)
        if (dbRet == []):
            return jsonify(success = False, message = "User does not exist")
        else:
            return jsonify(success = True, message = "Got user data", data = dbRet[0])
                       

@app.route('/signout', methods=['POST'])
def signOut():
    ##do something with token
    return "Signing out not impl"

@app.route('/signup', methods=['POST'])
def signUp():
    firstname = request.form['firstname']
    familyname = request.form['familyname']
    gender = request.form['gender']
    email = request.form['email']
    city = request.form['city']
    country = request.form['country']
    password = request.form['password']

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
    print(user[0][1])

    success = True
    message = "User is logged in"
    
    if (user == []):
        success = False
        message = "User does not exist"
        return jsonify(message = message, success = success)
    else:
        if (user[0][1] == password):
            return jsonify(message = message, success = success, token = storedToken)
        else:
            success = False
            message = "Incorrect password"
            return jsonify(message = message, success = success)


@app.route('/changePassword', methods=['POST'])
def changePassword():
    token = storedToken ##
    oldPassword = request.form['oldPassword']

    return "not done yet"

                       
                       
    

if __name__ == '__main__':
    app.run(debug=True)






def changePassword(token, oldPassword, newPassword):
    pass

#signIn("nicsi918@student.liu.se", "")
