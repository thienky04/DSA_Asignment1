#include "main.h"
enum CustomerType
{
	CTS,
	CL
};
class Ref_Customer
{
public:
	Restaurant::customer *refCustomer;
	CustomerType type;
	Ref_Customer *next;
	Ref_Customer *prev;
	Ref_Customer(){};
	~Ref_Customer(){};
	Ref_Customer(Restaurant::customer *cus, Ref_Customer *next = nullptr, Ref_Customer *prev = nullptr)
	{
		this->refCustomer = cus;
		this->next = next;
		this->prev = prev;
	};
};
class Ref_List
{
public:
	Ref_Customer *head;
	Ref_Customer *tail;
	int size;
	int totalEnergy;
	Ref_List() : head{nullptr}, tail{nullptr}, size(0), totalEnergy(0){};
	~Ref_List(){};
	void rotate_Ref(); // used for REVERSAL METHOD
	void push_back(Restaurant::customer *newCus)
	{
		Ref_Customer *newRef = new Ref_Customer(newCus);
		if (!head)
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
	};
	Restaurant::customer *remove_Reference(Ref_Customer *removeCus) // this method remove the Ref to the removeCus and return
	{
		if (!removeCus)
		{
			throw std::out_of_range("Segmentation fault at remove_Reference method!");
		}
		Restaurant::customer *cus = removeCus->refCustomer;
		if (removeCus == this->head)
		{
			if (size == 1)
			{
				this->head = this->tail = nullptr;
			}
			else
			{
				Ref_Customer *newHead = removeCus->next;
				if (newHead)
				{
					newHead->prev = nullptr;
					removeCus->next = nullptr;
					head = newHead;
				}
			}
			removeCus->refCustomer = nullptr;
			delete removeCus;
		}
		else if (removeCus == tail)
		{
			Ref_Customer *newTail = removeCus->prev;
			newTail->next = nullptr;
			removeCus->prev = nullptr;
			tail = newTail;
			removeCus->refCustomer = nullptr;
			delete removeCus;
		}
		else
		{
			Ref_Customer *left = removeCus->prev;
			Ref_Customer *right = removeCus->next;
			left->next = right;
			right->prev = left;
			removeCus->next = removeCus->prev = nullptr;
			delete removeCus;
		}
		size--;
		return cus;
	};
	Ref_Customer *refToCustomer(Restaurant::customer *cus) // return the reference to this customer (to delete...)
	{
		if (!cus)
		{
			throw std::out_of_range("Segmentation fault at refToCustomer method!");
		}
		Ref_Customer *ref = head;
		while (ref)
		{
			if (ref->refCustomer == cus)
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
	Restaurant::customer *head;
	Restaurant::customer *tail;
	void clear()
	{
		int tempSize = size;
		for (int i = 0; i < tempSize; i++)
		{
			Restaurant::customer *removed = this->head;
			this->remove_Out_Of_Queue(this->head);
			delete removed;
		}
	}
	int size;
	int CTS_Energy_Sum_W;
	int CL_Energy_Sum_W;
	// int max_Abs_Energy;
	// int index_Of_Last_Max;
	bool isFull()
	{
		return size >= MAXSIZE;
	};
	bool search_Name_Outside(const string &name)
	{
		if (size == 0)
			return false;
		Restaurant::customer *check = head;
		while (check)
		{
			if (check->name == name)
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
	void push_back(Restaurant::customer *newCus)
	{
		if (!head)
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
		this->size++;
		if (newCus->energy > 0)
		{
			this->CTS_Energy_Sum_W += newCus->energy;
		}
		else if (newCus->energy < 0)
		{
			this->CL_Energy_Sum_W -= newCus->energy;
		}
	};
	void remove_Out_Of_Queue(Restaurant::customer *removeCus)
	{
		if (removeCus == head)
		{
			if (this->size == 1)
			{
				this->size = 0;
				this->head = nullptr;
				this->tail = nullptr;
			}
			else
			{
				Restaurant::customer *newHead = head->next;
				newHead->prev = nullptr;
				head->next = nullptr;
				head = newHead;
			}
		}
		else if (removeCus == tail)
		{
			Restaurant::customer *newTail = tail->prev;
			newTail->next = nullptr;
			tail->prev = nullptr;
			tail = newTail;
		}
		else
		{
			Restaurant::customer *left = removeCus->prev;
			Restaurant::customer *right = removeCus->next;
			left->next = right;
			right->prev = left;
			removeCus->prev = removeCus->next = nullptr;
		}
		this->size--;
		if (removeCus->energy > 0)
		{
			this->CTS_Energy_Sum_W -= removeCus->energy;
		}
		else if (removeCus->energy < 0)
		{
			this->CL_Energy_Sum_W += removeCus->energy;
		}
	};
	Waiting_Queue() : head(nullptr), tail(nullptr), size(0), CTS_Energy_Sum_W(0), CL_Energy_Sum_W(0){};
	~Waiting_Queue()
	{
		this->clear();
	};
};
class imp_res : public Restaurant
{
public:
	int size;
	customer *lastChangedPlace;
	int CTS_energySum;
	int CL_energySum;
	Ref_List *timer_Queue;
	Waiting_Queue *waiting_Queue;
	imp_res() : size(0), lastChangedPlace(nullptr), CTS_energySum(0), CL_energySum(0)
	{
		timer_Queue = new Ref_List();
		waiting_Queue = new Waiting_Queue();
	};
	~imp_res()
	{
		this->BLUE(size);
		delete this->timer_Queue;
		delete this->waiting_Queue;
	};
	bool res_Is_Full() { return size >= MAXSIZE; };
	void insert_Second_Case(customer *newCus)
	{
		int Res = 0;
		int tempDiff = 0;
		Restaurant::customer *Y = this->lastChangedPlace;
		Restaurant::customer *iter = this->lastChangedPlace;
		for (int i = 0; i < size; i++)
		{
			tempDiff = abs(iter->energy - newCus->energy);
			if (tempDiff > Res)
			{
				Res = tempDiff;
				Y = iter;
			}
			iter = iter->next;
		}
		if (Y->energy >= newCus->energy)
		{
			this->insert_Right_Table(newCus, Y);
		}
		else
		{
			this->insert_Left_Table(newCus, Y);
		}
	}
	void insert_Left_Table(customer *newCus, customer *place)
	{
		Restaurant::customer *left = place->prev;
		left->next = newCus;
		newCus->prev = left;
		newCus->next = place;
		lastChangedPlace->prev = newCus;
		this->lastChangedPlace = newCus;
		this->size++;
		if (newCus->energy > 0)
		{
			this->CTS_energySum += newCus->energy;
		}
		else if (newCus->energy < 0)
		{
			this->CL_energySum -= newCus->energy;
		}
	}
	void insert_Right_Table(customer *newCus, customer *place)
	{
		Restaurant::customer *right = place->next;
		right->prev = newCus;
		newCus->next = right;
		newCus->prev = place;
		place->next = newCus;
		this->lastChangedPlace = newCus;
		this->size++;
		if (newCus->energy > 0)
		{
			this->CTS_energySum += newCus->energy;
		}
		else if (newCus->energy < 0)
		{
			this->CL_energySum -= newCus->energy;
		}
	}
	bool search_Name_Insise(const string &name)
	{
		if (this->size == 0)
			return false;
		Restaurant::customer *check = this->lastChangedPlace;
		for (int i = 0; i < size; i++)
		{
			if (check->name == name)
			{
				return true;
			}
			check = check->next;
		}
		return false;
	}
	void remove_Customer_Table(customer *removeCus) // if the last changed is remove
	{
		if (!removeCus)
		{
			throw std::out_of_range("Segmentation fault at remove_Customer_Table method!");
		}
		if (size == 1)
		{
			this->lastChangedPlace = nullptr;
			this->size--;
		}
		else
		{
			Restaurant::customer *left = removeCus->prev;
			Restaurant::customer *right = removeCus->next;
			left->next = right;
			right->prev = left;
			if (removeCus->energy > 0)
			{
				this->lastChangedPlace = right;
			}
			else
			{
				this->lastChangedPlace = left;
			}
			removeCus->next = nullptr;
			removeCus->prev = nullptr;
			size--;
		}
		if (removeCus->energy > 0)
		{
			this->CTS_energySum -= removeCus->energy;
		}
		else
		{
			this->CL_energySum += removeCus->energy;
		}
	}
	void RED(string name, int energy)
	{
		if (energy == 0 || (this->res_Is_Full() && this->waiting_Queue->isFull()) || this->search_Name_Insise(name) || this->waiting_Queue->search_Name_Outside(name))
		{
			return;
		}
		Restaurant::customer *newCus = new customer(name, energy, nullptr, nullptr);
		if (size == 0)
		{
			this->lastChangedPlace = newCus;
			this->size++;
			newCus->next = newCus->prev = newCus;
			this->timer_Queue->push_back(newCus);
			return;
		}
		else if (this->size < MAXSIZE / 2)
		{
			if (energy > this->lastChangedPlace->energy)
			{
				this->insert_Right_Table(newCus, this->lastChangedPlace);
			}
			else
			{
				this->insert_Left_Table(newCus, this->lastChangedPlace);
			}
			this->timer_Queue->push_back(newCus);
		}
		else if (this->size >= MAXSIZE / 2 && this->size < MAXSIZE)
		{
			this->insert_Second_Case(newCus);
			this->timer_Queue->push_back(newCus);
		}
		else if (this->size == MAXSIZE)
		{
			this->waiting_Queue->push_back(newCus);
		}
	}
	void BLUE(int num)
	{
		if (num <= size)
		{
			for (int i = 0; i < num; i++)
			{
				Restaurant::customer *removedCus = this->timer_Queue->remove_Reference(this->timer_Queue->head);
				this->remove_Customer_Table(removedCus);
				// delete removedCus;
			}
		}
		else if (num > size)
		{
			int firstSize = size;
			for (int i = 0; i < firstSize; i++)
			{
				Restaurant::customer *removedCus = this->timer_Queue->remove_Reference(this->timer_Queue->head);
				this->remove_Customer_Table(removedCus);
				delete removedCus;
			}
		}
	}
	void PURPLE(){

	};
	void REVERSAL(){};
	void UNLIMITED_VOID(){};
	void DOMAIN_EXPANSION()
	{
		// logicControl used to control the logic so that we dont need to duplicate the code for if else
		bool logicControl = this->CTS_energySum + this->waiting_Queue->CL_Energy_Sum_W < this->CL_energySum + this->waiting_Queue->CL_Energy_Sum_W;
		Ref_Customer *refCusIter = this->timer_Queue->head;
		Ref_Customer *tempRef = nullptr;
		Restaurant::customer *cusIter = nullptr;
		Restaurant::customer *tempCus = nullptr;
		int resSize = this->size;
		int waitSize = this->waiting_Queue->size;
		for (int i = 0; i < resSize; i++)
		{
			cusIter = refCusIter->refCustomer;
			tempRef = refCusIter->next;
			if ((cusIter->energy < 0) ^ logicControl)
			{
				this->timer_Queue->remove_Reference(refCusIter);
				cusIter->print();
				this->remove_Customer_Table(cusIter);
				delete cusIter;
				cusIter = nullptr;
			}
			refCusIter = tempRef;
		}
		cusIter = this->waiting_Queue->head;
		for (int i = 0; i < waitSize; i++)
		{
			tempCus = cusIter->next;
			if ((cusIter->energy < 0) ^ logicControl)
			{
				this->waiting_Queue->remove_Out_Of_Queue(cusIter);
				cusIter->print();
				delete cusIter;
			}
			cusIter = tempCus;
		}
	}
	void LIGHT(int num)
	{
		if (num == 0)
		{
			Restaurant::customer *cusIter = this->waiting_Queue->head;
			for (int i = 0; i < this->waiting_Queue->size; i++)
			{
				cusIter->print();
				cusIter = cusIter->next;
			}
		}
		else if (num > 0)
		{
			Restaurant::customer *cusIter = this->lastChangedPlace;
			for (int i = 0; i < this->size; i++)
			{
				cusIter->print();
				cusIter = cusIter->next;
			}
		}
		else if (num < 0)
		{
			Restaurant::customer *cusIter = this->lastChangedPlace;
			for (int i = 0; i < this->size; i++)
			{
				cusIter->print();
				cusIter = cusIter->prev;
			}
		}
	}
};