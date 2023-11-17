import json

from http import parse_request, Http_response

BUFFER_SIZE = 4096

def handle_client(client_fd):
    data = client_fd.recv(BUFFER_SIZE).decode("utf-8")
    request = parse_request(data)
    if request == None:
        print("Empty request!")
        client_fd.close()
        return
    
    if (request.path == "/" or request.path == "/index.html") and request.method == "GET":
        response = root_page()
    elif request.path == "/login.html" and request.method == "GET":
        response = login_page()
    elif request.path == "/register.html" and request.method == "GET":
        response = register_page()
    elif request.path == "/api/message" and request.method == "GET":
        response = get_message()
    elif request.path == "/api/left_message" and request.method == "POST":
        response = left_message(request)
    elif request.path == "/api/login" and request.method == "POST":
        response = login(request)
    elif request.path == "/api/register" and request.method == "POST":
        response = register(request)
    else:
        response = Http_response("HTTP/1.1", "404", "Not Found", {}, "")
    client_fd.send(bytes(response.stringify(), "utf-8"))
    
    client_fd.close()
    print("Connection closed!")
    

def root_page():
    profile = open("web/index.html", "r")
    response = Http_response("HTTP/1.1", "200", "OK", {}, profile.read())
    profile.close()
    return response

def get_message():
    messages = open("database/messages.json", "r")
    response = Http_response("HTTP/1.1", "200", "OK", {}, messages.read())
    messages.close()
    return response

def left_message(request):
    messages = json.load(open("database/messages.json", "r"))
    message = json.loads(request.body)
    messages.append(message)    
    json.dump(messages, open("database/messages.json", "w"))
    return Http_response("HTTP/1.1", "200", "OK", {}, "")

def login_page():
    login = open("web/login.html", "r")
    response = Http_response("HTTP/1.1", "200", "OK", {}, login.read())
    login.close()
    return response

def register_page():
    register = open("web/register.html", "r")
    response = Http_response("HTTP/1.1", "200", "OK", {}, register.read())
    register.close()
    return response

def login(request):
    account_data = json.load(open("database/account.json", "r"))
    account = json.loads(request.body)
    for i in account_data:
        if i["account"] == account["account"] and i["password"] == account["password"]:
            response = Http_response("HTTP/1.1", "200", "OK", {}, "")
            return response
    return Http_response("HTTP/1.1", "401", "Unauthorized", {}, "")

def register(request):
    account_data = json.load(open("database/account.json", "r"))
    account = json.loads(request.body)
    for i in account_data:
        if i["account"] == account["account"]:
            return Http_response("HTTP/1.1", "409", "Conflict", {}, "")
    account_data.append(account)
    json.dump(account_data, open("database/account.json", "w"))
    return Http_response("HTTP/1.1", "200", "OK", {}, "")