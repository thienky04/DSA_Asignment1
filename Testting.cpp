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
        node *newNode = new node(data);
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            this->tail->next = newNode;
            newNode->prev = tail;
            this->tail = newNode;
        }
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
                int temp = left->data;
                left->data = right->data;
                right->data = temp;
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
                int temp = leftIter->data;
                leftIter->data = rightIter->data;
                rightIter->data = temp;
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
};
int main()
{
    sortList *ssort = new sortList();
    ssort->Init(1000);
    cout<<"Swap time: "<<ssort->shellSort()<<endl;
}
