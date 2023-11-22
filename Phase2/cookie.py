import json
import random
import datetime

TIME_OUT = 300

def generate_cookie(account):
    cookies_data = json.load(open("database/cookies.json", "r"))
    cookie = str(random.randint(1000000, 10000000))
    cookies_data.append({"cookie_id": cookie, "Time": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"), "account": account})
    json.dump(cookies_data, open("database/cookies.json", "w"))

    return "cookie_id=" + cookie

def remove_cookie(account):
    cookies_data = json.load(open("database/cookies.json", "r"))
    for i in cookies_data:
        if i["account"] == account:
            cookies_data.remove(i)
            break
    json.dump(cookies_data, open("database/cookies.json", "w"))

def get_account_by_cookie(cookie_id):
    cookies_data = json.load(open("database/cookies.json", "r"))
    for i in cookies_data:
        if i["cookie_id"] == cookie_id:
            return i["account"]
    return ""

def check_cookie(cookie_id):
    if cookie_id == "":
        return False
    cookies = json.load(open("database/cookies.json", "r"))
    for i in cookies:
        if i["cookie_id"] == cookie_id:
            if (datetime.datetime.now() - datetime.datetime.strptime(i["Time"], "%Y-%m-%d %H:%M:%S")).seconds < TIME_OUT:
                return True
            else:
                return False
    return False
    