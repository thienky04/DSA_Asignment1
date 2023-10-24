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
	bool inTable;
	CustomerType type;
	int orderIndex;
	Ref_Customer *next;
	Ref_Customer *prev;
	Ref_Customer(){};
	~Ref_Customer(){};
	void setTableStatus(bool status)
	{
		this->inTable = status;
	}
	bool isInTable()
	{
		return this->inTable;
	}
	Ref_Customer(Restaurant::customer *cus, int order = 0, Ref_Customer *next = nullptr, Ref_Customer *prev = nullptr)
	{
		this->refCustomer = cus;
		this->next = next;
		this->prev = prev;
		this->inTable = false;
		this->orderIndex = order;
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
	~Ref_List(){};																		 // used for REVERSAL METHOD
	void push_back(Restaurant::customer *newCus, bool cus_status = false, int order = 0) // cus_status = true: customer is in table
	{
		Ref_Customer *newRef = new Ref_Customer(newCus,order);
		newRef->setTableStatus(cus_status);
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
		Ref_Customer *ref = tail;
		while (ref)
		{
			if (ref->refCustomer == cus)
				return ref;
			else
			{
				ref = ref->prev;
			}
		}
		throw std::out_of_range("Cannot find Customer");
	}
};
class Waiting_Queue : public Ref_List
{
public:
	int timerIndex;
	void print()
	{
		Ref_Customer *cusIter = head;
		for (int i = 0; i < size; i++)
		{
			cusIter->refCustomer->print();
			cusIter = cusIter->next;
		}
	}
	void clear()
	{
		// int tempSize = size;
		// for (int i = 0; i < tempSize; i++)
		// {
		// 	Restaurant::customer *removed = this->head;
		// 	this->remove_Out_Of_Queue(this->head);
		// 	delete removed;
		// }
	}
	int CTS_Energy_Sum_W;
	int CL_Energy_Sum_W;
	// int max_Abs_Energy;
	// int index_Of_Last_Max;
	bool isFull()
	{
		return size >= MAXSIZE;
	};

	void push_back_wQueue(Restaurant::customer *newCus)
	{
		this->push_back(newCus, timerIndex);
		timerIndex++;
		if (newCus->energy > 0)
		{
			this->CTS_Energy_Sum_W += newCus->energy;
		}
		else if (newCus->energy < 0)
		{
			this->CL_Energy_Sum_W -= newCus->energy;
		}
	};
	Restaurant::customer *remove_Out_Of_Queue(Ref_Customer *removeCus)
	{

		if (removeCus->refCustomer->energy > 0)
		{
			this->CTS_Energy_Sum_W -= removeCus->refCustomer->energy;
		}
		else if (removeCus->refCustomer->energy < 0)
		{
			this->CL_Energy_Sum_W += removeCus->refCustomer->energy;
		}
		return this->remove_Reference(removeCus);
	};
	void swap(Ref_Customer *&clock_wise, Ref_Customer *&anti_clock_wise, int interval)
	{
		if (clock_wise == head)
		{
			this->head = anti_clock_wise;
		}
		if(anti_clock_wise == tail)
		{
			this->tail = clock_wise;
		}
		Ref_Customer *left = clock_wise->prev;
		Ref_Customer *right = clock_wise->next;
		if (interval != 1)
		{

			clock_wise->next = anti_clock_wise->next;
			clock_wise->prev = anti_clock_wise->prev;
			if (anti_clock_wise->next)
			{
				anti_clock_wise->next->prev = clock_wise;
			}
			anti_clock_wise->prev->next = clock_wise;
			if (left)
			{
				left->next = anti_clock_wise;
			}
			anti_clock_wise->prev = left;
			anti_clock_wise->next = right;
			right->prev = anti_clock_wise;
		}
		else
		{
			clock_wise->next = anti_clock_wise->next;
			if (anti_clock_wise->next)
			{
				anti_clock_wise->next->prev = clock_wise;
			}
			if (clock_wise->prev)
			{
				clock_wise->prev->next = anti_clock_wise;
			}
			anti_clock_wise->prev = clock_wise->prev;
			clock_wise->prev = anti_clock_wise;
			anti_clock_wise->next = clock_wise;
		}

		left = clock_wise;
		clock_wise = anti_clock_wise;
		anti_clock_wise = left;
	}
	bool canSwap(Ref_Customer *leftIter, Ref_Customer *rightIter)
	{
		return (abs(leftIter->refCustomer->energy) < abs(rightIter->refCustomer->energy)) || ((abs(leftIter->refCustomer->energy) == abs(rightIter->refCustomer->energy)) && leftIter->orderIndex < rightIter->orderIndex);
	}
	int sortBack(Ref_Customer *changed, int interval)
	{
		Ref_Customer *right = changed;
		Ref_Customer *left = right;
		for (int i = 0; i < interval; i++)
		{

			if (!left)
			{
				return 0;
			}
			left = left->prev;
		}
		int countBack = 0;
		while (left)
		{
			if (canSwap(left, right))
			{
				swap(left, right, interval);
				// cout << "sort back with " << interval << endl;
				// this->print();
				countBack++;
			}
			right = left;
			for (int i = 0; i < interval; i++)
			{
				if (!left)
				{
					return countBack;
				}
				left = left->prev;
			}
		}
		return countBack;
	};
	int sortWithK(int interval, int length)
	{
		Ref_Customer *leftIter = this->head;
		Ref_Customer *rightIter = this->head;
		int count = 0;
		for (int i = 0; i < interval; i++)
		{
			rightIter = rightIter->next;
		}
		for (int i = 0; i < length - interval; i++)
		{
			if (canSwap(leftIter, rightIter))
			{
				swap(leftIter, rightIter, interval);
				count += 1;
				// cout << "sort with " << interval << endl;
				// this->print();
				count += sortBack(leftIter, interval);
				leftIter = rightIter;
				for (int i = 0; i < interval; i++)
				{
					if (!leftIter)
					{
						return 0;
					}
					leftIter = leftIter->prev;
				}
			}
			leftIter = leftIter->next;
			rightIter = rightIter->next;
		}
		return count;
	}
	int shellSort()
	{
		int length = findLastMaxEnergy() + 1;
		int interval = length / 2;
		int count = 0;
		while (interval)
		{
			count += this->sortWithK(interval, length);
			interval = interval / 2;
		}
		return count;
	}
	int findLastMaxEnergy()
	{
		Ref_Customer *iter = this->head;
		int max = 0;
		int index = 0;
		int orderIndex = 0;
		int tempEnergy = 0;
		for (int i = 0; i < this->size; i++)
		{
			tempEnergy = iter->refCustomer->energy;
			if (tempEnergy > max || (tempEnergy == max && iter->orderIndex > orderIndex))
			{
				max = tempEnergy;
				index = i;
				orderIndex = iter->orderIndex;
			}
			iter = iter->next;
		}
		return index;
	}
	Waiting_Queue() : CTS_Energy_Sum_W(0), CL_Energy_Sum_W(0), timerIndex(0){};
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
	void insert_anti_clock_wise_Case(customer *newCus)
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
	void queueToTable()
	{
		if (this->waiting_Queue->size == 0)
			return;
		else
		{
			while (this->size != MAXSIZE)
			{
				if (this->waiting_Queue->size == 0)
				{
					return;
				}
				else
				{
					this->pushIntoTable(this->waiting_Queue->remove_Out_Of_Queue(this->waiting_Queue->head)); // pop the customer out and then push into the table
				}
			}
		}
	}
	void pushIntoTable(Restaurant::customer *newCus)
	{
		if (size == 0)
		{
			this->lastChangedPlace = newCus;
			this->size++;
			newCus->next = newCus->prev = newCus;
			this->timer_Queue->push_back(newCus, true);
		}
		else if (this->size < MAXSIZE / 2)
		{
			if (newCus->energy > this->lastChangedPlace->energy)
			{
				this->insert_Right_Table(newCus, this->lastChangedPlace);
			}
			else
			{
				this->insert_Left_Table(newCus, this->lastChangedPlace);
			}
			this->timer_Queue->push_back(newCus, true);
		}
		else if (this->size >= MAXSIZE / 2 && this->size < MAXSIZE)
		{
			this->insert_anti_clock_wise_Case(newCus);
			this->timer_Queue->push_back(newCus, true);
		}
	}
	void RED(string name, int energy)
	{
		if (energy == 0 || (this->res_Is_Full() && this->waiting_Queue->isFull()) || this->search_Name_Insise(name))
		{
			return;
		}
		Restaurant::customer *newCus = new customer(name, energy, nullptr, nullptr);
		if (!this->res_Is_Full())
		{
			this->pushIntoTable(newCus);
		}
		else
		{
			this->timer_Queue->push_back(newCus, false);
			this->waiting_Queue->push_back_wQueue(newCus);
		}
	}
	void BLUE(int num)
	{
		int outNum = num < size ? num : size;
		Ref_Customer *Cus_iterator = this->timer_Queue->head;
		Ref_Customer *temp_iterator = nullptr;
		int count = 0;
		while (count != outNum)
		{
			temp_iterator = Cus_iterator->next;
			if (Cus_iterator->isInTable())
			{
				Restaurant::customer *removedCus = this->timer_Queue->remove_Reference(Cus_iterator);
				this->remove_Customer_Table(removedCus);
				delete removedCus;
				count++;
			}
			Cus_iterator = temp_iterator;
		}
		this->queueToTable();
	}
	void swapTwoCus(Restaurant::customer *&clock_wise, Restaurant::customer *&anti_clock_wise)
	{
		Restaurant::customer  *left = clock_wise->prev;
		Restaurant::customer  *right = clock_wise->next;
		if ( clock_wise->prev != anti_clock_wise)
		{
			clock_wise->next = anti_clock_wise->next;
			clock_wise->prev = anti_clock_wise->prev;
			if (anti_clock_wise->next)
			{
				anti_clock_wise->next->prev = clock_wise;
			}
			anti_clock_wise->prev->next = clock_wise;
			if (left)
			{
				left->next = anti_clock_wise;
			}
			anti_clock_wise->prev = left;
			anti_clock_wise->next = right;
			right->prev = anti_clock_wise;
		}
		else
		{
			clock_wise->next = anti_clock_wise->next;
			if (anti_clock_wise->next)
			{
				anti_clock_wise->next->prev = clock_wise;
			}
			if (clock_wise->prev)
			{
				clock_wise->prev->next = anti_clock_wise;
			}
			anti_clock_wise->prev = clock_wise->prev;
			clock_wise->prev = anti_clock_wise;
			anti_clock_wise->next = clock_wise;
		}
		left = clock_wise;
		clock_wise = anti_clock_wise;
		anti_clock_wise = left;
	}
	void PURPLE()
	{
		// cout<<"Before short: "<<endl;
		// this->LIGHT(0);
		int count = this->waiting_Queue->shellSort();
		// cout<<"After short: "<< count<<endl;
		// this->LIGHT(0);
		this->BLUE(count % MAXSIZE);
		// cout<<"After BLUE: "<<endl;
		// this->LIGHT(0);
	};
	void Reverse(bool logicControl, Restaurant::customer* origin)
	{
		
		Restaurant::customer* anti_clock = origin;
		Restaurant::customer* clock = origin->prev;
		while(anti_clock != clock)
		{
			if((anti_clock->energy > 0) ^ logicControl)
			{
				anti_clock = anti_clock->next;
				continue;
			}
			if((clock->energy > 0) ^ logicControl)
			{
				clock = clock->prev;
				continue;
			}
			swapTwoCus(anti_clock,clock);
			if(anti_clock->next == clock)
				break;
			anti_clock = anti_clock->next;
			clock = clock->prev;
		}
		
	}
	
	void REVERSAL(){	
		bool logicControl = this->lastChangedPlace->energy > 0;
		Restaurant::customer* origin = this->lastChangedPlace;
		Restaurant::customer* origin2 = this->lastChangedPlace;
		for(int i = 0;i<this->size;i++)
		{
			if(origin2->energy > 0 ^ logicControl)
			{
				break;
			}
			origin2 = origin2->next;
		}
		this->Reverse(logicControl, origin);
		this->Reverse(!logicControl,origin2);
		cout<<"After Reverse"<<endl;
	};
	void UNLIMITED_VOID(){
		//cout<<"Void"<<endl;
		if(this->size < 4) return;
		Restaurant::customer* leftIter = this->lastChangedPlace;
		Restaurant::customer* rightIter = this->lastChangedPlace;
		Restaurant::customer* iter  = this->lastChangedPlace;
		Restaurant::customer* leftResult = this->lastChangedPlace;
		Restaurant::customer* rightResult = this->lastChangedPlace;
		Restaurant::customer* leftCurrent = this->lastChangedPlace;
		int totalSum = 0;
		int currentSum = 0;
		int windowSum = 0;
		for(int i = 1;i<4;i++)
		{
			windowSum+= rightIter->energy;
			rightIter = rightIter->next;
		}
		windowSum+=rightIter->energy;
		totalSum = windowSum;
		rightResult = rightIter;
		for(int i = 0;i<this->size - 4;i++)
		{
			if(iter->energy < iter->energy + currentSum)
			{
				currentSum = iter->energy;
				leftCurrent = iter;
			}
			else
			{
				currentSum = iter->energy + currentSum;
			}
			windowSum = windowSum + rightIter->next->energy - leftIter->energy;
			leftIter = leftIter->next;
			rightIter = rightIter->next;
			if(windowSum <=totalSum)
			{
				totalSum = windowSum;
				leftResult = leftIter;
				rightResult = rightIter;
			}
			if(windowSum + currentSum <=totalSum)
			{
				totalSum = windowSum+currentSum;
				leftResult = leftCurrent;
				rightResult = rightIter;
			}
			iter=iter->next;
		}
		iter = leftResult;
		int minCus = leftResult->energy;
		int count = 1;
		Restaurant::customer* printIter = iter;
		while(iter != rightResult)
		{
			if(iter->energy<minCus)
			{
				minCus = iter->energy;
				printIter = iter;
			}
			count++;
			iter= iter->next;
		}
		
		if(rightResult->energy < minCus)
		{
			minCus = rightResult->energy;
			printIter = rightResult;
		}
		for(int i =0;i<count;i++)
		{
			printIter->print();
			if(printIter == rightResult)
			{
				printIter = leftResult;
			}
			else
			{
				printIter = printIter->next;
			}
		}
	};
	void DOMAIN_EXPANSION()
	{
		cout << "BD Duoi" << endl;
		// logicControl used to control the logic so that we dont need to duplicate the code for if else
		bool logicControl = this->CTS_energySum + this->waiting_Queue->CTS_Energy_Sum_W < this->CL_energySum + this->waiting_Queue->CL_Energy_Sum_W;
		Ref_Customer *refCusIter = this->timer_Queue->tail;
		Ref_Customer *tempRef = nullptr;
		Restaurant::customer *cusIter = nullptr;
		Restaurant::customer *tempCus = nullptr;
		bool tempStatus = true;
		int resSize = this->timer_Queue->size;
		for (int i = 0; i < resSize; i++)
		{
			cusIter = refCusIter->refCustomer;
			tempRef = refCusIter->prev;
			if ((cusIter->energy < 0) ^ logicControl)
			{
				tempStatus = refCusIter->isInTable();
				this->timer_Queue->remove_Reference(refCusIter);
				cusIter->print();
				if (tempStatus)
				{
					this->remove_Customer_Table(cusIter);
				}
				else
				{
					this->waiting_Queue->remove_Out_Of_Queue(this->waiting_Queue->refToCustomer(cusIter));
				}
				delete cusIter;
				cusIter = nullptr;
			}
			refCusIter = tempRef;
		}
		cout << "Duoi xong" << endl;
		this->queueToTable();
	}
	void LIGHT(int num)
	{
		if (num == 0)
		{
			Ref_Customer *cusIter = this->waiting_Queue->head;
			for (int i = 0; i < this->waiting_Queue->size; i++)
			{
				cusIter->refCustomer->print();
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