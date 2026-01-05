#ifndef ANTRIAN_PRIORITAS_H
#define ANTRIAN_PRIORITAS_H

#include <iostream>
#include "pasien.h"
#include "data_manager.h"

using namespace std;

struct NodePrioritas {
    Pasien data;
    NodePrioritas* next;
};

class AntrianPrioritas {
private:
    NodePrioritas* rear;

public:
    AntrianPrioritas() {
        rear = NULL;
    }

    bool kosong() const {
        return rear == NULL;
    }

    // enqueue manual (update file)
    void enqueue(Pasien p) {
        NodePrioritas* baru = new NodePrioritas;
        baru->data = p;

        if (kosong()) {
            rear = baru;
            rear->next = rear;
        } else {
            NodePrioritas* bantu = rear->next;
            NodePrioritas* prev = rear;

            do {
                if (p.prioritas < bantu->data.prioritas)
                    break;
                prev = bantu;
                bantu = bantu->next;
            } while (bantu != rear->next);

            prev->next = baru;
            baru->next = bantu;

            if (prev == rear)
                rear = baru;
        }

        DataManager::ubahStatus(p.id, MENUNGGU);
        DataManager::ubahLayanan(p.id, "Antrian Prioritas");
    }

    // enqueue internal (UNTUK LOAD, TIDAK UPDATE FILE)
    void enqueueInternal(Pasien p) {
        NodePrioritas* baru = new NodePrioritas;
        baru->data = p;

        if (kosong()) {
            rear = baru;
            rear->next = rear;
        } else {
            NodePrioritas* bantu = rear->next;
            NodePrioritas* prev = rear;

            do {
                if (p.prioritas < bantu->data.prioritas)
                    break;
                prev = bantu;
                bantu = bantu->next;
            } while (bantu != rear->next);

            prev->next = baru;
            baru->next = bantu;

            if (prev == rear)
                rear = baru;
        }
    }

    void dequeue() {
        if (kosong()) {
            cout << "Antrian prioritas kosong\n";
            return;
        }

        NodePrioritas* front = rear->next;
        Pasien p = front->data;

        if (rear == front) {
            rear = NULL;
        } else {
            rear->next = front->next;
        }

        delete front;

        DataManager::ubahStatus(p.id, DIPERIKSA);
        DataManager::ubahLayanan(p.id, "Pemeriksaan");

        cout << "Pasien prioritas dipanggil\n";
    }

    void tampil() const {
        if (kosong()) {
            cout << "Antrian prioritas kosong\n";
            return;
        }

        NodePrioritas* bantu = rear->next;
        do {
            cout << bantu->data.id << " - "
                 << bantu->data.nama << " | Prioritas: "
                 << bantu->data.prioritas << endl;
            bantu = bantu->next;
        } while (bantu != rear->next);
    }

    void clear() {
        while (!kosong()) {
            NodePrioritas* front = rear->next;
            if (rear == front) {
                delete front;
                rear = NULL;
            } else {
                rear->next = front->next;
                delete front;
            }
        }
    }

    // load dari file TANPA update status / layanan
    void loadDariFile() {
        clear();

        Pasien* data;
        int n;
        DataManager::load(data, n);

        for (int i = 0; i < n; i++) {
            if (data[i].status == MENUNGGU && data[i].prioritas < 3) {
                enqueueInternal(data[i]);
            }
        }

        if (n > 0) delete[] data;
    }

    void menu() {
        int pilih;
        do {
            cout << "\n=== MENU ANTRIAN PRIORITAS ===\n";
            cout << "1. Muat Antrian Prioritas\n";
            cout << "2. Tampil Antrian\n";
            cout << "3. Panggil Pasien\n";
            cout << "0. Kembali\n";
            cout << "Pilih: ";
            cin >> pilih;

            switch (pilih) {
                case 1:
                    loadDariFile();
                    cout << "Antrian prioritas dimuat\n";
                    break;
                case 2:
                    tampil();
                    break;
                case 3:
                    dequeue();
                    break;
            }
        } while (pilih != 0);
    }
};

#endif
