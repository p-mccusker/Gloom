#include "MsgQueue.h"

MsgQueue::MsgQueue(const int& maxLen)
{
    _len = 0;
    _maxLen = maxLen;
}

MsgQueue::~MsgQueue()
{
    auto currNode = _head;
    auto lastNode = _head;
    while (currNode != nullptr) {
        lastNode = currNode;
        currNode = currNode->Next();
        delete lastNode;
        lastNode = nullptr;
    }
}

void MsgQueue::Enqueue(std::string data)
{
    if (isEmpty()) {
        _head = new Node(data);
        _len++;
    }
    else if (_len < _maxLen) {
        Node& lastNode = this->operator[](_len - 1);
        lastNode.setNext(new Node(data));
        _len++;
    }
    else {
        Dequeue();
        Enqueue(data);
    }
}

void MsgQueue::Dequeue()
{
    if (!isEmpty()) {
        auto oldHead = _head;
        _head = _head->Next();
        _len--;

        delete oldHead;
        oldHead = nullptr;
    } 
}

Node& MsgQueue::operator[](const int& index)
{
    auto currNode = _head;
    int cnt = 0;
    while (cnt < index) {
        currNode = currNode->Next();
        cnt++;
    }
    return *currNode;
}
