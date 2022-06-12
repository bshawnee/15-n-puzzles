#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <unistd.h>
#include <fstream>
#include <map>
#include <boost/algorithm/string.hpp>
constexpr std::size_t OutlineThickness = 12;

static const char* gHelp = R"(
HELP:
'-size' [x] [y]
'-solvable' [0] or [1]
'-aimode' [0] or [1]
Run without args equal : -size 3 3 -solvable 1 -aimode 0
)";

std::map<std::string, int> argumentSettings(const char **argv)
{
    std::map<std::string, int> settings;
    for (int i = 0; argv[i] != nullptr; i++) {
        std::string argument(argv[i]);
        boost::algorithm::to_lower(argument);
        if (argument == "-size") {
            i++;
            int tmp;
            if (argv[i] == nullptr) {
                throw std::logic_error("argument '-size' is not specified\n");
            }
            else {
                try {
                    tmp = std::stoi(argv[i]);
                    if (tmp <= 0) {
                        throw std::exception();
                    }
                } catch (std::exception&) {
                    throw std::logic_error("argument '-size' is not specified\n");
                }
            }
            settings["sizeX"] = tmp;

            i++;
            if (argv[i] == nullptr) {
                throw std::logic_error("argument '-size' is not specified\n");
            }
            else {
                try {
                    tmp = std::stoi(argv[i]);
                    if (tmp <= 0) {
                        throw std::exception();
                    }
                } catch (std::exception&) {
                    throw std::logic_error("argument '-size' is not specified\n");
                }
            }
            settings["sizeY"] = tmp;
        }
        else if (argument == "-solvable") {
            i++;
            std::string_view solvable(argv[i]);
            int tmp;
            if (solvable.empty()) {
                throw std::logic_error("argument '-solvable' is not specified\n");
            }
            try {
                tmp = std::stoi(solvable.data());
            }
            catch (std::exception&) {
                throw std::logic_error("argument '-solvable' is not specified\n");
            }
            settings["solvable"] = static_cast<bool>(tmp);
        }
        else if (argument == "-aimode") {
            settings["aimode"] = 1;
        }
    }
    if (settings.find("sizeX") == settings.end()) {
        settings["sizeX"] = 3;
    }
    if (settings.find("sizeY") == settings.end()) {
        settings["sizeY"] = 3;
    }
    if (settings.find("solvable") == settings.end()) {
        settings["solvable"] = true;
    }
    if (settings.find("aimode") == settings.end()) {
        settings["aimode"] = false;
    }
    return settings;
}

int main(int argc, const char **argv) {

    std::map<std::string, int> settings;
    try {
        settings = argumentSettings(argv);
    }
    catch(std::exception& e) {
        std::cerr << e.what() << gHelp;
        return 1;
    }
    Game::y15 = settings["sizeY"];
    Game::x15 = settings["sizeX"];
    sf::RenderWindow window(sf::VideoMode(Game::y15 * buttonSize, Game::x15 * buttonSize), "15", 7);
    window.setFramerateLimit(60);

    std::string exec = "python3 ../gen.py -s ";
    exec += argc != 2 ? std::to_string(3) : argv[1];
    exec += " > genValue.txt";
    system(exec.c_str());
    sleep(1);
    std::ifstream file("genValue.txt", std::ios_base::in);
    std::vector<int> mapGen;
    if (file.is_open()) {
        std::cout << "File open\n";
    }
    std::string line;
    while (std::getline(file, line)) {
        if (*line.begin() != '#') {
            mapGen.emplace_back(std::stoi(line));
        }
    }
    mapGen.erase(mapGen.begin());
    Game game(mapGen);
    while (window.isOpen()) {
        sf::Event windowEvent;
        window.clear();
        if (window.pollEvent(windowEvent) && windowEvent.type == sf::Event::Closed) {
            window.close();
            break;
        }
        if (!game.goingAnimation() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            game.interact(sf::Mouse::getPosition(window));
        }
        game.render(window);
        if (game.needUpdate())
        window.display();
    }
    return 0;
}
