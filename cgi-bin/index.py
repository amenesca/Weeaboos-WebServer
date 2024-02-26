#!/usr/bin/env python3
import cgi
import os

# Cabeçalhos MIME
print("Content-Type: text/plain")  # Indica o tipo de conteúdo como texto simples
print()  # Imprime uma linha em branco para indicar o fim dos cabeçalhos

# Receber dados do formulário
form = cgi.FieldStorage()

nome = form.getvalue("nome")
email = form.getvalue("email")
imagem = form["imagem"]

if imagem.filename:  # Verifica se um arquivo foi selecionado
    # Define o caminho para onde a imagem será movida
    destino = os.path.join("../enviados", imagem.filename)

    # Move a imagem para o destino
    with open(destino, "wb") as f:
        f.write(imagem.file.read())

    # Retorna o caminho da imagem enviada
    print(destino)
else:
    # Retorna uma mensagem de erro se nenhum arquivo foi selecionado
    print("Nenhuma imagem selecionada para enviar.")