#include <iostream>
using namespace std;
class node
{
public:
    node *next;
    node *prev;
    int data;
    node(int data)
    {
        this->data = data;
        this->prev = this->next = nullptr;
    }
};
class sortList
{
public:
    node *head;
    node *tail;
    int length;
    sortList()
    {
        this->head = this->tail = nullptr;
        this->length = 0;
    }
    void print()
    {
        node *iter = this->head;
        while (iter)
        {
            cout << iter->data << " ";
            iter = iter->next;
        }
        cout << endl;
    }
    void add(int data)
    {
        node *newnode = new node(data);
        if (!head)
        {
            head = tail = newnode;
        }
        else
        {
            this->tail->next = newnode;
            newnode->prev = tail;
            this->tail = newnode;
        }
        this->length++;
    }
    void Init(int k)
    {
        for (int i = 1; i <= k; i++)
        {
            this->add(i);
        }
        this->length = k;
    };
    int sortBack(node *changed, int interval)
    {
        node *right = changed;
        node *left = right;
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
            if (left->data < right->data)
            {
                std::swap(left, right);
                cout << "sort back with " << interval << endl;
                this->print();
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
    int sortWithK(int interval)
    {
        node *leftIter = this->head;
        node *rightIter = this->head;
        int count = 0;
        for (int i = 0; i < interval; i++)
        {
            rightIter = rightIter->next;
        }
        for (int i = 0; i < length - interval; i++)
        {
            if (leftIter->data < rightIter->data)
            {
                //std::swap(left, right);
                count += 1;
                cout << "sort with " << interval << endl;
                this->print();
                count += sortBack(leftIter, interval);
            }
            leftIter = leftIter->next;
            rightIter = rightIter->next;
        }
        return count;
    }
    int shellSort()
    {
        int interval = this->length / 2;
        int count = 0;
        while (interval)
        {
            count += this->sortWithK(interval);
            interval = interval / 2;
        }
        return count;
    }
    void UNLIMITED_VOID()
    {
        if (this->length < 4)
            return;
        node *leftIter = this->head;
        node *rightIter = this->head;
        node *iter = this->head;
        node *leftResult = this->head;
        node *rightResult = this->head;
        node *leftCurrent = this->head;
        int totalSum = 0;
        int currentSum = 0;
        int windowSum = leftIter->data;
        for (int i = 1; i < 4; i++)
        {
            windowSum += rightIter->data;
            rightIter = rightIter->next;
        }
        totalSum = windowSum;
        rightResult = rightIter;
        for (int i = 0; i < this->length - 4; i++)
        {
            if (iter->data > iter->data + currentSum)
            {
                currentSum = iter->data;
                leftCurrent = iter;
            }
            else
            {
                currentSum = iter->data + currentSum;
            }
            windowSum = windowSum + rightIter->next->data - leftIter->data;
            leftIter = leftIter->next;
            rightIter = rightIter->next;
            if (windowSum > totalSum)
            {
                totalSum = windowSum;
                leftResult = leftIter;
                rightResult = rightIter;
            }
            if (windowSum + currentSum > totalSum)
            {
                totalSum = windowSum + currentSum;
                leftResult = leftCurrent;
                rightResult = rightIter;
            }
            iter = iter->next;
        }
        iter = leftResult;
        cout<<"Done"<<" ";
        while(iter != rightResult)
        {
            cout<<iter->data<<" ";
            iter = iter->next;
        }
        cout<<rightResult->data<<endl;
    };
};

int main()
{
    sortList *ssort = new sortList();
    ssort->add(2);
    ssort->add(3);
    ssort->add(1);
    ssort->add(-7);
    ssort->add(6);
    ssort->add(-5);
    ssort->add(-4);
    ssort->add(4);
    ssort->add(3);
    ssort->add(3);
    ssort->add(2);
    ssort->add(-9);
    ssort->add(-5);
    ssort->add(6);
    ssort->add(4);
    ssort->print();
    ssort->UNLIMITED_VOID();
}
