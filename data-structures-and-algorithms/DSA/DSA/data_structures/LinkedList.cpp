#include "LinkedList.h"

ListNode::ListNode()
{
	next = nullptr;
	value = 0;
}

ListNode::ListNode(float val)
{
	next = nullptr;
	value = val;
}

ListNode::ListNode(float val, shared_ptr<ListNode> next)
{
	this->next = next;
	value = val;
}

LinkedList::LinkedList()
{
	head = nullptr;
	tail = nullptr;
	m_size = 0;
}

LinkedList::LinkedList(float value)
{
	head = make_shared<ListNode>(new ListNode(value));
	tail = head;
	m_size = 1;
}

int LinkedList::size() const
{
	return m_size;
}

void LinkedList::prepend(float value)
{
	head = make_shared<ListNode>(new ListNode(value), head);
}

void LinkedList::append(float value)
{
	tail->next = make_shared<ListNode>(new ListNode(value));
	tail = tail->next;
}

void LinkedList::insert(float value, int index)
{
}

void LinkedList::remove(int index)
{
	// traverse through list until we hit index, then update pointers
}

float LinkedList::pop()
{
	return 0.0f;
}

int LinkedList::find(float value) const
{
	return 0;
}
