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
def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d


def dict_from_row(self, row):
    return dict(zip(row.keys(), row))

def postMessage(toEmail, fromEmail, message):
    c = get_db()
    c.execute("INSERT INTO posts (toEmail, fromEmail, message) VALUES (?,?,?)", (toEmail,fromEmail,message))
    c.commit()
    c.close()
    
def getMessages(toEmail):
    c = get_db()
    c.row_factory = dict_factory
    cur = c.cursor()
    cur.execute("SELECT * FROM posts WHERE toEmail = (?)", (toEmail,))
    retVal = cur.fetchall()
    c.commit()
    c.close()
    return retVal

def createUser(email, password, firstname, familyname, gender, city, country):
    if not getUser(email):        
        c = get_db()
        c.execute("INSERT INTO users (email, password, firstname, familyname, gender, city, country) VALUES (?,?,?,?,?,?,?)",
              (email, password, firstname, familyname, gender, city, country))
        c.commit()
        c.close()
        return True
    else:
        return False

def getUser(email):
    c = get_db()
    c.row_factory = dict_factory
    cur = c.cursor()
    cur.execute("SELECT * FROM users WHERE email = (?)", (email,))
    retVal = cur.fetchone()
    #print(retVal)
    c.close()
    return retVal

def changePassword(email, oldPassword, newPassword):
    c = get_db()
    cur = c.cursor()
    cur.execute("SELECT * FROM users WHERE email = (?) AND password = (?)", (email, oldPassword))
    retVal = cur.fetchall()
    if (retVal == []):
        c.close()
        return False
    else:
        cur.execute("UPDATE users SET password = (?) WHERE email = (?) AND password = (?)", (newPassword, email, oldPassword))
        c.commit()
        c.close()
        return True

def close(): 
    get_db().close()


#print(changePassword("nicsi918@student.liu.se", "asd", "dsggga"))

#print(getUser("nicsi918@student.liu.se"))
#print(createUser("nicsi918@student.liu.se","","","","","",""))

