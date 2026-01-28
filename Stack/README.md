# Stack&lt;T&gt;

`Stack<T>`는 LIFO(Last-In, First-Out, 후입선출) 원칙을 따르며, 요소를 연속된 메모리 공간에 저장하는 가변 크기 배열 컨테이너입니다.  
내부적으로 메모리를 직접 관리하며, 요소 추가에 따라 필요한 용량을 자동으로 확장합니다.

---

## 시간 복잡도 (Time Complexity)

- 맨 위 요소 접근: O(1)
- 삽입: 분할 상환 O(1)
- 삭제: O(1)

---

## 인터페이스 (Interface)

### 생성자 / 소멸자 (Constructors / Destructor)

- `Stack()`  
  빈 스택을 생성합니다.

- `Stack(const Stack& other)`  
  복사 생성자. `other`의 모든 요소를 깊은 복사합니다.

- `Stack(Stack&& other)`  
  이동 생성자. `other`의 자원 소유권을 이전합니다.

- `~Stack()`  
  모든 요소를 제거하고 할당된 메모리를 해제합니다.

### 대입 연산자 (Assignment Operators)

- `Stack& operator=(const Stack& other)`  
  복사 대입 연산자.

- `Stack& operator=(Stack&& other)`  
  이동 대입 연산자.

### 요소 접근 (Element Access)

- `T& Peek()`  
  `const T& Peek()`  
  맨 위 요소에 대한 참조를 반환합니다.  
  스택이 비어 있을 경우 동작이 정의되지 않습니다.

### 용량 관리 (Capacity)

- `bool IsEmpty()`  
  스택이 비어 있으면 `true`를 반환합니다.

- `std::size_t Size()`  
  현재 저장된 요소의 개수를 반환합니다.

### 수정자 (Modifiers)

- `void Push(const T& value)`  
  `void Push(T&& value)`  
  스택의 맨 위에 요소를 추가합니다.

- `void Emplace(Args&&... args)`  
  스택의 맨 위에 요소를 직접 생성하여 추가합니다.

- `void Pop()`  
  맨 위 요소를 제거합니다.  
  스택이 비어 있을 경우 동작이 정의되지 않습니다.

- `void Clear()`  
  모든 요소를 제거합니다. 할당된 용량은 유지됩니다.

- `void Swap(Stack& other)`  
  두 스택의 내용을 서로 교환합니다.

### 비멤버 함수 (Non-member Functions)

- `bool operator==(const Stack& lhs, const Stack& rhs)`  
  두 스택의 크기와 모든 요소가 같은지 비교합니다.

- `auto operator<=>(const Stack& lhs, const Stack& rhs)`  
  두 스택을 사전순으로 비교합니다.

---

## 사용 예시 (Usage Example)

```cpp
#include <iostream>
#include "Stack.h"

int main()
{
    Stack<int> stack;
    
    stack.Push(1);                  // [1]
    stack.Push(2);                  // [1, 2]
    stack.Emplace(3);               // [1, 2, 3]
    
    std::cout << "Stack size: " << stack.Size() << "\n";
    std::cout << "Top element: " << stack.Peek() << "\n";
    
    stack.Pop();
    std::cout << "After pop, top: " << stack.Peek() << "\n";
    
    stack.Push(99);                 // [1, 2, 99]
    
    while (!stack.IsEmpty())
    {
        std::cout << "Popping: " << stack.Peek() << "\n";
        stack.Pop();
    }
    
    return 0;
}
