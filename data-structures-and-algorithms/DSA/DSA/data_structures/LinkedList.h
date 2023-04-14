#pragma once
#include <memory>

using std::shared_ptr;
using std::make_shared;

struct ListNode
{
	shared_ptr<ListNode> next;
	float value;

	ListNode();
	ListNode(float val);
	ListNode(float val, shared_ptr<ListNode> next);
};

class LinkedList
{
public:
	std::shared_ptr<ListNode> head;
	std::shared_ptr<ListNode> tail;

private:
	int m_size;

public:
	LinkedList();
	LinkedList(float value);

	int size() const;
	void prepend(float value);
	void append(float value);
	void insert(float value, int index);
	void remove(int index);
	float pop();

	int find(float value) const;
};

