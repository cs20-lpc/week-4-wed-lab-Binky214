// I still don't know the difference between i++ and ++i but AI seems to favor ++i over i++ so you'll see a mix and hopefully it doesn't cause issues in the code


template <typename T>
DoublyList<T>::DoublyList()
: header(new Node), trailer(new Node) {
    header->next  = trailer;
    trailer->prev = header;
}

template <typename T>
DoublyList<T>::DoublyList(const DoublyList<T>& copyObj)
: header(new Node), trailer(new Node) {
    copy(copyObj);
}

template <typename T>
DoublyList<T>& DoublyList<T>::operator=(const DoublyList<T>& rightObj) {
    if (this != &rightObj) {
        clear();
        copy(rightObj);
    }
    return *this;
}

template <typename T>
DoublyList<T>::~DoublyList() {
    clear();
    delete header;
    delete trailer;
    header = trailer = nullptr;
}

template <typename T>
void DoublyList<T>::append(const T& elem) {
    Node* newNode = new Node;
    newNode->value = elem;
    newNode->next = nullptr;
    newNode->prev = nullptr;

    //empty list check
    if (header == nullptr) {
        header = newNode;
        trailer = newNode;
    }
    else {
        newNode->prev = trailer;
        trailer->next = newNode;
        trailer = newNode;
    }

    this->length++;

}

template <typename T>
void DoublyList<T>::clear() {
    for (Node* curr = nullptr; header->next != trailer; ) {
        curr = header->next->next;
        delete header->next;
        header->next = curr;
    }

    trailer->prev = header;
    this->length  = 0;
}

template <typename T>
void DoublyList<T>::copy(const DoublyList<T>& copyObj) {
    this->length   = copyObj.length;
    Node* myCurr   = header;
    Node* copyCurr = copyObj.header->next;

    while (copyCurr != copyObj.trailer) {
        Node* n      = new Node(copyCurr->value);
        myCurr->next = n;
        n->prev      = myCurr;
        myCurr       = n;
        copyCurr     = copyCurr->next;
    }

    myCurr->next  = trailer;
    trailer->prev = myCurr;
}

template <typename T>
T DoublyList<T>::getElement(int position) const {
    if (position < 0 || position >= this->length) {
        throw std::out_of_range("Out of bounds");
    }

    Node* curr;

    curr = header;

    for (int i = 0; i < position; i++) {
        curr = curr -> next;
    }

    return curr -> value;
}

template <typename T>
int DoublyList<T>::getLength() const {
    return this->length;
}


template <typename T>
void DoublyList<T>::insert(int position, const T& elem) {

    if (position < 0 || position > this->length) {
        throw std::out_of_range("Index out of bounds");
    }

    Node* newNode = new Node(elem);

    //if empty list
    if (this->length == 0) {
        header = trailer = newNode;
    }

    //insert at head
    else if (position == 0) {
        newNode->next = header;
        header->prev = newNode;
        header = newNode;
    }

    //insert at end (basically append)
    else if (position == this->length) {
        newNode->prev = trailer;
        trailer->next = newNode;
        trailer = newNode;
    }
    
    //insert somewhere inside
    else {
        Node* curr;

        //AI recommends that I put this where it chooses direction depending on if position is closer to begin/end nodes
        if (position < this->length / 2) {
            curr = header;
            for (int i = 0; i < position; ++i) {
                curr = curr->next;
            }
        } else {
            curr = trailer;
            for (int i = this->length - 1; i >= position; --i) {
                curr = curr->prev;
            }
        }

        newNode->prev = curr->prev;
        newNode->next = curr;

        curr->prev->next = newNode;
        curr->prev = newNode;
    }

    this->length++;
}

template <typename T>
bool DoublyList<T>::isEmpty() const {
    return this->length  == 0
        && header->next  == trailer
        && trailer->prev == header;
}

template <typename T>
void DoublyList<T>::remove(int position) {
    
    if (position < 0 || position >= this->length) {
        throw std::out_of_range("Index out of bounds");
    }

    Node* deleteNode;

    //1 element
    if (this->length == 1) {
        deleteNode = header;
        header = nullptr;
        trailer = nullptr;
    }
    //remove head
    else if (position == 0) {
        deleteNode = header;
        header = header->next;
        header->prev = nullptr;
    }
    //remove trailer
    else if (position == this->length - 1) {
        deleteNode = trailer;
        trailer = trailer->prev;
        trailer->next = nullptr;
    }
    //remove somewhere in middle
    else {
        Node* curr;

        //AI recommends that I put this where it chooses direction depending on if position is closer to begin/end nodes
        if (position < this->length / 2) {
            curr = header;
            for (int i = 0; i < position; ++i) {
                curr = curr->next;
            }
        } else {
            curr = trailer;
            for (int i = this->length - 1; i > position; --i) {
                curr = curr->prev;
            }
        }

        deleteNode = curr;
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
    }

    delete deleteNode;
    this->length--;
}

template <typename T>
bool DoublyList<T>::search(const T& elem) const {
    Node* curr = header;

    while (curr != nullptr) {
        if (curr->value == elem) {
            return true;
        }
        curr = curr -> next;
    }
    return false;
}

template <typename T>
void DoublyList<T>::replace(int position, const T& elem) {
    if (position < 0 || position >= this->length) {
        throw std::out_of_range("Index out of bounds");
    }

    Node* curr;

    //AI recommends that I put this where it chooses direction depending on if position is closer to begin/end nodes
    if (position < this->length / 2) {
        curr = header;
        for (int i = 0; i < position; ++i) {
            curr = curr->next;
        }
    } else {
        curr = trailer;
        for (int i = this->length - 1; i > position; --i) {
            curr = curr->prev;
        }
    }

    curr->value = elem;
}

template <typename T>
ostream& operator<<(ostream& outStream, const DoublyList<T>& myObj) {
    if (myObj.isEmpty()) {
        outStream << "List is empty, no elements to display.\n";
    }
    else {
        typename DoublyList<T>::Node* curr = myObj.header->next;
        while (curr != myObj.trailer) {
            outStream << curr->value;
            if (curr->next != myObj.trailer) {
                outStream << " <--> ";
            }
            curr = curr->next;
        }
        outStream << endl;
    }

    return outStream;
}
