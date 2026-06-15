#ifndef DUALSTACK_H
#define DUALSTACK_H

#include <string>

using namespace std;

template <typename T>
class DualStack {
    private:
        int size;
        T* leftTop;
        T* rightTop;
        T* container;
        int leftNumber;
        int rightNumber;

    public:
        DualStack(int size) {
            this->size = size;
            container = new T[this->size];
            leftTop = container;
            rightTop = container + (size-1);
            this->leftNumber = 0;
            this->rightNumber = 0;
        }

        DualStack(const DualStack<T>& otherObj) :
            size(otherObj.size), leftNumber(otherObj.leftNumber),   rightNumber(otherObj.rightNumber)
        {
            this->container = new T[otherObj.size];
            for(int i=0; i<size; i++) {
                this->container[i] = otherObj.container[i];
            }
            this->leftTop = this->container + (otherObj.leftTop - otherObj.container);
            this->rightTop = this->container + (otherObj.rightTop - otherObj.container);
        }

        DualStack<T>& operator= (const DualStack<T>& otherObj) {
            if(this == &otherObj) {
                return *this;
            }

            delete [] container;

            this->size = otherObj.size;
            this->leftNumber = otherObj.leftNumber;
            this->rightNumber = otherObj.rightNumber;
            this->container = new T[otherObj.size];
            for(int i=0; i<size; i++) {
                this->container[i] = otherObj.container[i];
            }
            this->leftTop = this->container + (otherObj.leftTop - otherObj.container);
            this->rightTop = this->container + (otherObj.rightTop - otherObj.container);

            return *this;
        }

        ~DualStack() {
            delete [] container;
            container = nullptr;
        }

        bool isFull() const {
            if (this->leftNumber + this->rightNumber >= this->size) {
                return true;
            }
            return false;
        }

        bool isLeftStackEmpty() const {
            if(this->leftNumber == 0) {
                return true;
            }
            return false;
        }

        bool pushLeftStack(const T& obj) {
            if(this->isFull()) {
                return false;
            }

            if(this->isLeftStackEmpty()) {
                container[leftTop] = obj;
            } else {
                container[++leftTop] = obj;
            }
            this->leftNumber ++;
            return true;
        }

        bool popLeftStack(T& obj) {
            if(this->isLeftStackEmpty()) {
                return false;
            }
            if (this->leftTop != container) {
                obj = container[leftTop--];
            } else {
                obj = container[leftTop];
            }
            this->leftNumber --;
            return true;
        }

        bool topLeftStack(T& obj) const {
            if(this->isLeftStackEmpty()) {
                return false;
            }
            obj = container[this->leftTop];
            return true;
        }

        int sizeLeftStack() const {
            return this->leftNumber;
        }


        bool isRightStackEmpty() const {
            if(this->rightNumber == 0) {
                return true;
            }
            return false;
        }

        bool pushRightStack(const T& obj) {
            if(this->isFull()) {
                return false;
            }

            if(this->isRightStackEmpty()) {
                container[this->rightTop] = obj;
            } else {
                container[--rightTop] = obj;
            }
            this->rightNumber ++;
            return true;
        }

        bool popRightStack(T& obj) {
            if(this->isRightStackEmpty()) {
                return false;
            }
            if(rightTop != container+(size-1)) {
                obj = container[rightTop++];
            } else {
                obj = container[rightTop];
            }
            this->rightNumber --;
            return true;
        }

        bool topRightStack(T& obj) const {
            if(this->isRightStackEmpty()) {
                return false;
            }
            obj = container[this->rightTop];
            return true;
        }

        int sizeRightStack() const {
            return this->rightNumber;
        }



        bool pushDualStack(const T& obj) {
            return this->pushLeftStack(obj);
        }

        bool popDualStack(string value, T& obj) {
            T currentObj = nullptr;
            while(!this->isLeftStackEmpty()) {
                this->popLeftStack(currentObj);
                if(currentObj == value){
                    break;
                }
                this->pushRightStack(currentObj);
            }
            T tempObj = nullptr;
            while(!this->isRightStackEmpty()) {
                this->popRightStack(tempObj);
                this->pushLeftStack(tempObj);
            }

            if(currentObj == value) {
                obj = currentObj;
                return true;
            }
            return false;
        }

};

#endif // DUALSTACK_H
