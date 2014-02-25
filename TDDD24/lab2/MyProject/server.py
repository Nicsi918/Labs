from flask import Flask, render_template, request
app = Flask(__name__)
import sys
import database_helper


@app.route('/')
def hello_world():
    #return render_template("test.html", title = 'Home')
    print("Sending template")
    return render_template("client.html", title = 'Home')

@app.route('/signup/<fname>')
def signup(fname):
    return fname;

@app.route('/test', methods = ['POST', 'GET'])
def testing():

    print(request)
    if (request.method == 'POST'):
        f = request.form['name']
        print(f)
        print("POST----")
        #for key in f.keys():
        #for value in f.getlist(key):
        #print key,":",value
    else:
        data = request.args.get('name', '')
        print(data)
    return "TESTed"

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

    return dbRet

#@app.route('/add_entry', methods = ['POST', 'GET'])
@app.route('/add_entry')
def add_entry():
    #print(request.form['email'])
    #print(request.form['password'])
    print("woked")
    #sys.stdout.flush()
    return "DERERP"
    #return render_template("client.html", title = 'Home', success=json.dumps(True))

if __name__ == '__main__':
    app.run()





#@app.route('/login', methods=['POST'])
#def test():
 #   if request.method == 'POST':
 #       print(request.form['username'])            
    

@app.route('/signin', methods=['POST'])
def signIn():
    email = request.form['email']
    password = request.form['password']
    user = database_helper.getUser(email)
    database_helper.close()
    print(user)

    ##check if exists,
    ## log in
    pass

def postMessage(token, content, toEmail):

    ##check if logged in
    database_helper.postMessage()
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


    

def changePassword(token, oldPassword, newPassword):
    pass

#signIn("nicsi918@student.liu.se", "")
