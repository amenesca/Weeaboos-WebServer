#include "./RequestParser.hpp"

int main() {
	RequestParser request;

	std::string line = "GET / HTTP/1.1\r\n\
Host: localhost:18000\r\n\
Connection: keep-alive\r\n\
Cache-Control: max-age=0\r\n\
sec-ch-ua: \"Not_A Brand\";v=\"8\", \"Chromium\";v=\"120\", \"Microsoft Edge\";v=\"120\"\r\n\
sec-ch-ua-mobile: ?0\r\n\
sec-ch-ua-platform: \"Windows\"\r\n\
DNT: 1\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,/;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n\
Sec-Fetch-Site: none\r\n\
Sec-Fetch-Mode: navigate\r\n\
Sec-Fetch-User: ?1\r\n\
Sec-Fetch-Dest: document\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Accept-Language: pt-BR,pt;q=0.9,en-US;q=0.8,en;q=0.7\r\n";

	request.parse(line);

    for (const auto& pair : request.getHeaders()) {
        std::cout << pair.first << " : " << pair.second << std::endl;
    }


	return (0);
}
