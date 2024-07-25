#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex resource1, resource2;

void transaction1() {
    lock(resource1, resource2); // Bloqueia os dois recursos

    cout << "Transacao 1 adquiriu recurso 1 e 2." << endl;

    // Simula um processamento
    this_thread::sleep_for(chrono::milliseconds(50));

    cout << "Transacao 1 completa." << endl;
    cout << endl;
    resource1.unlock(); // Libera só o recurso 1
}

void transaction2() {
    this_thread::sleep_for(chrono::milliseconds(70)); // Leve delay para priorizar a transação 1

    if (resource1.try_lock()) {
        cout << "Transacao 2 adquiriu recurso 1." << endl;
        if (resource2.try_lock()) {
            resource2.lock();
            cout << "Transacao 2 adquiriu recurso 2." << endl;
        } else {
            cout << "Transacao 2 falhou ao adquirir recurso 2" << endl;
            cout << "Transacao 2 ferindo a transacao 1." << endl;
            resource2.unlock(); // Fere a transação 1
            resource2.lock();
            cout << "Transacao 2 adquiriu recurso 2." << endl;
            cout << "Transacao 2 completa." << endl;
        }
    } else {
        cout << "Transacao 2 falhou ao adquirir recurso 1." << endl;
    }
}

int main() {
    thread t1(transaction1);
    thread t2(transaction2);

    t1.join();
    t2.join();

    return 0;
}
