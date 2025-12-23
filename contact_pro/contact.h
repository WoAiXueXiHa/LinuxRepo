#include <string>
using namespace std;

class contact{
private:
  string _name;
  int _sex;   // 1-男 2-女
  string _phone;
  string _address;

public:
  // 构造
  contact(){}
  contact(string name, int sex, string phone, string address)
    :_name(name)
     ,_sex(sex)
     ,_phone(phone)
     ,_address(address)
  {}

  // 设置属性
  void set_name(string name){ _name = name; }
  bool set_sex(int sex){
    if(sex == 1 || sex == 2){
      _sex = sex;
      return true;
    }
    return false;
  }
  void set_phone(string phone){ _phone = phone; }
  void set_address(string address){ _address = address; }

  // 获取属性
  string get_name() const { return _name; }
  int get_sex() const { return _sex; }
  string get_phone() const { return _phone; }
  string get_address() const { return _address; }
};
