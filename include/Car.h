#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

// 车辆类
// 存储车牌号、入场/离场时间戳、调试追加时间等信息
class Car {
    private:
        string id;              // 车牌号（唯一标识）
        bool isInPark;          // 是否在停车场内
        time_t inStackTime;     // 入场时间戳（调用 inPark 时记录）
        time_t maintainingTime; // 最近一次查询时间戳（调用 markMaintainingTimeStamp 时记录）
        time_t outStackTime;    // 离场时间戳（调用 outPark 时记录）
        time_t extraTime;       // 调试追加时间（秒），通过 addExtraTime 累加

    public:
        // 有参构造函数：指定车牌号，其余字段初始化
        Car(string id) {
            this->id = id;
            this->isInPark = false;
            this->inStackTime = 0;
            this->outStackTime = 0;
            this->extraTime = 0;
        }

        // 默认构造函数：车牌号为空
        Car() {
            this->id = "";
            this->isInPark = false;
            this->inStackTime = 0;
            this->maintainingTime = 0;
            this->outStackTime = 0;
            this->extraTime = 0;
        }

        // 标记入场：记录入场时间戳，设置在场状态
        void inPark() {
            this->isInPark = true;
            this->inStackTime = time(nullptr);
        }

        // 标记查询时间戳：更新 maintainingTime 为当前时间
        void markMaintainingTimeStamp() {
            this->maintainingTime = time(nullptr);
        }

        // 标记离场：记录离场时间戳，清除在场状态
        void outPark() {
            this->outStackTime = time(nullptr);
            this->isInPark = false;
        }

        // 获取累计停车时间（查询时使用）：(maintainingTime - inStackTime) + extraTime
        time_t getMaintainingTime() const {
            return (this->maintainingTime - this->inStackTime) + this->extraTime;
        }

        // 获取总停车时间（离场时使用）：(outStackTime - inStackTime) + extraTime
        time_t getPakingTime() const {
            return (this->outStackTime - this->inStackTime) + this->extraTime;
        }

        // 运算符重载：比较两个 Car 是否相同（按车牌号）
        bool operator== (const Car& otherObj) const {
            return (this->id == otherObj.getId());
        }

        // 运算符重载：比较 Car 与字符串是否相同（按车牌号）
        bool operator== (const string& otherObj) const {
            return (this->id == otherObj);
        }

        // 获取车牌号
        string getId() const {
            return this->id;
        }

        // 设置调试追加时间
        void setExtraTime(time_t targetTime) {
            this->extraTime = targetTime;
        }

        // 获取调试追加时间
        time_t getExtraTime() {
            return this->extraTime;
        }

        // 输出运算符重载：输出车牌号
        friend ostream& operator<<(ostream& os, const Car& car) {
            os << car.getId();
            return os;
        }

};

#endif // CAR_H
