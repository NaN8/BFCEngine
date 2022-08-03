#ifndef __BFCUTILITY__H
#define __BFCUTILITY__H

#include <BFCBase.h>


// Implementation in header file because vc++ doesn't respect inline function in cpp file very well

//************************************************************************
//
//						BFCCopy
//
//************************************************************************


inline BFCVoid *BFCCopy(BFCVoid *dest,BFCVoid *src,size_t count)
{
	return memcpy(dest,src,count);
}


//************************************************************************
//
//						BFCVoidNode
//
//************************************************************************


class BFCVoidNode : public BFCShortBase
{
	friend class BFCVoidList;

protected:

	BFCVoid *ptr;
	BFCVoidNode *prev;
	BFCVoidNode *next;
	BFCInt MyID;		// With add methods node are inserted in order, respecting the MyID value
						// My Id = -1 means not inserted in the list. With multiple list MyID work only on the first list !!!
						// My Id doesn't work with insert method and with add method with put_begin = true!!!
	BFCBool ForceDelete;	// Force the delete of the node when removed even if Remove is used and not RemoveDelete

public:

	BFCVoidNode *GetPrev() { return prev; }
	BFCVoidNode *GetNext() { return next; }
	BFCVoid *GetPtr() { return ptr; }
	BFCVoid SetForceDelete(BFCBool val) { ForceDelete = val; }
	BFCVoid SetPrev(BFCVoidNode *p) { prev = p; }
	BFCVoid SetNext(BFCVoidNode *p) { next = p; }
	BFCVoid SetPtr(BFCVoid *p) { ptr = p; }
	BFCVoid DeletePtr() { if (ptr) delete ptr; ptr=NULL; }

	BFCVoidNode() : ptr(NULL), prev(0), next(0), MyID(-1), ForceDelete(false) { }
};


//************************************************************************
//
//						BFCVoidTreeNode
//
//************************************************************************


class BFCVoidTreeNode : public BFCShortBase
{
	friend class BFCVoidTreeList;
	BFCVoid *ptr;
	BFCVoidTreeNode *prev;
	BFCVoidTreeNode *next;
	BFCVoidTreeNode *child;
	BFCVoidTreeNode *parent;

public:

	BFCVoidTreeNode *GetPrev() { return prev; }
	BFCVoidTreeNode *GetNext() { return next; }
	BFCVoidTreeNode *GetChild() { return child; }
	BFCVoidTreeNode *GetParent() { return parent; }
	BFCVoid *GetPtr() { return ptr; }
	BFCVoid SetPrev(BFCVoidTreeNode *p) { prev = p; }
	BFCVoid SetNext(BFCVoidTreeNode *p) { next = p; }
	BFCVoid SetChild(BFCVoidTreeNode *p) { child = p; }
	BFCVoid SetParent(BFCVoidTreeNode *p) { parent = p; }
	BFCVoid SetPtr(BFCVoid *p) { ptr = p; }
	BFCVoid DeletePtr() { if (ptr) delete ptr; ptr=NULL; }

	BFCVoidTreeNode() : ptr(NULL), prev(0), next(0), child(0), parent(0) { }
};


//************************************************************************
//
//						BFCVoidList
//
//************************************************************************


class BFCVoidList : public BFCShortBase
{
	BFCBool check_exist;		// in certain cases to avoid going through to list twice, for internal code of the class only
	BFCVoidNode *head;
	BFCVoidNode *index;			// index for GetFirst and GetNext methods
	BFCVoidNode *index2;		// index2 for GetFirst2 and GetNext2 methods
	BFCUInt Number;				// number of node
	BFCBool RespectMyId;

public:

	// Constructor

	BFCVoidList() : head(NULL), index(NULL), index2(NULL), check_exist(true), Number(0), RespectMyId(true) { };

	// Destructor

	~BFCVoidList()
	{
		if (head != NULL) RemoveAll();
	}

	// method

	BFCVoid SetRespectMyId(BFCBool val) { RespectMyId = val; }

	BFCVoid CopyOfPtr(BFCVoidList &list)
	{
		RemoveAll();
		BFCVoid *node = list.GetFirstPtr();
		while (node)
		{
			Add(node);
			node = list.GetNextPtr();
		}
	}

	BFCUInt GetNumber() { return Number; }

	//--------------------------------------
	BFCVoid SetHead(BFCVoidNode *l)
	{
		// Root has always prev = NULL !
		l->prev = NULL;
		head = l;
	};

	BFCVoid SetHead(BFCVoid *v)
	{
		if (head==NULL) head = new BFCVoidNode;
		head->ptr = v;
	};
	//--------------------------------------
	const BFCVoidNode * GetHead() const
	{
		return head;
	};

	BFCVoid *GetVoid() const
	{
		return head->ptr;
	};
	//--------------------------------------
	BFCVoid *GetFirst()
	{
		index = head;
		return index;
	}

	BFCVoid *GetFirstPtr()
	{
		index = head;
		if (index)
		{
			return index->ptr;
		}
		else return NULL;
	}

	BFCVoid *GetFirstPtr2()
	{
		index2 = head;
		if (index2)
		{
			return index2->ptr;
		}
		else return NULL;
	}

	BFCVoid *GetNext()
	{
		if (!index) return NULL;
		index = index->next;
		return index;
	}

	BFCVoid *GetNextPtr()
	{
		if (!index) return NULL;
		index = index->next;
		if (index)
		{
			return index->ptr;
		}
		else return NULL;
	}

	BFCVoid *GetNextPtr2()
	{
		if (!index2) return NULL;
		index2 = index2->next;
		if (index2)
		{
			return index2->ptr;
		}
		else return NULL;
	}

	BFCVoid *GetPrev()
	{
		index = index->prev;
		return index;
	}
	BFCVoid *GetPrevPtr()
	{
		index = index->prev;
		if (index)
		{
			return index->ptr;
		}
		else return NULL;
	}

	BFCVoid *GetPrevPtr2()
	{
		index2 = index2->prev;
		if (index2)
		{
			return index2->ptr;
		}
		else return NULL;
	}

	// Don't forget MyId is not valid if put_begin = true
	//--------------------------------------
	BFCVoid Add(BFCVoidNode *v, BFCBool put_begin = false)
	{
		BFCBool no_my_id;       // no_my_id is set to true if the node exist in another list
		BFCInt find_id = 0;		// try to assign 0 first as ID
		Number++;
		if (v->MyID!=-1) no_my_id = true; else no_my_id = false;
		BFCVoidNode *node = head;
		// if no node exist in the list
		if (node == NULL)
		{
			SetHead(v);
			if (!no_my_id) v->MyID = find_id;
			return;
		}
		// if first node has not an ID = 0 or if put_begin = true
		if ( (node->MyID != find_id && RespectMyId) || put_begin)
		{
			node->prev = v;
			v->next = node;
			SetHead(v);
			if (!no_my_id) v->MyID = find_id;
			return;
		}

		// here head != NULL
		while (node->next!=NULL)	// while stop if end of list or a MyID number is free
		{
			find_id++;
			if (RespectMyId) if (node->next->MyID==find_id) break; //a MyID number is free
			node = node->next;
		}
		if (!no_my_id) v->MyID = find_id;
		// if not at the end of the list
		if (node->next)
		{
			node->next->prev = v;
			v->next = node->next;
		}

		node->next = v;
		v->prev = node;
	}
	BFCVoid Add(BFCVoid *v, BFCBool put_begin = false, BFCBool force_delete = false)
	{
		// Create new node
		BFCVoidNode *node = new BFCVoidNode();
		node->ptr = v;
		node->ForceDelete = force_delete;
		// Add it
		Add(node, put_begin);
	}
	//--------------------------------------------------
	// Insert a node after another one
	BFCVoid Insert(BFCVoidNode *node,BFCVoidNode *after)
	{
		BFCVoidNode *save = node->next;
		Number++;

		after->next = node;
		node->prev = after;
		node->next = save;
		if (save) save->prev = node;
	}

	BFCBool Insert(BFCVoid *ptr,BFCVoidNode *after)
	{
		BFCVoidNode *node = Find(ptr);
		BFCAssert(node,"Try to insert a null node");
		Insert(node,after);
	}

	BFCBool Insert(BFCVoidNode *node,BFCVoid *ptr)
	{
		BFCVoidNode *after = Find(ptr);
		BFCAssert(node,"Try to insert after a null node");
		Insert(node,after);
	}

	BFCBool Insert(BFCVoid *ptr,BFCVoid *after_ptr)
	{
		BFCVoidNode *node = Find(ptr);
		BFCVoidNode *after = Find(after_ptr);
		BFCAssert(node,"Try to insert a null node");
		BFCAssert(after_ptr,"Try to insert after a null node");
		Insert(node,after);
	}

	//--------------------------------------------------
	BFCBool Exist(BFCVoidNode *ptr) const
	{
		BFCVoidNode *node = head;
		while (node)
		{
			// is it this one?
			if (node == ptr)
			{
				return true;
			}
			node = node->next;
		}
		// nothing found
		return false;
	}

	// Find node with BFCVoid = ptr
	BFCVoidNode *Find(BFCVoid *ptr) const
	{
		BFCVoidNode *node = head;
		while (node)
		{
			// is it this one?
			if (node->ptr == ptr)
			{
				return node;
			}
			node = node->next;
		}
		// nothing found
		return NULL;
	}

	// Find node with BFCVoid = ptr and next or previous node = ptr2. Return the first of the two nodes
	BFCVoidNode *FindNear(BFCVoid *ptr,BFCVoid *ptr2) const
	{
		BFCVoidNode *node = head;
		while (node)
		{
			// is it this one?
			if (node->ptr == ptr)
			{
				if (node->next) if (node->next->ptr == ptr2) return node;
				if (node->prev) if (node->prev->ptr == ptr2) return node->prev;
			}
			node = node->next;
		}
		// nothing found
		return NULL;
	}

	// Find node number n in the list
	BFCVoidNode *Find(BFCUInt n) const
	{
		// set counter to 0
		BFCUInt c = 0;
		BFCVoidNode *node = head;
		while (node)
		{
			// is it this one?
			if (c == n)
			{
				return node;
			}
			node = node->next;
			// increment counter
			c++;
		}
		// nothing found
		return NULL;
	}

	// Find and return void pointer of node number n 
	BFCVoid *FindPtr(BFCUInt n) const
	{
		BFCVoidNode *node = Find(n);
		if (node) return node->GetPtr(); else return NULL;
	}

	//--------------------------------------------------
	// Remove (and delete if del = true !! ) v
	BFCBool Remove(BFCVoidNode *v,BFCBool del = true)
	{
		if (check_exist)
		if (!Exist(v))
		{
			BFCAssert(0,"Trying to remove a node not in the list");
			return false;
		}
		Number--;
		check_exist = true;
		// the first one ?
		if (!v->prev)
		{
			head = v->next;
			if (v->next) v->next->prev = NULL;
			// even if RemoveDelete is not used, node ptr can be deleted by force
			if (v->ForceDelete) v->DeletePtr();
			if (del) delete v;
			return true;
		}
		// exist a next one ?
		if (v->next)
		{
			v->prev->next = v->next;
			v->next->prev = v->prev;
		} else
		{
			// the last one
			v->prev->next = NULL;
		}
		// even if RemoveDelete is not used, node ptr can be deleted by force
		if (v->ForceDelete) v->DeletePtr();
		if (del) delete v;
		return true;
	}


	// Remove (and delete !! ) node with BFCVoid = ptr

	BFCBool Remove(BFCVoid *ptr)
	{
		BFCVoidNode *node = Find(ptr);
		if (node)
		{
			check_exist = false;
			Remove(node);
			return true;
		} else
		{
			return false;
		}
	}

	// Remove (and delete !! ) the 2 nodes with BFCVoid = ptr and with previous or next one = ptr2

	BFCBool RemoveNear(BFCVoid *ptr,BFCVoid *ptr2)
	{
		BFCVoidNode *node = FindNear(ptr,ptr2);
		if (node)
		{
			check_exist = false;
			// Remove next one first
			Remove(node->next);
			Remove(node);
			return true;
		} else
		{
			return false;
		}
	}

	// Remove (and delete !! ) node number n

	BFCBool Remove(BFCUInt n)
	{
		BFCVoidNode *node = Find(n);
		if (node)
		{
			check_exist = false;
			Remove(node);
			return true;
		} else
		{
			return false;
		}
	}

	// Remove (and delete !! ) v and destroy internal pointer
	BFCBool RemoveDelete(BFCVoidNode *v)
	{
		if (check_exist)
		if (!Exist(v))
		{
			BFCAssert(0,"Trying to remove a node not in the list");
			return false;
		}
		Number--;
		check_exist = true;
		// the first one ?
		if (!v->prev)
		{
			head = v->next;
			head->prev = NULL;
			v->DeletePtr();
			delete v;
			return true;
		}
		// exist a next one ?
		if (v->next)
		{
			v->prev->next = v->next;
			v->next->prev = v->prev;
		} else
		{
			// the last one
			v->prev->next = NULL;
		}
		v->DeletePtr();
		delete v;
		return true;
	}

	// Remove node with BFCVoid = ptr and destroy internal pointer

	BFCBool RemoveDelete(BFCVoid *ptr)
	{
		BFCVoidNode *node = Find(ptr);
		if (node)
		{
			check_exist = false;
			RemoveDelete(node);
			return true;
		} else
		{
			return false;
		}
	}

	// Remove node number n and destroy internal pointer

	BFCBool RemoveDelete(BFCUInt n)
	{
		BFCVoidNode *node = Find(n);
		if (node)
		{
			check_exist = false;
			RemoveDelete(node);
			return true;
		} else
		{
			return false;
		}
	}

	// Remove all

	BFCVoid RemoveAll()
	{
		BFCVoidNode *node = head;
		BFCVoidNode *save = head;
		head = NULL;
		while (node)
		{
			save = node;
			node = node->next;
			delete save;
		}
		Number = 0;
	}

	// Remove all and destroy internal pointers

	BFCVoid RemoveDeleteAll()
	{
		BFCVoidNode *node = head;
		BFCVoidNode *save = head;
		head = NULL;
		while (node)
		{
			save = node;
			node = node->next;
			save->DeletePtr();
			delete save;
		}
		Number = 0;
	}

};


//************************************************************************
//
//						BFCVoidTreeList
//
//************************************************************************


class BFCVoidTreeList : public BFCShortBase
{

	BFCVoidTreeNode *root;
	BFCVoidTreeNode *index;			// index for GetFirst GetNext GetChild GetParent methods

public:

	// Constructor

	BFCVoidTreeList() : root(NULL), index(NULL) { };

	// method

	//--------------------------------------
	BFCVoid SetRoot(BFCVoidTreeNode *l)
	{
		// Root has always prev = NULL and Parent = NULL !
		l->prev = NULL;
		l->parent = NULL;
		root = l;
	};

	BFCVoid SetHead(BFCVoid *v)
	{
		if (root==NULL) root = new BFCVoidTreeNode;
		root->ptr = v;
	};
	//--------------------------------------
	const BFCVoidTreeNode * GetRoot() const
	{
		return root;
	};

	BFCVoid *GetVoid() const
	{
		return root->ptr;
	};
	//--------------------------------------
	BFCVoidTreeNode *GetFirst()
	{
		index = root;
		return index;
	}

	BFCVoid *GetFirstPtr()
	{
		index = root;
		if (index)
		{
			return index->ptr;
		}
		else return NULL;
	}

	BFCVoidTreeNode *GetNext()
	{
		index = index->next;
		return index;
	}

	BFCVoid *GetNextPtr()
	{
		index = index->next;
		if (index)
		{
			return index->ptr;
		}
		else return NULL;
	}

	BFCVoid *GetPrevPtr()
	{
		index = index->prev;
		if (index)
		{
			return index->ptr;
		}
		else return NULL;
	}

	BFCVoidTreeNode *GetPrev()
	{
		index = index->prev;
		return index;
	}

	//--------------------------------------
	BFCVoid Add(BFCVoid *parent,BFCVoid *v)
	{
		BFCVoidTreeNode *new_node;
		// Check if parent exist
		BFCVoidTreeNode *found = Find(parent);
		if (found)
		{
			new_node = new BFCVoidTreeNode;
			new_node->ptr = v;
			Add(found, new_node);
		} else BFCAssert(0,"Try to insert a tree node with a parent inexistant");
	}
	//--------------------------------------
	BFCVoid Add(BFCVoidTreeNode *parent,BFCVoidTreeNode *v)
	{
		BFCVoidTreeNode *node = Find(v);
		if (node) BFCAssert(0,"Node already exist in the tree");
		// if adding root
		if (parent == NULL)
		{	
			if (root)
			{
				node = root;
				while (node->next)
				{
					node = node->next;
				}
				node->next = v;
				v->prev = node;
				v->child = node->child;
				v->parent = node->parent;
			} else
			{
				root = v;
			}
			return;
		}
		if (parent->child)
		{
			parent = parent->child;
			while (parent->next) parent = parent->next;
			parent->next = v;
			v->prev = parent;
			v->child = parent->child;
			v->parent = parent->parent;
		} else
		{
			while (parent->next) parent = parent->next;
			parent->next = v;
			v->prev = parent;
			v->child = parent->child;
			v->parent = parent->parent;
		}
	}
	//--------------------------------------------------
	// Find node with BFCVoid = ptr
	BFCVoidTreeNode *Find(BFCVoid *ptr)
	{
		BFCVoidTreeNode *tmptree=root;
		if (tmptree==NULL) return NULL;
		while (1) 
		{
			if (tmptree->GetPtr() == ptr) return tmptree;
			if (tmptree->child) tmptree = tmptree->child; else 
			{
				if (tmptree->next) tmptree = tmptree->next; else
				{
					if  (tmptree->parent)
					{
						while (!tmptree->parent->next)
						{
							if (tmptree->parent->parent)
							{
								tmptree = tmptree->parent;
							} else
							{
								return NULL;
							}
						}
						tmptree = tmptree->parent->next;
					}
					else return NULL;
				}
			}
		}
		return NULL;
	}

	//--------------------------------------------------
	// Find node
	BFCVoidTreeNode *Find(BFCVoidTreeNode *node)
	{
		BFCVoidTreeNode *tmptree=root;
		if (tmptree==NULL) return NULL;
		while (1) 
		{
			if (tmptree == node) return tmptree;
			if (tmptree->child) tmptree = tmptree->child; else 
			{
				if (tmptree->next) tmptree = tmptree->next; else
				{
					if  (tmptree->parent)
					{
						while (!tmptree->parent->next)
						{
							if (tmptree->parent->parent)
							{
								tmptree = tmptree->parent;
							} else
							{
								return NULL;
							}
						}
						tmptree = tmptree->parent->next;
					}
					else return NULL;
				}
			}
		}
		return NULL;
	}

	//--------------------------------------
	// Remove (and delete !! ) v and child
	BFCVoid Remove(BFCVoidTreeNode *v, BFCBool delete_it = true)
	{
		BFCVoidTreeNode *save=v;	
		if (v == NULL) BFCAssert(0,"Try to remove a null tree node");	
		if (v->child != NULL) 
		{
			v = v->child;
			
			while (1)
			{
				while (1)
				{
					while (v->next!=NULL) v = v->next;
					if (v->child!=NULL) 
					{
						v = v->child;
					} else
					{
						break;
					}
				}
				if (v->prev) 
				{
					v = v->prev;
					if (delete_it) delete v->next;
					v->next=NULL;
				} else 
				{
					v = v->parent;
					if (delete_it) delete v->child;
					v->child = NULL;
					if (v == save) break;
				}
			}
		};
		if (save->parent)
		{
			if (save->prev)
			{
				save->prev->next = save->next;
				if (save->next) save->next->prev = save->prev;
			} else
			{
				if (save->next)
				{
					save->parent->child = save->next;
					save->next->prev = NULL;
				} else
				{
					save->parent->child = NULL;
				}
			}		
		} else 
		{
			if (save->prev)
			{
				save->prev->next = save->next;
				if (save->next) save->next->prev = save->prev;
			} else
			{
				if (save->next)
				{
					root = save->next;	
				} else
				{
					root = NULL;
				}
			}
		}
		if (delete_it) delete save;
	}

	// Remove node (with BFCVoid = ptr) and child
	BFCVoid Remove(BFCVoid *ptr, BFCBool delete_it = true)
	{
		BFCVoidTreeNode *tmp=Find(ptr);
		if (tmp==NULL) BFCAssert(0,"Remove node didn't found the node to remove");
		Remove(tmp, delete_it);
	}

	//--------------------------------------
	// Remove (and delete !! ) v and child ( delete also internal pointer)
	BFCVoid RemoveDelete(BFCVoidTreeNode *v)
	{
		BFCVoidTreeNode *save=v;	
		if (v == NULL) BFCAssert(0,"Try to remove a null tree node");	
		if (v->child != NULL) 
		{
			v = v->child;
			
			while (1)
			{
				while (1)
				{
					while (v->next!=NULL) v = v->next;
					if (v->child!=NULL) 
					{
						v = v->child;
					} else
					{
						break;
					}
				}
				if (v->prev) 
				{
					v = v->prev;
					v->next->DeletePtr();
					delete v->next;
					v->next=NULL;
				} else 
				{
					v = v->parent;
					v->child->DeletePtr();
					delete v->child;
					v->child = NULL;
					if (v == save) break;
				}
			}
		};
		if (save->parent)
		{
			if (save->prev)
			{
				save->prev->next = save->next;
				if (save->next) save->next->prev = save->prev;
			} else
			{
				if (save->next)
				{
					save->parent->child = save->next;
					save->next->prev = NULL;
				} else
				{
					save->parent->child = NULL;
				}
			}		
		} else 
		{
			if (save->prev)
			{
				save->prev->next = save->next;
				if (save->next) save->next->prev = save->prev;
			} else
			{
				if (save->next)
				{
					root = save->next;	
				} else
				{
					root = NULL;
				}
			}
		}
		save->DeletePtr();
		delete save;
	}

	// Remove node (with BFCVoid = ptr) and child
	BFCVoid RemoveDelete(BFCVoid *ptr)
	{
		BFCVoidTreeNode *tmp=Find(ptr);
		if (tmp==NULL) BFCAssert(0,"Remove node didn't found the node to remove");
		RemoveDelete(tmp);
	}

	// Remove all

	BFCVoid RemoveAll()
	{
		Remove(root);
	}

	// Remove all and destroy internal pointers

	BFCVoid RemoveDeleteAll()
	{
		RemoveDelete(root);
	}
};

//*****************************************************************************************************
//
//
//
//								Global Utility Function
//
//
//
//
//*****************************************************************************************************


extern BFCChar *Float2Char(BFCFloat f);



#endif