#pragma once

#include "def.h"

struct Node {
public:
	Node(const std::string& msg) { _data = msg, _next = nullptr; }
	~Node() { }

	Node* Next() { return _next; }
	std::string Data() { return _data; }
	void setNext(Node* next) { _next = std::move(next); }
	friend std::ostream& operator<<(std::ostream& os, const Node& node) {
		os << node._data;
		return os; }

public:
	std::string _data;
	Node* _next = nullptr;
};

class MsgQueue {
public:
	MsgQueue(const int& maxLen=15);
	~MsgQueue();

	void Enqueue(std::string data);
	void Dequeue();

	bool isEmpty() { return _len == 0; }

	Node& operator[](const int& index);

	friend std::ostream& operator<<(std::ostream& os, const MsgQueue& log) {
		auto currNode = log._head;
		while (currNode != nullptr) {
			os << currNode << '\n';
			currNode = currNode->Next();
		}
		return os;
	}
private:
	int _maxLen, _len;
	Node* _head = nullptr;

};