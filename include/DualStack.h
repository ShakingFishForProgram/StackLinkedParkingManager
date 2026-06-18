#ifndef DUALSTACK_H
#define DUALSTACK_H

#include <iostream>
#include <string>

using namespace std;

// 共享双栈模板类
// 左栈从 container[0] 向右增长，右栈从 container[size-1] 向左增长
template <typename T>
class DualStack {
    protected:
        int size;          // 底层数组总容量
        T* leftTop;        // 左栈栈顶指针，指向左栈最上方元素
        T* rightTop;       // 右栈栈顶指针，指向右栈最上方元素
        T* container;      // 底层数组首地址
        int leftNumber;    // 左栈当前元素个数
        int rightNumber;   // 右栈当前元素个数

    public:
        // 构造函数，传入数组总容量
        DualStack(int size) {
            this->size = size;
            container = new T[this->size];
            leftTop = container;                    // 左栈初始指向数组首
            rightTop = container + (size-1);        // 右栈初始指向数组尾
            this->leftNumber = 0;
            this->rightNumber = 0;
        }

        // 拷贝构造函数
        DualStack(const DualStack<T>& otherObj) :
            size(otherObj.size), leftNumber(otherObj.leftNumber),   rightNumber(otherObj.rightNumber)
        {
            this->container = new T[otherObj.size];
            for(int i=0; i<size; i++) {
                this->container[i] = otherObj.container[i];
            }
            // 重新计算指针偏移量，确保指向新数组的正确位置
            this->leftTop = this->container + (otherObj.leftTop - otherObj.container);
            this->rightTop = this->container + (otherObj.rightTop - otherObj.container);
        }

        // 赋值运算符
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

        // 析构函数，释放数组内存
        ~DualStack() {
            delete [] container;
            container = nullptr;
        }

        // 判断双栈是否已满（左栈元素数 + 右栈元素数 ≥ 总容量）
        bool isFull() const {
            if (this->leftNumber + this->rightNumber >= this->size) {
                return true;
            }
            return false;
        }

        // 判断左栈是否为空
        bool isLeftStackEmpty() const {
            if(this->leftNumber == 0) {
                return true;
            }
            return false;
        }

        // 左栈压入元素，成功返回 true，栈满返回 false
        bool pushLeftStack(const T& obj) {
            if(this->isFull()) {
                return false;
            }

            if(this->isLeftStackEmpty()) {
                *(leftTop) = obj;
            } else {
                *(++leftTop) = obj;     // 左栈指针右移后赋值
            }
            this->leftNumber ++;
            return true;
        }

        // 左栈弹出元素，存入 obj，成功返回 true，空栈返回 false
        bool popLeftStack(T& obj) {
            if(this->isLeftStackEmpty()) {
                return false;
            }
            if (this->leftTop != container) {
                obj = *(leftTop--);     // 取值后左栈指针左移
            } else {
                obj = *(leftTop);       // 只剩一个元素，直接取值不减指针
            }
            this->leftNumber --;
            return true;
        }

        // 取左栈栈顶元素（不弹出），成功返回 true
        bool topLeftStack(T& obj) const {
            if(this->isLeftStackEmpty()) {
                return false;
            }
            obj = *(this->leftTop);
            return true;
        }

        // 返回左栈当前元素个数
        int sizeLeftStack() const {
            return this->leftNumber;
        }


        // 判断右栈是否为空
        bool isRightStackEmpty() const {
            if(this->rightNumber == 0) {
                return true;
            }
            return false;
        }

        // 右栈压入元素，成功返回 true，栈满返回 false
        bool pushRightStack(const T& obj) {
            if(this->isFull()) {
                return false;
            }

            if(this->isRightStackEmpty()) {
                *(this->rightTop) = obj;
            } else {
                *(--rightTop) = obj;    // 右栈指针左移后赋值
            }
            this->rightNumber ++;
            return true;
        }

        // 右栈弹出元素，存入 obj，成功返回 true
        bool popRightStack(T& obj) {
            if(this->isRightStackEmpty()) {
                return false;
            }
            if(rightTop != container+(size-1)) {
                obj = *(rightTop++);    // 取值后右栈指针右移
            } else {
                obj = *(rightTop);      // 只剩一个元素，直接取值不移指针
            }
            this->rightNumber --;
            return true;
        }

        // 取右栈栈顶元素（不弹出），成功返回 true
        bool topRightStack(T& obj) const {
            if(this->isRightStackEmpty()) {
                return false;
            }
            obj = *(this->rightTop);
            return true;
        }

        // 返回右栈当前元素个数
        int sizeRightStack() const {
            return this->rightNumber;
        }

        // 双栈压入：默认压入左栈
        bool pushDualStack(const T& obj) {
            return this->pushLeftStack(obj);
        }

        // 双栈弹出（核心：搜索 + 让路 + 恢复）
        // 从左栈中查找值为 value 的元素，弹出后存入 obj
        // 左栈中目标上方的元素全部临时移入右栈（让路），之后再恢复
        bool popDualStack(string value, T& obj) {
            T currentObj;
            // 让路阶段：从左栈栈顶依次弹出，找到目标
            while(!this->isLeftStackEmpty()) {
                this->popLeftStack(currentObj);
                if(currentObj == value){
                    break;              // 找到目标，结束循环
                }
                this->pushRightStack(currentObj);  // 非目标移入右栈
            }
            // 恢复阶段：将右栈中的元素全部弹回左栈
            T tempObj;
            while(!this->isRightStackEmpty()) {
                this->popRightStack(tempObj);
                this->pushLeftStack(tempObj);
            }

            // 判断是否找到目标
            if(currentObj == value) {
                obj = currentObj;
                return true;
            }
            return false;               // 左栈中未找到目标
        }

        // 显示左栈所有元素（从左到右）
        void showLeftStack() const {
            T* pointer = this->container;
            if(this->isLeftStackEmpty()) {
                cout << "Left Stack Is Empty!";
                return;
            }

            do {
                cout << *(pointer) << " ";
                pointer ++;
            } while (pointer <= this->leftTop);
        }

        // 显示右栈所有元素（从右到左）
        void showRightStack() const {
            T* pointer = this->container + (this->size - 1);
            if(this->isRightStackEmpty()) {
                cout << "Right Stack Is Empty!";
                return;
            }
            do {
                cout << *(pointer) << " ";
                pointer --;
            } while (pointer >= this->rightTop);
        }

};

#endif // DUALSTACK_H
