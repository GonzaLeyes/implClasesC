#include<iostream>;
#include<sstream>;
#include<vector>;

using namespace std;

enum class EGender
{
	MALE,FEMALE
};


class Account {
private:
	string number; //numero de cuenta
	double residue; //saldo en cuenta
	const static double minResidue; //Saldo minimo en la cuenta
	int withdrawals; //Contador de retiros
	int consignments; //Contador de consignaciones

	Customer* cliente;

	void setResidue(double residue);		//Establecer el saldo de la cuenta
	static string parseString(double valor);				//Concatener string con numeros. Se necesita librerir <sstream>

public:
	Account(Customer* cliente, string cuenta, double saldo); //Constructor(Cliente ,Numero de cuenta, saldo de cuenta)
	~Account() { cout << "Se elimino la cuenta " << this->number << endl; }; //A moodo de prueba, no se escribe cout en un destructor
	void setNumber(string cuenta);			//Establecer el numero de la cuenta
	string getNumber();						//Obtener el numero de la cuenta
	double getResidue();					//Obtener el saldo de la cuenta
	int getWithdrawals();					//Obtener el contador de retiros
	int getConsignments();					//Obtener el contador de consignaciones
	void consign(double);					//Actualiza el saldo de la cuenta con el valor a consignar
	bool withDraw(double);					//Obtiene el saldo a retirar y devuelve si se puede o no, 
											//se tiene en cuenta el valor a retirar, el saldo y el saldo minimo
	bool transfer(Account* a, double t);		//Devuelve si se puede transferir de una a otra(cuenta origne es la que llama
	string toString();						//Visualiza el valor de los atributos
	void setCustomer(Customer*);			//Setea el objeto customer creado como variable
	Customer* getCustomer();
};

const double Account::minResidue = 50000;


Account::Account(Customer* c, string cuenta, double saldo) {
	setCustomer(c);
	this->number = cuenta;
	this->residue = saldo;
	this->consignments = 0;
	this->withdrawals = 0;
}

void Account::setNumber(string n) {
	this->number = n;
}

void Account::setResidue(double r) {
	this->residue = r;
}

void Account::setCustomer(Customer* c) {
	Account::cliente=c;
}

Customer* Account::getCustomer() {
	return cliente;
}

string Account::getNumber() {
	return this->number;
}

double Account::getResidue() {
	return this->residue;
}

int Account::getWithdrawals() {
	return this->withdrawals;
}

int Account::getConsignments() {
	return this->consignments;
}

void Account::consign(double s) {
	setResidue(getResidue() + s);
	this->consignments++;
}

bool Account::withDraw(double s) {
	if (getResidue() - s >= minResidue)
	{
		setResidue(getResidue() - s);
		withdrawals++;
		return true;
	}
	return false;
}

bool Account::transfer(Account* a2, double transf) {
	if (this->withDraw(transf))
	{
		a2->consign(transf);
		return true;
	}
	return false;
}

string Account::parseString(double num) {
	ostringstream aux;
	aux << num;
	return aux.str();
}

string Account::toString() {
	return "Cuenta nro: " + this->number + "\n" + "Saldo: " + parseString(residue) + "\n" +
		"Ingresos: " + parseString(this->consignments) + "\n" + "Egresos: " + parseString(this->withdrawals) + "\n";
}

/*--------------------------------------Clase Customer(Cliente)-------------------------------*/

class Customer {
private:
	string idCustomer; //Id del cliente
	string firstName; //Primer Nombre del cliente
	string lastName;  //Apellido del cliente
	string phone; // Telefono del cliente

	EGender gender; //Sexo del cliente
	vector<Account*> accounts; // Se debe hacer el include de <vector> - es un vector de cuentas del cliente

public:
	Customer(string id, string nombre, string apellido, string phone, EGender gender);
	~Customer();
	void setIdCustomer(string id);
	void setFirstName(string nombre);
	void setLastName(string apellido);
	void setPhone(string phone);
	void setGender(EGender);
	string getIdCustomer();
	string getFirstName();
	string getLastName();
	string getPhone();
	EGender getGender();
	void addAccount(Account* cuenta); //Agrega una cuenta en el vector de cuentas del cliente
	vector<Account*> getAccounts(); //Devuelve el listado de cuentas del cliente, se debe llamar con un for each
	string toString();
};

Customer::Customer(string id, string nombre, string apellido, string phone, EGender gender) {
	this->idCustomer = id;
	this->firstName = nombre;
	this->lastName = apellido;
	this->phone = phone;
	this->gender = gender;
}

Customer::~Customer() {
	for (Account* account : accounts)
	{
		delete(account);
	}
}

void Customer::setIdCustomer(string id) {
	this->idCustomer = id;
}

void Customer::setFirstName(string nombre) {
	this->firstName = nombre;
}

void Customer::setLastName(string apellido) {
	this->lastName = apellido;
}

void Customer::setPhone(string phone) {
	this->phone = phone;
}

void Customer::setGender(EGender g) {
	this->gender = g;
}

string Customer::getIdCustomer()
{
	return this->idCustomer;
}

string Customer::getFirstName() {
	return this->firstName;
}

string Customer::getLastName() {
	return this->lastName;
}

string Customer::getPhone() {
	return this->phone;
}

EGender Customer::getGender() {
	return gender;
}


void Customer::addAccount(Account* cuenta) {
	accounts.push_back(cuenta); // Agrega el final del arreglo la cuenta recibida
}

vector<Account*> Customer::getAccounts() {
	return vector<Account*>(Customer::accounts); //Este es el metodo original
	//return accounts; //Servira asi??
}

string Customer::toString() {
	string sexo;
	if (this->getGender()==EGender::FEMALE)
	{
		sexo = "Femenino";
	}
	else
	{
		sexo = "Masculino";
	}

	return "Cliente nro: " + this->idCustomer + " Nombre: " + this->firstName + " Apellido: " +
		this->lastName + " Telefono: " + this->phone + " Sexo: " + sexo;
}



/*--------------------------------------Clase Banco-------------------------------*/


class ManagementBank{
private:

	vector<Account*> cuentas;		//Vector de todas las cuentas
	vector<Customer*> clientes;		//Vector de todos los clientes

public:

	ManagementBank() {};
	~ManagementBank() {};
	Customer* findCustomer(string cliente);						//Busca si el clinte existe y devuelve el puntero
	bool addCustomer(string id, string nombre, string apellido, string phone, EGender gender);
	Account* findAccount(string cuenta);
	bool addAccount(string cliente, string cuenta, double saldo);
	bool consign(string cuenta, double saldo);
	bool withDraw(string cuenta, double saldo);
	bool transfer(string cOrigen, string cDestino, double saldo);
	vector<Account*> getAccounts();
	vector<Customer*> getCustomers();

};

Customer* ManagementBank::findCustomer(string id) {
	for (Customer* customer : clientes)
	{
		if (customer->getIdCustomer().compare(id)==0)
		{
			return customer;;
		}
	}
	return NULL;
}

bool ManagementBank::addCustomer(string id, string nombre, string apellido, string phone, EGender gender) {
	if (!findCustomer(id))
	{
		clientes.push_back(new Customer(id, nombre, apellido, phone, gender));
		return true;
	}
	return false;
}

Account* ManagementBank::findAccount(string cuenta) {
	for (Account*cuent : cuentas)
	{
		if (cuent->getNumber().compare(cuenta)==0)
		{
			return cuent;
		}
	}
	return NULL;
}

bool ManagementBank::addAccount(string cliente, string cuenta, double saldo) {
	Customer* clienteAux = findCustomer(cliente);
	if (clienteAux)
	{
		if (!findAccount(cuenta))
		{
			Account* cuentaAux = new Account(clienteAux, cuenta, saldo);
			cuentas.push_back(cuentaAux);
			clienteAux->addAccount(cuentaAux);
			return true;
		}
		return false;
	}
	return false;
}

bool ManagementBank::consign(string cuenta, double saldo) {
	Account* cuentaAux= findAccount(cuenta);
	if (cuentaAux) {
		cuentaAux->consign(saldo);

		return true;
	}

	return false;
}

bool ManagementBank::withDraw(string cuenta, double saldo) {
	Account* cuentaAux = findAccount(cuenta);
	if (cuentaAux) {
		cuentaAux->withDraw(saldo);

		return true;
	}

	return false;
}

bool ManagementBank::transfer(string cuenta1, string cuenta2, double saldo) {
	Account* cuentaAux = findAccount(cuenta1);
	Account* cuentaAux2 = findAccount(cuenta2);
	if (cuentaAux && cuentaAux2) {
		return cuentaAux->transfer(cuentaAux2, saldo);

	}

	return false;
}

vector<Customer*> ManagementBank::getCustomers() {
	return vector<Customer*>(clientes);
}

vector<Account*> ManagementBank::getAccounts() {
	return vector<Account*>(cuentas);
}