import json

class Http_response:
    def __init__(self, protocol, status_code, status_text, headers, body) -> None:
        self.protocol = protocol
        self.status_code = status_code
        self.status_text = status_text
        self.headers = headers
        self.body = body

    def byteify(self) -> bytes:
        output = f"{self.protocol} {self.status_code} {self.status_text}\r\n"
        for header in self.headers:
            output += f"{header}: {self.headers[header]}\r\n"
        output += "\r\n"
        output = str.encode(output)
        
        output_body = bytes()
        if type(self.body) == str:
            output_body = str.encode(self.body)
        elif type(self.body) == bytes:
            output_body = self.body

        output += output_body
        return output 
    

class Http_request:
    def __init__(self, method, path, protocol, headers, body):
        self.method = method
        self.path = path
        self.protocol = protocol
        self.headers = headers
        self.body = body

    def byteify(self):
        output = f"{self.method} {self.path} {self.protocol}\r\n"
        for header in self.headers:
            output += f"{header}: {self.headers[header]}\r\n"
        output += "\r\n"
        output = str.encode(output)
        
        output_body = bytes()
        if type(self.body) == str:
            output_body = str.encode(self.body)
        elif type(self.body) == bytes:
            output_body = self.body

        output += output_body
        return output 

def parse_response(data) -> Http_response:
    if data == "":
        return None
    if data.find("\r\n\r\n") == -1:
        Header = data
        Body = ""
    else:
        Header, Body = data.split("\r\n\r\n")

    Header = Header.split("\r\n")
    protocol, status_code, status_text = Header[0].split(" ")
    headers = {}
    for header in Header[1:]:
        header, value = header.split(": ")
        headers[header] = value
    return Http_response(protocol, status_code, status_text, headers, Body)

def parse_request(data) -> Http_request:
    if data == "":
        return None
    if data.find("\r\n\r\n") == -1:
        Header = data
        Body = ""
    else:
        Header, Body = data.split("\r\n\r\n")

    Header = Header.split("\r\n")
    method, path, protocol = Header[0].split(" ")
    headers = {}
    for header in Header[1:]:
        header, value = header.split(": ")
        headers[header] = value
    return Http_request(method, path, protocol, headers, Body)


