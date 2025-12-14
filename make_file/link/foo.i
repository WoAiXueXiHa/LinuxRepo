# 1 "foo.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "foo.cpp"
# 1 "add.h" 1
       

template <class T>
T add(T a, T b){
  return a + b;
}
# 2 "foo.cpp" 2

int foo(){
  return add(10,30);
}
