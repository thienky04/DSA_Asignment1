#include "main.h"
enum CustomerType  {CTS,CL};
class Ref_Customer
{
public:
	Restaurant::customer** refCustomer;
	CustomerType type;
	Ref_Customer* next;
	Ref_Customer *prev;
	Restaurant::customer* & operator*() { return *(this->refCustomer);};
	Ref_Customer(){};
	~Ref_Customer(){};
	Ref_Customer(Restaurant::customer* cus, Ref_Customer* next = nullptr,Ref_Customer* prev = nullptr){
	*refCustomer = cus;
	this->next = next;
	this->prev = prev;
	};
};
class Ref_List
{
public:
	Ref_Customer* head;
	Ref_Customer* tail;
	int size;
	int totalEnergy;
	Ref_List():head{nullptr},tail{nullptr},size(0),totalEnergy(0){};
	~Ref_List(){};
	void rotate_Ref(); // used for REVERSAL METHOD
	void push_back(Restaurant::customer* newCus)
	{
		Ref_Customer* newRef = new Ref_Customer(newCus);
		if(!head)
		{
			head = tail = newRef;
		}
		else
		{
			tail->next = newRef;
			newRef->prev = tail;
			tail = newRef;
		}
		size++;
		totalEnergy += newCus->energy;
	};
	Restaurant::customer* & remove_Reference(Ref_Customer* removeCus )// this method remove the Ref to the removeCus and return 
	{
		Restaurant::customer* cus = *(removeCus->refCustomer);
		if(removeCus==head)
		{
			Ref_Customer* newHead = removeCus->next;
			newHead->prev = nullptr;
			removeCus->next=nullptr;
			head = newHead;
			removeCus->refCustomer = nullptr;
			delete removeCus;
		}
		else if(removeCus == tail)
		{
			Ref_Customer* newTail = removeCus->prev;
			newTail->next = nullptr;
			removeCus->prev=nullptr;
			tail = newTail;
			removeCus->refCustomer = nullptr;
			delete removeCus;
		}
		else
		{
			Ref_Customer* left = removeCus->prev;
			Ref_Customer* right = removeCus->next;
			left->next = right;
			right->prev = left;
			removeCus->next = removeCus->prev = nullptr;
			delete removeCus;
		}
		size --;
		totalEnergy -= cus->energy;
		return cus;
	};
	Ref_Customer*& refToCustomer(Restaurant::customer* cus) //return the reference to this customer (to delete...)
	{
			Ref_Customer* ref = head;
			while(ref)
			{
				if(*(ref->refCustomer) == cus)
				return ref;
				else
				{
					ref = ref->next;
				}
			}
	}
};
class Waiting_Queue
{
public:
	Restaurant::customer* head;
	Restaurant::customer* tail;
	int size;
	// int max_Abs_Energy;
	// int index_Of_Last_Max;
	bool isFull(){
		return size >= MAXSIZE;
	};
	bool search_Name_Outside(const string& name)
	{
		Restaurant::customer* check = head;
		while(check)
		{
			if(check->name==name)
			{
				return true;
			}
			else
			{
				check = check->next;
			}
		} 
		return false;
	};
	int shellShort();
	void push_back(Restaurant::customer* newCus)
	{
		if(!head)
		{
			head = tail = newCus;
			// this->index_Of_Last_Max = 0;
			// this->max_Abs_Energy = abs(newCus->energy);
		}
		else
		{
			tail->next = newCus;
			newCus->prev = tail;
			tail = newCus;
			// if(abs(newCus->energy) >= this->max_Abs_Energy)
			// {
			// 	this->max_Abs_Energy = abs(newCus->energy);
			// 	this->index_Of_Last_Max = size;
			// }
		}
		size++;
		};
	void remove_Out_Of_Queue(Restaurant::customer* removeCus)
	{
		if(removeCus == head)
		{
			Restaurant::customer* newHead = head->next;
			newHead->prev= nullptr;
			head->next= nullptr;
			head = newHead;
		}
		else if(removeCus == tail)
		{
			Restaurant::customer* newTail = tail->prev;
			newTail->next = nullptr;
			tail->prev = nullptr;
			tail = newTail;
		}
		else
		{
			Restaurant::customer* left = removeCus->prev;
			Restaurant::customer* right = removeCus->next;
			left->next=right;
			right->prev=left;
			removeCus->prev = removeCus->next = nullptr;
		}
		this->size --;
	};
	Waiting_Queue():head(nullptr),tail(nullptr){};
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
	bool res_Is_Full(){return size>=MAXSIZE;};
	void insert_Left(customer* newCus, customer* place)
	{
		place->prev = newCus;
		this->lastChangedPlace = newCus;
	}
	void insert_Right(customer* newCus, customer* place)
	{
		place->next = newCus;
		this->lastChangedPlace = newCus;
	}
	bool search_Name_Insise(const string& name)
	{
		if(this->size == 0) return false;
		Restaurant::customer* check = this->lastChangedPlace;
		for(int i =0;i<size;i++)
		{
			if(check->name == name)
			{
				return true;
			}
			check = check->next;
		}
		return false;
	}
	void removeCustomer(customer* removeCus) // if the last changed is remove
	{
		if(size == 1){
			 
		}
		Restaurant::customer* left = removeCus->prev;
		Restaurant::customer* right = removeCus->next;
		size --;

	}
	void RED(string name, int energy);
	void BLUE(int num);
	void PURPLE();
	void REVERSAL();
	void UNLIMITED_VOID();
	void DOMAIN_EXPANSION();
	void LIGHT(int num);


// 	public:	
// 		imp_res() {

// 		};

// 		void RED(string name, int energy)
// 		{
// 			cout << name << " " << energy << endl;
// 			customer *cus = new customer (name, energy, nullptr, nullptr);
// 		}
// 		void BLUE(int num)
// 		{
// 			cout << "blue "<< num << endl;
// 		}
// 		void PURPLE()
// 		{
// 			cout << "purple"<< endl;
// 		}
// 		void REVERSAL()
// 		{
// 			cout << "reversal" << endl;
// 		}
// 		void UNLIMITED_VOID()
// 		{
// 			cout << "unlimited_void" << endl;
// 		}
// 		void DOMAIN_EXPANSION()
// 		{
// 			cout << "domain_expansion" << endl;
// 		}
// 		void LIGHT(int num)
// 		{
// 			cout << "light " << num << endl;
// 		}
	
 };