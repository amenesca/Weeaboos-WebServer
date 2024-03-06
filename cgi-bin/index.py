#!/usr/bin/env python3
import cgi
import os

form = cgi.FieldStorage()

fileitem = form['file']

if fileitem.filename:
   if not os.path.exists("../enviados/"):
      os.makedirs("../enviados/")
   
   open("../enviados/" + fileitem.filename, "wb").write(fileitem.file.read())
   message = 'The file "' + fileitem.filename + '" was uploaded successfully'
 
else:
   message = 'No file was uploaded'

print(message)