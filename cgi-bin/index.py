#!/usr/bin/python3

import cgi
import os

print("CHEGOU NO PYTHON")
for key, value in os.environ.items():
    print(f"{key}: {value}")
form = cgi.FieldStorage()
nome = form.getvalue("nome")
email = form.getvalue("email")
print(nome)
print(form) # imprime o objeto form como um dicionário


