#include <iostream>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace httplib;
using namespace std;

int get_time() {
	Client cli("http://worldtimeapi.org");
	auto res = cli.Get("/api/timezone/Europe/Simferopol");
	if (res) {
		if (res->status == 200) {
			json j = json::parse(res->body);
			return j["unixtime"];
		}
	}

}

json get_weather(int time) {
	Client cli("http://api.openweathermap.org");
	auto res = cli.Get("/data/2.5/onecall?lat=44.952116&lon=34.102411&units=metric&lang=ru&exclude=daily&appid=81bd532f5074849455017bcd3a66171a");
	if (res) {
		if (res->status == 200) {
			json j = json::parse(res->body);
			json hourly = j["hourly"].get<json>();

			json hour_forecast;

			int last = hourly.size() - 1;

			if (hourly[last]["dt"] < time) {
				return json::object();
			}

			for (int i = 0; i <= last; i++) {
				if (hourly[i]["dt"] >= time) {
					return hourly[i];
				}
			}

			return json::object();
		} else {
			return json::object();
		}
	} else {
		return json::object();
	}
}

// В этой функции формируем ответ сервера на запрос
void gen_response(const Request& req, Response& res) {
	// Команда set_content задаёт ответ сервера и тип ответа:
	// Hello, World! - тело ответа
	// text/plain - MIME тип ответа (в данном случае обычный текст)
	res.set_content(to_string(get_time()), "text/plain");
}

void raw_response(const Request& req, Response& res) {
	int current_time = get_time();
	json current_weather = get_weather(current_time);

	json result;

	result["temp"] = current_weather["temp"];
	result["description"] = current_weather["weather"][0]["description"];

	res.set_content(result.dump(4), "text/json");
}

int main() {
	Server svr;                    // Создаём сервер (пока-что не запущен)
	svr.Get("/", gen_response);    // Вызвать функцию gen_response если кто-то обратиться к корню "сайта"
	svr.Get("/raw", raw_response);
	std::cout << "Start server... OK\n";
	svr.listen("localhost", 3000); // Запускаем сервер на localhost и порту 1234
}