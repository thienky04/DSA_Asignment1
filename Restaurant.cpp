#include "main.h"
enum CustomerType  {CTS,CL};
class Ref_Customer
{
public:
	Restaurant::customer** refCustomer;
	CustomerType type;
	Ref_Customer* next;
	Restaurant::customer* & operator*() { return *(this->refCustomer);};
	Ref_Customer(){};
	~Ref_Customer(){};
	Ref_Customer(Restaurant::customer** cus, Ref_Customer* next = nullptr):refCustomer{cus}{};
};
class Ref_List
{
public:
	Ref_Customer** head;
	Ref_Customer** tail;
	int size;
	int totalEnergy;
	Ref_List():head{nullptr},tail{nullptr},size(0),totalEnergy(0){};
	~Ref_List(){};
	void rotate_Ref(); // used for REVERSAL METHOD
	void push_back(Restaurant::customer* newCus);
	Restaurant::customer* & remove_Reference(Ref_Customer* removeCus );// this method remove the Ref to the removeCus and return 
	Ref_Customer*& refToCustomer(Restaurant::customer* cus);//return the reference to this customer (to delete...)
};
class Waiting_Queue
{
public:
	Restaurant::customer* head;
	Restaurant::customer* tail;
	int max_Abs_Energy;
	int index_Of_Last_Max;
	bool isFull();
	bool search_Name_Outside(const string& name);
	int shellShort();
	void push_back(Restaurant::customer* newCus);
	void remove_Customer(Restaurant::customer* removeCus);
	Waiting_Queue():head(nullptr),tail(nullptr),max_Abs_Energy(-1),index_Of_Last_Max(-1){};
	~Waiting_Queue(){};
};
class imp_res : public Restaurant
{
public:
	int size;
	customer* lastChangedPlace;
	int CTS_energySum;
	int CL_energySum;
	Ref_List* timer_Queue;
	Waiting_Queue* waiting_Queue;
	imp_res():size(0),lastChangedPlace(nullptr),CTS_energySum(0),CL_energySum(0){
		timer_Queue = new Ref_List();
		waiting_Queue = new Waiting_Queue();
	};
	~imp_res(){};
	bool res_Is_Full();
	void insert_Left(customer* newCus, customer* place);
	void insert_Right(customer* newCus, customer* place);
	bool search_Name_Insise(const string& name);
	void removeCustomer(customer* removeCus);
	void RED(string name, int energy);
	void BLUE(int num);
	void PURPLE();
	void REVERSAL();
	void UNLIMITED_VOID();
	void DOMAIN_EXPANSION();
	void LIGHT(int num);


	public:	
		imp_res() {

		};

		void RED(string name, int energy)
		{
			cout << name << " " << energy << endl;
			customer *cus = new customer (name, energy, nullptr, nullptr);
		}
		void BLUE(int num)
		{
			cout << "blue "<< num << endl;
		}
		void PURPLE()
		{
			cout << "purple"<< endl;
		}
		void REVERSAL()
		{
			cout << "reversal" << endl;
		}
		void UNLIMITED_VOID()
		{
			cout << "unlimited_void" << endl;
		}
		void DOMAIN_EXPANSION()
		{
			cout << "domain_expansion" << endl;
		}
		void LIGHT(int num)
		{
			cout << "light " << num << endl;
		}
};