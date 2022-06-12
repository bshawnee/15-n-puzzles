#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <unistd.h>
#include <fstream>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

static const char* gHelp = R"(
HELP:
'-size' [x]
'-solvable' [0] or [1]
'-aimode' [0] or [1]
Run without args equal : -size 3 -solvable 1 -aimode 0
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
            settings["size"] = tmp;
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
        else if (argument == "-file") {
            i++;
            if (argv[i] == nullptr) {
                throw std::logic_error("argument '-file' is not specified\n");
            }
            settings["file"] = i;
        }
    }
    if (settings.find("file") == settings.end()) {
        settings["file"] = 0;
    }
    if (settings.find("size") == settings.end()) {
        settings["size"] = 3;
    }
    if (settings.find("solvable") == settings.end()) {
        settings["solvable"] = true;
    }
    if (settings.find("aimode") == settings.end()) {
        settings["aimode"] = false;
    }
    return settings;
}

std::vector<int> readFromFile(const std::string& fileName)
{
    std::ifstream file(fileName, std::ios_base::in);
    std::vector<int> ret;

    if (!file.is_open()) {
        throw std::logic_error("File not found\n");
    }

    std::string line;
    while (std::getline(file, line)) {
        auto comment = std::find_if(line.begin(), line.end(), [](auto c) {
            if (c == '#') return true;
            else return false;
        });
        auto end = std::find_if(line.begin(), comment, [](auto c) {
            if (c != ' ' && !std::isdigit(c)) return true;
            else return false;
        });
        if (end != comment) {
            throw std::logic_error("Invalid file");
        }
        if (end == line.begin()) {
            continue;
        }
        std::string forTokens(line.begin(), end);
        boost::char_separator<char> sep(" ");
        boost::tokenizer<boost::char_separator<char>> tokens(forTokens, sep);
        for(const auto& token : tokens) {
            ret.emplace_back(std::stoi(token));
        }
    }
    if (ret.size() < 5) {
        throw std::logic_error("Invalid file");
    }
    if (*ret.begin() * *ret.begin() != ret.size() - 1) {
        throw std::logic_error("Invalid file");
    }
    return ret;
}

int main([[maybe_unused]] int argc, const char **argv) {

    std::map<std::string, int> settings;
    try {
        settings = argumentSettings(argv);
    }
    catch(std::exception& e) {
        std::cerr << e.what() << gHelp;
        return 1;
    }
    std::string fileName;
    if (settings["file"] == 0) {
        std::string exec = "python3 ../gen.py ";
        exec += std::to_string(Game::x15);
        exec += settings["solvable"] == true ? " -s " : " -u ";
        exec += "> genValue.txt";
        system(exec.c_str());
        sleep(1);
        fileName = "genValue.txt";
    }
    else {
        fileName = argv[settings["file"]];
    }
    std::vector<int> mapGen;
    try {
        mapGen = readFromFile(fileName);
        settings["size"] = *mapGen.begin();
        mapGen.erase(mapGen.begin());

    } catch (std::exception& e) {
        std::cerr << e.what() << gHelp;
        return 1;
    }
    Game game(mapGen);
    Game::y15 = settings["size"];
    Game::x15 = settings["size"];
    sf::RenderWindow window(sf::VideoMode(Game::y15 * buttonSize, Game::x15 * buttonSize), "15", 7);
    window.setFramerateLimit(60);
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
