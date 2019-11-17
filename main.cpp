#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <vector>

int data_index = 1;
int observer_index = 1;

int get_random() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist;
  return dist(mt);
}

class Observer {
 public:
  virtual void update(int value) = 0;
  virtual std::string name() = 0;
};

class Subject {
 public:
  virtual void registerObserver(std::shared_ptr<Observer> o) = 0;
  virtual void unregisterObserver(std::shared_ptr<Observer> o) = 0;
  virtual void notifyObservers() = 0;
};

class SpecialData : public Subject {
 public:
  SpecialData()
      : m_current_value(get_random()),
        m_name(std::string{"Datapoint-" + std::to_string(data_index)}) {}

  void registerObserver(std::shared_ptr<Observer> o) {
    std::cout << "\n"
              << name() << " is registering observer" << o->name() << std::endl;
    m_observers.push_back(o);
  }
  void unregisterObserver(std::shared_ptr<Observer> o) {
    for (auto it = m_observers.begin(); it != m_observers.end(); it++) {
      if (*it == o) {
        std::cout << "\n"
                  << o->name() << " is being unregistered from " << name()
                  << std::endl;
        m_observers.erase(it);
        break;
      }
    }
  }
  void notifyObservers() {
    for (const auto o : m_observers) {
      o->update(m_current_value);
    }
  }

  void work() {
    int old = m_current_value;
    m_current_value = get_random();
    if (m_current_value != old) {
      std::cout << "\n"
                << name() << " has updated its responsible value.\n"
                << std::endl;
      notifyObservers();
    }
  }

  std::string name() { return m_name; }

 private:
  std::vector<std::shared_ptr<Observer> > m_observers;
  int m_current_value;
  std::string m_name;
};

class Display : public Observer {
 public:
  Display() : m_name({"Observer-" + std::to_string(observer_index)}) {}
  void display() {
    std::cout << name() << " observed value: " << m_value << std::endl;
  }
  void update(int value) {
    m_value = value;
    display();
  }

  std::string name() { return m_name; }

 private:
  int m_value;
  std::string m_name;
};

int main(int argc, char** argv) {
  std::shared_ptr<Display> s_ptr_display = std::make_shared<Display>();
  observer_index++;
  std::shared_ptr<Display> s_ptr_display2 = std::make_shared<Display>();
  observer_index++;
  std::shared_ptr<Display> s_ptr_display3 = std::make_shared<Display>();
  observer_index++;

  SpecialData s_data = *(new SpecialData());
  data_index++;
  s_data.registerObserver(s_ptr_display);
  s_data.registerObserver(s_ptr_display2);
  s_data.registerObserver(s_ptr_display3);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  SpecialData s_data2 = *(new SpecialData());
  data_index++;
  s_data2.registerObserver(s_ptr_display);
  s_data2.registerObserver(s_ptr_display2);
  s_data2.registerObserver(s_ptr_display3);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  SpecialData s_data3 = *(new SpecialData());
  data_index++;
  s_data3.registerObserver(s_ptr_display);
  s_data3.registerObserver(s_ptr_display2);
  s_data3.registerObserver(s_ptr_display3);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  s_data.work();
  s_data2.work();
  s_data3.work();
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  s_data.work();
  s_data2.work();
  s_data3.work();
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  s_data.work();
  s_data2.work();
  s_data3.work();
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  s_data.unregisterObserver(s_ptr_display);
  s_data.unregisterObserver(s_ptr_display2);
  s_data.unregisterObserver(s_ptr_display3);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  s_data2.unregisterObserver(s_ptr_display);
  s_data2.unregisterObserver(s_ptr_display2);
  s_data2.unregisterObserver(s_ptr_display3);
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  s_data3.unregisterObserver(s_ptr_display);
  s_data3.unregisterObserver(s_ptr_display2);
  s_data3.unregisterObserver(s_ptr_display3);

  return 0;
}
