#ifndef PARKINGDOUBLYLINKEDLIST_H
#define PARKINGDOUBLYLINKEDLIST_H

#include <string>
#include "Car.h"
#include "DoublyLinkedList.h"
#include "ParkingInterfaces.h"

// 停车场双向链表业务类
// 继承 DoublyLinkedList<Car>，管理便道（等待区）车辆
class ParkingDoublyLinkedList : public DoublyLinkedList<Car> {
    public:
        // 按车牌号查找车辆在链表中的索引，不存在返回 -1
        int indexOfString(const string key) {
            Node<Car>* currentNode = this->firstNode;
            int currentIndex = 0;
            while (currentNode != nullptr) {
                if(currentNode->value.getId() == key) {
                    break;
                }
                currentNode= currentNode->next;
                currentIndex ++;
            }
            if(currentNode != nullptr) {
                if(currentNode->value.getId() == key) {
                    return currentIndex;
                }
            }
            return -1;
        }

        // 生成便道车辆信息接口
        // 便道车辆尚未进场，停车时间强制为 0
        ParkingDoublyLinkedListInterface& getParkingMessage() {
            ParkingDoublyLinkedListInterface* pdll = new ParkingDoublyLinkedListInterface(this->size);
            Node<Car>* currentNode = this->firstNode;
            for(int i=0; i<this->size; i++) {
                pdll->container[i].id = currentNode->value.getId();
                pdll->container[i].totalTime = 0;               // 便道车辆时间计为 0

                currentNode = currentNode->next;
            }

            return *pdll;
        }

        // 便道车辆不需要加时（空实现）
        bool addExtraTime(int seconds=0, int minutes=0, int hours=0) {
            return true;
        }
};

#endif
