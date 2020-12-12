#include <iostream>
#include <string>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;
using namespace std;
using namespace httplib;
 
// В этой функции формируем ответ сервера на запрос
void gen_response(const Request& req, Response& res) {
    // Выводим на экран тело запроса
    std::cout << req.body.c_str();
    // Здесь будет ответ, пока-что взят пример из документации
    std::string str = u8R"(
    {
      "response": {
        "text": "Здравствуйте! Я помогу вам с покупками.",
        "tts": "Здравствуйте! Я помог+у вас с пок+упками.",
        "buttons": [
            {
                "title": "Помощь",
                "payload": {},
                "hide": true
            }
        ],
        "end_session": false
      },
      "version": "1.0"
    })";
    // Отправляем ответ
    res.set_content(str, "text/json; charset=UTF-8");
}

void get_webhooks_html(const Request& req, Response& res) {
    string path = "Webhooks.html";

    string html;
    ifstream file;
    file.open(path);

    if (!file.is_open()) {
        res.set_content("Ошибка считывания файла", "text/plain");
        return;
    }

    getline(file, html, '\0');

    file.close();

    res.set_content(html, "text/html");
}

void get_FragmentWEB_html(const Request& req, Response& res) {
    string path = "FragmentWEB.html";
    string url_placeholder = "{Webhook URL}";

    string html;
    ifstream file;
    file.open(path);

    if (!file.is_open()) {
        res.set_content("Ошибка считывания файла", "text/plain");
        return;
    }

    getline(file, html, '\0');

    file.close();
    
    size_t found = html.find(url_placeholder);
    html.replace(found, url_placeholder.length(), "Привет");

    res.set_content(html, "text/html");
}

void get_config(const Request& req, Response& res) {
    string fath = "config.json";

    string config_string;
    ifstream file;
    file.open(fath);

    if (!file.is_open()) {
        res.set_content("Ошибка считывания файла", "text/plain");
        return;
    }

    getline(file, config_string, '\0');

    file.close();

    json config = json::parse(config_string);

    res.set_content(config.dump(4), "text/json");
}

int main() {

    Server svr;                            // Создаём сервер (пока-что не запущен)
    svr.Get("/fragment", get_FragmentWEB_html);
    svr.Get("/webhooks", get_webhooks_html);
    svr.Get("/json", get_config);
    svr.Post("/", gen_response);           // Вызвать функцию gen_response на post запрос
    std::cout << "Start server... OK\n"; // cout использовать нельзя
    svr.listen("localhost", 1234);         // Запускаем сервер на localhost и порту 1234
}
