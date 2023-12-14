import json
from http_class import parse_request, Http_response
from cookie import generate_cookie, remove_cookie, get_account_by_cookie, check_cookie
from youtube import get_comment_api


BUFFER_SIZE = 4096

def handle_client(client_fd):
    data = client_fd.recv(BUFFER_SIZE).decode("utf-8")
    request = parse_request(data)
    if request == None:
        print("Empty request!")
        client_fd.close()
        return
    
    print(request.method, request.path)
    
    if (request.path == "/" or request.path == "/index.html") and request.method == "GET":
        response = root_page()
    elif request.path == "/login.html" and request.method == "GET":
        response = login_page()
    elif request.path == "/register.html" and request.method == "GET":
        response = register_page()
    elif request.path == "/video.html" and request.method == "GET":
        response = video_page()
    elif request.path == "/rick_roll.mp4" and request.method == "GET":
        response = get_rick_roll()
    elif request.path == "/api/get_account" and request.method == "GET":
        response = get_account(request)
    elif request.path == "/api/message" and request.method == "GET":
        response = get_message()
    elif request.path == "/api/left_message" and request.method == "POST":
        response = left_message(request)
    elif request.path == "/api/login" and request.method == "POST":
        response = login(request)
    elif request.path == "/api/logout" and request.method == "DELETE":
        response = logout(request)
    elif request.path == "/api/register" and request.method == "POST":
        response = register(request)
    elif request.path == "/api/get_poke" and request.method == "POST":
        response = get_poke(request)
    elif request.path == "/api/poke" and request.method == "POST":
        response = poke(request)
    elif request.path == "/api/comment" and request.method == "GET":
        response = get_comment()
    else:
        response = Http_response("HTTP/1.1", "404", "Not Found", {}, "")
    client_fd.send(response.byteify())
    
    
    client_fd.close()
    print("Connection closed!")
    

def root_page():
    profile = open("web/index.html", "r")
    response = Http_response("HTTP/1.1", "200", "OK", {}, profile.read())
    profile.close()
    return response

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

def video_page():
    video = open("web/video.html", "r")
    response = Http_response("HTTP/1.1", "200", "OK", {}, video.read())
    video.close()
   
    return response

def get_rick_roll():
    rick_roll = open("database/rick_roll.mp4", "rb")
    data = rick_roll.read()
    response = Http_response("HTTP/1.1", "200", "OK", {
        "Content-Type": "video/mp4",
        "Content-Length": str(len(data))
    }, data)
    rick_roll.close()

    return response

def get_account(request):
    if "Cookie" in request.headers:
        cookie = request.headers["Cookie"]
        cookies = cookie.split(";")
        cookie_id = ""
        for i in cookies:
            if i.split("=")[0].strip() == "cookie_id":
                cookie_id = i.split("=")[1]
                break
        
        account = ""
        if check_cookie(cookie_id):
            account = get_account_by_cookie(cookie_id)
        account_string = json.dumps({"account": account})
        return Http_response("HTTP/1.1", "200", "OK", {}, account_string)
    else:
        return Http_response("HTTP/1.1", "200", "OK", {}, json.dumps({"account": ""}))

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

def login(request):
    account_data = json.load(open("database/account.json", "r"))
    account = json.loads(request.body)
    for i in account_data:
        if i["account"] == account["account"] and i["password"] == account["password"]:
            remove_cookie(account["account"])
            cookie = generate_cookie(i["account"])
            response = Http_response("HTTP/1.1", "200", "OK", {"Set-Cookie": cookie}, "")
            return response
    return Http_response("HTTP/1.1", "401", "Unauthorized", {}, "")

def logout(request):
    cookie = request.headers["Cookie"]
    cookies = cookie.split(";")
    cookie_id = ""
    for i in cookies:
        if i.split("=")[0].strip() == "cookie_id":
            cookie_id = i.split("=")[1]
            break
    remove_cookie(get_account_by_cookie(cookie_id))
    return Http_response("HTTP/1.1", "200", "OK", {}, "")

def register(request):
    account_data = json.load(open("database/account.json", "r"))
    account = json.loads(request.body)
    for i in account_data:
        if i["account"] == account["account"]:
            return Http_response("HTTP/1.1", "409", "Conflict", {}, "")
    account_data.append(account)
    json.dump(account_data, open("database/account.json", "w"))
    return Http_response("HTTP/1.1", "200", "OK", {}, "")

def get_poke(request):
    account = json.loads(request.body)
    poke_data = json.load(open("database/poke.json", "r"))
    for i in poke_data:
        if i["account"] == account["account"]:
            return Http_response("HTTP/1.1", "200", "OK", {}, json.dumps({"poke": i["poke"]}))
    print("No account")
    return Http_response("HTTP/1.1", "200", "OK", {}, json.dumps({"poke": []}))

def poke(request):
    poke_data = json.load(open("database/poke.json", "r"))

    poke = json.loads(request.body)

    exist = False
    for i in poke_data:
        if i["account"] == poke["to"]:
            for j in i["poke"]:
                if j["from"] == poke["from"]:
                    exist = True
                    break
            break
    if exist:
        return Http_response("HTTP/1.1", "200", "OK", {}, "")
    
    poke_times = 0
    for i in poke_data:
        if i["account"] == poke["from"]:
            for j in i["poke"]:
                if j["from"] == poke["to"]:
                    poke_times = j["times"]
                    i["poke"].remove(j)
                    break
            break

    exist = False
    for i in poke_data:
        if i["account"] == poke["to"]:
            exist = True
            break
    if not exist:
        poke_data.append({"account": poke["to"], "poke": []})

    
    for i in poke_data:
        if i["account"] == poke["to"]:
            i["poke"].append({"from": poke["from"], "times": poke_times + 1})
            break

    json.dump(poke_data, open("database/poke.json", "w"))
    return Http_response("HTTP/1.1", "200", "OK", {}, "")

    
def get_comment():
    data = get_comment_api()
    
    return Http_response("HTTP/1.1", "200", "OK", {}, json.dumps(data))