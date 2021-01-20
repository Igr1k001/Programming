#include <iostream>
#include <cpp_httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

using json = nlohmann::json;
using namespace httplib;
using namespace std;

string read_html_from_disk() {
	string path = "htmlFragment.html";

	string html;
	ifstream file;

	file.open(path);
	if (!file.is_open()) {
		return "Ошибка чтения файла";
	}

	getline(file, html, '\0');

	file.close();

	return html;
}

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
		}
		else {
			return json::object();
		}
	}
	else {
		return json::object();
	}
}

// В этой функции формируем ответ сервера на запрос
void gen_response(const Request& req, Response& res) {
	int current_time = get_time();
	json current_weather = get_weather(current_time);

	string description = current_weather["weather"][0]["description"];
	int temp = current_weather["temp"];
	string icon = current_weather["weather"][0]["icon"];

	string html = read_html_from_disk();

	string descriptionPlaceholder = "{hourly[i].weather[0].description}";
	size_t found = html.find(descriptionPlaceholder);
	html.replace(found, descriptionPlaceholder.length(), description);

	string iconPlaceholder = "{hourly[i].weather[0].icon}";
	size_t found2 = html.find(iconPlaceholder);
	html.replace(found2, iconPlaceholder.length(), icon);

	string tempPlaceholder = "{hourly[i].temp}";
	size_t found3 = html.find(tempPlaceholder);
	html.replace(found3, tempPlaceholder.length(), to_string(temp));
	size_t found4 = html.find(tempPlaceholder);
	html.replace(found4, tempPlaceholder.length(), to_string(temp));
	
	res.set_content(html, "text/html");
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
	Server svr;                    // Создаём сервер
	svr.Get("/", gen_response);    // Вызвать функцию gen_response если кто-то обратиться к корню "сайта"
	svr.Get("/raw", raw_response);
	std::cout << "Start server... OK\n";
	svr.listen("localhost", 3000); // Запускаем сервер на localhost и порту 3000
}