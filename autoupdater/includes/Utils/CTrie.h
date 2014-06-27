#if !defined _CTrie_include
#define _CTrie_include

#include <string.h>

template <typename T>
class CTrie
{
public:
	struct Node
	{
		T value;
		char key;
		int depth;
		bool set;
		struct Node * backwardNode;
		struct Node * traverseNode;
		struct Node * leftNode;
		struct Node * rightNode;
	};
private:
	struct Node * m_FirstNode;

	struct Node * getNewNode()
	{
		struct Node * m_Node = new Node;
		m_Node->set = 0;
		m_Node->backwardNode = 0;
		m_Node->traverseNode = 0;
		m_Node->leftNode = 0;
		m_Node->rightNode = 0;
		m_Node->depth = 0;
		m_Node->key = 0;
		return m_Node;
	}

	bool deleteNode(struct Node * node)
	{
		if(node == 0)
			return false;

		struct Node * m_Node = node;
		struct Node * m_TmpNode;
		
		do
		{
			m_TmpNode = m_Node;
			m_Node = m_Node->rightNode;
			if(m_TmpNode->traverseNode != 0)
				deleteNode(m_TmpNode->traverseNode);
		} while(m_Node != 0);

		delete node;
		return true;
	}

public:	
	CTrie()
	{
		m_FirstNode = getNewNode();	
	}

	// Define our classy operator[]
	T operator[](const char * key)
	{
		struct Node * m_Node = findNode(key);
		if(m_Node != 0 && m_Node->set)
			return m_Node->value;
		return 0;
	}

	// Insert a new element into the tree
	bool insertElement(const char * key, const T value)
	{
		if(key == 0)
			return false;
	
		struct Node * m_Node = findNode(key);

		if(m_Node != 0)
			m_Node->value = value;
		else
		{
			struct Node * m_NewNode = 0;
			
			m_Node = findClosestNode(key);
			if(m_Node == 0)
			{
				m_Node = m_FirstNode;
				if(m_Node->set == false && m_Node->traverseNode == 0 && m_Node->rightNode == 0 && m_Node->leftNode == 0)
					m_NewNode = m_Node;
			}
			else if(m_Node->traverseNode == 0)
			{
				m_Node->traverseNode = getNewNode();
				m_Node->traverseNode->depth = m_Node->depth+1;
				m_Node->traverseNode->backwardNode = m_Node;
				m_NewNode = m_Node->traverseNode;
			}
			else
			{
				m_Node = m_Node->traverseNode;
			}

			if(m_NewNode == 0)
			{
				while(m_Node->leftNode != 0 && m_Node->leftNode->key > key[m_Node->depth]) {m_Node = m_Node->leftNode;}
				while(m_Node->rightNode != 0 && m_Node->rightNode->key < key[m_Node->depth]) {m_Node = m_Node->rightNode;}
				

				m_NewNode = getNewNode();
				m_NewNode->depth = m_Node->depth;
				m_NewNode->backwardNode = m_Node->backwardNode;
				if(m_Node->key >= key[m_Node->depth])
				{
					m_NewNode->leftNode = m_Node->leftNode;
					m_NewNode->rightNode = m_Node;
					m_Node->leftNode = m_NewNode;
				}
				else
				{
					m_NewNode->rightNode = m_Node->rightNode;
					m_NewNode->leftNode = m_Node;
					m_Node->rightNode = m_NewNode;
				}

				int m_iNodes = 0;
				struct Node * m_TmpNode = m_Node;
				while(m_TmpNode->leftNode != 0){m_TmpNode = m_TmpNode->leftNode;};
				while(m_TmpNode->rightNode != 0){m_TmpNode = m_TmpNode->rightNode;++m_iNodes;};
				for(int i=0;i<m_iNodes/2;++i){m_TmpNode = m_TmpNode->leftNode;}
				if(m_Node->backwardNode != 0)
					m_TmpNode->backwardNode->traverseNode = m_TmpNode;
				else
					m_FirstNode = m_TmpNode;
			}

			int m_iLen = strlen(key);
			for(int i = m_NewNode->depth;i<m_iLen;++i)
			{
				m_NewNode->key = key[i];
				if(i != m_iLen-1)
				{
					m_NewNode->traverseNode = getNewNode();
					m_NewNode->traverseNode->depth = m_NewNode->depth + 1;
					m_NewNode->traverseNode->backwardNode = m_NewNode;
					m_NewNode = m_NewNode->traverseNode;
				}
			}

			m_NewNode->value = value;
			m_NewNode->set = true;
		}

		return true;
	}

	// Delete an element from the tree
	bool deleteElement(const char * key)
	{
		if(key == 0)
			return false;
		struct Node * m_Node = findNode(key);
		if(m_Node == 0)
			return false;

		struct Node * m_TmpNode;
		for(int i=strlen(key);i>0;--i)
		{
			if(m_Node->traverseNode == 0)
			{
				if(m_Node->leftNode != 0)
				{
					m_Node->leftNode->rightNode = m_Node->rightNode;
					if(m_Node->rightNode != 0)
						m_Node->rightNode->leftNode = m_Node->leftNode;
				}
				else if(m_Node->rightNode != 0)
				{
					m_Node->rightNode->leftNode = 0;
					m_Node->backwardNode->traverseNode = m_Node->rightNode;
				}

				m_TmpNode = m_Node;
				m_Node = m_Node->backwardNode;
				if(m_TmpNode->leftNode == 0)
					m_Node->traverseNode = 0;
				delete m_TmpNode;
			}
			else
			{
				m_Node->set = false;
				break;
			}
		}

		return true;
	}

	// Find a node in the tree
	struct Node * findNode(const char * key)
	{
		if(key == 0)
			return 0;

		int m_iIdx = 0;
		int m_iLen = strlen(key);

		struct Node * m_Node = m_FirstNode;

		do
		{
			if(m_Node->key == key[m_iIdx])
			{
				if(m_iIdx == m_iLen - 1)
					return m_Node;
				else
					m_Node = m_Node->traverseNode;
				++m_iIdx;
			}
			else
			{
				if(m_Node->key < key[m_Node->depth] && m_Node->rightNode != 0 && m_Node->rightNode->key > key[m_Node->depth])
					return 0;
				else if(m_Node->key < key[m_Node->depth])
					m_Node = m_Node->rightNode;
				else
					m_Node = m_Node->leftNode;
			}
		} while (m_Node != 0 && m_iIdx != m_iLen);

		return 0;
	}

	// Find the closest node in the tree
	struct Node * findClosestNode(const char * key)
	{
		if(key == 0)
			return 0;

		int m_iIdx = 0;
		int m_iLen = strlen(key);

		struct Node * m_Node = m_FirstNode;
		struct Node * m_Closest = 0;

		do
		{
			if(m_Node->key == key[m_iIdx])
			{
				m_Closest = m_Node;
				if(m_iIdx == m_iLen - 1)
					return m_Node;
				else
					m_Node = m_Node->traverseNode;
				++m_iIdx;
			}
			else
				if(m_Node->key < key[m_Node->depth] && m_Node->rightNode != 0 && m_Node->rightNode->key > key[m_Node->depth])
					return m_Node->backwardNode;
				else if(m_Node->key < key[m_Node->depth])
					m_Node = m_Node->rightNode;
				else
					m_Node = m_Node->leftNode;
		} while (m_Node != 0 && m_iIdx != m_iLen);

		return m_Closest;
	}

	// Iterate through the whole trie
	struct Node * iterateTrie(struct Node * node = 0)
	{
		struct Node * m_Node = node;
		if(node == 0)
		{
			m_Node = m_FirstNode;
			while(m_Node->leftNode != 0 || m_Node->traverseNode != 0){if(m_Node->leftNode == 0){m_Node = m_Node->traverseNode;} else {m_Node = m_Node->leftNode;}};
			return m_Node;
		}

		while(m_Node->backwardNode != 0 || m_Node->rightNode != 0)
		{
			if(m_Node->rightNode != 0)
			{
				m_Node = m_Node->rightNode;
				if(m_Node->traverseNode != 0)
				{
					m_Node = m_Node->traverseNode;
					while(m_Node->leftNode != 0 || m_Node->traverseNode != 0){if(m_Node->leftNode == 0){m_Node = m_Node->traverseNode;} else {m_Node = m_Node->leftNode;}};
					return m_Node;
				}
				else if(m_Node->set)
					return m_Node;
			}
			else if(m_Node->backwardNode != 0)
			{
				m_Node = m_Node->backwardNode;
				if(m_Node->set)
					return m_Node;
			}
		}

		return 0;
	}

	// Return the key of a specific node, return value must be freed
	void getNodeKey(struct Node * node = 0, char * buffer = 0, int maxlen = 0)
	{
		if(node == 0)
			return;
		struct Node * m_iNode = node;
		int m_iSkip = m_iNode->depth-maxlen;
		int written = 0;
		do
		{
			if (m_iSkip <= 0)
			{
				buffer[m_iNode->depth] = m_iNode->key;
				++written;
			}
			else
				--m_iSkip;
			m_iNode = m_iNode->backwardNode;
		} while(m_iNode != 0);
		buffer[written] = 0;
	}

	// Traverse node and delete all memory
	~CTrie()
	{
		deleteNode(m_FirstNode);
	}
};

#endif
