#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

//struct
struct DetailPesanan {
    string nama;
    int jumlah;
};

struct Pesanan {
    string namaPemesan;
    bool isKilat;
    vector<DetailPesanan> detailPesanan;
    string catatan;
    int totalHarga;
    string status;
};

// array
const int menuSize = 5;
string menuItems[menuSize] = {"Paha Atas + Nasi", "Dada Tulang + Nasi", "Paha Bawah + Nasi", "Dada Lembut + Nasi", "Sayap + Nasi"};
int menuPrices[menuSize] = {14000, 14000, 12000, 13000, 11000};
string menuCodes[menuSize] = {"PAN", "DTN", "PBN", "DLN", "SN"};


queue<Pesanan> regularQueue;
list<Pesanan> kilatQueue;

void displayMenu() {
    cout << "Menu (Take Away Only)" << endl;
    cout << "1. Membuat pesanan baru" << endl;
    cout << "2. Menampilkan seluruh pesanan" << endl;
    cout << "3. Selesai" << endl;
    cout << "Apa yang anda ingin lakukan: ";
}

void displayPesanan(const Pesanan& p) {
    cout << "Tipe Pesanan: " << (p.isKilat ? "Kilat" : "Reguler") << endl;
    cout << "Nama Pemesan: " << p.namaPemesan << endl;
    for (const auto& item : p.detailPesanan) {
        cout << item.nama << " (" << item.jumlah << ")" << endl;
    }
    cout << "Catatan: " << (p.catatan.empty() ? "-" : p.catatan) << endl;
    cout << "Total Pembayaran: " << p.totalHarga << endl;
    cout << "Status Pesanan: " << p.status << endl;
}

// create
void buatPesanan() {
    Pesanan p;
    cout << "Nama: ";
    cin >> p.namaPemesan;
    cout << "(biaya kilat: +30000)" << endl;
    cout << "Tipe Pesanan (r/k): ";
    char tipe;
    cin >> tipe;
    
    if (tipe == 'r') {
        p.isKilat = false;
        system("cls");
    } else if (tipe == 'k') {
        p.isKilat = true;
        system("cls");
    } else {
        cout << "pilihan tidak valid" << endl;
        return;
    }
    
    cout << "Menu:" << endl;
    for (int i = 0; i < menuSize; ++i) {
        cout << i+1 << ". " << menuItems[i] << " price: " << menuPrices[i] << " (" << menuCodes[i] << ")" << endl;
    }

    cout << "(d untuk selesai)" << endl;
    
    string input;
    while (true) {
        cout << "Nomor Pesanan, Jumlah: ";
        cin >> input;
        if (input == "d") {
            system("cls");
            break;
        } else if (input == "x") {
            cout << "pesanan dibatalkan" << endl;
            system("cls");
            return;
        } else {
            // delete
            if (input.find('-') != string::npos) {
                // Menghandle format nomorPesanan- untuk pengurangan jumlah
                int nomorPesanan = stoi(input.substr(0, input.find('-')));
                
                // Validasi nomor pesanan
                if (nomorPesanan < 1 || nomorPesanan > 5) {
                    cout << "Nomor pesanan tidak valid." << endl;
                    continue;
                }
    
                // Cari pesanan di daftar
                string namaPesanan = menuItems[nomorPesanan - 1];
                auto klt = find_if(p.detailPesanan.begin(), p.detailPesanan.end(), [&namaPesanan](const auto& detail) {
                    return detail.nama == namaPesanan;
                });
    
                if (klt != p.detailPesanan.end()) {
                    if (klt->jumlah > 1) {
                        klt->jumlah -= 1;
                    } else {
                        p.detailPesanan.erase(klt); // Hapus pesanan jika jumlahnya menjadi 0
                    }
                    // Tampilkan detail pesanan yang sudah diupdate
                    cout << "Pesanan saat ini:" << endl;
                    for (const auto& detail : p.detailPesanan) {
                        cout << detail.nama << " (" << detail.jumlah << ")" << endl;
                    }
                } else {
                    cout << "Pesanan yang dipilih tidak valid." << endl;
                }
            } else {
                // Menghandle format nomorPesanan,jumlah untuk penambahan jumlah
                int nomorPesanan = stoi(input.substr(0, input.find(',')));
                int jumlah = stoi(input.substr(input.find(',') + 1));
                
                // Validasi nomor pesanan
                if (nomorPesanan < 1 || nomorPesanan > 5) {
                    cout << "Nomor pesanan tidak valid." << endl;
                    continue;
                }
    
                // Tambah pesanan baru
                string namaPesanan = menuItems[nomorPesanan - 1];
                auto klt = find_if(p.detailPesanan.begin(), p.detailPesanan.end(), [&namaPesanan](const auto& detail) {
                    return detail.nama == namaPesanan;
                });
    
                if (klt != p.detailPesanan.end()) {
                    klt->jumlah += jumlah;
                } else {
                    p.detailPesanan.push_back({namaPesanan, jumlah});
                }
    
                // Tampilkan detail pesanan yang sudah diupdate
                cout << "Pesanan saat ini:" << endl;
                for (const auto& detail : p.detailPesanan) {
                    cout << detail.nama << " (" << detail.jumlah << ")" << endl;
                }
            }
        }
    }


    cout << "Catatan: ";
    cin.ignore();
    getline(cin, p.catatan);
    system("cls");
    
    p.totalHarga = 0;
    for (const auto& detail : p.detailPesanan) {
        int index = -1;
        for (int i = 0; i < menuSize; ++i) {
            if (menuItems[i] == detail.nama) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            p.totalHarga += detail.jumlah * menuPrices[index];
        }
    }


    if (p.isKilat) {
        p.totalHarga += 30000;
        p.status = "Sedang dibuat";
        kilatQueue.push_back(p);
    } else {
        p.status = "Sedang dibuat";
        regularQueue.push(p);
    }



    displayPesanan(p);
    cout << "Sudah dibayar (y): ";
    char sudahDibayar;
    cin >> sudahDibayar;
    if (sudahDibayar == 'y') {
        cout << "Terimakasih atas kerjasamanya" << endl;
        cout << "(x untuk Kembali): ";
        char kembali;
        cin >> kembali;
        system("cls");
        if (kembali == 'x') {
            return;
        }
    } else {
        cout << "tidak valid" << endl;
    }
}

// read
void tampilkanSeluruhPesanan() {
    if (regularQueue.empty() && kilatQueue.empty()) {
        cout << "belum ada pesanan. x untuk Kembali: ";
        char kembali;
        cin >> kembali;
        if (kembali == 'x') {
            system("cls");
            return;
        }
    } else {
        // sorting
        cout << "SELURUH PESANAN" << endl;
        int nomorAntrean = 1;
        vector<Pesanan> daftarPesanan;
        
        // auto&: memberikan perubahan tanpa perlu membuat salinan dari element itu sendiri
        // intinya elementnya akan berubah tanpa perlu repot kita tulis dari awal 
        for (auto& p : kilatQueue) {
            daftarPesanan.push_back(p);
            if (nomorAntrean == 1) {
                p.status = "Sedang dibuat";
            } else {
                p.status = to_string(nomorAntrean - 1) + " antrean lagi";
            }
            cout << nomorAntrean++ << ". " << p.namaPemesan << ": ";
            for (const auto& item : p.detailPesanan) {
                cout << item.nama << "(" << item.jumlah << "), ";
            }
            cout << "- " << p.status << endl;
        }


        queue<Pesanan> tempQueue = regularQueue;
        int totalKilat = kilatQueue.size();
        int totalAntrean = nomorAntrean;
        while (!tempQueue.empty()) {
            Pesanan p = tempQueue.front();
            tempQueue.pop();
            daftarPesanan.push_back(p);
            if (nomorAntrean == 1) {
                p.status = "Sedang dibuat";
            } else {
                p.status = to_string(nomorAntrean - 1) + " antrean lagi";
            }
            cout << nomorAntrean++ << ". " << p.namaPemesan << ": ";
            for (const auto& item : p.detailPesanan) {
                cout << item.nama << "(" << item.jumlah << "), ";
            }
            cout << "- " << p.status << endl;
        }

        
        cout << "(d untuk menyelesaikan pesanan)" << endl;
        cout << "(x untuk Kembali)" << endl;
        cout << "Apa yang ingin dilakukan: ";
        
        string action;
        cin >> action;
        system("cls");
        if (action == "d") {
            if (!kilatQueue.empty()) {
                kilatQueue.pop_front();
            } else if (!regularQueue.empty()) {
                regularQueue.pop();
            }
            cout << "Pesanan selesai." << endl;
        } else if (action == "x") {
            return;
        } else {
            // searching
            int nomorAntrean = stoi(action);
            if (nomorAntrean <= daftarPesanan.size()) {
                const Pesanan& pesanan = daftarPesanan[nomorAntrean - 1];
                

                // Tampilkan detail pesanan
                cout << "Detail Pesanan (Nomor Antrean " << nomorAntrean << "):" << endl;
                cout << "Tipe Pesanan: " << (pesanan.isKilat ? "Kilat" : "Reguler") << endl;
                cout << "Nama Pemesan: " << pesanan.namaPemesan << endl;
                
                // Tampilkan detail setiap item pesanan
                cout << "Pesanan: ";
                for (size_t i = 0; i < pesanan.detailPesanan.size(); ++i) {
                    const DetailPesanan& detail = pesanan.detailPesanan[i];
                    cout << detail.nama << " (" << detail.jumlah << ")";
                    if (i != pesanan.detailPesanan.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;

                cout << "Catatan: " << pesanan.catatan << endl;
                cout << "Status Pesanan: " << pesanan.status << endl;
                cout << endl;

                // Menampilkan opsi untuk update atau kembali
                cout << "Pilihan:" << endl;
                cout << "1. Update" << endl;
                cout << "(x untuk Kembali)" << endl;
                cout << "Apa yang hendak dilakukan: ";
                string updateAction;
                cin >> updateAction;
                system("cls");
                // update
                if (updateAction == "1") {
                    cout << "Catatan sebelumnya: " << pesanan.catatan << endl;
                    cout << "Masukkan catatan baru (hapus sebelumnya dengan '-'): ";
                    string newCatatan;
                    cin.ignore();
                    getline(cin, newCatatan);
                    if (newCatatan != "-") {
                        Pesanan& mutablePesanan = daftarPesanan[nomorAntrean - 1];
                        mutablePesanan.catatan = newCatatan;
                        // Update in the respective queue
                        if (mutablePesanan.isKilat) {
                            //auto: mendeklarasikan variabel secara otomatis saat menginputkan nilai
                            auto klt = find_if(kilatQueue.begin(), kilatQueue.end(),
                                              [&](const Pesanan& p) { return p.namaPemesan == mutablePesanan.namaPemesan; });
                            if (klt != kilatQueue.end()) klt->catatan = newCatatan;
                        } else {
                            queue<Pesanan> tempQueue;
                            while (!regularQueue.empty()) {
                                Pesanan p = regularQueue.front();
                                regularQueue.pop();
                                if (p.namaPemesan == mutablePesanan.namaPemesan) {
                                    p.catatan = newCatatan;
                                }
                                tempQueue.push(p);
                            }
                            regularQueue = tempQueue;
                        }
                    } else {
                        Pesanan& mutablePesanan = daftarPesanan[nomorAntrean - 1];
                        mutablePesanan.catatan = "";
                        // Update in the respective queue
                        if (mutablePesanan.isKilat) {
                            auto klt = find_if(kilatQueue.begin(), kilatQueue.end(),
                                              [&](const Pesanan& p) { return p.namaPemesan == mutablePesanan.namaPemesan; });
                            if (klt != kilatQueue.end()) klt->catatan = "";
                        } else {
                            queue<Pesanan> tempQueue;
                            while (!regularQueue.empty()) {
                                Pesanan p = regularQueue.front();
                                regularQueue.pop();
                                if (p.namaPemesan == mutablePesanan.namaPemesan) {
                                    p.catatan = "";
                                }
                                tempQueue.push(p);
                            }
                            regularQueue = tempQueue;
                        }
                    }
                    cout << "Catatan diperbarui." << endl;
                    // Tampilkan ulang detail pesanan yang sudah diperbarui
                    const Pesanan& updatedPesanan = daftarPesanan[nomorAntrean - 1];
                    cout << "Detail Pesanan (Nomor Antrean " << nomorAntrean << "):" << endl;
                    cout << "Tipe Pesanan: " << (updatedPesanan.isKilat ? "Kilat" : "Reguler") << endl;
                    cout << "Nama Pemesan: " << updatedPesanan.namaPemesan << endl;
                    cout << "Pesanan: ";
                    for (size_t i = 0; i < updatedPesanan.detailPesanan.size(); ++i) {
                        const DetailPesanan& detail = updatedPesanan.detailPesanan[i];
                        cout << detail.nama << " (" << detail.jumlah << ")";
                        if (i != updatedPesanan.detailPesanan.size() - 1) {
                            cout << ", ";
                        }
                    }
                    cout << endl;
                    cout << "Catatan: " << updatedPesanan.catatan << endl;
                    cout << "Status Pesanan: " << updatedPesanan.status << endl;
                    cout << endl;

                    tampilkanSeluruhPesanan();
                } else if (updateAction == "x") {
                    return;
                } else {
                    cout << "pilihan tidak valid" << endl;
                }
            } else {
                cout << "Nomor antrean tidak valid." << endl;
            }
        }
    }
}

int main() {
    while (true) {
        displayMenu();
        int choice;
        cin >> choice;
        if (choice == 1) {
            buatPesanan();
        } else if (choice == 2) {
            tampilkanSeluruhPesanan();
        } else if (choice == 3) {
            system("cls");
            cout << "Selesai.";
            break;
        }else {
            cout << "pilihan tidak valid" << endl;
        }
    }
    return 0;
}

// vector: jumlah datanya lebih dinamis daripada array (bisa nambah atau kurang suka2, kalau array manut deklarasi awal)
// stoi: mengubah data string ke integer
// getline: supaya program membaca seluruh data dari awal hingga akhir
// ignore: supaya karakter ENTER saat setelah menginputkan sesuatu tidak terbaca (untuk mengirimkan inputan biasanya kan kita tekan ENTER)
// auto: mendeklarasikan variabel secara otomatis saat menginputkan nilai
// auto&: memberikan perubahan tanpa perlu membuat salinan dari element itu sendiri. Intinya elementnya akan berubah tanpa perlu repot kita tulis dari awal 
// klt: digunakan untuk menyisipkan pesanan kilat pada posisi yang tepat dalam daftar pesanan.