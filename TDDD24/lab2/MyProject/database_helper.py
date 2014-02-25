import sqlite3 
from flask import g 
 
def connect_db(): 
 return sqlite3.connect("database.db") 
 
def get_db():
    #db = getattr(g, 'db', None) 
    #if db is None: 
    db = connect_db() 
    return db 
 
"""
def init(): 
 c = get_db() 
 c.execute("drop table if exists entries") 
 c.execute("create table entries (id integer primary key, name text,message text)") 
 c.commit() 
 
def add_message(name,message): 
 c = get_db() 
 c.execute("insert into entries (name,message) values (?,?)", (name,message)) 
 c.commit()
"""

def postMessage(toEmail, fromEmail, message):
    c = get_db()
    c.execute("INSERT INTO posts (toEmail, fromEmail, message) VALUES (?,?,?)", (toEmail,fromEmail,message))
    c.commit()

def getMessages(toEmail):
    c = get_db()
    cur = c.cursor()
    cur.execute("SELECT * FROM posts WHERE toEmail = (?)", (toEmail,))
    retVal = cur.fetchall()

    print(retVal)
    c.commit()

    return retVal

def createUser(email, password, firstname, familyname, gender, city, country):
    if (getUser(email) == []):        
        c = get_db()
        c.execute("INSERT INTO users (email, password, firstname, familyname, gender, city, country) VALUES (?,?,?,?,?,?,?)",
              (email, password, firstname, familyname, gender, city, country))
        c.commit()
        return True
    else:
        return False

def getUser(email):
    c = get_db()
    cur = c.cursor()
    cur.execute("SELECT * FROM users WHERE email = (?)", (email,))
    retVal = cur.fetchall()
    return retVal

 
def close(): 
    get_db().close()

getUser("KLANGKLNAG")

