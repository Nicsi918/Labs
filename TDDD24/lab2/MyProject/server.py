from flask import Flask, render_template, request
app = Flask(__name__)
import sys


@app.route('/')
def hello_world():
    #return render_template("test.html", title = 'Home')
    print("Sending template")
    return render_template("client.html", title = 'Home')


@app.route('/add_entry', methods = ['POST'])
def add_entry():
    print(request.form['email'])
    print(request.form['password'])
    print("woked")
    sys.stdout.flush()
    return render_template("client.html", title = 'Home')

if __name__ == '__main__':
    app.run()



#@app.route('/login', methods=['POST'])
#def test():
 #   if request.method == 'POST':
 #       print(request.form['username'])            
    

def signIn(email, password):
    pass

def postMessage(token, content, toEmail):
    pass

def getUserDataByToken(token):
    pass

def getUserDataByEmail(token, email):
    pass

def getUserMessagesByToken(token):
    pass

def getUserMessagesByEmail(token, email):
    pass

def signOut(token):
    pass

def signUp(formData):
    pass

def changePassword(token, oldPassword, newPassword):
    pass

