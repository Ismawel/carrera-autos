#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>

std::mutex mtx;
bool carrera_terminada = false;

void carrera(int id, int distancia_total, int &lugar, int &distancia_avanzada) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> avance(1, 10);
    std::uniform_int_distribution<> tiempo(100, 500);

    while (distancia_avanzada < distancia_total) {
        int metros = avance(gen);
        distancia_avanzada += metros;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(tiempo(gen)));

        std::lock_guard<std::mutex> lock(mtx);
        if (distancia_avanzada >= distancia_total) {
            distancia_avanzada = distancia_total;
            lugar++;
            std::cout << "Auto" << id << " avanza " << metros 
                      << " metros y termina la carrera en el lugar " << lugar << "!" << std::endl;
        } else {
            std::cout << "Auto" << id << " avanza " << metros 
                      << " metros (total: " << distancia_avanzada << " metros)" << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia_total> <numero_de_autos>" << std::endl;
        return 1;
    }

    int distancia_total = std::stoi(argv[1]);
    int numero_de_autos = std::stoi(argv[2]);

    std::vector<std::thread> autos;
    std::vector<int> distancias(numero_de_autos, 0);
    int lugar = 0;

    for (int i = 0; i < numero_de_autos; ++i) {
        autos.push_back(std::thread(carrera, i, distancia_total, std::ref(lugar), std::ref(distancias[i])));
    }

    for (auto &auto_thread : autos) {
        auto_thread.join();
    }

    std::cout << "La carrera ha terminado!" << std::endl;
    return 0;
}
