//TODO Adauga MongoDB in loc de .txt
//TODO Refactor all
//TODO Fix bug gestionare meniu (problema la schimbare pret)
//TODO Eventual GUI????

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>

#define MAX_CHAR 64

typedef struct {
    int zi, luna, an;
} Data;


//Clasa de baza ingredient, mostenim specificele de aici
class Ingredient {
protected:
    char* nume{};
    char* tara_provenienta{};
public:
    Ingredient() //Constructor fara paramteri
    {
        nume = new char[MAX_CHAR];
        tara_provenienta = new char[MAX_CHAR];
        strcpy(nume,"");
        strcpy(tara_provenienta, "");
    }

    explicit Ingredient(const char* nume)//Constructor cu parametrii
    {
        this->nume = new char[MAX_CHAR];
        strcpy(this->nume, nume);

        tara_provenienta = new char[MAX_CHAR];
        strcpy(tara_provenienta,"Romania");
    }

    Ingredient(const char *nume, const char *tara_provenienta)//Constructor cu parametrii
    {
        this->nume = new char[MAX_CHAR];
        this->tara_provenienta = new char[MAX_CHAR];

        strcpy(this->nume, nume);
        strcpy(this->tara_provenienta, tara_provenienta);
    }

    Ingredient(const Ingredient& i)//Constructor de copiere
    {
        nume = new char[MAX_CHAR];
        tara_provenienta = new char[MAX_CHAR];
        strcpy(nume, i.nume);
        strcpy(tara_provenienta, i.tara_provenienta);
    }

    virtual ~Ingredient()//Destructor
    {
        delete[] nume;
        delete[] tara_provenienta;
    }

    //Mai sus, constructori si destructori

    char* getNume() {return nume;}
    void setNume(const char* nume_nou) {strcpy(nume, nume_nou);}

    char* getTaraProvenienta() {return tara_provenienta;}
    void setTaraProvenienta(const char* tara_provenienta_noua) {strcpy(tara_provenienta,tara_provenienta_noua);}


    virtual void afisare()//Functie de afisare a ingredientului
    {
        std::cout << "Nume: " << nume << std::endl;
        std::cout << "Tara provenienta: " << tara_provenienta << std::endl;
    }

    //Returneaza randul de text de copiat in fisierul folosit ca baza de date
    virtual std::string textFisier()
    {
        return "INGREDIENT;" + std::string(nume) + ";" + std::string(tara_provenienta);
    }

    //Mai jos, overload la operatori, neterminati
    Ingredient& operator=(const Ingredient& i)
    {
        if(this != &i) {
            strcpy(nume, i.nume);
            strcpy(tara_provenienta, i.tara_provenienta);
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& ost, const Ingredient& i);
    friend std::istream& operator>>(std::istream& ist, Ingredient& i);

    //Operatorii neterminati:
    char& operator[](int index)
    {
        return nume[index];
    }

    Ingredient operator+(const Ingredient&)
    {
        return *this;
    }

    Ingredient& operator++()
    {
        return *this;
    }

    Ingredient operator++(int)
    {
        return *this;
    }

    virtual Ingredient operator!()
    {
        return *this;
    }

    virtual bool operator<(const Ingredient& i)//Functie virtuala, overload la operatorul <
    {
        if(strcmp(nume, i.nume) > 0)
            return true;
        else
            return false;
    }

    virtual bool operator==(const Ingredient& i) //Functie virtuala, overload la operatorul ==
    {
        if (strcmp(nume, i.nume) != 0 || strcmp(tara_provenienta, i.tara_provenienta) != 0)
            return false;
        else
            return true;
    }

};
//Operatorul de extragere
std::ostream& operator<<(std::ostream &ost, const Ingredient &i)
{
    ost << "Nume: " << i.nume << std::endl;
    ost << "Tara provenienta: " << i.tara_provenienta << std::endl;
    return ost;
}

//Operatorul de insertie
std::istream& operator>>(std::istream& ist, Ingredient& i)
{
    std::cout<<"Introduceti numele:";
    ist >> i.nume;
    std::cout <<"Introduceti tara provenienta:";
    ist >> i.tara_provenienta;
    return ist;
}


//Clase derivate din clasa Ingredient
class Lactata : public Ingredient
{
private:
    Data data_expirare{};
public:
    Lactata():Ingredient()
    {
        data_expirare = {0,0,0};
    }

    Lactata(const char* nume, const char* tara_provenienta, int zi, int luna, int an): Ingredient(nume, tara_provenienta)
    {
        data_expirare = {zi, luna, an};
    }

    Lactata(const char* nume, const char* tara_provenienta, const char* data): Ingredient(nume, tara_provenienta)
    {
        char* ptr;
        char* copie_data = new char[32];
        strcpy(copie_data, data);
        ptr = std::strtok(copie_data, "/");
        data_expirare.zi = std::stoi(ptr);
        ptr = strtok(nullptr, "/");
        data_expirare.luna = std::stoi(ptr);
        ptr=strtok(nullptr, "/");
        data_expirare.an = std::stoi(ptr);
        delete[] copie_data;
    }

    Lactata(const char* nume, const char* tara_provenienta, Data data): Ingredient(nume, tara_provenienta), data_expirare(data){}

    ~Lactata() override
    {
        delete[] nume;
        delete[] tara_provenienta;
    }

    Lactata(const Lactata& l): Ingredient(l)
    {
        data_expirare = l.data_expirare;
    }

    void afisare() override
    {
        std::cout << "Nume: " << nume << std::endl;
        std::cout << "Tara provenienta: " << tara_provenienta << std::endl;
        std::cout<< "Data expirare: " << data_expirare.zi << '/' << data_expirare.luna << '/' << data_expirare.an <<std::endl;
    }

    std::string textFisier() override
    {
        return "LACTATA;" + std::string(nume) + ';' + std::string(tara_provenienta) + ';' +
                std::to_string(data_expirare.zi) + '/' +
                std::to_string(data_expirare.luna) + '/' +
                std::to_string(data_expirare.an);
    }

    friend std::ostream& operator<<(std::ostream& ost, const Lactata& i);
    friend std::istream& operator>>(std::istream& ist, Lactata& i);
};

std::ostream &operator<<(std::ostream &ost, const Lactata &i) {
    ost << "Nume: " << i.nume << std::endl;
    ost << "Tara provenienta: " << i.tara_provenienta << std::endl;
    ost<< "Data expirare: " << i.data_expirare.zi << '/' << i.data_expirare.luna << '/' << i.data_expirare.an <<std::endl;
    return ost;
}

std::istream &operator>>(std::istream &ist, Lactata &i) {
    std::cout<<"Introduceti numele:";
    ist >> i.nume;
    std::cout <<"Introduceti tara provenienta:";
    ist >> i.tara_provenienta;
    std::cout <<"Introduceti data expirare: " <<std::endl;
    std::cout <<"Zi: ";
    ist >> i.data_expirare.zi;
    std::cout <<"Luna: ";
    ist >> i.data_expirare.luna;
    std::cout <<"An: ";
    ist >> i.data_expirare.an;
    return ist;
}

//La clasele derivate din ingrediente de mai jos, functiile functioneaza la fel ca cele
//Din clasa de baza Ingredient, modificate doar pentru a adauga specificele ingredientelor
class Cereala: public Ingredient
{
private:
    char* categorie;
public:
    Cereala(): Ingredient()
    {
        categorie = new char[MAX_CHAR];
        strcpy(categorie, "");
    }

    Cereala(const char* nume, const char* categorie):
            Ingredient(nume)
    {
        this->categorie = new char[MAX_CHAR];
        strcpy(this->categorie, categorie);
    }

    Cereala(const char* nume, const char* tara_provenienta, const char* categorie):
        Ingredient(nume, tara_provenienta)
    {
        this->categorie = new char[MAX_CHAR];
        strcpy(this->categorie, categorie);
    }

    Cereala(const Cereala& c):
        Ingredient(c)
    {
        this->categorie = new char[MAX_CHAR];
        strcpy(this->categorie, c.categorie);
    }

    ~Cereala() override
    {
        delete[] nume;
        delete[] tara_provenienta;
        delete[] categorie;
    }

    void afisare() override
    {
        std::cout << "Nume: " << nume << std::endl;
        std::cout << "Tara provenienta: " << tara_provenienta << std::endl;
        std::cout<< "Categorie: " << categorie << std::endl;
    }

    std::string textFisier() override
    {
        return "CEREALA;" + std::string(nume) + ";" + std::string(tara_provenienta) + ';' +
            std::string(categorie);
    }
};


//Clasa Depozit, cu un set care stocheaza perechi de tipul (Ingredient, numar_stoc)
class Depozit {
private:
    std::set<std::pair<Ingredient*, int>> ingrediente_depozitate; //Aici implementat SET
public:
    Depozit() = default;

    ~Depozit()
    {
        std::set<std::pair<Ingredient*, int>>::iterator it;
        for(it = ingrediente_depozitate.begin(); it != ingrediente_depozitate.end(); it++)
            delete[] it->first;
    }

    //Citire ingrediente din fisierul text depozit
    void citireDinFisier(const char *nume_fisier) {
        std::string linie;
        std::ifstream fisier_depozit;
        fisier_depozit.open(nume_fisier);
        if(fisier_depozit.is_open())
        {
            //Citim linie cu linie
            while(std::getline(fisier_depozit, linie))
            {
                //Impartim linia in cuvinte
                std::stringstream ss(linie);
                std::string cuvant;
                std::getline(ss, cuvant, ';');
                std::string nume, tara_provenienta, cantitate;
                //In functie de primul cuvant, identificam tipul de ingredient si facem clasa in functie de el
                //Apoi, adaugam ingredientul, alaturi de cantitate
                if(cuvant == "INGREDIENT")
                {
                    std::getline(ss, nume, ';');
                    std::getline(ss, tara_provenienta, ';');
                    std::getline(ss, cantitate, ';');
                    Ingredient* ingredient= new Ingredient(nume.c_str(), tara_provenienta.c_str());
                    adaugaIngredient(ingredient, stoi(cantitate));
                }
                else if(cuvant == "LACTATA")
                {
                    std::string data;
                    std::getline(ss, nume, ';');
                    std::getline(ss, tara_provenienta, ';');
                    std::getline(ss, data, ';');
                    std::getline(ss, cantitate, ';');
                    Lactata* lactata = new Lactata(nume.c_str(), tara_provenienta.c_str(), data.c_str());
                    adaugaIngredient(lactata, stoi(cantitate));
                }
                else if(cuvant == "CEREALA")
                {
                    std::string categorie;
                    std::getline(ss, nume, ';');
                    std::getline(ss, tara_provenienta, ';');
                    std::getline(ss, categorie, ';');
                    std::getline(ss, cantitate, ';');
                    Cereala* cereala = new Cereala(nume.c_str(), tara_provenienta.c_str(), categorie.c_str());
                    adaugaIngredient(cereala, stoi(cantitate));
                }

            }

        }
    }


    //Salvam ingredientele in fisier, in general la oprirea programului
    void salvareInFisier(const char *nume_fisier)
    {
        std::ofstream fisier_depozit(nume_fisier);
        if(fisier_depozit.is_open()) {
            std::set<std::pair<Ingredient*, int>>::iterator it;
            for (it = ingrediente_depozitate.begin(); it != ingrediente_depozitate.end(); it++)
            {
                //Apelam functia textFisier() din clasele de Ingredient
                //ca sa usuram munca aici
                fisier_depozit << it->first->textFisier();
                fisier_depozit << ";" << it->second << std::endl;
            }
            fisier_depozit.close();
        }
        else
            std::cout << "Eroare la deschiderea fisierului depozit!" << std::endl;
    }

    //Eroare, din nu stiu ce motiv
    void citireDinFisierBinar(const char* nume_fisier)
    {
        std::string linie;
        std::ifstream fisier_depozit;
        fisier_depozit.open(nume_fisier, std::ios::out | std::ios::binary);
        if(fisier_depozit.is_open())
        {
            while(std::getline(fisier_depozit, linie))
            {
                std::stringstream ss(linie);
                std::string cuvant;
                std::getline(ss, cuvant, ';');
                std::string nume, tara_provenienta, cantitate;
                if(cuvant == "INGREDIENT")
                {
                    std::getline(ss, nume, ';');
                    std::getline(ss, tara_provenienta, ';');
                    std::getline(ss, cantitate, ';');
                    Ingredient* ingredient= new Ingredient(nume.c_str(), tara_provenienta.c_str());
                    adaugaIngredient(ingredient, stoi(cantitate));
                }
                else if(cuvant == "LACTATA")
                {
                    std::string data;
                    std::getline(ss, nume, ';');
                    std::getline(ss, tara_provenienta, ';');
                    std::getline(ss, data, ';');
                    std::getline(ss, cantitate, ';');
                    Lactata* lactata = new Lactata(nume.c_str(), tara_provenienta.c_str(), data.c_str());
                    adaugaIngredient(lactata, stoi(cantitate));
                }
                else if(cuvant == "CEREALA")
                {
                    std::string categorie;
                    std::getline(ss, nume, ';');
                    std::getline(ss, tara_provenienta, ';');
                    std::getline(ss, categorie, ';');
                    std::getline(ss, cantitate, ';');
                    Cereala* cereala = new Cereala(nume.c_str(), tara_provenienta.c_str(), categorie.c_str());
                    adaugaIngredient(cereala, stoi(cantitate));
                }

            }

        }
    }


    //Asta parea sa mearga, nu am putut sa o probez bine fara aia de citire de mai sus
    void salvareInFisierBinar(const char* nume_fisier)
    {
        std::ofstream fisier_depozit(nume_fisier, std::ios::out | std::ios::binary);
        if(fisier_depozit.is_open()) {
            std::set<std::pair<Ingredient*, int>>::iterator it;
            for (it = ingrediente_depozitate.begin(); it != ingrediente_depozitate.end(); it++)
            {
               fisier_depozit.write(it->first->textFisier().c_str(), MAX_CHAR);
                fisier_depozit.write(";",MAX_CHAR);
                fisier_depozit.write(std::to_string(it->second).c_str(),MAX_CHAR);
                fisier_depozit.write(";",MAX_CHAR);
            }
            fisier_depozit.close();
        }
        else
            std::cout << "Eroare la deschiderea fisierului depozit!" << std::endl;
    }

    //Adauga perechea ingredient, cantitate in set-ul ingrediente_depozitate
    void adaugaIngredient(Ingredient* i, int cantitate)
    {
        ingrediente_depozitate.insert(std::make_pair(i, cantitate));
    }

    //Functie de afisare toate ingredientele din set
    void afiseazaIngredienteDepozitate()
    {
        std::set<std::pair<Ingredient*, int>>::iterator it;
        for(it = ingrediente_depozitate.begin(); it != ingrediente_depozitate.end(); it++)
        {
            it->first->afisare();
            std::cout<<"Cantitate: " << it->second << " kg" <<std::endl << std::endl;
        }
    }

    std::set<std::pair<Ingredient*, int>> getIngrediente() {return ingrediente_depozitate;}

};

//Clasa reteta, cu nume si o lista de pasi ai retetei
class Reteta
{
private:
    std::string nume;
    std::list<std::string> lista_pasi; //Aici implementata LIST
public:
    Reteta(std::string nume, std::list<std::string> pasi): nume(nume), lista_pasi(lista_pasi) {}

    void adaugaPas(std::string pas)
    {
        lista_pasi.push_back(pas);
    }

    void removePas(std::string pas)
    {
        if(!lista_pasi.empty())
            lista_pasi.remove(pas);
        else
            std::cout<<"Lista de pasi este goala!";
    }

    void afiseaza()
    {
        std::cout << nume << " : " << std::endl;
        int contor = 1;
        for(std::list<std::string>::iterator it = lista_pasi.begin(); it!=lista_pasi.end(); it++)
            std::cout << contor << ". " << (*it) <<std::endl;
    }
};

//Produs dedicat vanzarii
class Produs
{
protected:
    std::string nume;
    std::vector<Ingredient*> lista_ingrediente; //Aici implementat VECTOR
    int gramaj;
public:
    //Constructor
    Produs(std::string nume, std::vector<Ingredient*> lista_ing, int g):
        nume(nume), lista_ingrediente(lista_ing), gramaj(g) {};

    virtual void afisare() = 0; //Functia de afisare PUR VIRTUALA
    virtual void schimbaSpecific(std::string) = 0; //Functie PUR VIRTUALA

    std::string getNume(){return nume;}

    void adaugaIngredient(Ingredient* i)
    {
        if(std::find(lista_ingrediente.begin(), lista_ingrediente.end(), i) != lista_ingrediente.end())
            lista_ingrediente.push_back(i);
    }
    std::vector<Ingredient*> getListaIngrediente() {return lista_ingrediente;}
};

//Mai jos, clase derivate din produs
//Cu constructori, functii de afisare si schimbare specific
class ProdusSarat : public Produs
{
private:
    std::string condiment;
public:

    //Constructori
    ProdusSarat(): Produs("", {}, 0), condiment(""){}
    ProdusSarat(std::string nume, std::vector<Ingredient*> lista_ing, int g, std::string condiment):
        Produs(nume, lista_ing, g), condiment(condiment) {}

    void afisare()//Functia de afisare
    {
        std::cout << nume << " cu " << condiment << std::endl;
    }

    void schimbaSpecific(std::string condimentNou){condiment = condimentNou;}

    friend std::istream& operator>>(std::istream& ist, ProdusSarat& i);
};

std::istream& operator>>(std::istream& ist, ProdusSarat& i)
{
    std::cout<<"\nNume produs: ";
    ist>>i.nume;
    std::cout<<"Gramaj: ";
    ist>>i.gramaj;
    std::cout<<"Condiment: ";
    ist>>i.condiment;
    return ist;
}

class ProdusDulce : public Produs
{
private:
    std::string topping;
public:
    ProdusDulce(): Produs("", {}, 0), topping(""){}
    ProdusDulce(std::string nume, std::vector<Ingredient*> lista_ing, int g, std::string topping):
            Produs(nume, lista_ing, g), topping(topping) {}

    void afisare()
    {
        std::cout << nume << " cu " << topping << std::endl;
    }

    void schimbaSpecific(std::string toppingNou){topping = toppingNou;}
    friend std::istream& operator>>(std::istream& ist, ProdusDulce& i);
};

std::istream& operator>>(std::istream& ist, ProdusDulce& i)
{
    std::cout<<"\nNume produs: ";
    ist>>i.nume;
    std::cout<<"Gramaj: ";
    ist>>i.gramaj;
    std::cout<<"Topping: ";
    ist>>i.topping;
    return ist;
}


//Clasa meniu, care afiseaza produsele si pretul, salvate intr un vectori de perechi de tipul
// (Produs, pret), salvate in meniu
class Meniu
{
private:
    std::vector<std::pair<Produs*, int>> meniu;
public:
    void afiseazaMeniu()
    {
        int contor = 1;
        for (auto it:meniu)
        {
            std::cout << contor++ << ". ";
            it.first->afisare();
            std::cout << "Pret - " << it.second << " lei" <<std::endl;
        }
    }

    void afiseazaMeniuFaraPret()
    {
        for (auto it:meniu)
        {
            it.first->afisare();
        }
    }

    void adaugaProdus(Produs* p, int pret)
    {
        meniu.emplace_back(p, pret);
    }

    void schimbaPret(std::string nume, int pret)
    {
        for(auto it:meniu)
            if(it.first->getNume() == nume)
                it.second = pret;
    }
    std::vector<std::pair<Produs*, int>> getMeniu() {return meniu;}
};


//Clasa de statistici simple
class Statistici
{
private:
    Depozit* depozit;
    Meniu* meniu;
    int vanzari = 0;
public:
    Statistici(Depozit* d, Meniu* m): depozit(d), meniu(m) {}
    void adaugaVanzari(int nr = 1) { vanzari += nr;}
    int getNrVanzari() { return vanzari;}
    int nrIngrediente() {return depozit->getIngrediente().size();}
    int nrProduse() {return meniu->getMeniu().size();}


};


//De aici pleaca functionalitatea
//Avem acces la depozit, meniu si statistici, si le apelam
class Gestionare
{
private:
    Depozit* depozit;
    Meniu* meniu;
    Statistici* statistici;

    std::vector<Reteta*> lista_retete;
public:
    Gestionare(Depozit* d, Meniu* m, Statistici* s): depozit(d), meniu(m), statistici(s){}

    Meniu* getMeniu() {return meniu;}
    Depozit* getDepozit() {return depozit;}

    void adaugaProdus(Produs* p, int pret){meniu->adaugaProdus(p, pret);}

    void vanzareNoua(int nrProduse) {statistici->adaugaVanzari(nrProduse);}

    void schimbaPretProdus(std::string nume, int pret)
    {
        meniu->schimbaPret(nume, pret);
    }

    void adaugaIngredient(Ingredient* i, int cantitate)
    {
        depozit->adaugaIngredient(i, cantitate);
    }

    int getNrVanzari() {statistici->getNrVanzari();}
    int getNrIngrediente() {statistici->nrIngrediente();}
    int getNrProduse() {statistici->nrProduse();}
};


//Clasa de interfata text din consola
//Toate afisarile si citirile se fac aici
//Apeleaza functiile din clasa Gestionare, pentru functionalitate
//La inceputul programului, se intra ca si client sau administrator
//Clientul are acces la meniu
//Administratorul la orice tine de gestionare
class InterfataUser
{
private:
    Gestionare* g;
    int user; //0 - client, 1 - personal
public:

    //Tot afisajul in consola pleaca de la functia start (e ultima functie din clasa asta)
    //Te uiti ce functii se apeleaza din ea, ca sa urmaresti parcurgerea din consola
    //In functie de ce comanda scrii, se alege ce functie sa se apeleze mai departe
    //in switch-uri

    InterfataUser(Gestionare* g): user(0), g(g){}
    InterfataUser(Gestionare* g, int opt): g(g), user(opt) {}

    void startClient()
    {
        int opt = -1;
        while(opt != 0) {
            std::cout << "\n0. Iesire patiserie" << std::endl;
            g->getMeniu()->afiseazaMeniu();
            std::cout << "Alege comanda: ";
            std::cin >> opt;
            if (opt != 0) {
                int cantitate;
                std::cout << "Numar produse: ";
                std::cin >> cantitate;
                g->vanzareNoua(cantitate);
            }
        }

    }

    //Citim produsul si apelam functia de adaugare din gestionare
    void adaugaProdus()
    {
        int opt;
        do {
            std::cout << "\n1 - Dulce\n2 - Sarat\n";
            std::cout << "Optiune: ";
            std::cin >> opt;
        }while(opt!= 1 && opt != 2);
        if (opt == 1) {
            ProdusDulce *p = new ProdusDulce();
            std::cin >> *p;
            std::cout << "Pret: ";
            int pret;
            std::cin >> pret;
            g->adaugaProdus(p, pret);

        }else if (opt == 2)
        {
            ProdusSarat* p = new ProdusSarat();
            std::cin>>*p;
            std::cout<<"Pret: ";
            int pret;
            std::cin>>pret;
            g->adaugaProdus(p,pret);
        }
    }

    void schimbaPretProdus()
    {
        g->getMeniu()->afiseazaMeniuFaraPret();
        std::string nume;
        std::cout<<"Nume produs: ";
        std::cin>> nume;
        int pret;
        std::cout<<"Pret nou: ";
        std::cin>>pret;
        g->schimbaPretProdus(nume, pret);
    }

    //Functia de afisare a meniului
    void gestionareMeniu()
    {
        int opt = -1;
        while(opt != 0)
        {
            std::cout << "\n0. Iesire meniu" << std::endl;
            std::cout << "1. Adauga produs" << std::endl;
            std::cout << "2. Schimba pret produs" << std::endl;
            std::cout << "Optiune: ";
            std::cin>>opt;

            switch(opt){
                case 1: {
                    adaugaProdus(); break;
                }
                case 2: {
                    schimbaPretProdus(); break;
                }
                default:{}
            }
        }
    }

    void adaugaIngredient()
    {
        std::cout << "1 - Lactata\n";
        std::cout << "2 - Cereala\n";
        std::cout << "3 - Fruct\n";
        std::cout << "4 - Carne\n";

        int opt;
        int cantitate;
        std::cout<<"Optiune: ";
        std::cin >> opt;
        //Citim ingredientul si cantitatea, si il adaugam prin functia din Gestionare
        switch(opt)
        {
            case 1: {Lactata* l = new Lactata();
                    std::cin>>*l;
                    std::cout<<"Cantitate: ";
                    std::cin>>cantitate;
                    g->adaugaIngredient(l,cantitate);
                    break;};
            case 2:{Cereala* l = new Cereala();
                    std::cin>>*l;
                    std::cout<<"Cantitate: ";
                    std::cin>>cantitate;
                    g->adaugaIngredient(l,cantitate);
                    break;}
            case 3:{Ingredient* l = new Ingredient();
                    std::cin>>*l;
                    std::cout<<"Cantitate: ";
                    std::cin>>cantitate;
                    g->adaugaIngredient(l,cantitate);
                    break;}
            case 4:{Ingredient* l = new Ingredient();
                    std::cin>>*l;
                    std::cout<<"Cantitate: ";
                    std::cin>>cantitate;
                    g->adaugaIngredient(l,cantitate);
                    break;}
            default:{}

        }
    }

    //Meniu statistici
    void vizionareStatistici()
    {
        int opt;
        do {
            std::cout << "\n0 - Iesire meniu\n";
            std::cout << "1 - Numar vanzari\n";
            std::cout << "2 - Numar ingrediente\n";
            std::cout << "3 - Numar produse\n";
            std::cout << "Optiune: ";
            std::cin >> opt;
            switch(opt)
            {
                    case 1: {std::cout<< "Nr vanzari: " << g->getNrVanzari(); break;}
                    case 2: {std::cout<<"Nr ingrediente: " << g->getNrIngrediente(); break;}
                    case 3: {std::cout<<"Nr produse: "<<g->getNrProduse(); break;}
                    default: {}
            }
        }while(opt != 0);
    }

    void gestionareDepozit()
    {
        int opt = -1;
        while(opt != 0)
        {
            std::cout << "\n0. Iesire meniu" << std::endl;
            std::cout << "1. Adauga ingredient" << std::endl;
            std::cout << "2. Afisare ingrediente" << std::endl;
            std::cout << "Optiune: ";
            std::cin>>opt;

            switch(opt){
                case 1: {
                    adaugaIngredient(); break;
                }
                case 2: {
                    g->getDepozit()->afiseazaIngredienteDepozitate(); break;
                }
                default:{}
            }
        }
    }


    void startPersonal()
    {
        int opt = -1;
        while(opt != 0)
        {
            std::cout << "\n0. Iesire meniu" << std::endl;
            std::cout << "1. Gestionare meniu" << std::endl;
            std::cout << "2. Gestionare depozit" << std::endl;
            std::cout << "3. Statistici" << std::endl;
            std::cout << "Optiune: ";
            std::cin>>opt;

            switch(opt)
            {
                case 1: {gestionareMeniu(); break;}
                case 2: {gestionareDepozit(); break;}
                case 3: {vizionareStatistici(); break;}
                default:{}
            }

        }
    }

    //De aici incepe interfata grafica
    void start()
    {
        while(user == 0 || user == 1) {
            std::cout << "\nMod operare:\n0 - Client\n1 - Personal\n2 - Iesire program\n";
            std::cin >> user;
            if (user == 0)
                startClient();
            else
                startPersonal();
        }
    }
};


int main() {
    //Cream niste produse din burta, sa le adaugam
    ProdusDulce* pd1 = new ProdusDulce("Corn",{},50,"Ciocolata");
    ProdusDulce* pd2 = new ProdusDulce("Placinta",{},50,"Mar");
    ProdusSarat* ps1 = new ProdusSarat("Covridog",{},80,"Mustar");
    ProdusSarat* ps2 = new ProdusSarat("Covridog",{},80,"Ketchup");


    //Cream toate clasele de care avem nevoie
    Meniu* m = new Meniu();
    Depozit* d = new Depozit();
    Statistici* s = new Statistici(d, m);
    Gestionare* g = new Gestionare(d, m, s);

    InterfataUser* ui = new InterfataUser(g);



    //Adaugam produsele de mai sus
    m->adaugaProdus(pd1, 2);
    m->adaugaProdus(pd2, 2);
    m->adaugaProdus(ps1, 3);
    m->adaugaProdus(ps1, 3);

    //m->afiseazaMeniu();

    //Citim din fisierul text restul de produse
    d->citireDinFisier("../depozit.txt");


    //Apelam functia start din InterfataUser, de unde incepe si programul in consola propriu-zis
    ui->start();

    //La final, salvam in fisier modificarile, sa le avem si la urmatoarea rulare
    d->salvareInFisier("../depozit.txt");

    //d->afiseazaIngredienteDepozitate();



    return 0;
}

//Programul esti dedicat gestionarii unei patiserii.
//Prezinta doua modalitati de operare: client si personal
//Pe calea de client, se afiseara meniul disponibil si un cos de cumparaturi
//Pe calea de personal, se pot accesa urmatoarele:
//- Meniul, cu posibilitatea de modificare a acestuia
//- Statistici referitoare la patiserie
//- O lista de retete, cu posibilitatea de modificare a acestora
//- Stocurile de ingrediente din depozit

