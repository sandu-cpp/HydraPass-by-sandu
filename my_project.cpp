//Hello! im new developer at C++ language that my fist project 
//Привет я новый С++ разработчик это мой первый проект надеюсь ты оценишь
#include <iostream>
#include <string> 
#include <fstream>
#include <vector>

//here main logic all manager
//здесь находится главный класс который отвечает за всю логику софта
class PasswordManager {
private:
    std::vector<std::string> serv;
    std::vector<std::string> login;
    std::vector<std::string> password;
    
    const std::string secretKey = "XorKey90";
    const std::string filename = "vault.bin";

    
    std::string xorCipher(const std::string& data) {
        std::string result = data;
        for (size_t i = 0; i < data.size(); ++i) {
            result[i] = data[i] ^ secretKey[i % secretKey.size()];
        }
        return result;
    }

public:
    //here project load all your password
    //здесь подгружаются все твои пароли
    PasswordManager() {
        loadFromFile();
    }

    //that functional add new password 
    //эта функция добавляет новый пароль
    void add(const std::string& s, const std::string& l, const std::string& p) {
        serv.push_back(s);
        login.push_back(l);
        password.push_back(p);
        saveToFile();
        std::cout << "Успешно сохранено!" << std::endl;
    }

    //that functional delete password's
    //здесь функциия удаляет пароль
    void removeLast() {
        if (serv.empty()) {
            std::cout << "База данных пуста!" << std::endl;
            return;
        }
        serv.pop_back();
        login.pop_back();
        password.pop_back();
        saveToFile();
        std::cout << "Последняя запись удалена!" << std::endl;
    }

    //here we can output all youre password
    //здесь ты можешь увидеть все свои пароли
    void viewAll() {
        if (serv.empty()) {
            std::cout << "У вас нет сохраненных паролей." << std::endl;
            return;
        }
        std::cout << "\n--- СПИСОК ПАРОЛЕЙ ---" << std::endl;
        for (size_t i = 0; i < serv.size(); ++i) {
            std::cout << "[" << i + 1 << "] Сервис: " << serv[i] 
                      << " | Логин: " << login[i] 
                      << " | Пароль: " << password[i] << std::endl;
        }
        std::cout << "----------------------\n" << std::endl;
    }

    //that funnc save all you password
    //эта функция сохраняет все твои файлы
    void saveToFile() {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        size_t size = serv.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        for (size_t i = 0; i < size; ++i) {
            std::string encServ = xorCipher(serv[i]);
            std::string encLogin = xorCipher(login[i]);
            std::string encPass = xorCipher(password[i]);

            size_t sLen = encServ.size(), lLen = encLogin.size(), pLen = encPass.size();

            file.write(reinterpret_cast<const char*>(&sLen), sizeof(sLen));
            file.write(encServ.data(), sLen);
            file.write(reinterpret_cast<const char*>(&lLen), sizeof(lLen));
            file.write(encLogin.data(), lLen);
            file.write(reinterpret_cast<const char*>(&pLen), sizeof(pLen));
            file.write(encPass.data(), pLen);
        }
        file.close();
    }

    //that func load password at secret file
    //эта функция подгружает пароли
    void loadFromFile() {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) return;

        serv.clear(); login.clear(); password.clear();
        size_t size = 0;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        for (size_t i = 0; i < size; ++i) {
            size_t sLen = 0, lLen = 0, pLen = 0;

            file.read(reinterpret_cast<char*>(&sLen), sizeof(sLen));
            std::string encServ(sLen, ' '); file.read(&encServ[0], sLen);

            file.read(reinterpret_cast<char*>(&lLen), sizeof(lLen));
            std::string encLogin(lLen, ' '); file.read(&encLogin[0], lLen);

            file.read(reinterpret_cast<char*>(&pLen), sizeof(pLen));
            std::string encPass(pLen, ' '); file.read(&encPass[0], pLen);

            serv.push_back(xorCipher(encServ));
            login.push_back(xorCipher(encLogin));
            password.push_back(xorCipher(encPass));
        }
        file.close();
    }
};

int main() {
    PasswordManager manager;
    bool runtime = true;

    while (runtime) {
        int choice;

        std::cout << "Приветствую! я твой менеджер паролей ;)" << std::endl;
        std::cout << "[1] Добавить пароль" << std::endl;
        std::cout << "[2] Удалить пароль (последний)" << std::endl;
        std::cout << "[3] Посмотреть пароли" << std::endl;
        std::cout << "[0] Выход" << std::endl;

        std::cin >> choice;

        if (choice == 0) {
            std::cout << "Пока!" << std::endl;
            runtime = false;
            return 0;
        }

        if (choice == 1) {
            std::string s, l, p;
            std::cout << "Введите сервис: "; std::cin >> s;
            std::cout << "Введите логин: ";  std::cin >> l;
            std::cout << "Введите пароль: "; std::cin >> p;
            manager.add(s, l, p);
        }

        if (choice == 2) {
            manager.removeLast();
        }

        if (choice == 3) {
            manager.viewAll();
        }

        if (choice >= 4) {
            std::cout << "Такой опции нет! :(" << std::endl;
        }
    }
    return 0;
}
//i did it manager just for fun but if you rate my project i can did update! maybe i did AES-256 :)
//я сделал этот менеджер по фану но если вам понравится я смогу его обновлять! может быть я сделаю шифрование AES-256 :)
